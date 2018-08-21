#!/python27/python
# -*- coding: utf-8 -*-

import os
import sys
import re
import glob
import fnmatch
import subprocess
import getpass
import hashlib
import zipfile
import xml.etree.ElementTree as ET
from optparse import OptionParser


## -----------------
## Global variables.
## -----------------

TAG_ROOT       = "opm"

TAG_CONFIG     = "config"
TAG_PROTOCOL   = "protocol"
TAG_USER       = "user"
TAG_HOST       = "host"
TAG_PORT       = "port"
TAG_REMOTEPATH = "remote_path"

TAG_PACKAGE  = "package"
TAG_CHECKSUM = "checksum"
TAG_ARCHIVE  = "archive"

ATTR_ROOT_VERSION    = "version"
ATTR_CHECKSUM_METHOD = "method"

VERSION_MIN = "0.1"
METHOD_MD5  = "md5"
ARCHIVE_ZIP = "zip"

XMLEXT = ".xml"
ZIPEXT = ".zip"

MESSAGE_WARNING  = "[WARNING] "
MESSAGE_ERROR    = "[ERROR] "

SCRIPT_PATH = os.path.abspath(__file__)
RUNTIME_DIR = os.path.dirname(SCRIPT_PATH)

CURL_COMMAND  = "curl"
CURL_INSECURE = "--insecure"
CURL_LIST     = "--list-only"

OPMHOME     = os.environ["OPMHOME"]              # OPM Resource HOME directory.
OPMBIN      = os.path.join(OPMHOME, "bin")       # Registered PATH.
OPMALTER    = os.path.join(OPMHOME, "alter")     # Registered PATH (Alternative files).
OPMCACHE    = os.path.join(OPMHOME, "cache")     # Package cache.
OPMAPPS     = os.path.join(OPMHOME, "apps")      # Application data.
OPMBACKUP   = os.path.join(OPMHOME, "backup")    # Backup.
OPMSETUP    = os.path.join(OPMHOME, "setup")     # Auto load all architecture.
OPMSETUP32  = os.path.join(OPMHOME, "setup-x86") # Auto load x86 architecture.
OPMSETUP64  = os.path.join(OPMHOME, "setup-x64") # Auto load x86_64 architecture.
#OPMSETUP32 = os.environ["OPMSETUP32"]
#OPMSETUP64 = os.environ["OPMSETUP64"]
CONFIGFILE  = os.path.join(OPMHOME, "opm.xml")   # Configure file.

COMMANDLINE = OptionParser()
COMMANDLINE.add_option("-l", "--list", dest="list", action="store_true"
    , default=False, help="Print cached package list.")
COMMANDLINE.add_option("-i", "--install", dest="install", metavar="PACKAGE"
    , default="", help="Install package.")
COMMANDLINE.add_option("-u", "--update", dest="update", action="store_true"
    , default=False, help="Update package database.")
COMMANDLINE.add_option("-c", "--check", dest="check", action="store_true"
    , default=False, help="Check the package error.")
COMMANDLINE.add_option("--upload", dest="upload", metavar="FILE"
    , default="", help="Force upload file.")
COMMANDLINE.add_option("--remote", dest="remote", action="store_true"
    , default=False, help="Print remote package list.")
COMMANDLINE.add_option("-v", "--verbose", dest="verbose", action="store_true"
    , default=False, help="Print verbose information.")
(OPTIONS, ARGS) = COMMANDLINE.parse_args()


## --------
## Utility.
## --------

def getPassword():
    return getpass.getpass("Password:")

def existAndMkdir(dir):
    if os.path.isdir(dir) == False:
        os.mkdir(dir)

def getUriPath(prefix, suffix = ""):
    if prefix[len(prefix) - 1] == "/":
        return prefix + suffix
    return prefix + "/" + suffix


## ---------------------
## class implementation.
## ---------------------

