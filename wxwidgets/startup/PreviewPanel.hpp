/**
 * @file   PreviewPanel.hpp
 * @brief  PreviewPanel class prototype.
 * @author zer0
 * @date   2016-08-04
 */

#ifndef __INCLUDE_LIBMUGCUP__WXBARISTA_PREVIEWPANEL_HPP__
#define __INCLUDE_LIBMUGCUP__WXBARISTA_PREVIEWPANEL_HPP__

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
class PreviewPanel : public wxPanel
{
public:
    wxDECLARE_EVENT_TABLE();
    wxDECLARE_CLASS(PreviewPanel);

public:
    PreviewPanel(wxWindow      * parent
               , wxWindowID      id     = wxID_ANY
               , wxPoint const & pos    = wxDefaultPosition
               , wxSize  const & size   = wxDefaultSize);
    virtual ~PreviewPanel();

// Event methods.
protected:
    void onPaint(wxPaintEvent & event);
};

#endif // __INCLUDE_LIBMUGCUP__WXBARISTA_PREVIEWPANEL_HPP__

