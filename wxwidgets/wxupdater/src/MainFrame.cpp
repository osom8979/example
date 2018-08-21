/// @file MainFrame.cpp
/// @brief MainFrame class implementation.
/// @author zer0
/// @date 2014-09-04

#include "MainFrame.h"
#include "UpdateThread.h"

#include <wx/socket.h>

#define DEFAULT_WINDOW_STYLE \
    wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)

#define CANCEL_MESSAGEBOX_TITLE "Question"
#define CANCEL_MESSAGEBOX_MESSAGE "Are you sure to quit?"

#define LABEL_CHECK_VERSION "Check version file ..."
#define LABEL_DOWNLOAD_FILE "Download file ..."
#define LABEL_EXTRACT_FILE "Extract file ..."
#define LABEL_MOVE_FILE "Move files ..."

#define SCREEN_SIZE_WIDTH  350
#define SCREEN_SIZE_HEIGHT 150

#define ID_PANEL 100
#define ID_STATIC_TEXT 101
#define ID_PROGRESSBAR 102
#define ID_BUTTON_CANCEL 103

const int kDefaultBorderSize = 10;
const int kMaxProgressValue = 100;

// -----------------------
// MainFrame Custom EVENT.
// -----------------------

wxDEFINE_EVENT(EVENT_DOWNLOADING, wxCommandEvent);
wxDEFINE_EVENT(EVENT_CHECK_VERSION_COMPLETED, wxCommandEvent);
wxDEFINE_EVENT(EVENT_DOWNLOAD_COMPLETED, wxCommandEvent);
wxDEFINE_EVENT(EVENT_UNZIP_COMPLETED, wxCommandEvent);
wxDEFINE_EVENT(EVENT_RENAME_COMPLETED, wxCommandEvent);
wxDEFINE_EVENT(EVENT_ERROR, wxCommandEvent);

// ----------------------
// MainFrame EVENT Table.
// ----------------------

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
//{
    EVT_CLOSE(MainFrame::onClose)
    EVT_BUTTON(ID_BUTTON_CANCEL, MainFrame::onClickCancel)

    EVT_COMMAND(wxID_ANY, EVENT_DOWNLOADING, MainFrame::onDownloading)
    EVT_COMMAND(wxID_ANY, EVENT_CHECK_VERSION_COMPLETED, MainFrame::onCheckVersionCompleted)
    EVT_COMMAND(wxID_ANY, EVENT_DOWNLOAD_COMPLETED, MainFrame::onDownloadCompleted)
    EVT_COMMAND(wxID_ANY, EVENT_UNZIP_COMPLETED, MainFrame::onUnzipCompleted)
    EVT_COMMAND(wxID_ANY, EVENT_RENAME_COMPLETED, MainFrame::onRenameCompleted)
    EVT_COMMAND(wxID_ANY, EVENT_ERROR, MainFrame::onError)
//}
wxEND_EVENT_TABLE()

// -------------------------
// MainFrame implementation.
// -------------------------

MainFrame::MainFrame(const wxString& title) :
        wxFrame(NULL, wxID_ANY, title, wxDefaultPosition,
                wxSize(SCREEN_SIZE_WIDTH, SCREEN_SIZE_HEIGHT), DEFAULT_WINDOW_STYLE)
{
    // ---------------
    // Create Widgets.
    _main_panel = new wxPanel(this, ID_PANEL);
    _main_vbox = new wxBoxSizer(wxVERTICAL);

    _label = new wxStaticText(_main_panel, ID_STATIC_TEXT, wxT(LABEL_CHECK_VERSION));
    _main_vbox->Add(_label, 0, wxALL | wxALIGN_LEFT, kDefaultBorderSize);

    _progress = new wxGauge(_main_panel, ID_PROGRESSBAR, kMaxProgressValue, wxDefaultPosition,
            wxDefaultSize,
            wxGA_SMOOTH, wxDefaultValidator, wxT(""));
    _main_vbox->Add(_progress, 0, wxALL | wxEXPAND, kDefaultBorderSize);

    _cancel = new wxButton(_main_panel, ID_BUTTON_CANCEL, wxT("Cancel"));
    _cancel->Disable();
    _main_vbox->Add(_cancel, 0, wxALL | wxALIGN_RIGHT, kDefaultBorderSize);

    _main_panel->SetSizer(_main_vbox);

    this->Centre();

    // ---------------
    // Start Download.
    UpdateThread * update_thread = new UpdateThread(this);
    update_thread->Create();
    update_thread->Run();
}

