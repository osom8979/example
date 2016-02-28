#include "tutorial.h"

class MyFrameT25: public wxFrame
{
public:
    MyFrameT25(const wxString& title);
    void OnClick(wxCommandEvent& event);
};

class MyPanel: public wxPanel
{
public:
    MyPanel(wxFrame *frame, int id);
    void OnClick(wxCommandEvent& event);
};

class MyButton: wxButton
{
public:
    MyButton(MyPanel *panel, int id, const wxString &label);
    void OnClick(wxCommandEvent& event);

};

const int ID_BUTTON = 1;

MyFrameT25::MyFrameT25(const wxString& title) :
        wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(250, 130))
{
    MyPanel *panel = new MyPanel(this, -1);

    new MyButton(panel, ID_BUTTON, wxT("Ok"));

    Connect(ID_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrameT25::OnClick));

    Centre();
}

void MyFrameT25::OnClick(wxCommandEvent& event)
{
    std::cout << "event reached frame class" << std::endl;
    event.Skip(); // IMPORTANT!!
}

MyPanel::MyPanel(wxFrame *frame, int id) :
        wxPanel(frame, id)
{
    Connect(ID_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyPanel::OnClick));
}

void MyPanel::OnClick(wxCommandEvent& event)
{
    std::cout << "event reached panel class" << std::endl;
    event.Skip(); // IMPORTANT!!
}

MyButton::MyButton(MyPanel *mypanel, int id, const wxString& label) :
        wxButton(mypanel, id, label, wxPoint(15, 15))
{
    Connect(ID_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyButton::OnClick));
}

void MyButton::OnClick(wxCommandEvent& event)
{
    std::cout << "event reached button class" << std::endl;
    event.Skip(); // IMPORTANT!!
}

int t25()
{
    MyFrameT25 * frame = new MyFrameT25(wxT("MyFrame"));
    frame->Show(true);
    return EXIT_SUCCESS;
}

