#!/usr/bin/env python
# -*- coding: utf-8 -*-

from .common import *
from .cache import *
import os
import sys
import json
import pprint

CMAKE_KEY = 'cmake'
DEBUG_KEY = 'debug'
SCRIPT_KEY = 'script'

class Project:
    """
    opm-vim project class.
    """

    json_path = str()
    json_data = dict()
    prefix = 'build-'
    setting_dir = '.opvim'
    cache_name = 'opvim.cache.json'
    cache = object()

    def __init__(self, json_path, **kwargs):
        self.json_path = json_path
        self.json_data = loadJsonData(json_path)

        if 'prefix' in kwargs:
            self.prefix = kwargs['prefix']
        if 'setting_dir' in kwargs:
            self.setting_dir = kwargs['setting_dir']
        if 'cache_name' in kwargs:
            self.cache_name = kwargs['cache_name']

        exists_json = self.json_data is not None
        if exists_json and not os.path.exists(self.setting_dir):
            os.mkdir(self.setting_dir)

        cache_path = os.path.join(self.setting_dir, self.cache_name)
        self.cache = Cache(cache_path, auto_create=exists_json)

        ## After initialization.
        if not self.getMode():
            self.setAutoMode()

    def getMode(self):
        return self.cache.getMode()

    def setMode(self, mode):
        self.cache.setMode(mode)
        self.cache.save()

    def setAutoMode(self):
        modes = self.getModes()
        if modes:
            self.setMode(modes[0])

    def __str__(self):
        return '{}:{}'.format(self.json_path, self.getMode())

    def exists(self):
        """ Exists json dictionary? """
        return self.json_data is not None

    def getRoot(self):
        """ Project root directory. """
        return os.path.abspath(os.path.join(self.json_path, os.pardir))

    def getDictionary(self):
        """ Total json dictionary. """
        return self.json_data

    ## -----------------------
    ## JSON First Depth Nodes.
    ## -----------------------

    def getModes(self):
        """ cmake keys. """
        if self.exists() and CMAKE_KEY in self.json_data:
            return list(self.json_data[CMAKE_KEY].keys())
        return list()

    def getDebugKeys(self):
        """ debug keys. """
        if self.exists() and DEBUG_KEY in self.json_data:
            return list(self.json_data[DEBUG_KEY].keys())
        return list()

    def getScriptKeys(self):
        """ script keys. """
        if self.exists() and SCRIPT_KEY in self.json_data:
            return list(self.json_data[SCRIPT_KEY].keys())
        return list()

    ## -----------------
    ## Key & Mode query.
    ## -----------------

    def existsMode(self, mode):
        return mode in self.getModes()

    def existsCurrentMode(self):
        return self.existsMode(self.getMode())

    def existsDebugKey(self, key):
        return key in self.getDebugKeys()

    def existsScriptKey(self, key):
        return key in self.getScriptKeys()

    ## ----------------
    ## CMake directory.
    ## ----------------

    def getCMakeDictionary(self, mode):
        if self.exists() and mode in self.getModes():
            return self.json_data[CMAKE_KEY][mode]
        return dict()

    def getDefaultCMakeDirectory(self, mode):
        return self.prefix + mode

    def getCMakeDirectory(self, mode):
        mode_data = self.getCMakeDictionary(mode)
        if mode_data and 'dir' in mode_data:
            return mode_data['dir'].strip()
        return self.getDefaultCMakeDirectory(mode).strip()

    def getCurrentCMakeDirectory(self):
        return self.getCMakeDirectory(self.getMode())

    ## ------------
    ## CMake flags.
    ## ------------

    def getDefaultCMakeBuildType(self, mode):
        lower_mode = mode.lower()
        if lower_mode == 'debug':
            return 'Debug'
        if lower_mode == 'release':
            return 'Release'
        if lower_mode == 'release':
            return 'RelWithDebInfo'
        if lower_mode == 'minsizerel':
            return 'MinSizeRel'
        return str()

    def getDefaultCMakeFlags(self, mode):
        result = str()
        result += ' -DCMAKE_EXPORT_COMPILE_COMMANDS=ON'
        result += ' -DCMAKE_BUILD_TYPE={}'.format(self.getDefaultCMakeBuildType(mode))
        return result

    def getCMakeFlags(self, mode):
        mode_data = self.getCMakeDictionary(mode)
        if mode_data and 'flags' in mode_data:
            return mode_data['flags'].strip()
        return self.getDefaultCMakeFlags(mode).strip()

    def getCurrentCMakeFlags(self):
        return self.getCMakeFlags(self.getMode())

    def getBuildFlags(self, mode):
        mode_data = self.getCMakeDictionary(mode)
        if mode_data and 'build' in mode_data:
            return mode_data['build'].strip()
        return str()

    def getCurrentBuildFlags(self):
        return self.getBuildFlags(self.getMode())

    ## -----------------
    ## Debug operations.
    ## -----------------

    def getDebugDictionary(self, key):
        if self.exists() and key in self.getDebugKeys():
            return self.json_data[DEBUG_KEY][key]
        return dict()

    def getDebugCwd(self, key):
        debug_data = self.getDebugDictionary(key)
        if debug_data and 'cwd' in debug_data:
            return debug_data['cwd'].strip()
        return self.getRoot()

    def getDebugType(self, key):
        debug_data = self.getDebugDictionary(key)
        if debug_data and 'type' in debug_data:
            checked_type = getCheckedDebugType(debug_data['type'].strip())
            if checked_type:
                return checked_type
        return str()

    def getDebugCmds(self, key):
        debug_data = self.getDebugDictionary(key)
        if debug_data and 'cmds' in debug_data:
            return debug_data['cmds'].strip()
        return str()

    ## ------------------
    ## Script operations.
    ## ------------------

    def getScriptDictionary(self, key):
        if self.exists() and key in self.getScriptKeys():
            return self.json_data[SCRIPT_KEY][key]
        return dict()

    def getScriptCwd(self, key):
        script_data = self.getScriptDictionary(key)
        if script_data and 'cwd' in script_data:
            return script_data['cwd'].strip()
        return self.getRoot()

    def getScriptCmds(self, key):
        script_data = self.getScriptDictionary(key)
        if script_data and 'cmds' in script_data:
            return script_data['cmds'].strip()
        return str()

    ## ----------------------
    ## Preview this instance.
    ## ----------------------

    def preview(self, show_error=True):
        print('Project root: {}'.format(self.getRoot()))
        print('OPM-VIM json: {}'.format(self.json_path))
        print('Cache json: {}'.format(self.cache.json_path))
        print('Current mode: {}'.format(self.getMode()))

        if not self.exists():
            print('-- Not found project --')
            return

        for mode in self.getModes():
            print('[MODE:{}]:'.format(mode))
            print(' - dir: {}'.format(self.getCMakeDirectory(mode)))
            print(' - flags: {}'.format(self.getCMakeFlags(mode)))
            print(' - build: {}'.format(self.getBuildFlags(mode)))

        for debug_key in self.getDebugKeys():
            is_success = True
            debug_cwd = self.getDebugCwd(debug_key)
            if not os.path.isdir(debug_cwd):
                debug_cwd = '-- ERROR --'
                is_success = False
            debug_type = self.getDebugType(debug_key)
            if not debug_type:
                debug_type = '-- ERROR --'
                is_success = False
            debug_cmds = self.getDebugCmds(debug_key)
            if not debug_cmds:
                debug_cmds = '-- ERROR --'
                is_success = False
            if show_error or is_success:
                print('[DEBUG:{}]:'.format(debug_key))
                print(' - cwd: {}'.format(debug_cwd))
                print(' - type: {}'.format(debug_type))
                print(' - cmds: {}'.format(debug_cmds))

        for script_key in self.getScriptKeys():
            is_success = True
            script_cwd = self.getScriptCwd(script_key)
            if not os.path.isdir(script_cwd):
                script_cwd = '-- ERROR --'
                is_success = False
            script_cmds = self.getScriptCmds(script_key)
            if not script_cmds:
                script_cmds = '-- ERROR --'
                is_success = False
            if show_error or is_success:
                print('[SCRIPT:{}]:'.format(script_key))
                print(' - cwd: {}'.format(script_cwd))
                print(' - cmds: {}'.format(script_cmds))

        #pprint.pprint(self.json_data)
        pass


## -----
## Utils
## -----

def getDefaultProject(force_reload=False):
    global project
    try:
        project
    except NameError:
        force_reload = True

    if force_reload:
        project = Project(getDefaultProjectJsonPath(),
                          prefix=getDefaultBuildPrefix(),
                          setting_dir=getProjectSettingDir(),
                          cache_name=getProjectCacheJsonName())
    return project

def previewDefaultProject(show_error=True):
    getDefaultProject().preview(show_error)

def getFirstMode():
    modes = getDefaultProject().getModes()
    if modes:
        return modes[0]
    else:
        return None

