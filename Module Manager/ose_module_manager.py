import json
import sys
import re
import subprocess
import os
import shlex
import inspect
import shutil
import errno

print("OSE Module Manager")

#probably only works if run from inside "Module Manager" directory
settingsFile = open("settings.json", "r+")
contents = settingsFile.read()
contentsObj = json.loads(contents)



def writeSettingsFile():
    #https://stackoverflow.com/questions/21035762/python-read-json-file-and-modify
    settingsFile.seek(0)        # <--- should reset file position to the beginning.
    json.dump(contentsObj, settingsFile, indent=4)
    settingsFile.truncate()     # remove remaining part



#TODO - check if version is up to date
def checkVersions():
    pass


#check if all required dirs are specified
def checkDirs():
    if (not "user_dirs" in contentsObj) or (not isinstance(contentsObj["user_dirs"], dict)) or (len(contentsObj["user_dirs"]) == 0):
        print("no user dir found")
        possibleDir = os.path.expanduser("~/Documents/Origami_Sheep_Engine")        #TODO - test this on MACOS and Linux
        print("Would you like to create the user directory " + possibleDir + " ?")
        resp = input("Y/n ")
        if resp == "Y" or resp == "y" or resp == "":
            contentsObj["user_dirs"] = {"dir1": possibleDir}
            writeSettingsFile()
    else:
        print(str(len(contentsObj["user_dirs"])) +" user dirs found")



def copyFileOrDir(src, dest):
    try:
        shutil.copytree(src, dest)
    except OSError as err:
        if err.errno == errno.ENOTDIR:
            shutil.copy(src, dest)
        else:
            raise



def getGitProjectName(gitpath):
    end = os.path.splitext(gitpath.strip('/').strip('\\').split('/')[-1].split('\\')[-1])[0]
    print(end)
    return end



#add a user directory
def addudir(command):
    #shlex split preserves quoted strings
    try:
        args = shlex.split(command, posix=False)        #TODO - test this on MACOS and Linux
        if len(args) > 2:
            name = args[1].strip()
            udir = args[2].strip()
            if (not "user_dirs" in contentsObj) or (not isinstance(contentsObj["user_dirs"], dict)):
                contentsObj["user_dirs"] = {name: udir}
            else:
                contentsObj["user_dirs"][name] = udir
            writeSettingsFile()
            print("added user directory " + udir)
        else:
            print("error: addudir requires 2 args, <udir unique name> <directory path>")
    except ValueError:
        print("error: directory path is corrupt")



def updatemodule(profilepath, module):
    print("yohoho")
    if "repo" in module:
        #cwd = os.path.dirname(os.path.abspath(inspect.stack()[0][1]))
        pdir = profilepath;
        localrepo = pdir + "/origami_sheep_engine/" + getGitProjectName(module["repo"])
        print(localrepo)
        subprocess.call(["git", "clone", module["repo"], localrepo])
        os.chdir(localrepo + getGitProjectName(module["repo"]))
        subprocess.call(["git", "pull", "origin", "master"])
        os.chdir(pdir)



def cloneblueprint(blueprint, blueprintname, profilename):
    #ensure all required fields are present
    print("repo" in blueprint and "modules" in blueprint)
    if "repo" in blueprint and "modules" in blueprint:
        repo = blueprint["repo"]
        udir = contentsObj["user_dirs"][contentsObj["profiles"][profilename]["udir"]]
        #ensure udir exists and repo is a string
        print(not udir is None and isinstance(repo, str))
        if not udir is None and isinstance(repo, str):
            #clone the blueprint
            blueprintpath = udir+"/Profiles/"+profilename+"/origami_sheep_engine/"+blueprintname
            subprocess.call(["git", "clone", repo, blueprintpath])
            #get the modules from the manifest file
            with open(blueprintpath + "/manifest.json", "r") as manifestfile:
                contentsman = manifestfile.read()
                contentsmanObj = json.loads(contentsman)
                blueprint["modules"] = contentsmanObj["blueprint"]["modules"]



def clonedependencies(blueprint, profilename):
    #ensure all required fields are present
    if "repo" in blueprint and "modules" in blueprint:
        udir = contentsObj["user_dirs"][contentsObj["profiles"][profilename]["udir"]]
        repo = blueprint["repo"]
        #ensure udir exists and repo is a string
        if not udir is None and isinstance(repo, str):
            #for each module, clone the modules dependencies
            for modulename in blueprint["modules"]:
                module = blueprint["modules"][modulename]
                for depname in module["dependencies"]:
                    dep = module["dependencies"][depname]
                    #if the dependency is in a git repo, clone the repo
                    if "repo" in dep:
                        subprocess.call(["git", "clone", dep["repo"], udir+"/Profiles/"+profilename+"/dependencies/"+depname])



#TODO - fetch code from remote repositories to update profiles' modules
def updateprofiledependencies(command):
    try:
        args = shlex.split(command, posix=False)        #TODO - test this on MACOS and Linux
        if len(args) > 1:
            profilename = args[1]
            if profilename in contentsObj["profiles"]:
                profile = contentsObj["profiles"][profilename]
                udirname = profile["udir"]
                if udirname in contentsObj["user_dirs"]:
                    udir = contentsObj["user_dirs"][udirname]
                    for bpkey in profile["blueprints"]:
                        for modulekey in profile["blueprints"][bpkey]["modules"]:
                            module = profile["blueprints"][bpkey]["modules"][modulekey]
                            updatemodule(udir+"/Profiles/"+profilename, module)
                            for depkey in module["dependencies"]:
                                dep = module["dependencies"][depkey]
                                if "repo" in dep:
                                    print("cloning repo " + dep["repo"])
                                    subprocess.call(["git", "clone", dep["repo"], udir+"/Profiles/"+profilename+"/dependencies/"+depkey])
            else:
                print("error: profile " + name + " does not exist")
        else:
            print("error: update requires 1 argument, <profile name>")
    except ValueError:
        print("error: command is corrupt")



