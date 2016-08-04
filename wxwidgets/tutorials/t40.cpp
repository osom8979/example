#include "tutorial.h"

class MyFrameT40: public wxFrame
{
public:
    MyFrameT40(const wxString& title);
};

MyFrameT40::MyFrameT40(const wxString& title) :
        wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(300, 200))
{
    wxImage::AddHandler(new wxJPEGHandler);
    wxScrolledWindow *sw = new wxScrolledWindow(this);

    wxBitmap bmp(wxT("castle.jpg"), wxBITMAP_TYPE_JPEG);
    wxStaticBitmap *sb = new wxStaticBitmap(sw, -1, bmp);

    int width = bmp.GetWidth();
    int height = bmp.GetHeight();

    sw->SetScrollbars(10, 10, width / 10, height / 10);
    sw->Scroll(50, 10);

    Center();
}

int t40()
{
    MyFrameT40 * frame = new MyFrameT40(wxT("MyFrame"));
    frame->Show(true);
    return EXIT_SUCCESS;
}

