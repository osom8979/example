/**
 * @file   Downloader.hpp
 * @brief  Downloader class prototype.
 * @author zer0
 * @date   2017-04-02
 */

#ifndef __INCLUDE_DSN__DOWNLOADER_HPP__
#define __INCLUDE_DSN__DOWNLOADER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <wx/wx.h>

class MainFrame;
class wxHTTP;

/**
 * Downloader class.
 *
 * @author zer0
 * @date   2017-04-02
 */
class Downloader : public wxThread
{
private:
    MainFrame * _parent;
    int _id;
    wxString _host;
    wxString _path;
    int _port;

private:
    wxHTTP * _http;

public:
    Downloader(MainFrame * parent, int id, wxString const & host, wxString const & path, int port = 80);
    virtual ~Downloader();

public:
    void postProgressSet(wxString const & msg);
    void postProgressRun(wxString const & msg, int progress);
    void postProgressEnd(wxString const & msg);
    void postProgressErr(wxString const & msg, int error);

protected:
    virtual void * Entry() wxOVERRIDE;

public:
    bool connect();
    bool request(wxOutputStream & output);
};

#endif // __INCLUDE_DSN__DOWNLOADER_HPP__

