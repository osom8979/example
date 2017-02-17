/**
 * @file   main.cpp
 * @brief  Program entry-point.
 * @author zer0
 * @date   2017-02-17
 */

#include <wx/wx.h>
#include <wx/webview.h>

#define TITLE "wxGoogleMap"
#define URL   "map.html" // "http://naver.com"

class MainFrame : public wxFrame
{
public:
    MainFrame() : wxFrame(NULL, wxID_ANY, wxT(TITLE))
    {
        wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(wxWebView::New(this, wxID_ANY, wxT(URL)), wxSizerFlags().Expand().Proportion(1));
        SetSizer(sizer);
        Show();
    }
};

class MapApp : public wxApp
{
private:
    MainFrame * _frame;

public:
    virtual bool OnInit() override
    {
        _frame = new MainFrame();
        return true;
    }
};

// IMPLEMENT_CLASS(Application, wxApp)
IMPLEMENT_APP(MapApp);

