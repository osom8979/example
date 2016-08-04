#include "tutorial.h"

// pen styles:
// wxSOLID
// wxDOT
// wxLONG_DASH
// wxSHORT_DASH
// wxDOT_DASH
// wxTRANSPARENT

class MyFrameT45: public wxFrame
{
public:
    MyFrameT45(const wxString& title);

    void OnPaint(wxPaintEvent& event);
};

MyFrameT45::MyFrameT45(const wxString& title) :
        wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(360, 180))
{
    this->Connect(wxEVT_PAINT, wxPaintEventHandler(MyFrameT45::OnPaint));
    this->Centre();
}

void MyFrameT45::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);

    wxColour col1, col2;

    col1.Set(wxT("#0c0c0c"));
    col2.Set(wxT("#000000"));

    wxBrush brush(wxColour(255, 255, 255), wxTRANSPARENT);
    dc.SetBrush(brush);

    dc.SetPen(wxPen(col1, 1, wxSOLID));
    dc.DrawRectangle(10, 15, 90, 60);

    dc.SetPen(wxPen(col1, 1, wxDOT));
    dc.DrawRectangle(130, 15, 90, 60);

    dc.SetPen(wxPen(col1, 1, wxLONG_DASH));
    dc.DrawRectangle(250, 15, 90, 60);

    dc.SetPen(wxPen(col1, 1, wxSHORT_DASH));
    dc.DrawRectangle(10, 105, 90, 60);

    dc.SetPen(wxPen(col1, 1, wxDOT_DASH));
    dc.DrawRectangle(130, 105, 90, 60);

    dc.SetPen(wxPen(col1, 1, wxTRANSPARENT));
    dc.DrawRectangle(250, 105, 90, 60);
}

int t45()
{
    MyFrameT45 * frame = new MyFrameT45(wxT("MyFrame"));
    frame->Show(true);
    return EXIT_SUCCESS;
}

