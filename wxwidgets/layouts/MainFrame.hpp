/**
 * @file   MainFrame.hpp
 * @brief  wxWidgets main frame.
 * @author zer0
 * @date   2017-04-26
 */

#ifndef __INCLUDE_LIBMUGCUP__WXBARISTA_MAINFRAME_HPP__
#define __INCLUDE_LIBMUGCUP__WXBARISTA_MAINFRAME_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <wx/wx.h>

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title);
    ~MainFrame();
};

#endif // __INCLUDE_LIBMUGCUP__WXBARISTA_MAINFRAME_HPP__

