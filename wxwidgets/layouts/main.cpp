/**
 * @file   main.cpp
 * @brief  Program entry-point.
 * @author zer0
 * @date   2017-04-26
 */

#include <iostream>

#include <wx/wx.h>
#include <MainFrame.hpp>

#define TITLE "Layouts"

class Application final : public wxApp
{
public:
    Application() { /* EMPTY. */ }
    ~Application() { /* EMPTY. */ }

public:
    virtual bool OnInit() override
    {
        try {
            ::wxInitAllImageHandlers();
            auto * mainframe = new MainFrame(wxT(TITLE));
            mainframe->Show(true);
            wxApp::SetTopWindow(mainframe);
        } catch (...) {
            std::cerr << "Unknown exception.";
            return false;
        }

        return true;
    }
};

IMPLEMENT_APP(Application);

