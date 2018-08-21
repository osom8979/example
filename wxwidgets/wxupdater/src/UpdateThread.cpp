/// @file UpdateThread.cpp
/// @brief Update wxThread class implementation.
/// @author zer0
/// @date 2014-09-04

#include "UpdateThread.h"
#include "UpdateConfig.h"
#include "MainFrame.h"
#include "log.h"

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

#define HTTP_CONTENT_TYPE_HEADER "Content-type"
#define HTTP_CONTENT_TYPE_DATA "text/html; charset=utf-8"
#define HTTP_REQUEST_TIMEOUT 10
#define HTTP_CONNECT_RETRY 5
#define HTTP_DEFAULT_PORT 80

UpdateThread::UpdateThread(MainFrame * parent) :
        wxThread(wxTHREAD_DETACHED)
{
    _parent = parent;
    _http_ready = false;

    _config = new (std::nothrow) UpdateConfig();
    _config->readOrSave();
    // _config->printLog();

    initHttp();
}

bool UpdateThread::initHttp()
{
    wxApp::IsMainLoopRunning();
    wxSocketBase::Initialize();

    _http = new (std::nothrow) wxHTTP();
    _http->SetHeader(wxT(HTTP_CONTENT_TYPE_HEADER), wxT(HTTP_CONTENT_TYPE_DATA));
    _http->SetTimeout(HTTP_REQUEST_TIMEOUT);

    wxString host = _config->getHost();
    wxString port = _config->getPort();
    long port_number = HTTP_DEFAULT_PORT;
    port.ToLong(&port_number);

    // Connect...
    for (int count = 0; count < HTTP_CONNECT_RETRY; count++) {
        if (_http->Connect(host, port_number) == true) {
            if (_http->GetError() == wxPROTO_NOERR) {
                _http_ready = true;
            } else {
                _http_ready = false;
            }
            break;
        }
    }

    return _http_ready;
}

UpdateThread::~UpdateThread()
{
    _http->Close();
    delete _http;
    delete _config;
}

void * UpdateThread::Entry()
{
    const int kProgressStepReay = 0;
    const int kProgressStep1 = 1;
    const int kProgressStep2 = 25;
    const int kProgressStep3 = 50;
    const int kProgressStep4 = 75;
    const int kProgressStep5 = 99;
    const int kProgressStep6 = 100;

    const int kExitCodeSuccess = 0;
    const int kExitCodeFalse = 1;

    _parent->sendDownloadingEvent(kProgressStepReay);

    // STEP #01: Check version.
    wxString download_filename = version();
    download_filename.Trim();
    if (download_filename.IsEmpty() == false) {
        _parent->sendDownloadingEvent(kProgressStep1);
        _parent->sendCheckVersionEvent();
    } else {
        _parent->sendError(wxT("Check version false."));
        return (wxThread::ExitCode) kExitCodeFalse;
    }
    _LOG(wxT("Download filename: ") + download_filename);

    // STEP #02: Create TEMP File.
    wxString temp_file_path = wxFileName::CreateTempFileName(wxT("wxup"));
    _LOG(wxT("Temp file path: ") + temp_file_path);
    _parent->sendDownloadingEvent(kProgressStep2);

    // STEP #03: Download ZIP File.
    bool download_result = download(download_filename, temp_file_path);
    _LOG(wxString::Format(wxT("Download result: %d"), download_result));
    if (download_result == true) {
        _parent->sendDownloadingEvent(kProgressStep3);
        _parent->sendDownloadCompleted();
    } else {
        _parent->sendError(wxT("File download false."));
        return (wxThread::ExitCode) kExitCodeFalse;
    }

    // STEP #04: Create TEMP Directory.
    // wxString temp_extract_dir = temp_file_path + wxT("_extract");
    wxString temp_extract_dir = ::wxGetCwd();
    if (wxDirExists(temp_extract_dir) == false) {
        wxFileName::Mkdir(temp_extract_dir);
    }
    _parent->sendDownloadingEvent(kProgressStep4);

    // STEP #05: Extract ZIP File.
    bool unzip_result = unzip(temp_file_path, temp_extract_dir);
    _LOG(wxString::Format(wxT("Extract result: %d"), unzip_result));

    if (unzip_result == true) {
        _parent->sendDownloadingEvent(kProgressStep5);
        _parent->sendUnzipCompleted();
    } else {
        _parent->sendError(wxT("Extract false."));
        return (wxThread::ExitCode) kExitCodeFalse;
    }

    // ENDING!
    _parent->sendDownloadingEvent(kProgressStep6);
    _parent->sendRenameCompleted();
    return (wxThread::ExitCode) kExitCodeSuccess;
}

wxString UpdateThread::version()
{
    wxString result = wxEmptyString;
    wxStringOutputStream output(&result);
    wxString url_suffix = _config->getVersionPrefix() + _config->getVersionName();

    if (request(url_suffix, output) == true) {
        return result;
    }
    return wxEmptyString;
}

bool UpdateThread::download(const wxString & download_filename, const wxString & path)
{
    wxFileOutputStream output(path);
    wxString url_suffix = _config->getDownloadPrefix() + download_filename;

    return request(url_suffix, output);
}

bool UpdateThread::unzip(const wxString & zip_path, const wxString & target_dir)
{
    wxFileInputStream in(zip_path);
    wxZipInputStream zip(in);

    wxZipEntry * entry;

    entry = zip.GetNextEntry();
    if (entry == NULL) {
        return false;
    }

    bool result = true;

    while (entry != NULL) {
        wxString name = entry->GetName();
        wxString target_file_path = target_dir + wxFileName::GetPathSeparator() + name;

        // Directory or File ?
        if (entry->IsDir() == true) {
            int perm = entry->GetMode();
            wxFileName::Mkdir(target_file_path, perm, wxPATH_MKDIR_FULL);
        } else {
            zip.OpenEntry(*entry);
            if (zip.CanRead() == false) {
                _LOG(wxT("Can not read zip entry: ") + name);
                result = false;
                break;
            }

            if (wxFileExists(target_file_path) == true) {
                wxRemoveFile(target_file_path);
            }
            _LOG(target_file_path);

            wxFileOutputStream file(target_file_path);
            zip.Read(file);
        }

        delete entry;
        entry = zip.GetNextEntry();
    }

    return result;
}

bool UpdateThread::rename(const wxString & source, const wxString & destination)
{
    return false;
}

bool UpdateThread::request(const wxString & url_suffix, wxOutputStream & output)
{
    if (_http_ready == false) {
        return false;
    }

    wxInputStream * input = _http->GetInputStream(url_suffix);
    _LOG(wxString::Format(wxT("Request suffix: %s(%p)"), url_suffix, input));

    if (input != NULL) {
        input->Read(output);
        wxDELETE(input);
        return true;
    }
    return false;
}