## Example:
## <?xml version="1.0" encoding="utf-8" ?>
## <opm version="0.1">
##     <config>
##         <protocol>ssh</protocol>
##         <user>root</user>
##         <host>127.0.0.1</host>
##         <port>22</port>
##         <remote_path>/var/opm</remote_path>
##     </config>
## </opm>
class Config:
    protocol    = "ssh"
    user        = "root"
    host        = "127.0.0.1"
    port        = "22"
    remote_path = "/var/opm"
    version     = VERSION_MIN

    def __init__(self):
        pass

    def read(self, filename):
        doc = ET.parse(filename)
        root = doc.getroot()

        #check the root tag.
        if root.tag == TAG_ROOT:
            self.version = root.attrib[ATTR_ROOT_VERSION]

        config = root.find(TAG_CONFIG)
        self.protocol    = config.find(TAG_PROTOCOL).text
        self.user        = config.find(TAG_USER).text
        self.host        = config.find(TAG_HOST).text
        self.port        = config.find(TAG_PORT).text
        self.remote_path = config.find(TAG_REMOTEPATH).text
        #except (ET.ParseError, AttributeError) as e:
        pass

    def checkVersion(self, ver = VERSION_MIN):
        if self.version == ver:
            return True
        return False

    def uri(self):
        return self.protocol + "://" + self.host + ":" + self.port + self.remote_path


## Example:
## <?xml version="1.0" encoding="utf-8" ?>
## <opm version="0.1">
##     <package>
##         <archive>zip</archive>
##         <checksum method="md5">1539b3afacd34284c0ecd6ad75d2952a</checksum>
##     </package>
## </opm>
class Package:
    name     = ""
    checksum = ""
    method   = ""
    archive  = ""
    version  = VERSION_MIN

    def __init__(self, name):
        self.name = name

    def getPackageNameInfoArray(self):
        return self.name.split("-")
    def getPackageName(self):
        return self.getPackageNameInfoArray()[0]
    def getPackageVersion(self):
        return self.getPackageNameInfoArray()[1]
    def getPackageDate(self):
        return self.getPackageNameInfoArray()[2]
    def getPackagePlatform(self):
        return self.getPackageNameInfoArray()[3]

    def getCacheFilename(self):
        return self.name + "." + self.archive
    def getDatabseFilename(self):
        return self.name + XMLEXT

    def getCacheFilepath(self):
        return os.path.join(OPMCACHE, self.getCacheFilename())
    def getDatabaseFilepath(self):
        return os.path.join(OPMCACHE, self.getDatabseFilename())

    def getCacheFileChecksum(self):
        return hashlib.md5(open(self.getCacheFilepath(), "rb").read()).hexdigest()
    def equalChecksum(self):
        if self.getCacheFileChecksum() == self.checksum:
            return True
        return False

    def existCacheFile(self):
        return os.path.isfile(self.getCacheFilepath())
    def existDatabaseFile(self):
        return os.path.isfile(self.getDatabaseFilepath())

    def read(self):
        doc = ET.parse(self.getDatabaseFilepath())
        root = doc.getroot()

        #check the root tag.
        if root.tag == TAG_ROOT:
            self.version = root.attrib[ATTR_ROOT_VERSION]

        package = root.find(TAG_PACKAGE)
        checksum_tag  = package.find(TAG_CHECKSUM)
        self.checksum = checksum_tag.text
        self.method   = checksum_tag.attrib[ATTR_CHECKSUM_METHOD]
        self.archive  = package.find(TAG_ARCHIVE).text

    def install(self):
        MAXSTEP = 5
        CURSTEP = 1

        print "[%d/%d] Exist database file..." % (MAXSTEP, CURSTEP)
        if self.existDatabaseFile() == False:
            return False

        CURSTEP += 1
        print "[%d/%d] Read database file..." % (MAXSTEP, CURSTEP)
        try:
            self.read()
        except (ET.ParseError, AttributeError) as e:
            print MESSAGE_ERROR + e
            return False

        CURSTEP += 1
        print "[%d/%d] Check checksum string..." % (MAXSTEP, CURSTEP)
        if self.equalChecksum() == False:
            print MESSAGE_ERROR + "Not equal checksum %s != %s." % (self.getCacheFileChecksum(), self.checksum)
            return False

        CURSTEP += 1
        print "[%d/%d] Exist cache file..." % (MAXSTEP, CURSTEP)
        if self.existCacheFile() == False:
            print MESSAGE_ERROR + "Exist cache file."
            return False

        CURSTEP += 1
        print "[%d/%d] Extract all files..." % (MAXSTEP, CURSTEP)
        cachefile = zipfile.ZipFile(self.getCacheFilepath(), "r")
        #tempdir = tempfile.mkdtemp()
        #cachefile.extractall(tempdir)
        for filename in cachefile.namelist():
            cachefile.extract(filename, OPMHOME)
        cachefile.close()
        return True


