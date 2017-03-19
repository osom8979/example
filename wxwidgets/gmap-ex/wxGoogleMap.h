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

#include <atomic>

#include <wx/wx.h>
#include <json.h>

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
    /**
     * Callback of wxGoogleMap widgets.
     *
     * @remarks
     *  The return value of all methods:    @n
     *   - True: Consume the current event. @n
     *   - False: Delegate the event to the parent class. (Default)
     */
    struct Callback
    {
        // @formatter:off
        virtual bool OnNavigating   (wxWebViewEvent & event) { return false; }
        virtual bool OnNavigated    (wxWebViewEvent & event) { return false; }
        virtual bool OnLoaded       (wxWebViewEvent & event) { return false; }
        virtual bool OnTitleChanged (wxWebViewEvent & event) { return false; }
        virtual bool OnError        (wxWebViewEvent & event) { return false; }
        // @formatter:on
    };

private:
    wxWebView  * _web;
    wxBoxSizer * _sizer;

private:
    std::atomic_bool _first_load;
    Callback * _callback;

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
    // @formatter:off
    void OnNavigating   (wxWebViewEvent & event);
    void OnNavigated    (wxWebViewEvent & event);
    void OnLoaded       (wxWebViewEvent & event);
    void OnTitleChanged (wxWebViewEvent & event);
    void OnError        (wxWebViewEvent & event);
    // @formatter:on

public:
    // @formatter:off
    inline void setCallback(Callback * callback)
    { _callback = callback; }
    // @formatter:on

public:
    void Reload();
    void RunScript(wxString const & script);
    void RunUrl(wxString const & url);
    void RunHtml(wxString const & html, wxString const & scheme);
    void RunFile(wxString const & path);

public:
    void ShowDebuggingPopup(bool enable = true);
    wxString GetDomResult();
    std::size_t GetPathsCount();

public:
    void LogMessage(wxString const & message);
    void JsLogMessage(wxString const & message);
    void Toast(wxString const & message);

public:
    /**
     * Don't use this method.
     *
     * @deprecated
     */
    static wxString GetDomResult(wxString const & full_html);

public:
    template <typename Predicated>
    static void ForeachXmlElement(Json::Value const & element, Predicated predicated)
    {
        Json::ArrayIndex const SIZE = element.size();
        for (Json::ArrayIndex i = 0; i < SIZE; ++i) {
            predicated(element[i]);
        }
    }
};

#endif // __INCLUDE_WX_GOOGLEMAP_HPP__

