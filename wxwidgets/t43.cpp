#include "tutorial.h"

class MyFrameT43: public wxFrame
{
public:
    MyFrameT43(const wxString & title);

    void OnPaint(wxPaintEvent & event);
};

MyFrameT43::MyFrameT43(const wxString& title) :
        wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(250, 150))
{
    Connect(wxEVT_PAINT, wxPaintEventHandler(MyFrameT43::OnPaint));
    Centre();
}

void MyFrameT43::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);

    dc.DrawText(wxT("Лев Николaевич Толстoй"), 40, 60);
    dc.DrawText(wxT("Анна Каренина"), 70, 80);
}

int t43()
{
    MyFrameT43 * frame = new MyFrameT43(wxT("MyFrame"));
    frame->Show(true);
    return EXIT_SUCCESS;
}

