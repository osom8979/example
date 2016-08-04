#include "tutorial.h"

class MyFrameT44: public wxFrame
{
public:
    MyFrameT44(const wxString & title);

    void OnPaint(wxPaintEvent & event);
};

MyFrameT44::MyFrameT44(const wxString& title) :
        wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(280, 180))
{
    this->Connect(wxEVT_PAINT, wxPaintEventHandler(MyFrameT44::OnPaint));
    srand(time(NULL));
    this->Centre();
}

void MyFrameT44::OnPaint(wxPaintEvent & event)
{
    wxPaintDC dc(this);

    wxCoord x = 0;
    wxCoord y = 0;

    wxSize size = this->GetSize();

    for (int i = 0; i < 1000; i++) {
        x = rand() % size.x + 1;
        y = rand() % size.y + 1;
        dc.DrawPoint(x, y);
    }
}

int t44()
{
    MyFrameT44 * frame = new MyFrameT44(wxT("MyFrame"));
    frame->Show(true);
    return EXIT_SUCCESS;
}

