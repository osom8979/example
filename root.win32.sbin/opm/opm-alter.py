#!/python27/python
# -*- coding: utf-8 -*-

import os
import sys

OPMHOME  = os.environ["OPMHOME"]
OPMALTER = os.path.join(OPMHOME, "alter")
OPMBIN   = os.path.join(OPMHOME, "bin")

BATEXT = ".bat"

def createCmdAlter(package, link):
    package_path  = os.path.join(OPMBIN, package + BATEXT)
    link_path  = os.path.join(OPMALTER, link + BATEXT)

    if os.path.isfile(package_path) == True:
        if os.path.isfile(link_path) == True:
            os.remove(link_path)
        file = open(link_path, 'wb')
        file.write("@call " + package_path)
        file.close()
        return True
    return False

def createBashAlter(link):
    link_path  = os.path.join(OPMALTER, link)

    if os.path.isfile(link_path) == True:
        os.remove(link_path)
    file = open(link_path, 'wb')
    file.write("cmd /c '@call " + link + BATEXT + "'")
    file.close()
    return True

if __name__ == "__main__":
    if len(sys.argv) >= 3:
        link = sys.argv[1]
        package = sys.argv[2]

        if os.path.exists(OPMHOME) == False:
            os.mkdirs(OPMHOME)
        if os.path.exists(OPMBIN) == False:
            os.mkdir(OPMBIN)
        if os.path.exists(OPMALTER) == False:
            os.mkdir(OPMALTER)

        if os.path.isfile(os.path.join(OPMBIN, package + BATEXT)) == False:
            print "Not found " + package + " script."
            exit(1)

        result = createCmdAlter(package, link)
        if result == True:
            createBashAlter(link)
            print "Create a " + link + " alternative script."
            exit(0)
        else:
            print "Can't create a " + link + " alternative script."
    else:
        print "You need to give some command."
        print "Usage: " + __file__ + " {link} {package}"
    exit(1)
