#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""! Project tools script.
@author zer0
@date   2015-12-19
"""

import os
import sys
import __main__ as main
from optparse import OptionParser

SCRIPT_PATH = os.path.abspath(main.__file__)
RUNTIME_DIR = os.path.dirname(SCRIPT_PATH)

TEMPLATE_DIR = os.path.join(RUNTIME_DIR, 'template')

PROJECT_CMAKE_NAME = 'project.cmake'
TEMPLATE_EXTENSION = '.in'
PROJECT_CMAKE_TEMPLATE_NAME = PROJECT_CMAKE_NAME + TEMPLATE_EXTENSION
PROJECT_CMAKE_TEMPLATE = os.path.join(TEMPLATE_DIR, PROJECT_CMAKE_TEMPLATE_NAME)
CONFIG_CMAKE_PATH = 'config.cmake'
CONFIG_COMMAND = 'cmake -L -P' + CONFIG_CMAKE_PATH

CMD_MAP = {
    'help' : 'show this help message and exit.',
}

CMD_MESSAGE = "\nCommand list:\n"
for key, val in CMD_MAP.iteritems():
    CMD_MESSAGE += '{}\t{}\n'.format(key, val)

def printCommandHelp():
    print CMD_MESSAGE

def getEnvironmentPath():
    return SCRIPT_PATH

def parseArguments(argv, print_help=True):
    ENABLE_DEFAULT_HELP_MESSAGE = False
    command = None

    if len(argv) >= 2:
        arg1 = argv[1]
        for cursor in CMD_MAP.keys():
            if cursor == arg1:
                command = cursor
                argv.pop(1)
                break

    parser = OptionParser(add_help_option=ENABLE_DEFAULT_HELP_MESSAGE)
    parser.set_usage('Usage: ' + sys.argv[0] + ' {COMMAND} {OPTIONS}')
    parser.add_option('-c', '--config-path',
                      dest='config_path',
                      metavar='PATH',
                      default=CONFIG_CMAKE_PATH,
                      help='Set the configure file path.')
    options, args = parser.parse_args(argv)

    if print_help is True and (command is None or command == CMD_HELP):
        parser.print_help()
        printCommandHelp()

    return command, options

# ------------
# ENTRY-POINT.
# ------------

def main():
    command, options = parseArguments(sys.argv)
    if command is None or command == ENV.CMD_HELP:
        return
    eval('main_{}(options)'.format(command))

if __name__ == '__main__':
    main()