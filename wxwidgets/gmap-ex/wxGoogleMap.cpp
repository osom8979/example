/**
 * @file   wxGoogleMap.cpp
 * @brief  Google-Map custom widgets implementation.
 * @author zer0
 * @date   2017-02-17
 */

#include <wxGoogleMap.h>

#include <cassert>
#include <string>
#include <vector>
#include <map>

#include <wx/webview.h>
#include <wx/regex.h>

#include <tinyxml2.h>

#define DEFAULT_ID_MAP_RESULT "map_result"

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

/**
 * @def GOOGLE_MAP_URL
 *
 * Google-Map request uri.
 */
#define GOOGLE_MAP_URI "https://maps.googleapis.com/maps/api/js?v=3&key=AIzaSyBV3gzNt1Gy8oyyp7UrxiobSEWzbCDg_EI&libraries=geometry&callback=initGoogleMap"


// @formatter:off
static char const * const DEFAULT_HTML =
        "<!DOCTYPE html>"
        "<html lang='ko' style='height:100%;'>"
        "<head>"
        "    <meta http-equiv='Content-Type' content='text/html; charset=utf-8' />"
        "    <meta name='viewport' content='width=device-width, initial-scale=1' />"
        "    <script async type='text/javascript' src='" GOOGLE_MAP_URI "'></script>"
        "    <script type='text/javascript'>"
        "        var g_first_position = { lat:" DEFAULT_LATITUDE ", lng:" DEFAULT_LONGITUDE "};"
        "        var g_map;"
R"JAVASCRIPT(

function initGoogleMap() {
    var options = {
        zoom: 13,
        center: new google.maps.LatLng(g_first_position),
        mapTypeId : google.maps.MapTypeId.ROADMAP
    };
    g_map = new google.maps.Map(document.getElementById('map_canvas'), options);

    initGuidePopup(g_map);
    initDebuggingPopup(g_map);
}

function initGuidePopup(map) {
    var icons = {
        legacy_path: {
            name: 'LEGACY PATH',
            icon: 'http://maps.google.com/mapfiles/ms/icons/red-dot.png'
        },
        new_path: {
            name: 'NEW PATH',
            icon: 'http://maps.google.com/mapfiles/ms/icons/green-dot.png'
        }
    };

    var guide_popup = document.getElementById('guide_popup');
    for (var key in icons) {
        var div = document.createElement('div');
        div.innerHTML = '<img src="' + icons[key].icon + '" style="vertical-align:middle; margin:2px">' + icons[key].name + '</img>';
        guide_popup.appendChild(div);
    }

    map.controls[google.maps.ControlPosition.RIGHT_TOP].push(guide_popup);
}

function initDebuggingPopup(map) {
    var debugging_popup = document.getElementById('debugging_popup');
    map.controls[google.maps.ControlPosition.LEFT_BOTTOM].push(debugging_popup);
}

//var g_markers = {};
//
//function createMarker(name, title, longitude, latitude)
//    var new_marker = new google.maps.Marker({
//        position: {lng: longitude, lat: latitude};,
//        map: g_map,
//        title: title
//    });
//    g_markers += {name:new_marker};
//    return new_marker;
//}
//
//function updateMarkerCount() {
//    document.getElementById('map_result').getElementById('g_markers').getElementById('length').textContent = g_markers.length;
//}

)JAVASCRIPT"
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
        "        .popup {"
        "            font-family: Arial, sans-serif;"
        "            background: #fff;"
        "            padding: 10px;"
        "            margin: 10px;"
        "            border: 3px solid #000;"
        "        }"
        "    </style>"
        "</head>"
        "<body style='height: 100%; margin: 0;' onload='onLoad()'>"
        "    <div id='map_canvas' class='map'></div>"
        "    <div id='" DEFAULT_ID_MAP_RESULT "' class='dom'>"
R"DATA_DOM(

<g_markers>
    <length>0</length>
</g_markers>

)DATA_DOM"
        "    </div>"
        "    <div id='guide_popup' class='popup'></div>"
        "    <div id='debugging_popup' class='popup'>"
        "        <form action='request://SUBMIT' method='get'><input type='submit' value='JS->CPP'></form>"
        "    </div>"
        "</body>"
        "</html>";
// @formatter:on

