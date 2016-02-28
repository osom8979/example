#include "tutorial.h"

// The following classes are derived wxDC classes:
// wxBufferedDC
// wxBufferedPaintDC
// wxPostScriptDC
// wxMemoryDC
// wxPrinterDC
// wxScreenDC
// wxClientDC
// wxPaintDC
// wxWindowDC

class MyFrameT42: public wxFrame
{
public:
    MyFrameT42(const wxString& title);

    void OnPaint(wxPaintEvent& event);
};

MyFrameT42::MyFrameT42(const wxString& title) :
        wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(280, 180))
{
    this->Connect(wxEVT_PAINT, wxPaintEventHandler(MyFrameT42::OnPaint));
    this->Centre();
}

void MyFrameT42::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);

    wxCoord x1 = 50, y1 = 60;
    wxCoord x2 = 190, y2 = 60;

    dc.DrawLine(x1, y1, x2, y2);
}

int t42()
{
    MyFrameT42 * frame = new MyFrameT42(wxT("MyFrame"));
    frame->Show(true);
    return EXIT_SUCCESS;
}

