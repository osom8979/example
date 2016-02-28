#include "tutorial.h"

class MyFrameT28: public wxFrame
{
public:
    MyFrameT28(const wxString& title);

    void ShowMessage1(wxCommandEvent & event);
    void ShowMessage2(wxCommandEvent & event);
    void ShowMessage3(wxCommandEvent & event);
    void ShowMessage4(wxCommandEvent & event);
};

const int ID_INFO = 1;
const int ID_ERROR = 2;
const int ID_QUESTION = 3;
const int ID_ALERT = 4;

MyFrameT28::MyFrameT28(const wxString& title) :
        wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(210, 110))
{
    wxPanel *panel = new wxPanel(this, wxID_ANY);

    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
    wxGridSizer *gs = new wxGridSizer(2, 2, 2, 2);

    wxButton *btn1 = new wxButton(panel, ID_INFO, wxT("Info"));
    wxButton *btn2 = new wxButton(panel, ID_ERROR, wxT("Error"));
    wxButton *btn3 = new wxButton(panel, ID_QUESTION, wxT("Question"));
    wxButton *btn4 = new wxButton(panel, ID_ALERT, wxT("Alert"));

    Connect(ID_INFO, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrameT28::ShowMessage1));
    Connect(ID_ERROR, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MyFrameT28::ShowMessage2));
    Connect(ID_QUESTION, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MyFrameT28::ShowMessage3));
    Connect(ID_ALERT, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MyFrameT28::ShowMessage4));

    gs->Add(btn1, 1, wxEXPAND);
    gs->Add(btn2, 1);
    gs->Add(btn3, 1);
    gs->Add(btn4, 1);

    hbox->Add(gs, 0, wxALL, 15);
    panel->SetSizer(hbox);

    Center();
}

void MyFrameT28::ShowMessage1(wxCommandEvent& event)
{
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Download completed"), wxT("Info"), wxOK);
    dial->ShowModal();
}

void MyFrameT28::ShowMessage2(wxCommandEvent& event)
{
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Error loading file"), wxT("Error"),
    wxOK | wxICON_ERROR);
    dial->ShowModal();
}

void MyFrameT28::ShowMessage3(wxCommandEvent& event)
{
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Are you sure to quit?"), wxT("Question"),
    wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
    dial->ShowModal();
}

void MyFrameT28::ShowMessage4(wxCommandEvent& event)
{
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Unallowed operation"),
            wxT("Exclamation"),
            wxOK | wxICON_EXCLAMATION);
    dial->ShowModal();
}

int t28()
{
    MyFrameT28 * frame = new MyFrameT28(wxT("MyFrame"));
    frame->Show(true);
    return EXIT_SUCCESS;
}

