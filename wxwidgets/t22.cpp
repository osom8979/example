#include "tutorial.h"

// wxFlexGridSizer:
// This sizer is similar to wxGridSizer.
// It does also lay out its widgets in a two dimensional table.
// It adds some flexibility to it.
// wxGridSizer cells are of the same size.
// All cells in wxFlexGridSizer have the same height in a row.
// All cells have the same width in a column.
// But all rows and columns are not necessarily the same height or width.

class MyFrameT22: public wxFrame
{
public:
    MyFrameT22(const wxString& title);
};

MyFrameT22::MyFrameT22(const wxString& title) :
        wxFrame(NULL, -1, title, wxPoint(-1, -1), wxSize(270, 220))
{
    wxPanel *panel = new wxPanel(this, -1);

    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

    wxFlexGridSizer *fgs = new wxFlexGridSizer(3, 2, 9, 25);

    wxStaticText *thetitle = new wxStaticText(panel, -1, wxT("Title"));
    wxStaticText *author = new wxStaticText(panel, -1, wxT("Author"));
    wxStaticText *review = new wxStaticText(panel, -1, wxT("Review"));

    wxTextCtrl *tc1 = new wxTextCtrl(panel, -1);
    wxTextCtrl *tc2 = new wxTextCtrl(panel, -1);
    wxTextCtrl *tc3 = new wxTextCtrl(panel, -1, wxT(""), wxPoint(-1, -1), wxSize(-1, -1),
    wxTE_MULTILINE);

    fgs->Add(thetitle);
    fgs->Add(tc1, 1, wxEXPAND);
    fgs->Add(author);
    fgs->Add(tc2, 1, wxEXPAND);
    fgs->Add(review, 1, wxEXPAND);
    fgs->Add(tc3, 1, wxEXPAND);

    fgs->AddGrowableRow(2, 1);
    fgs->AddGrowableCol(1, 1);

    hbox->Add(fgs, 1, wxALL | wxEXPAND, 15);
    panel->SetSizer(hbox);

    Centre();
}

int t22()
{
    MyFrameT22 * frame = new MyFrameT22(wxT("MyFrame"));
    frame->Show(true);
    return EXIT_SUCCESS;
}

