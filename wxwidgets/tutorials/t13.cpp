#include "tutorial.h"

class MyFrameT13: public wxFrame
{
public:
    MyFrameT13(const wxString& title);

    void OnQuit(wxCommandEvent & event);

    wxMenuBar *menubar;
    wxMenu *file;
    wxMenu *imp;
    wxMenuItem *quit;

};

MyFrameT13::MyFrameT13(const wxString& title) :
        wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(280, 180))
{
    menubar = new wxMenuBar;
    file = new wxMenu;

    file->Append(wxID_ANY, wxT("&New"));
    file->Append(wxID_ANY, wxT("&Open"));
    file->Append(wxID_ANY, wxT("&Save"));
    file->AppendSeparator();

    imp = new wxMenu;
    imp->Append(wxID_ANY, wxT("Import newsfeed list..."));
    imp->Append(wxID_ANY, wxT("Import bookmarks..."));
    imp->Append(wxID_ANY, wxT("Import mail..."));

    file->AppendSubMenu(imp, wxT("I&mport"));

    quit = new wxMenuItem(file, wxID_EXIT, wxT("&Quit\tCtrl+W"));
    file->Append(quit);

    menubar->Append(file, wxT("&File"));
    SetMenuBar(menubar);

    Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyFrameT13::OnQuit));
    Centre();
}

void MyFrameT13::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

int t13()
{
    MyFrameT13 * frame = new MyFrameT13(wxT("MyFrame"));
    frame->Show(true);
    return EXIT_SUCCESS;
}
