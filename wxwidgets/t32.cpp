#include "tutorial.h"

class MyFrameT32: public wxFrame
{
public:
    MyFrameT32(const wxString& title);

    void OnToggle(wxCommandEvent& event);

    wxCheckBox *m_cb;
};

const int ID_CHECKBOX = 100;

MyFrameT32::MyFrameT32(const wxString& title) :
        wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(270, 150))
{
    wxPanel *panel = new wxPanel(this, wxID_ANY);

    m_cb = new wxCheckBox(panel, ID_CHECKBOX, wxT("Show title"), wxPoint(20, 20));
    m_cb->SetValue(true);
    Connect(ID_CHECKBOX, wxEVT_COMMAND_CHECKBOX_CLICKED,
            wxCommandEventHandler(MyFrameT32::OnToggle));
    Centre();
}

void MyFrameT32::OnToggle(wxCommandEvent& WXUNUSED(event))
{
    if (m_cb->GetValue()) {
        this->SetTitle(wxT("CheckBox"));
    } else {
        this->SetTitle(wxT(" "));
    }
}

int t32()
{
    MyFrameT32 * frame = new MyFrameT32(wxT("MyFrame"));
    frame->Show(true);
    return EXIT_SUCCESS;
}

