/**
 * @file   wxGoogleMap.h
 * @brief  Google-Map custom widgets.
 * @author zer0
 * @date   2017-02-17
 */

#ifndef __INCLUDE_WX_GOOGLEMAP_HPP__
#define __INCLUDE_WX_GOOGLEMAP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <wx/wx.h>

// Forward declaration.
class wxBoxSizer;
class wxWebView;
class wxWebViewEvent;

/**
 * Google-Map custom widgets.
 *
 * @author zer0
 * @date   2017-02-17
 */
class wxGoogleMap : public wxPanel
{
private:
    wxDECLARE_EVENT_TABLE();

public:
    constexpr static char const * const wxGoogleMapDefaultUrl = "wxGoogleMap";
    constexpr static wxWindowID const wxGoogleMapWebViewId = 10001;

private:
    wxWebView  * _web;
    wxBoxSizer * _sizer;

public:
    wxGoogleMap();
    wxGoogleMap(wxWindow * parent,
                wxWindowID winid = wxID_ANY,
                wxPoint const & pos = wxDefaultPosition,
                wxSize const & size = wxDefaultSize,
                long style = wxTAB_TRAVERSAL | wxNO_BORDER,
                wxString const & name = wxPanelNameStr);
    virtual ~wxGoogleMap();

private:
    wxGoogleMap(wxGoogleMap const & obj) = delete;
    wxGoogleMap(wxGoogleMap && obj) = delete;

private:
    wxGoogleMap & operator =(wxGoogleMap const & obj) = delete;
    wxGoogleMap & operator =(wxGoogleMap && obj) = delete;

protected:
    /** Initialize widget's internals. */
    void Init();

protected:
    void OnLoaded(wxWebViewEvent & event);
    void OnTitleChanged(wxWebViewEvent & event);
    void OnError(wxWebViewEvent & event);
};

#endif // __INCLUDE_WX_GOOGLEMAP_HPP__

