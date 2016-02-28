#include "tutorial.h"

class MyFrameT12: public wxFrame
{
public:
    MyFrameT12(const wxString& title);

    void OnQuit(wxCommandEvent& event);

    wxMenuBar *menubar;
    wxMenu *file;

};

MyFrameT12::MyFrameT12(const wxString& title) :
        wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(280, 180))
{

    menubar = new wxMenuBar;
    file = new wxMenu;
    file->Append(wxID_EXIT, wxT("&Quit"));
    menubar->Append(file, wxT("&File"));
    SetMenuBar(menubar);

    Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyFrameT12::OnQuit));
    Centre();

}

void MyFrameT12::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

int t12()
{
    MyFrameT12 * frame = new MyFrameT12(wxT("MyFrame"));
    frame->Show(true);
    return EXIT_SUCCESS;
}
