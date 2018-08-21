/**
 * @file   main.cpp
 * @brief  Program entry-point.
 * @author zer0
 * @date   2017-04-01
 */

#include <wx/wx.h>
#include <dsn/MainFrame.hpp>

/**
 * Application class.
 *
 * @author zer0
 * @date   2017-04-01
 */
class Application : public wxApp
{
private:
    MainFrame * _frame;

public:
    // @formatter:off
    Application() : _frame(nullptr)
    { /* EMPTY. */ }
    virtual ~Application()
    { /* EMPTY. */ }
    // @formatter:on

public:
    virtual bool OnInit() wxOVERRIDE
    {
        ::wxInitAllImageHandlers();

        try {
            _frame = new MainFrame();
        } catch (...) {
            return false;
        }
        return true;
    }

    virtual int OnExit() wxOVERRIDE
    {
        return 0;
    }
};

wxIMPLEMENT_APP(Application);