MainFrame::~MainFrame()
{
}

// --------------
// EVENT methods.
// --------------

void MainFrame::onClickCancel(wxCommandEvent& event)
{
    if (showExitMessageBox() == wxID_YES) {
        Close(true);
    }
}

void MainFrame::onClose(wxCloseEvent& event)
{
    if (event.CanVeto() == true) {
        if (showExitMessageBox() != wxID_YES) {
            event.Veto();
            return;
        }
    }

    // you may also do:  event.Skip();
    // since the default event handler does call Destroy(), too
    Destroy();
}

// -------------
// RECV MESSAGE.
// -------------

void MainFrame::onDownloading(wxCommandEvent & event)
{
    _progress->SetValue(event.GetInt());
}
void MainFrame::onCheckVersionCompleted(wxCommandEvent & event)
{
    _label->SetLabel(wxT(LABEL_DOWNLOAD_FILE));
}
void MainFrame::onDownloadCompleted(wxCommandEvent & event)
{
    _label->SetLabel(wxT(LABEL_EXTRACT_FILE));
}
void MainFrame::onUnzipCompleted(wxCommandEvent & event)
{
    _label->SetLabel(wxT(LABEL_MOVE_FILE));
}
void MainFrame::onRenameCompleted(wxCommandEvent & event)
{
    const wxString kCaption = wxT("Message");
    const wxString kMessage = wxT("Update complete !");
    wxMessageBox(kMessage, kCaption);
    Close(true);
}
void MainFrame::onError(wxCommandEvent & event)
{
    const wxString kCaption = wxT("Abort");
    wxMessageBox(event.GetString(), kCaption, wxCLOSE | wxICON_WARNING);
    Close(true);
}

// -------------
// SEND MESSAGE.
// -------------

void MainFrame::sendDownloadingEvent(const int progress)
{
    wxCommandEvent event(EVENT_DOWNLOADING);
    event.SetInt(progress);
    wxPostEvent(this, event);
}
void MainFrame::sendCheckVersionEvent()
{
    wxCommandEvent event(EVENT_CHECK_VERSION_COMPLETED);
    wxPostEvent(this, event);
}
void MainFrame::sendDownloadCompleted()
{
    wxCommandEvent event(EVENT_DOWNLOAD_COMPLETED);
    wxPostEvent(this, event);
}
void MainFrame::sendUnzipCompleted()
{
    wxCommandEvent event(EVENT_UNZIP_COMPLETED);
    wxPostEvent(this, event);
}
void MainFrame::sendRenameCompleted()
{
    wxCommandEvent event(EVENT_RENAME_COMPLETED);
    wxPostEvent(this, event);
}

void MainFrame::sendError(const wxString & msg)
{
    wxCommandEvent event(EVENT_ERROR);
    event.SetString(msg);
    wxPostEvent(this, event);
}

// ---------------
// Static methods.
// ---------------

int MainFrame::showExitMessageBox()
{
    wxMessageDialog * dialog = new wxMessageDialog(NULL, wxT(CANCEL_MESSAGEBOX_MESSAGE),
            wxT(CANCEL_MESSAGEBOX_TITLE),
            wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
    int result = dialog->ShowModal();
    dialog->Destroy();

    return result;
}
