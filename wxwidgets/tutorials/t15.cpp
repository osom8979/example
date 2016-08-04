#include "tutorial.h"

class MyFrameT15: public wxFrame
{
public:
    MyFrameT15(const wxString& title);

    void OnQuit(wxCommandEvent& event);

    wxToolBar *toolbar1;
    wxToolBar *toolbar2;
};

MyFrameT15::MyFrameT15(const wxString& title) :
        wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(280, 180))
{
    wxImage::AddHandler(new wxPNGHandler);

    wxBitmap exit(wxT("application-exit-3.png"), wxBITMAP_TYPE_PNG);
    wxBitmap newb(wxT("document-new.png"), wxBITMAP_TYPE_PNG);
    wxBitmap open(wxT("document-open.png"), wxBITMAP_TYPE_PNG);
    wxBitmap save(wxT("document-save.png"), wxBITMAP_TYPE_PNG);

    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

    toolbar1 = new wxToolBar(this, wxID_ANY);
    toolbar1->AddTool(wxID_ANY, newb, wxT(""));
    toolbar1->AddTool(wxID_ANY, open, wxT(""));
    toolbar1->AddTool(wxID_ANY, save, wxT(""));
    toolbar1->Realize();

    toolbar2 = new wxToolBar(this, wxID_ANY);
    toolbar2->AddTool(wxID_EXIT, exit, wxT("Exit application"));
    toolbar2->Realize();

    vbox->Add(toolbar1, 0, wxEXPAND);
    vbox->Add(toolbar2, 0, wxEXPAND);

    SetSizer(vbox);

    Connect(wxID_EXIT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MyFrameT15::OnQuit));

    Centre();
}

void MyFrameT15::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

int t15()
{
    MyFrameT15 * frame = new MyFrameT15(wxT("MyFrame"));
    frame->Show(true);
    return EXIT_SUCCESS;
}
