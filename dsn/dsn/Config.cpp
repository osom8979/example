/**
 * @file   Config.cpp
 * @brief  Config class implementation.
 * @author zer0
 * @date   2017-04-01
 */

#include <dsn/Config.hpp>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/regex.h>

#if defined(WIN32)
#include <Windows.h>
#endif

#define CONFIG_NAME "config.ini"

#define CONFIG_SEPARATOR        "/"
#define CONFIG_KEY_GLOBAL_GROUP "global"
#define CONFIG_KEY_APP_GROUP    "app"
#define CONFIG_KEY_VIEW_GROUP   "view"
#define CONFIG_KEY_EXEC_GROUP   "exec"

#define CONFIG_HOMEDIR_VALUE_NAME "HOME_DIR"
#define CONFIG_WORKDIR_VALUE_NAME "WORK_DIR"
#define CONFIG_EXEDIR_VALUE_NAME  "EXE_DIR"

static wxString getExePathWithWin32()
{
#if defined(WIN32)
    char buffer[MAX_PATH] = {0,};
    DWORD const COPIED_LENGTH = GetModuleFileNameA(NULL, &buffer[0], MAX_PATH);
    return wxString::FromAscii(buffer, COPIED_LENGTH);
#else
    return wxT("");
#endif
}

Config::Config()
{
    // EMPTY.
}

Config::Config(wxString const & path)
{
    read(path);
}

Config::~Config()
{
    // EMPTY.
}

void Config::clear()
{
#ifndef _DSN_CONFIG_XX
#define _DSN_CONFIG_XX(type, group, key, def) set_##group##_##key();
#endif
    DSN_CONFIG_PROPERTIES(_DSN_CONFIG_XX);
    DSN_CONFIG_POD_PROPERTIES(_DSN_CONFIG_XX);
#undef _DSN_CONFIG_XX
}

void Config::setDefault()
{
#ifndef _DSN_CONFIG_XX
#define _DSN_CONFIG_XX(type, group, key, def) def_##group##_##key();
#endif
    DSN_CONFIG_PROPERTIES(_DSN_CONFIG_XX);
    DSN_CONFIG_POD_PROPERTIES(_DSN_CONFIG_XX);
#undef _DSN_CONFIG_XX
}

bool Config::read(wxString const & path)
{
    if (wxFileName::Exists(path) == false) {
        return false;
    }

    wxFileConfig config(wxEmptyString, wxEmptyString, path, wxEmptyString);

    std::size_t const GROUP_COUNT = config.GetNumberOfGroups();
    std::size_t const ENTRY_COUNT = config.GetNumberOfEntries();

    if (GROUP_COUNT == 0 && ENTRY_COUNT == 0) {
        return false;
    }

    bool result = true;

#ifndef _DSN_CONFIG_XX
#define _DSN_CONFIG_XX(type, group, key, def) result &= read_##group##_##key(config);
#endif
    DSN_CONFIG_PROPERTIES(_DSN_CONFIG_XX);
    DSN_CONFIG_POD_PROPERTIES(_DSN_CONFIG_XX);
#undef _DSN_CONFIG_XX

    return result;
}

bool Config::save(wxString const & path, bool with_debug)
{
    if (wxFileName::Exists(path)) {
        return false;
    }

    wxFileConfig config(wxEmptyString, wxEmptyString, path, wxEmptyString);
    bool result = true;

#ifndef _DSN_CONFIG_XX
#define _DSN_CONFIG_XX(type, group, key, def) result &= write_##group##_##key(config);
#endif
    DSN_CONFIG_PROPERTIES(_DSN_CONFIG_XX);
    DSN_CONFIG_POD_PROPERTIES(_DSN_CONFIG_XX);
#undef _DSN_CONFIG_XX

    result &= config.Flush();
    return result;
}

bool Config::saveDefault(bool with_debug)
{
    Config config;
    config.clear();
    config.setDefault();
    return config.save(Config::getDefaultPath(), with_debug);
}

bool Config::existsDefault()
{
    return Config::exists(Config::getDefaultPath());
}

wxString Config::getDefaultPath()
{
    wxString const EXE_DIR = getExeDir();
    if (EXE_DIR.empty()) {
        return wxT(CONFIG_NAME);
    }

    wxFileName path(EXE_DIR + wxFileName::GetPathSeparators() + wxT(CONFIG_NAME));
    path.MakeAbsolute();
    return path.GetFullPath();
}

wxString Config::getPath(wxString const & group, wxString const & key)
{
    return wxT(CONFIG_SEPARATOR) + group + wxT(CONFIG_SEPARATOR) + key;
}

int Config::replaceValue(wxString & str, wxString const & name, wxString const & value)
{
    return wxRegEx(wxString(wxT("\\${")) + name + wxString(wxT("}"))).ReplaceAll(&str, value);
}

int Config::replaceHomeDir(wxString & str)
{
    return Config::replaceValue(str, wxT(CONFIG_HOMEDIR_VALUE_NAME), wxFileName::GetHomeDir());
}

int Config::replaceWorkDir(wxString & str)
{
    return Config::replaceValue(str, wxT(CONFIG_WORKDIR_VALUE_NAME), wxFileName::GetCwd());
}

int Config::replaceExeDir(wxString & str)
{
    return Config::replaceValue(str, wxT(CONFIG_EXEDIR_VALUE_NAME), getExeDir());
}

int Config::replaceAll(wxString & str)
{
    return replaceHomeDir(str) + replaceWorkDir(str) + replaceExeDir(str);
}

wxString Config::reg(wxString const & str)
{
    wxString original(str);
    replaceAll(original);
    return original;
}

wxString Config::getExePath()
{
    wxString const EXE_PATH = wxStandardPaths::Get().GetExecutablePath();

    wxFileName path;
    if (EXE_PATH.empty()) {
        path.Assign(getExePathWithWin32(), wxPATH_NATIVE);
    } else {
        path.Assign(EXE_PATH);
    }

    path.MakeAbsolute();
    return path.GetFullPath();
}

wxString Config::getExeDir()
{
    return wxFileName(Config::getExePath()).GetPath(wxPATH_GET_VOLUME);
}

bool Config::exists(wxString const & path)
{
    return wxFileName::Exists(path);
}

void Config::print()
{
    wxLogMessage(wxT("Executable path: %s"), Config::getExePath());
    wxLogMessage(wxT("Config path: %s"), Config::getDefaultPath());

    wxString  exe_message = wxT("Executable dir: ${EXE_DIR}");
    wxString work_message = wxT("Working dir: ${WORK_DIR}");
    wxString home_message = wxT("Home dir: ${HOME_DIR}");

    replaceAll( exe_message);
    replaceAll(work_message);
    replaceAll(home_message);

    wxLogMessage( exe_message);
    wxLogMessage(work_message);
    wxLogMessage(home_message);
}

