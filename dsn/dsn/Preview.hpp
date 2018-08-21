/**
 * @file   Preview.hpp
 * @brief  Preview class prototype.
 * @author zer0
 * @date   2017-04-02
 */

#ifndef __INCLUDE_DSN__PREVIEW_HPP__
#define __INCLUDE_DSN__PREVIEW_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <wx/wx.h>
#include <wx/bitmap.h>

/**
 * Preview widget.
 *
 * @author zer0
 * @date   2017-04-02
 */
class Preview : public wxPanel
{
public:
    wxDECLARE_EVENT_TABLE();
    wxDECLARE_CLASS(Preview);

private:
    wxBitmap _bitmap;

public:
    Preview(wxString const & image_path,
            wxWindow * parent,
            wxWindowID winid = wxID_ANY,
            wxPoint const & pos = wxDefaultPosition,
            wxSize const & size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL | wxNO_BORDER,
            wxString const & name = wxPanelNameStr);
    virtual ~Preview();

// Event methods.
protected:
    void onPaint(wxPaintEvent & event);
    void OnEraseBackground(wxEraseEvent & event);

public:
    static void draw(wxDC & dc, wxBitmap const & bitmap, int x, int y);
};

#endif // __INCLUDE_DSN__PREVIEW_HPP__

