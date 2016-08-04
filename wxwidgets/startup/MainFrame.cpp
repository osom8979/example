/**
 * @file   MainFrame.cpp
 * @brief  wxWidgets main frame implementation.
 * @author zer0
 * @date   2016-08-04
 */

#include <wxbarista/MainFrame.hpp>

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_CLOSE(MainFrame::onClose)
EVT_WINDOW_DESTROY(MainFrame::onDestroy)
EVT_KEY_DOWN(MainFrame::onKeyDown)
EVT_MENU(ID_MENU__FILE_OPEN, MainFrame::onMenuFileOpen)
EVT_MENU(ID_MENU__FILE_EXIT, MainFrame::onMenuFileExit)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(wxString const & title) : wxFrame(nullptr, ID_MAINFRAME, title)
{
    createMenu();

    Centre();
    // wxPoint frame_position = GetPosition(); // You should be called Centre() method.

    _aui_manager.SetManagedWindow(this);
    _preview = new PreviewPanel(this, ID_PREVIEWPANEL, wxPoint(0, 0), GetSize());
    _preview->Bind(wxEVT_KEY_DOWN, &MainFrame::onKeyDown, this);
    _aui_manager.AddPane(_preview, wxCENTER, wxT("PreviewPanel"));
}

MainFrame::~MainFrame()
{
    _aui_manager.UnInit();
}

void MainFrame::createMenu()
{
    wxMenuBar * menu = new wxMenuBar();

    wxMenu * fileMenu = new wxMenu();
    fileMenu->Append(ID_MENU__FILE_OPEN, wxT("EMPTY"));
    fileMenu->AppendSeparator();
    fileMenu->Append(ID_MENU__FILE_EXIT, wxT("Exit"));
    menu->Append(fileMenu, wxT("&File"));

    SetMenuBar(menu);
}

// --------------
// EVENT methods.
// --------------

void MainFrame::onClose(wxCloseEvent & event)
{
    if (event.CanVeto() == true) {
        if (showExitMessageBox(this) != wxID_YES) {
            event.Veto();
            return;
        }
    }

    // you may also do:  event.Skip();
    // since the default event handler does call Destroy(), too
    Destroy();
}

void MainFrame::onDestroy(wxWindowDestroyEvent & event)
{
    DestroyChildren();
}

void MainFrame::onKeyDown(wxKeyEvent & event)
{
    switch (event.GetKeyCode()) {
    case WXK_ESCAPE:
        Close(true);
        break;
    }
}

// ------------
// MENU events.
// ------------

void MainFrame::onMenuFileOpen(wxCommandEvent & event)
{
}

void MainFrame::onMenuFileExit(wxCommandEvent & event)
{
    if (showExitMessageBox(this) == wxID_YES) {
        Close(true);
    }
}

int MainFrame::showExitMessageBox(wxWindow * parent)
{
    wxMessageDialog dialog(parent, wxT("Are you sure to quit?"), wxT("Question"), (wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION));
    int result = dialog.ShowModal();
    dialog.Destroy();
    return result;
}

