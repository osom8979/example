#include "tutorial.h"

class MyFrameT27: public wxFrame
{
public:
    MyFrameT27(const wxString& title);

};

MyFrameT27::MyFrameT27(const wxString& title) :
        wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(200, 150))
{
    wxPanel *panel = new wxPanel(this, -1);

    wxGridSizer *grid = new wxGridSizer(3, 2, 10, 10);

    grid->Add(new wxButton(panel, wxID_CANCEL), 0, wxTOP | wxLEFT, 9);
    grid->Add(new wxButton(panel, wxID_DELETE), 0, wxTOP, 9);
    grid->Add(new wxButton(panel, wxID_SAVE), 0, wxLEFT, 9);
    grid->Add(new wxButton(panel, wxID_EXIT));
    grid->Add(new wxButton(panel, wxID_STOP), 0, wxLEFT, 9);
    grid->Add(new wxButton(panel, wxID_NEW));

    panel->SetSizer(grid);
    Centre();
}

int t27()
{
    MyFrameT27 * frame = new MyFrameT27(wxT("MyFrame"));
    frame->Show(true);
    return EXIT_SUCCESS;
}

