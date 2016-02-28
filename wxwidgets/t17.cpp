#include "tutorial.h"

// wxBoxSizer
// wxStaticBoxSizer
// wxGridSizer
// wxFlexGridSizer
// wxGridBagSizer

class MyFrameT17: public wxFrame
{
public:
    MyFrameT17(const wxString& title);

    wxMenuBar *menubar;
    wxMenu *file;
    wxMenu *edit;
    wxMenu *help;
    wxTextCtrl *textctrl;
};

MyFrameT17::MyFrameT17(const wxString& title) :
        wxFrame(NULL, -1, title, wxPoint(-1, -1), wxSize(250, 180))
{
    menubar = new wxMenuBar;
    file = new wxMenu;
    edit = new wxMenu;
    help = new wxMenu;

    menubar->Append(file, wxT("&File"));
    menubar->Append(edit, wxT("&Edit"));
    menubar->Append(help, wxT("&Help"));
    SetMenuBar(menubar);

    textctrl = new wxTextCtrl(this, -1, wxT(""), wxPoint(-1, -1), wxSize(250, 150));

    Centre();
}

int t17()
{
    MyFrameT17 * frame = new MyFrameT17(wxT("MyFrame"));
    frame->Show(true);
    return EXIT_SUCCESS;
}

