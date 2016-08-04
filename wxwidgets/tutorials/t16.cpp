#include "tutorial.h"

class MyFrameT16: public wxFrame
{
public:
    MyFrameT16(const wxString& title);

    wxMenuBar *menubar;
    wxMenu *file;
    wxMenu *edit;
    wxMenu *help;
    wxTextCtrl *textctrl;
};

MyFrameT16::MyFrameT16(const wxString& title) :
        wxFrame(NULL, -1, title, wxPoint(-1, -1), wxSize(250, 180))
{
    wxPanel *panel = new wxPanel(this, -1);

    menubar = new wxMenuBar;
    file = new wxMenu;
    edit = new wxMenu;
    help = new wxMenu;

    menubar->Append(file, wxT("&File"));
    menubar->Append(edit, wxT("&Edit"));
    menubar->Append(help, wxT("&Help"));
    SetMenuBar(menubar);

    textctrl = new wxTextCtrl(panel, -1, wxT(""), wxPoint(-1, -1), wxSize(250, 150));

    Centre();
}

int t16()
{
    MyFrameT16 * frame = new MyFrameT16(wxT("MyFrame"));
    frame->Show(true);
    return EXIT_SUCCESS;
}

