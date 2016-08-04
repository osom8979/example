#include "tutorial.h"

class LeftPanel: public wxPanel
{
public:
    LeftPanel(wxPanel *parent);

    void OnPlus(wxCommandEvent & event);
    void OnMinus(wxCommandEvent & event);

    wxButton * _plus;
    wxButton * _minus;
    wxPanel * _parent;
    int _count;

};

class RightPanel: public wxPanel
{
public:
    RightPanel(wxPanel *parent);

    void OnSetText(wxCommandEvent & event);

    wxStaticText * _text;
};

class MyFrameT11: public wxFrame
{
public:
    MyFrameT11(const wxString& title);

    LeftPanel * _lp;
    RightPanel * _rp;
    wxPanel * _parent;
};

const int ID_PLUS = 101;
const int ID_MINUS = 102;

LeftPanel::LeftPanel(wxPanel * parent) :
        wxPanel(parent, -1, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN)
{
    _count = 0;
    _parent = parent;
    _plus = new wxButton(this, ID_PLUS, wxT("+"), wxPoint(10, 10));
    _minus = new wxButton(this, ID_MINUS, wxT("-"), wxPoint(10, 60));
    Connect(ID_PLUS, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LeftPanel::OnPlus));
    Connect(ID_MINUS, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LeftPanel::OnMinus));
}

void LeftPanel::OnPlus(wxCommandEvent & WXUNUSED(event))
{
    _count++;
    MyFrameT11 *frame = (MyFrameT11 *) _parent->GetParent();
    frame->_rp->_text->SetLabel(wxString::Format(wxT("%d"), _count));
}

void LeftPanel::OnMinus(wxCommandEvent & WXUNUSED(event))
{
    _count--;
    MyFrameT11 *frame = (MyFrameT11 *) _parent->GetParent();
    frame->_rp->_text->SetLabel(wxString::Format(wxT("%d"), _count));
}

RightPanel::RightPanel(wxPanel * parent) :
        wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(270, 150), wxBORDER_SUNKEN)
{
    _text = new wxStaticText(this, -1, wxT("0"), wxPoint(40, 60));
}

MyFrameT11::MyFrameT11(const wxString& title) :
        wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(290, 150))
{
    _parent = new wxPanel(this, wxID_ANY);

    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

    _lp = new LeftPanel(_parent);
    _rp = new RightPanel(_parent);

    hbox->Add(_lp, 1, wxEXPAND | wxALL, 5);
    hbox->Add(_rp, 1, wxEXPAND | wxALL, 5);

    _parent->SetSizer(hbox);

    this->Centre();
}

int t11()
{
    MyFrameT11 * frame = new MyFrameT11(wxT("MyFrame"));
    frame->Show(true);
    return EXIT_SUCCESS;
}
