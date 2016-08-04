#include "tutorial.h"

class MyFrameT30: public wxFrame
{
public:
    MyFrameT30(const wxString& title);

    void OnOpen(wxCommandEvent& event);

    wxStaticText *st;
};

const int ID_FONTDIALOG = 1;

MyFrameT30::MyFrameT30(const wxString & title) :
        wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(300, 200))
{
    wxPanel *panel = new wxPanel(this, -1);

    wxMenuBar *menubar = new wxMenuBar;
    wxMenu *file = new wxMenu;
    file->Append(ID_FONTDIALOG, wxT("&Change font"));
    menubar->Append(file, wxT("&File"));
    SetMenuBar(menubar);

    Connect(ID_FONTDIALOG, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyFrameT30::OnOpen));

    st = new wxStaticText(panel, wxID_ANY, wxT("The Agoge"), wxPoint(20, 20));

    Center();
}

void MyFrameT30::OnOpen(wxCommandEvent& WXUNUSED(event))
{
    wxFontDialog *fontDialog = new wxFontDialog(this);

    if (fontDialog->ShowModal() == wxID_OK) {
        st->SetFont(fontDialog->GetFontData().GetChosenFont());
    }
}

int t30()
{
    MyFrameT30 * frame = new MyFrameT30(wxT("MyFrame"));
    frame->Show(true);
    return EXIT_SUCCESS;
}

