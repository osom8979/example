/// @file UpdateConfig.cpp
/// @brief INI Configure helper class implementation.
/// @author zer0
/// @date 2014-09-04

#include "UpdateConfig.h"
#include "log.h"

#include <wx/fileconf.h>

#define CONFIG_NAME "config.ini"

#define CONFIG_SEPARATOR "/"

#define CONFIG_KEY_GLOBAL_GROUP "global"
#define CONFIG_KEY_ICON "icon"
#define CONFIG_KEY_HOST "host"
#define CONFIG_KEY_PORT "port"
#define CONFIG_KEY_VERSION_PREFIX "version_prefix"
#define CONFIG_KEY_VERSION_NAME "version_name"
#define CONFIG_KEY_DOWNLOAD_PREFIX "download_prefix"
#define CONFIG_KEY_DOWNLOAD_MD5_SUFFIX "download_md5_suffix"
#define CONFIG_KEY_DESTINATION_PATH "destination_path"
#define CONFIG_KEY_REVISION "revision"

#define CONFIG_ICON "icon.ico"
#define CONFIG_PROTOCOL "http"
#define CONFIG_HOST ""
#define CONFIG_PORT ""
#define CONFIG_VERSION_PREFIX "/"
#define CONFIG_VERSION_NAME "version"
#define CONFIG_DOWNLOAD_PREFIX "/"
#define CONFIG_DOWNLOAD_MD5_SUFFIX ".md5"
#define CONFIG_DESTINATION_PATH ""
#define CONFIG_REVISION "0"

UpdateConfig::UpdateConfig()
{
    _path = wxT(CONFIG_NAME);
    setDefault();
}

UpdateConfig::UpdateConfig(wxString & config_path)
{
    _path = config_path;
    setDefault();
}

UpdateConfig::~UpdateConfig()
{
}

void UpdateConfig::setDefault()
{
    _icon_name = wxT(CONFIG_ICON);
    _host = wxT(CONFIG_HOST);
    _port = wxT(CONFIG_PORT);
    _version_prefix = wxT(CONFIG_VERSION_PREFIX);
    _version_name = wxT(CONFIG_VERSION_NAME);
    _download_prefix = wxT(CONFIG_DOWNLOAD_PREFIX);
    _download_md5_suffix = wxT(CONFIG_DOWNLOAD_MD5_SUFFIX);
    _destination_path = wxT(CONFIG_DESTINATION_PATH);
    _revision = wxT(CONFIG_REVISION);
}

wxString UpdateConfig::getIconName() const
{
    return _icon_name;
}
wxString UpdateConfig::getHost() const
{
    return _host;
}
wxString UpdateConfig::getPort() const
{
    return _port;
}
wxString UpdateConfig::getVersionPrefix() const
{
    return _version_prefix;
}
wxString UpdateConfig::getVersionName() const
{
    return _version_name;
}
wxString UpdateConfig::getDownloadPrefix() const
{
    return _download_prefix;
}
wxString UpdateConfig::getDownloadMd5Suffix() const
{
    return _download_md5_suffix;
}
wxString UpdateConfig::getDestinationPath() const
{
    return _destination_path;
}
wxString UpdateConfig::getRevision() const
{
    return _revision;
}

void UpdateConfig::setIconName(wxString & value)
{
    _icon_name = value;
}
void UpdateConfig::setHost(wxString & value)
{
    _host = value;
}
void UpdateConfig::setPort(wxString & value)
{
    _port = value;
}
void UpdateConfig::setVersionPrefix(wxString & value)
{
    _version_prefix = value;
}
void UpdateConfig::setVersionName(wxString & value)
{
    _version_name = value;
}
void UpdateConfig::setDownloadPrefix(wxString & value)
{
    _download_prefix = value;
}
void UpdateConfig::setDownloadMd5Suffix(wxString & value)
{
    _download_md5_suffix = value;
}
void UpdateConfig::setDestinationPath(wxString & value)
{
    _destination_path = value;
}
void UpdateConfig::setRevision(wxString & value)
{
    _revision = value;
}

