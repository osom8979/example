#include "tutorial.h"

class MyFrameT24: public wxFrame
{
public:
    MyFrameT24(const wxString& title);

    void OnMove(wxMoveEvent & event);

    wxStaticText *st1;
    wxStaticText *st2;

};

MyFrameT24::MyFrameT24(const wxString& title) :
        wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(250, 130))
{
    wxPanel *panel = new wxPanel(this, -1);

    st1 = new wxStaticText(panel, -1, wxT(""), wxPoint(10, 10));
    st2 = new wxStaticText(panel, -1, wxT(""), wxPoint(10, 30));

    Connect(wxEVT_MOVE, wxMoveEventHandler(MyFrameT24::OnMove));

    Centre();
}

void MyFrameT24::OnMove(wxMoveEvent& event)
{
    wxPoint size = event.GetPosition();
    st1->SetLabel(wxString::Format(wxT("x: %d"), size.x));
    st2->SetLabel(wxString::Format(wxT("y: %d"), size.y));
}

int t24()
{
    MyFrameT24 * frame = new MyFrameT24(wxT("MyFrame"));
    frame->Show(true);
    return EXIT_SUCCESS;
}

