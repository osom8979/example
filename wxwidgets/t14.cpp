#include "tutorial.h"

class MyFrameT14: public wxFrame
{
public:
    MyFrameT14(const wxString& title);

    void OnQuit(wxCommandEvent& event);
};

MyFrameT14::MyFrameT14(const wxString& title) :
        wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(280, 180))
{
    wxImage::AddHandler(new wxPNGHandler);

    wxBitmap exit(wxT("application-exit-3.png"), wxBITMAP_TYPE_PNG);

    wxToolBar *toolbar = CreateToolBar();
    toolbar->AddTool(wxID_EXIT, exit, wxT("Exit application"));
    toolbar->Realize();

    Connect(wxID_EXIT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MyFrameT14::OnQuit));

    Centre();
}

void MyFrameT14::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

int t14()
{
    MyFrameT14 * frame = new MyFrameT14(wxT("MyFrame"));
    frame->Show(true);
    return EXIT_SUCCESS;
}
