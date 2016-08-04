#include "tutorial.h"

class MyFrameT10: public wxFrame
{
public:
    MyFrameT10(const wxString& title);
    void OnQuit(wxCommandEvent & event);
};

MyFrameT10::MyFrameT10(const wxString& title) :
        wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(250, 150))
{
    wxPanel *panel = new wxPanel(this, wxID_ANY);

    wxButton *button = new wxButton(panel, wxID_EXIT, wxT("Quit"), wxPoint(20, 20));
    Connect(wxID_EXIT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrameT10::OnQuit));
    button->SetFocus();
    Centre();
}

void MyFrameT10::OnQuit(wxCommandEvent & WXUNUSED(event))
{
    Close(true);
}

int t10()
{
    MyFrameT10 * frame = new MyFrameT10(wxT("MyFrame"));
    frame->Show(true);
    return EXIT_SUCCESS;
}
