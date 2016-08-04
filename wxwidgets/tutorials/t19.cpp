#include "tutorial.h"

class MyFrameT19: public wxFrame
{
public:
    MyFrameT19(const wxString& title);
};

MyFrameT19::MyFrameT19(const wxString& title) :
        wxFrame(NULL, -1, title, wxPoint(-1, -1), wxSize(300, 200))
{
    wxPanel *panel = new wxPanel(this, -1);

    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hbox1 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *hbox2 = new wxBoxSizer(wxHORIZONTAL);

    wxButton *ok = new wxButton(panel, -1, wxT("Ok"));
    wxButton *cancel = new wxButton(panel, -1, wxT("Cancel"));

    hbox1->Add(new wxPanel(panel, -1));
    vbox->Add(hbox1, 1, wxEXPAND);

    hbox2->Add(ok);
    hbox2->Add(cancel);

    vbox->Add(hbox2, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 10);
    panel->SetSizer(vbox);

    Centre();
}

int t19()
{
    MyFrameT19 * frame = new MyFrameT19(wxT("MyFrame"));
    frame->Show(true);
    return EXIT_SUCCESS;
}