// ---------------
namespace __impl {
// ---------------

// TinyXML2 operators.
using Document  = tinyxml2::XMLDocument;
using Element   = tinyxml2::XMLElement;
using Attribute = tinyxml2::XMLAttribute;
using Node      = tinyxml2::XMLNode;
using DomResult = std::vector<std::pair<std::string, std::string> >;

static char const * const TAG_DELIMITER = "/";
static char const * const ID_MAP_RESULT = DEFAULT_ID_MAP_RESULT;

std::vector<std::string> splitTokens(std::string const & source, std::string const & delimiter)
{
    if (source.empty() || delimiter.empty()) {
        return std::vector<std::string>();
    }

    std::vector<std::string> result;
    std::string token;

    std::size_t start = 0;
    std::size_t end   = source.find(delimiter);

    while (end != std::string::npos) {
        token = source.substr(start, end - start);
        if (token.empty() == false) {
            result.push_back(token);
        }

        // Calculate next token index.
        start = end + delimiter.length();
        end   = source.find(delimiter, start);
    }

    // Last token.
    token = source.substr(start, end);
    if (token.empty() == false) {
        result.push_back(token);
    }

    return result;
}

//static DomResult readMapResult(Document & doc, std::string const & path)
//{
//
//    auto paths = splitTokens(path, TAG_DELIMITER);
//    if (paths.empty()) {
//        auto cursor = div->FirstChild();
//        while (cursor != nullptr) {
//            std::make_pair(cursor->ToElement()->Name(), cursor->ToElement()->Value());
//            cursor = div->NextSibling();
//        }
//    }
//
//    if (div == nullptr) {
//        return DomResult();
//    }
//
//    DomResult result;
//}

static wxString getDomResult(wxString const & full_html)
{
    wxRegEx regex_open(R"(<[Dd][Ii][Vv][ ]*[Ii][Dd][ ]*=[ ]*["']map_result["'][^>]*>)");
    wxRegEx regex_close(R"(</[Dd][Ii][Vv]>)");

    std::size_t open_index, open_length;
    if (regex_open.Matches(full_html)) {
        regex_open.GetMatch(&open_index, &open_length, 0);
    }

    wxString sub = full_html.SubString(open_index + open_length, full_html.Length() - 1);

    std::size_t close_index, close_length;
    if (regex_close.Matches(sub)) {
        regex_close.GetMatch(&close_index, &close_length, 0);
    }

    auto result =sub.SubString(0, close_index - 1).Trim();
    return result;
}

static Element const * parseXml(std::string const & xml, std::string const & path)
{
//    auto element = doc.FirstChild()->ToElement();
//    for (auto & cursor : splitTokens(paths, TAG_DELIMITER)) {
//    }
}

static DomResult readMapResult(std::string const & xml, std::string const & paths)
{
    Document doc;
    if (doc.Parse(xml.c_str()) != tinyxml2::XML_SUCCESS) {
        return DomResult();
    }

    DomResult result;
    return result;
}

// ------------------
} // namespace __impl
// ------------------

static char const * const ABOUT_BLANK_URL = "about:blank";

static char const * const wxGoogleMapDefaultUrl = "file://wxgooglemap/";
static wxWindowID const wxGoogleMapWebViewId = 10001;

wxBEGIN_EVENT_TABLE(wxGoogleMap, wxPanel)
    EVT_WEBVIEW_NAVIGATING(wxGoogleMapWebViewId, wxGoogleMap::OnNavigating)
    EVT_WEBVIEW_NAVIGATED(wxGoogleMapWebViewId, wxGoogleMap::OnNavigated)
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
    _first_load.store(false);

    _web   = wxWebView::New(this, wxGoogleMapWebViewId, ABOUT_BLANK_URL);
    _sizer = new wxBoxSizer(wxVERTICAL);

    _sizer->Add(_web, wxSizerFlags().Expand().Proportion(1));
    _web->SetPage(wxString(DEFAULT_HTML), wxString(wxGoogleMapDefaultUrl));

    SetSizer(_sizer);
    Centre();
}

void wxGoogleMap::OnNavigating(wxWebViewEvent & event)
{
    wxString const REQUEST_URL = event.GetURL();
    if (REQUEST_URL.StartsWith("request:")) {
        event.Veto();
        wxLogMessage(wxT("wxGoogleMap::OnNavigating(") + REQUEST_URL + wxT(") Veto() RESULT:") + getDomResult());
    } else {
        // wxLogMessage(wxT("wxGoogleMap::OnNavigating(") + REQUEST_URL + wxT(") Allow()"));
    }
}

void wxGoogleMap::OnNavigated(wxWebViewEvent & event)
{
}

void wxGoogleMap::OnLoaded(wxWebViewEvent & event)
{
    wxString const REQUEST_URL = event.GetURL();

    if (REQUEST_URL.Lower() == ABOUT_BLANK_URL) {
        // EMPTY.
    } else if (REQUEST_URL.Lower() == wxGoogleMapDefaultUrl) {
        _first_load.store(true);
    }

    // wxLogMessage(wxT("wxGoogleMap::OnLoaded(") + REQUEST_URL + wxT(")"));
}

void wxGoogleMap::OnTitleChanged(wxWebViewEvent & event)
{
}

void wxGoogleMap::OnError(wxWebViewEvent & event)
{
    wxLogMessage(wxT("wxGoogleMap::OnError()"));
}

// -----------------
// User's operators.
// -----------------

wxString wxGoogleMap::getDomResult()
{
    wxLogMessage(__impl::getDomResult(_web->GetPageSource()));
    //return __impl::readMapResult(_web->GetPageSource());
    return "";
}

std::size_t wxGoogleMap::getMarkerCount()
{
//    assert(_web != nullptr);
//    _web->RunScript(wxT("javascript:updateMarkerCount();"));
//
//    auto element = __impl::readMapResult(_web->GetPageSource());
//    element->FirstChildElement("");
    return 0;
}

