#include "tutorial.h"

class MyFrameT23: public wxFrame
{
public:
    MyFrameT23(const wxString& title);

    void OnQuit(wxCommandEvent& event);

private:
    DECLARE_EVENT_TABLE()
};

MyFrameT23::MyFrameT23(const wxString& title) :
        wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(270, 150))
{
    wxPanel *panel = new wxPanel(this, wxID_ANY);
    wxButton *button = new wxButton(panel, wxID_EXIT, wxT("Quit"), wxPoint(20, 20));

    Centre();
}

void MyFrameT23::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

BEGIN_EVENT_TABLE(MyFrameT23, wxFrame)
// {
EVT_BUTTON(wxID_EXIT, MyFrameT23::OnQuit)
// }
END_EVENT_TABLE()

int t23()
{
    MyFrameT23 * frame = new MyFrameT23(wxT("MyFrame"));
    frame->Show(true);
    return EXIT_SUCCESS;
}