class RemoteFileInfo:
    split = []

    def __init__(self):
        pass

    def __init__(self, string):
        self.split = re.split("[\s]+", string)
        pass

    def getPermission(self):
        return self.split[0]
    def getHardlink(self):
        return self.split[1]
    def getUser(self):
        return self.split[2]
    def getGroup(self):
        return self.split[3]
    def getByteSize(self):
        return int(self.split[4])
    def getMonth(self):
        return self.split[5]
    def getDay(self):
        return self.split[6]
    def getTimeOrYear(self):
        return self.split[7]
    def getName(self):
        return self.split[8]

    def isDirectory(self):
        if self.getPermission()[0] == "d":
            return True
        return False


## ------------------
## Remote controller.
## ------------------

def uploadFile(remote_uri, filepath, user, password):
    try:
        command  = [CURL_COMMAND, CURL_INSECURE]
        command += ["-u", user + ":" + password]
        command += ["-T", filepath]
        command += [remote_uri]
        subprocess.check_output(command, shell=True)
        return True
    except subprocess.CalledProcessError as e:
        print MESSAGE_ERROR + " Process error."
    return False

def downloadRemoteFile(remote_uri, filepath, user, password):
    try:
        command  = [CURL_COMMAND, CURL_INSECURE]
        command += ["-u", user + ":" + password]
        command += [remote_uri]
        command += ["-o", filepath]
        subprocess.check_output(command, shell=True)
        return True
    except subprocess.CalledProcessError as e:
        print MESSAGE_ERROR + " Process error."
    return False

def downloadRemoteFileList(remote_uri, user, password):
    try:
        command  = [CURL_COMMAND, CURL_INSECURE]
        #command += [CURL_LIST]
        command += ["-u", user + ":" + password]
        command += [getUriPath(remote_uri)]
        output = subprocess.check_output(command, shell=True)

        SELF_DIR_NAME = "."
        PARENT_DIR_NAME = ".."
        infolist = []

        for cursor in output.strip().split("\n"):
            info = RemoteFileInfo(cursor)
            if info.getName() != SELF_DIR_NAME and info.getName() != PARENT_DIR_NAME:
                infolist += [info]
        return infolist
    except subprocess.CalledProcessError as e:
        print MESSAGE_ERROR + "Process error."
    return []

def downloadRecursiveRemoteFile(remote_uri, directory, user, password):
    print "Download remote file list: " + remote_uri
    list = downloadRemoteFileList(remote_uri, user, password)

    if len(list) == 0:
        print MESSAGE_ERROR + "Empty package list."
        return

    for cursor in list:
        name = cursor.getName()
        source = getUriPath(remote_uri, name)
        destination = os.path.join(directory, name)

        if cursor.isDirectory():
            # exist destination directory.
            if os.path.isdir(destination) == False:
                os.mkdir(destination)
            downloadRecursiveRemoteFile(source, destination, user, password)
        else:
            # exist destination file.
            if os.path.isfile(destination):
                print "Exist: " + destination
            else:
                print "Download: " + source
                downloadRemoteFile(source, destination, user, password)


## -----------------
## Local controller.
## -----------------

def getDatabseNameList(directory = OPMCACHE):
    files = []
    for filename in glob.glob(os.path.join(directory, "*" + XMLEXT)):
        files.append(os.path.basename(filename).replace(XMLEXT, ""))
    return files

def getNewestPackage(list, package_name):
    # check the perfect match.
    for cursor in list:
        if cursor == package_name:
            return cursor

    # check the prefix match.
    package_list = []
    for cursor in list:
        if cursor.find(package_name) == 0:
            package_list += [cursor]

    size = len(package_list)
    if size > 0:
        package_list.sort()
        return package_list[size - 1]
    return ""


## ------------
## entry point.
## ------------

def options_list():
    print "Print cached package list:"
    for cursor in getDatabseNameList():
        print "* " + cursor

