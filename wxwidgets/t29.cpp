#include "tutorial.h"

class MyFrameT29: public wxFrame
{
public:
    MyFrameT29(const wxString& title);

    void OnOpen(wxCommandEvent& event);

    wxTextCtrl *tc;
};

MyFrameT29::MyFrameT29(const wxString & title) :
        wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(300, 200))
{
    wxMenuBar *menubar = new wxMenuBar;
    wxMenu *file = new wxMenu;
    file->Append(wxID_OPEN, wxT("&Open"));
    menubar->Append(file, wxT("&File"));
    SetMenuBar(menubar);

    Connect(wxID_OPEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyFrameT29::OnOpen));

    tc = new wxTextCtrl(this, -1, wxT(""), wxPoint(-1, -1), wxSize(-1, -1), wxTE_MULTILINE);

    Center();
}

void MyFrameT29::OnOpen(wxCommandEvent& event)
{
    wxFileDialog * openFileDialog = new wxFileDialog(this);

    if (openFileDialog->ShowModal() == wxID_OK) {
        wxString fileName = openFileDialog->GetPath();
        tc->LoadFile(fileName);
    }
}

int t29()
{
    MyFrameT29 * frame = new MyFrameT29(wxT("MyFrame"));
    frame->Show(true);
    return EXIT_SUCCESS;
}

