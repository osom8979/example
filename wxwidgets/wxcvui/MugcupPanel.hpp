/**
 * @file   MugcupPanel.hpp
 * @brief  MugcupPanel class prototype.
 * @author zer0
 * @date   2016-08-04
 */

#ifndef __INCLUDE_EXAMPLE__WXWIDGETS_CVUI_MUGCUPPANEL_HPP__
#define __INCLUDE_EXAMPLE__WXWIDGETS_CVUI_MUGCUPPANEL_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <wx/wx.h>

/**
 * Image preview widget.
 *
 * @author zer0
 * @date   2016-08-04
 */
class MugcupPanel : public wxPanel
{
public:
    wxDECLARE_EVENT_TABLE();
    wxDECLARE_CLASS(MugcupPanel);

public:
    MugcupPanel(wxWindow      * parent,
                wxWindowID      id     = wxID_ANY,
                wxPoint const & pos    = wxDefaultPosition,
                wxSize  const & size   = wxDefaultSize);
    virtual ~MugcupPanel();

// Event methods.
protected:
    void onPaint(wxPaintEvent & event);
};

#endif // __INCLUDE_EXAMPLE__WXWIDGETS_CVUI_MUGCUPPANEL_HPP__