#make a new engine profile
def mkprofile(command):
    #shlex split preserves quoted strings
    try:
        args = shlex.split(command, posix=False)        #TODO - test this on MACOS and Linux
        if len(args) > 2:
            profilename = args[1].strip()
            udir = args[2].strip()
            #update settings file
            if "profiles" in contentsObj:
                contentsObj["profiles"][profilename] = {"blueprints": {}, "udir": udir}
            else:
                contentsObj["profiles"] = {profilename: {"blueprints" : {}, "udir": udir}}
            #copy OSE code into the profile directory
            copyFileOrDir("../OSE V2", contentsObj["user_dirs"][udir] + "/Profiles/" + profilename + "/origami_sheep_engine")
            #download blueprint files
            if len(args) > 3:
                blueprintag = args[3]
                #if the blueprint is std, then get from the standard module git repository
                if blueprintag == "std":
                    #contentsObj["profiles"][name]["modules"]["ProjectLoader"] = {"class": "ProjectLoaderXML", "dependencies": {"rapidxml": {"download": "https://sourceforge.net/projects/rapidxml/files/rapidxml/rapidxml%201.13/rapidxml-1.13.zip/download"}}}
                    #contentsObj["profiles"][name]["modules"]["WindowManager"] = {"class": "WindowManagerGLFW", "dependencies": {"GLFW": {"repo": "https://github.com/glfw/glfw.git", "branch": "master"}}}
                    blueprints = contentsObj["profiles"][profilename]["blueprints"]
                    blueprints["OSE-V2-STD-Modules"] = {"repo": "https://github.com/rexapex/OSE-V2-STD-Modules.git", "modules": {}}
                    cloneblueprint(blueprints["OSE-V2-STD-Modules"], "OSE-V2-STD-Modules", profilename)
                    clonedependencies(blueprints["OSE-V2-STD-Modules"], profilename)
            writeSettingsFile()
            print("created profile " + profilename + " in udir " + udir)
        else:
            print("error: mkprofile requires 2 args, <new profile name> <user directory name>")
    except ValueError:
        print("error: name and/or blueprint is corrupt")



#TODO - this function is probably quite important
def buildprofile(command):
    #shlex split preserves quoted strings
    try:
        args = shlex.split(command, posix=False)        #TODO - test this on MACOS and Linux
        if len(args) > 2:
            name = args[1]
            udir = args[2]
            #first, generate the EngineReferences.h file
            engineReferences =  "#pragma once\n"
            for module in contentsObj["profiles"][name]["modules"]:
                engineReferences += "#include \"OSE-V2-STD-Modules\ProjectLoaderXML.h\"\n"
            engineReferences += "namespace origami_sheep_engine {\n"
            for module in contentsObj["profiles"][name]["modules"]:
                engineReferences += ""
            engineReferences += "}\n"
            with open(contentsObj["user_dirs"][udir]+"/Profiles/"+name+"/origami_sheep_engine/EngineReferences.h", "w") as f:
                f.write(engineReferences)
        else:
            print("error: build requires 2 arg, <profile name> <user directory name>")
    except ValueError:
        print("error: command is corrupt")



def lsprofiles():
    if not ((not "profiles" in contentsObj)):
        for profilename in contentsObj["profiles"]:
            print("PROFILE " + profilename)
            profile = contentsObj["profiles"][profilename]
            for blueprintname in profile["blueprints"]:
                print("    BLUEPRINT " + blueprintname)
                blueprint = profile["blueprints"][blueprintname]
                for modulename in blueprint["modules"]:
                    print("        MODULE " + modulename + ": " + blueprint["modules"][modulename]["class"])



#parse command entered via the command line
def parseCommand(command):
    if command == "help" or command == "h":
        print("quit                                  quit and return the the command prompt")
        print("lsdirs                                list all currently configured user directories")
        print("addudir <directory path>              add a directory to the list of user directories")
        print("mkprofile <name> <blueprint>          make a new profile, name should be unique, blueprint details the default set of modules, use <blueprint> = std to download the default modules")
        print("update <profile name>                 update modules for the specified profile")
        print("build <profile name>                  TODO - should build OSE with listed modules")
    elif command == "lsdirs":
        if not ((not "user_dirs" in contentsObj) or (not isinstance(contentsObj["user_dirs"], dict))):
            for dirname in contentsObj["user_dirs"]:
                print(dirname + ": " + contentsObj["user_dirs"][dirname])
    elif command.startswith("addudir"):
        addudir(command)
    elif command == "lsprofiles":
        lsprofiles()
    elif command.startswith("mkprofile"):
        mkprofile(command)
    elif command.startswith("update"):
        updateprofiledependencies(command)
    elif command == "gettest":
        if not ((not "user_dirs" in contentsObj) or (not isinstance(contentsObj["user_dirs"], dict)) or ("dir1" in contentsObj["user_dirs"])):          #TODO - change so user specifies which udir
            subprocess.call(["git", "clone", "https://github.com/rexapex/OSE-V2-TestProject.git", contentsObj["user_dirs"][0]+"/Projects/OSE-V2-TestProject"])
    elif command == "build" or command == "b":
        buildprofile(command)
    elif command == "quit" or command == "q":
        sys.exit()
    else:
        print("command not recognised")



#loops until the exit command is entered
def loop():
    while True:
        command = input("> ")
        parseCommand(command)



checkVersions()
checkDirs()
loop()
