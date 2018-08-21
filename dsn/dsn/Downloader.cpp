/**
 * @file   Downloader.cpp
 * @brief  Downloader class implementation.
 * @author zer0
 * @date   2017-04-02
 */

#include <dsn/Downloader.hpp>
#include <dsn/MainFrame.hpp>

#include <wx/url.h>
#include <wx/wfstream.h>
#include <wx/sstream.h>
#include <wx/zipstrm.h>
#include <wx/filesys.h>
#include <wx/filefn.h>
#include <wx/thread.h>
#include <wx/filename.h>
#include <wx/protocol/http.h>

#include <memory.h>
#include <cassert>

#define HTTP_CONTENT_TYPE_HEADER "Content-type"
#define HTTP_CONTENT_TYPE_DATA   "text/html; charset=utf-8"

#define HTTP_REQUEST_TIMEOUT 5
#define HTTP_CONNECT_RETRY   3

Downloader::Downloader(MainFrame * parent,
                       int id,
                       wxString const & host,
                       wxString const & path,
                       int port)
        : _parent(parent), _id(id), _host(host), _path(path), _port(port)
{
    wxSocketBase::Initialize();

    _http = new wxHTTP();
    //_http->SetHeader(wxT(HTTP_CONTENT_TYPE_HEADER), wxT(HTTP_CONTENT_TYPE_DATA));
    _http->SetTimeout(HTTP_REQUEST_TIMEOUT);
}

Downloader::~Downloader()
{
    wxDELETE(_http);
}

void Downloader::postProgressSet(wxString const & msg)
{
    assert(_parent != nullptr);
    _parent->postProgressSet(_id, msg);
}

void Downloader::postProgressRun(wxString const & msg, int progress)
{
    assert(_parent != nullptr);
    _parent->postProgressRun(_id, msg, progress);
}

void Downloader::postProgressEnd(wxString const & msg)
{
    assert(_parent != nullptr);
    _parent->postProgressEnd(_id, msg);
}

void Downloader::postProgressErr(wxString const & msg, int error)
{
    assert(_parent != nullptr);
    _parent->postProgressErr(_id, msg, error);
}

void * Downloader::Entry()
{
    if (wxApp::IsMainLoopRunning()) {
        // Don't use the main loop.
        return ExitCode(-1);
    }

    return nullptr;
}

bool Downloader::connect()
{
    for (int count = 0; count < HTTP_CONNECT_RETRY; count++) {
        if (_http->Connect(_host, _port)) {
            wxProtocolError error = _http->GetError();
            if (error == wxPROTO_NOERR) {
                return true;
            } else {
                //wxLogMessage(wxT("HTTP Error: %d"), error);
                return false;
            }
        }
    }
    return false;
}

bool Downloader::request(wxOutputStream & output)
{
    assert(_http != nullptr);

    wxInputStream * input = _http->GetInputStream(_path);
    if (input == nullptr) {
        return false;
    }

    input->Read(output);
    wxDELETE(input);

    return true;
}

