/**
 * @file   main.cpp
 * @brief  Program entry-point.
 * @author zer0
 * @date   2016-08-04
 */

#include <iostream>

#include <wx/wx.h>
#include <MainFrame.hpp>

#define TITLE "wxBarista"

/**
 * wxWidgets application class.
 *
 * @author zer0
 * @date   2016-08-04
 */
class Application final : public wxApp
{
private:
    MainFrame * _mainframe; ///< wxWidgets MainFrame object.

public:
    Application() = default;
    ~Application() = default;

public:
    virtual bool OnInit() override
    {
        try {
            ::wxInitAllImageHandlers();
            _mainframe = new MainFrame(wxT(TITLE));
            _mainframe->Show(true);
            wxApp::SetTopWindow(_mainframe);
        } catch (...) {
            std::cerr << "Unknown exception.";
            return false;
        }

        return true;
    }
};

// IMPLEMENT_CLASS(Application, wxApp)
IMPLEMENT_APP(Application);

