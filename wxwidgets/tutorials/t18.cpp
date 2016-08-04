#include "tutorial.h"

// wxBoxSizer border flags.
// wxLEFT
// wxRIGHT
// wxBOTTOM
// wxTOP
// wxALL

// alignment flags.
// wxALIGN_LEFT
// wxALIGN_RIGHT
// wxALIGN_TOP
// wxALIGN_BOTTOM
// wxALIGN_CENTER_VERTICAL
// wxALIGN_CENTER_HORIZONTAL
// wxALIGN_CENTER

class MyFrameT18: public wxFrame
{
public:
    MyFrameT18(const wxString& title);

};

MyFrameT18::MyFrameT18(const wxString& title) :
        wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(250, 200))
{
    wxColour col1, col2;
    col1.Set(wxT("#4f5049"));
    col2.Set(wxT("#ededed"));

    wxPanel *panel = new wxPanel(this, -1);
    panel->SetBackgroundColour(col1);
    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

    wxPanel *midPan = new wxPanel(panel, wxID_ANY);
    midPan->SetBackgroundColour(col2);

    vbox->Add(midPan, 1, wxEXPAND | wxALL, 20);
    panel->SetSizer(vbox);

    Centre();
}

int t18()
{
    MyFrameT18 * frame = new MyFrameT18(wxT("MyFrame"));
    frame->Show(true);
    return EXIT_SUCCESS;
}

