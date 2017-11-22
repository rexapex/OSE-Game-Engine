import json
import sys

print("OSE Module Manager")

#probably only works if run from inside "Module Manager" directory
settingsFile = open("settings.json", "r")
contents = settingsFile.read()
contentsObj = json.loads(contents)


#TODO - check if version is up to date
def checkVersions():
    pass


#check if all required dirs are specified
def checkDirs():
    if (not "user_dirs" in contentsObj) or (len(contentsObj["user_dirs"]) == 0):
        print("no user dir specified")
    else:
        print(str(len(contentsObj["user_dirs"])) +" user dirs specified")


def parseCommand(command):
    if command.startswith("help") or command == "h":
        print("setudir <directory path>              add a directory to the list of user directories")
    elif command.startswith("setudir"):
        #TODO - use regex to split at whitespace unless in quotes
        args = command.split(" ")
        if len(args) > 1:
            for i in range(1, len(args)):
                udir = args[i].strip()
                print(udir)
        else:
            print("error: setudir requires 1 arg, <directory path>")
    elif command.startswith("exit"):
        sys.exit()


def loop():
    while True:
        command = input()
        parseCommand(command)



checkVersions()
checkDirs()
loop()
