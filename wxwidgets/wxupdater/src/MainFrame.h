/// @file MainFrame.h
/// @brief MainFrame class.
/// @author zer0
/// @date 2014-09-04

#ifndef __INCLUDE_WXUPDATE__MAINFRAME_H__
#define __INCLUDE_WXUPDATE__MAINFRAME_H__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <wx/wx.h>

class MainFrame: public wxFrame
{
private:
    wxPanel * _main_panel;
    wxBoxSizer * _main_vbox;

    wxStaticText * _label;
    wxGauge * _progress;
    wxButton * _cancel;

public:
    MainFrame(const wxString& title);
    virtual ~MainFrame();

protected:
    wxDECLARE_EVENT_TABLE();

protected:
    void onClose(wxCloseEvent & event);
    void onClickCancel(wxCommandEvent & event);

    void onDownloading(wxCommandEvent & event);
    void onCheckVersionCompleted(wxCommandEvent & event);
    void onDownloadCompleted(wxCommandEvent & event);
    void onUnzipCompleted(wxCommandEvent & event);
    void onRenameCompleted(wxCommandEvent & event);
    void onError(wxCommandEvent & event);

public:
    void sendDownloadingEvent(const int progress);
    void sendCheckVersionEvent();
    void sendDownloadCompleted();
    void sendUnzipCompleted();
    void sendRenameCompleted();
    void sendError(const wxString & msg);

public:
    static int showExitMessageBox();
};

#endif // __INCLUDE_WXUPDATE__MAINFRAME_H__