bool UpdateConfig::read()
{
    bool result = false;
    wxFileConfig * config = new (std::nothrow) wxFileConfig(wxEmptyString, wxEmptyString, _path,
            wxEmptyString, wxCONFIG_USE_RELATIVE_PATH);

    if (config == NULL) {
        return false;
    }

    int group_count = config->GetNumberOfGroups();
    int entry_count = config->GetNumberOfEntries();

    if (group_count != 0 || entry_count != 0) {
        result = true;
        result &= config->Read(getGlobalPath(CONFIG_KEY_ICON), &_icon_name);
        result &= config->Read(getGlobalPath(CONFIG_KEY_HOST), &_host);
        result &= config->Read(getGlobalPath(CONFIG_KEY_PORT), &_port);
        result &= config->Read(getGlobalPath(CONFIG_KEY_VERSION_PREFIX), &_version_prefix);
        result &= config->Read(getGlobalPath(CONFIG_KEY_VERSION_NAME), &_version_name);
        result &= config->Read(getGlobalPath(CONFIG_KEY_DOWNLOAD_PREFIX), &_download_prefix);
        result &= config->Read(getGlobalPath(CONFIG_KEY_DOWNLOAD_MD5_SUFFIX),
                &_download_md5_suffix);
        result &= config->Read(getGlobalPath(CONFIG_KEY_DESTINATION_PATH), &_destination_path);
        result &= config->Read(getGlobalPath(CONFIG_KEY_REVISION), &_revision);
    }
    delete config;
    return result;
}

bool UpdateConfig::save()
{
    bool result = false;
    wxFileConfig * config = new (std::nothrow) wxFileConfig(wxEmptyString, wxEmptyString, _path,
            wxEmptyString, wxCONFIG_USE_RELATIVE_PATH);

    if (config != NULL) {
        result = true;
        result &= config->Write(getGlobalPath(CONFIG_KEY_ICON), _icon_name);
        result &= config->Write(getGlobalPath(CONFIG_KEY_HOST), _host);
        result &= config->Write(getGlobalPath(CONFIG_KEY_PORT), _port);
        result &= config->Write(getGlobalPath(CONFIG_KEY_VERSION_PREFIX), _version_prefix);
        result &= config->Write(getGlobalPath(CONFIG_KEY_VERSION_NAME), _version_name);
        result &= config->Write(getGlobalPath(CONFIG_KEY_DOWNLOAD_PREFIX), _download_prefix);
        result &= config->Write(getGlobalPath(CONFIG_KEY_DOWNLOAD_MD5_SUFFIX),
                _download_md5_suffix);
        result &= config->Write(getGlobalPath(CONFIG_KEY_DESTINATION_PATH), _destination_path);
        result &= config->Write(getGlobalPath(CONFIG_KEY_REVISION), _revision);
        result &= config->Flush();
        delete config;
    }
    return result;
}

void UpdateConfig::readOrSave()
{
    if (read() == false) {
        save();
    }
}

void UpdateConfig::printLog()
{
    _LOG(getGlobalPath(CONFIG_KEY_ICON) + wxT(": ") + _icon_name);
    _LOG(getGlobalPath(CONFIG_KEY_HOST) + wxT(": ") + _host);
    _LOG(getGlobalPath(CONFIG_KEY_PORT) + wxT(": ") + _port);
    _LOG(getGlobalPath(CONFIG_KEY_VERSION_PREFIX) + wxT(": ") + _version_prefix);
    _LOG(getGlobalPath(CONFIG_KEY_VERSION_NAME) + wxT(": ") + _version_name);
    _LOG(getGlobalPath(CONFIG_KEY_DOWNLOAD_PREFIX) + wxT(": ") + _download_prefix);
    _LOG(getGlobalPath(CONFIG_KEY_DOWNLOAD_MD5_SUFFIX) + wxT(": ") + _download_md5_suffix);
    _LOG(getGlobalPath(CONFIG_KEY_DESTINATION_PATH) + wxT(": ") + _destination_path);
    _LOG(getGlobalPath(CONFIG_KEY_REVISION) + wxT(": ") + _revision);
}

wxString UpdateConfig::getGlobalPath(const wxString & key)
{
    return UpdateConfig::getPath(wxT(CONFIG_KEY_GLOBAL_GROUP), key);
}

wxString UpdateConfig::getPath(const wxString & group, const wxString & key)
{
    return wxT(CONFIG_SEPARATOR) + group + wxT(CONFIG_SEPARATOR) + key;
}
