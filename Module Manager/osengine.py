import json
import sys
import re
import subprocess
import os
import shlex
import inspect

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
    if (not "user_dirs" in contentsObj) or (not isinstance(contentsObj["user_dirs"], list)) or (len(contentsObj["user_dirs"]) == 0):
        print("no user dir found")
        possibleDir = os.path.expanduser("~/Documents/Origami_Sheep_Engine")        #TODO - test this on MACOS and Linux
        print("Would you like to create the user directory " + possibleDir + " ?")
        resp = input("Y/n ")
        if resp == "Y" or resp == "y" or resp == "":
            contentsObj["user_dirs"] = [possibleDir]
            writeSettingsFile()
    else:
        print(str(len(contentsObj["user_dirs"])) +" user dirs found")



#add a user directory
def addudir(command):
    #shlex split preserves quoted strings
    try:
        args = shlex.split(command, posix=False)        #TODO - test this on MACOS and Linux
        if len(args) > 1:
            for i in range(1, len(args)):
                udir = args[i].strip()
                if (not "user_dirs" in contentsObj) or (not isinstance(contentsObj["user_dirs"], list)):
                    contentsObj["user_dirs"] = [udir]
                else:
                    contentsObj["user_dirs"].append(udir)
                writeSettingsFile()
                print("added user directory " + udir)
        else:
            print("error: addudir requires 1 arg, <directory path>")
    except ValueError:
        print("error: directory path is corrupt")



def updatemodule(module):
    if "repo" in module:
        cwd = os.path.dirname(os.path.abspath(inspect.stack()[0][1]))
        localrepo = cwd + "/../OSE V2"
        subprocess.call(["git", "clone", module["repo"], localrepo])
        os.chdir(localrepo + "/OSE-V2-STD-Modules")
        subprocess.call(["git", "pull", "origin", "master"])
        os.chdir(cwd)



#TODO - fetch code from remote repositories to update profiles' modules
def updateprofile(command):
    try:
        args = shlex.split(command, posix=False)        #TODO - test this on MACOS and Linux
        if len(args) > 1:
            name = args[1]
            for modulekey in contentsObj["profiles"][name]["modules"]:
                module = contentsObj["profiles"][name]["modules"][modulekey]
                updatemodule(module)
                for depkey in module["dependencies"]:
                    dep = module["dependencies"][depkey]
                    if "repo" in dep:
                        print("cloning repo " + dep["repo"])
                        subprocess.call(["git", "clone", dep["repo"], contentsObj["user_dirs"][0]+"/Profiles/"+name+"/dependencies/"+depkey])
        else:
            print("error: update requires 1 argument, <profile name>")
    except ValueError:
        print("error: command is corrupt")



#make a new engine profile
def mkprofile(command):
    #shlex split preserves quoted strings
    try:
        args = shlex.split(command, posix=False)        #TODO - test this on MACOS and Linux
        if len(args) > 1:
            name = args[1]
            if "profiles" in contentsObj:
                contentsObj["profiles"][name] = {"modules": {}}
            else:
                contentsObj["profiles"][name] = {"modules" : {}}
            if len(args) > 2:
                blueprint = args[2]
                #if the blueprint is std, then get from the standard module git repository
                if blueprint == "std":
                    contentsObj["profiles"][name]["modules"]["ProjectLoader"] = {"name": "ProjectLoaderXML", "dependencies": {"rapidxml": {"download": "https://sourceforge.net/projects/rapidxml/files/rapidxml/rapidxml%201.13/rapidxml-1.13.zip/download"}}}
                    contentsObj["profiles"][name]["modules"]["WindowManager"] = {"name": "WindowManagerGLFW", "dependencies": {"GLFW": {"repo": "https://github.com/glfw/glfw.git", "branch": "master"}}}
                    updateprofile("update " + name)
            writeSettingsFile()
            print("created profile " + name)
        else:
            print("error: mkprofile requires 1 arg, <name>")
    except ValueError:
        print("error: name and/or blueprint is corrupt")



#TODO - this function is probably quite important
def build(command):
    #first, generate the EngineReferences.h file
    pass



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
        if not ((not "user_dirs" in contentsObj) or (not isinstance(contentsObj["user_dirs"], list))):
            for dir in contentsObj["user_dirs"]:
                print(dir)
    elif command.startswith("addudir"):
        addudir(command)
    elif command == "lsprofiles":
        if not ((not "profiles" in contentsObj)):
            for profile in contentsObj["profiles"]:
                print("PROFILE " + profile)
                for module in contentsObj["profiles"][profile]["modules"]:
                    print("    MODULE " + module + ": " + contentsObj["profiles"][profile]["modules"][module]["name"])
    elif command.startswith("mkprofile"):
        mkprofile(command)
    elif command.startswith("update"):
        updateprofile(command)
    elif command == "gettest":
        if not ((not "user_dirs" in contentsObj) or (not isinstance(contentsObj["user_dirs"], list)) or (len(contentsObj["user_dirs"]) == 0)):
            subprocess.call(["git", "clone", "https://github.com/rexapex/OSE-V2-TestProject.git", contentsObj["user_dirs"][0]+"/Projects/OSE-V2-TestProject"])
    elif command == "build" or command == "b":
        build(command)
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
