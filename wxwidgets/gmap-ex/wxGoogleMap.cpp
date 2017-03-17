/**
 * @file   wxGoogleMap.cpp
 * @brief  Google-Map custom widgets implementation.
 * @author zer0
 * @date   2017-02-17
 */

#include <wxGoogleMap.h>
#include <wx/webview.h>

/**
 * @def DEFAULT_LATITUDE
 *
 * Latitude ranges between -90 and 90 degrees, inclusive.
 * Values above or below this range will be clamped to the range [-90, 90].
 * This means that if the value specified is less than -90, it will be set to -90.
 * And if the value is greater than 90, it will be set to 90.
 */
#define DEFAULT_LATITUDE "37.5653161"

/**
 * @def DEFAULT_LONGITUDE
 *
 * Longitude ranges between -180 and 180 degrees, inclusive.
 * Values above or below this range will be wrapped so that they fall within the range.
 * For example, a value of -190 will be converted to 170.
 * A value of 190 will be converted to -170.
 * This reflects the fact that longitudes wrap around the globe.
 */
#define DEFAULT_LONGITUDE "126.9745883"

// @formatter:off
static char const * const DEFAULT_HTML =
        "<!DOCTYPE html>"
        "<html style='height: 100%;'>"
        "<head>"
        "    <meta http-equiv='Content-Type' content='text/html; charset=utf-8' />"
        "    <meta name='viewport' content='width=device-width, initial-scale=1' />"
        "    <script type='text/javascript' src='https://maps.googleapis.com/maps/api/js?sensor=true'></script>"
        "    <script type='text/javascript'>"
        "        var g_map;"
        "        function onLoad() {"
        "            var options = {"
        "                zoom: 13,"
        "                center: new google.maps.LatLng(" DEFAULT_LATITUDE "," DEFAULT_LONGITUDE "),"
        "                mapTypeId : google.maps.MapTypeId.ROADMAP"
        "            };"
        "            g_map = new google.maps.Map(document.getElementById('map_canvas'), options);"
        "        }"
        "    </script>"
        "    <style type='text/css'>"
        "        .map {"
        "            width: 100%;"
        "            height: 100%;"
        "        }"
        "        .dom {"
        "            display: none;"
        "            visibility: hidden;"
        "        }"
        "    </style>"
        "</head>"
        "<body style='height: 100%; margin: 0;' onload='onLoad()'>"
        "    <div id='map_canvas' class='map'></div>"
        "    <div id='map_result' class='dom'></div>"
        "</body>"
        "</html>";
// @formatter:on

wxBEGIN_EVENT_TABLE(wxGoogleMap, wxPanel)
    EVT_WEBVIEW_LOADED(wxGoogleMapWebViewId, wxGoogleMap::OnLoaded)
    EVT_WEBVIEW_TITLE_CHANGED(wxGoogleMapWebViewId, wxGoogleMap::OnTitleChanged)
    EVT_WEBVIEW_ERROR(wxGoogleMapWebViewId, wxGoogleMap::OnError)
wxEND_EVENT_TABLE()

wxGoogleMap::wxGoogleMap() : wxPanel()
{
    Init();
}

wxGoogleMap::wxGoogleMap(wxWindow * parent,
                         wxWindowID winid,
                         wxPoint const & pos,
                         wxSize const & size,
                         long style,
                         wxString const & name)
        : wxPanel(parent, winid, pos, size, style, name)
{
    Init();
}

wxGoogleMap::~wxGoogleMap()
{
    // EMPTY.
}

void wxGoogleMap::Init()
{
    _web   = wxWebView::New(this, wxGoogleMapWebViewId);
    _sizer = new wxBoxSizer(wxVERTICAL);

    _sizer->Add(_web, wxSizerFlags().Expand().Proportion(1));
    _web->SetPage(wxString(DEFAULT_HTML), wxString(wxGoogleMapDefaultUrl));

    SetSizer(_sizer);
    Centre();
}

void wxGoogleMap::OnLoaded(wxWebViewEvent & event)
{
}

void wxGoogleMap::OnTitleChanged(wxWebViewEvent & event)
{
}

void wxGoogleMap::OnError(wxWebViewEvent & event)
{
}

