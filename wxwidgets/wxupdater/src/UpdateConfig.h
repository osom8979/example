/// @file UpdateConfig.h
/// @brief INI Configure helper class.
/// @author zer0
/// @date 2014-09-04

#ifndef __INCLUDE_WXUPDATE__UPDATECONFIG_H__
#define __INCLUDE_WXUPDATE__UPDATECONFIG_H__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <wx/string.h>

// -----------
// INI Config.
// -----------

class UpdateConfig
{
public:
    UpdateConfig();
    UpdateConfig(wxString & config_path);
    virtual ~UpdateConfig();

private:
    UpdateConfig(const UpdateConfig & obj);
    void operator =(const UpdateConfig & obj);

private:
    void setDefault();

private:
    wxString _path;
    wxString getPath() const;

private:
    wxString _icon_name;
    wxString _host;
    wxString _port;
    wxString _version_prefix;
    wxString _version_name;
    wxString _download_prefix;
    wxString _download_md5_suffix;
    wxString _destination_path;
    wxString _revision;

public:
    wxString getIconName() const;
    wxString getHost() const;
    wxString getPort() const;
    wxString getVersionPrefix() const;
    wxString getVersionName() const;
    wxString getDownloadPrefix() const;
    wxString getDownloadMd5Suffix() const;
    wxString getDestinationPath() const;
    wxString getRevision() const;

public:
    void setIconName(wxString & value);
    void setHost(wxString & value);
    void setPort(wxString & value);
    void setVersionPrefix(wxString & value);
    void setVersionName(wxString & value);
    void setDownloadPrefix(wxString & value);
    void setDownloadMd5Suffix(wxString & value);
    void setDestinationPath(wxString & value);
    void setRevision(wxString & value);

public:
    bool read();
    bool save();
    void readOrSave();

public:
    void printLog();

public:
    wxString getGlobalPath(const wxString & key);

public:
    static wxString getPath(const wxString & group, const wxString & key);
};

#endif // __INCLUDE_WXUPDATE__UPDATECONFIG_H__
