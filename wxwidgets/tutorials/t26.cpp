#include "tutorial.h"

class MyFrameT26: public wxFrame
{
public:
    MyFrameT26(const wxString& title);

    void OnClose(wxCloseEvent& event);
};

MyFrameT26::MyFrameT26(const wxString& title) :
        wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(250, 130))
{

    Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MyFrameT26::OnClose));
    Centre();
}

void MyFrameT26::OnClose(wxCloseEvent& event)
{
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Are you sure to quit?"), wxT("Question"),
    wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);

    int ret = dial->ShowModal();
    dial->Destroy();

    if (ret == wxID_YES) {
        Destroy();
    } else {
        event.Veto();
    }
}

int t26()
{
    MyFrameT26 * frame = new MyFrameT26(wxT("MyFrame"));
    frame->Show(true);
    return EXIT_SUCCESS;
}

