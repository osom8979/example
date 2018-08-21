/// @file main.cpp
/// @brief Entry-Point.
/// @author zer0
/// @date 2014-09-03

#include "MainFrame.h"

#define TITLE_NAME "wxUpdate"
#define VERSION_STRING "0.1"
#define TOTAL_TITLE_STRING \
    TITLE_NAME " (" VERSION_STRING ")"

class Application: public wxApp
{
public:
    virtual bool OnInit()
    {
        MainFrame *simple = new MainFrame(wxT(TOTAL_TITLE_STRING));
        simple->Show(true);
        return true;
    }
};

IMPLEMENT_APP(Application)
