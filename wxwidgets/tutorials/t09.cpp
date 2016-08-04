#include "tutorial.h"

class MyFrameT09: public wxFrame
{
public:
    MyFrameT09(const wxString& title);
};

MyFrameT09::MyFrameT09(const wxString& title) :
        wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(250, 150))
{
    SetIcon(wxIcon(wxT("info.xpm")));
    Centre();
}

int t09()
{
    MyFrameT09 * frame = new MyFrameT09(wxT("MyFrame"));
    frame->Show(true);
    return EXIT_SUCCESS;
}
