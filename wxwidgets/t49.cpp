#include "tutorial.h"

class Widget: public wxPanel
{
public:
    Widget(wxPanel *parent, int id);

    wxPanel *m_parent;

    void OnSize(wxSizeEvent& event);
    void OnPaint(wxPaintEvent& event);
};

class MyFrameT49: public wxFrame
{
public:
    MyFrameT49(const wxString& title);

    void OnScroll(wxScrollEvent& event);
    int GetCurWidth();

    wxSlider *m_slider;
    Widget *m_wid;

    int cur_width;
};

int ID_SLIDER = 1;

int num[] = { 75, 150, 225, 300, 375, 450, 525, 600, 675 };
int asize = sizeof(num) / sizeof(num[1]);

Widget::Widget(wxPanel *parent, int id) :
        wxPanel(parent, id, wxDefaultPosition, wxSize(-1, 30), wxSUNKEN_BORDER)
{
    m_parent = parent;

    Connect(wxEVT_PAINT, wxPaintEventHandler(Widget::OnPaint));
    Connect(wxEVT_SIZE, wxSizeEventHandler(Widget::OnSize));
}

void Widget::OnPaint(wxPaintEvent& event)
{
    wxFont font(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false,
            wxT("Courier 10 Pitch"));

    wxPaintDC dc(this);
    dc.SetFont(font);
    wxSize size = GetSize();
    int width = size.GetWidth();

    MyFrameT49 *burn = (MyFrameT49 *) m_parent->GetParent();

    int cur_width = burn->GetCurWidth();

    int step = (int) round(width / 10.0);

    int till = (int) ((width / 750.0) * cur_width);
    int full = (int) ((width / 750.0) * 700);

    if (cur_width >= 700) {

        dc.SetPen(wxPen(wxColour(255, 255, 184)));
        dc.SetBrush(wxBrush(wxColour(255, 255, 184)));
        dc.DrawRectangle(0, 0, full, 30);
        dc.SetPen(wxPen(wxColour(255, 175, 175)));
        dc.SetBrush(wxBrush(wxColour(255, 175, 175)));
        dc.DrawRectangle(full, 0, till - full, 30);

    } else {

        dc.SetPen(wxPen(wxColour(255, 255, 184)));
        dc.SetBrush(wxBrush(wxColour(255, 255, 184)));
        dc.DrawRectangle(0, 0, till, 30);

    }

    dc.SetPen(wxPen(wxColour(90, 80, 60)));
    for (int i = 1; i <= asize; i++) {

        dc.DrawLine(i * step, 0, i * step, 6);
        wxSize size = dc.GetTextExtent(wxString::Format(wxT("%d"), num[i - 1]));
        dc.DrawText(wxString::Format(wxT("%d"), num[i - 1]), i * step - size.GetWidth() / 2, 8);
    }
}

void Widget::OnSize(wxSizeEvent& event)
{
    Refresh();
}

MyFrameT49::MyFrameT49(const wxString& title) :
        wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(350, 200))
{
    cur_width = 75;

    wxPanel *panel = new wxPanel(this, wxID_ANY);
    wxPanel *centerPanel = new wxPanel(panel, wxID_ANY);

    m_slider = new wxSlider(centerPanel, ID_SLIDER, 75, 0, 750, wxPoint(-1, -1), wxSize(150, -1),
    wxSL_LABELS);

    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *hbox2 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *hbox3 = new wxBoxSizer(wxHORIZONTAL);

    m_wid = new Widget(panel, wxID_ANY);
    hbox->Add(m_wid, 1, wxEXPAND);

    hbox2->Add(centerPanel, 1, wxEXPAND);
    hbox3->Add(m_slider, 0, wxTOP | wxLEFT, 35);

    centerPanel->SetSizer(hbox3);

    vbox->Add(hbox2, 1, wxEXPAND);
    vbox->Add(hbox, 0, wxEXPAND);

    panel->SetSizer(vbox);
    m_slider->SetFocus();

    Connect(ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(MyFrameT49::OnScroll));

    Centre();
}

void MyFrameT49::OnScroll(wxScrollEvent& WXUNUSED(event))
{
    cur_width = m_slider->GetValue();
    m_wid->Refresh();
}

int MyFrameT49::GetCurWidth()
{
    return cur_width;
}

int t49()
{
    MyFrameT49 * frame = new MyFrameT49(wxT("MyFrame"));
    frame->Show(true);
    return EXIT_SUCCESS;
}