def options_install(package_name):
    newest_package_name = getNewestPackage(getDatabseNameList(), package_name)

    if newest_package_name == "":
        print MESSAGE_ERROR + "Unknown package name."
        return

    package = Package(newest_package_name)
    if package.existDatabaseFile() == False:
        print MESSAGE_ERROR + "Not found %s package." % newest_package_name
        return

    print "Install: " + newest_package_name
    if package.install():
        print "Install successful."
    else:
        print MESSAGE_ERROR + "Install false."

def options_update(config, package_name = ""):
    user = config.user
    password = getPassword()

    if package_name == "":
        print "Download all..."
        downloadRecursiveRemoteFile(config.uri(), OPMCACHE, user, password)
    else:
        source = getUriPath(config.uri(), package_name)
        destination = os.path.join(OPMCACHE, package_name)

        if os.path.isfile(destination):
            print "Exist: " + destination
        else:
            print "Download: " + source
            downloadRemoteFile(source, destination, user, password)
    print "Update end."

def options_check(config):
    list = getDatabseNameList()
    total = len(list)
    equals = 0
    not_equals = 0

    if total == 0:
        print MESSAGE_ERROR + "Empty package list."
        return

    print "Check package checksum:"
    for cursor in getDatabseNameList():
        package = Package(cursor)
        print "Check %s..." % package.name

        package.read()

        if package.equalChecksum():
            equals += 1
        else:
            print MESSAGE_WARNING + "Not equals!"
            not_equals += 1
    print "Total(%d): Equals(%d), Not equals(%d)" % (total, equals, not_equals)

def options_upload(config, package_name):
    package = Package(package_name)

    if package.existDatabaseFile() == False:
        print MESSAGE_ERROR + "Not found %s package database." % package_name
        return

    package.read()
    if package.existCacheFile() == False:
        print MESSAGE_ERROR + "Not found %s package cache." % package_name
        return

    if package.equalChecksum() == False:
        print MESSAGE_ERROR + "Not equal checksum."
        return

    remote_cache = getUriPath(config.uri(), package.getCacheFilename())
    remote_database = getUriPath(config.uri(), package.getDatabseFilename())

    password = getPassword()
    local_cache = package.getCacheFilepath()
    local_database = package.getDatabaseFilepath()

    print "Upload %s package cache." % package_name
    uploadFile(remote_cache, local_cache, config.user, password)

    print "Upload %s package database." % package_name
    uploadFile(remote_database, local_database, config.user, password)

def options_remote(config):
    print "Download remote file list..."
    list = downloadRemoteFileList(config.uri(), config.user, getPassword())

    if len(list) == 0:
        print MESSAGE_ERROR + "Empty package list."
        return

    unique_list = []
    for cursor in list:
        if cursor.isDirectory():
            continue

        element_name = re.sub(r"\.xml|\.zip$", "", cursor.getName())
        if not element_name in unique_list:
            unique_list += [element_name]

    unique_list.sort()
    print "Print remote package list:"
    for cursor in unique_list:
        print "* " + cursor

def options_verbose(config):
    print "Print verbose information:"
    print "* remote: " + config.uri()
    print "* user: " + config.user

def initLayout():
    existAndMkdir(OPMHOME)
    existAndMkdir(OPMBIN)
    existAndMkdir(OPMALTER)
    existAndMkdir(OPMCACHE)
    existAndMkdir(OPMAPPS)
    existAndMkdir(OPMBACKUP)
    existAndMkdir(OPMSETUP)
    existAndMkdir(OPMSETUP32)
    existAndMkdir(OPMSETUP64)

def main():
    config = Config()
    config.read(CONFIGFILE)

    if config.checkVersion(VERSION_MIN):
        if OPTIONS.list:
            options_list()
        elif OPTIONS.install != "":
            options_install(OPTIONS.install)
        elif OPTIONS.update:
            options_update(config)
        elif OPTIONS.check:
            options_check(config)
        elif OPTIONS.upload != "":
            options_upload(config, OPTIONS.upload)
        elif OPTIONS.remote:
            options_remote(config)
        elif OPTIONS.verbose:
            options_verbose(config)
        else:
            print MESSAGE_ERROR + "Unknown argument."
    else:
        print MESSAGE_ERROR + "Mismatch version (%s != %s)." % (config.version, VERSION_MIN)

if __name__ == "__main__":
    initLayout()

    if len(sys.argv) >= 2:
        main()
    else:
        print MESSAGE_ERROR + "You need to give some command."
    pass
