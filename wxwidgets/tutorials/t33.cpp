#include "tutorial.h"

class MyFrameT33: public wxFrame
{
public:
    MyFrameT33(const wxString& title);

    wxSlider *slider;
    wxBitmapButton *button;
    int pos;

    void OnScroll(wxScrollEvent& event);
};

const int ID_SLIDER = 100;

MyFrameT33::MyFrameT33(const wxString& title) :
        wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(250, 130))
{
    wxImage::AddHandler(new wxPNGHandler);
    wxPanel *panel = new wxPanel(this);
    slider = new wxSlider(panel, ID_SLIDER, 0, 0, 100, wxPoint(10, 30), wxSize(140, -1));

    button = new wxBitmapButton(panel, wxID_ANY, wxBitmap(wxT("sound-mute.png"), wxBITMAP_TYPE_PNG),
            wxPoint(180, 20));

    Connect(ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(MyFrameT33::OnScroll));
    Center();
}

void MyFrameT33::OnScroll(wxScrollEvent& event)
{
    pos = slider->GetValue();

    if (pos == 0) {
        button->SetBitmapLabel(wxBitmap(wxT("sound-mute.png"), wxBITMAP_TYPE_PNG));
    } else if (pos > 0 && pos <= 30) {
        button->SetBitmapLabel(wxBitmap(wxT("sound-min.png"), wxBITMAP_TYPE_PNG));
    } else if (pos > 30 && pos < 80) {
        button->SetBitmapLabel(wxBitmap(wxT("sound-med.png"), wxBITMAP_TYPE_PNG));
    } else {
        button->SetBitmapLabel(wxBitmap(wxT("sound-max.png"), wxBITMAP_TYPE_PNG));
    }
}

int t33()
{
    MyFrameT33 * frame = new MyFrameT33(wxT("MyFrame"));
    frame->Show(true);
    return EXIT_SUCCESS;
}

