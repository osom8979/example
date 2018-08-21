/// @file UpdateThread.h
/// @brief Update wxThread class.
/// @author zer0
/// @date 2014-09-04

#ifndef __INCLUDE_WXUPDATE__UPDATETHREAD_H__
#define __INCLUDE_WXUPDATE__UPDATETHREAD_H__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <wx/wx.h>

class MainFrame;
class UpdateConfig;
class wxHTTP;

class UpdateThread: public wxThread
{
public:
    UpdateThread(MainFrame * parent);
    virtual ~UpdateThread();

private:
    bool initHttp();

// no-access.
private:
    UpdateThread(const UpdateThread & obj);
    void operator =(const UpdateThread & obj);

private:
    MainFrame * _parent;
    UpdateConfig * _config;
    wxHTTP * _http;
    bool _http_ready;

protected:
    virtual void * Entry();

private:
    wxString version();
    bool download(const wxString & filename, const wxString & path);
    bool unzip(const wxString & zip_path, const wxString & target_dir);
    bool rename(const wxString & source, const wxString & destination);

private:
    bool request(const wxString & path, wxOutputStream & output);
};

#endif // __INCLUDE_WXUPDATE__UPDATETHREAD_H__
