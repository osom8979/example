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
#include <utility>
#include <iostream>

#include <wx/webview.h>
#include <wx/regex.h>

#define ENABLE_WX_LOGGING

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
        "        function initGoogleMap() {"
        "            var options = {"
        "                zoom: 13,"
        "                center: new google.maps.LatLng(g_first_position),"
        "                mapTypeId : google.maps.MapTypeId.ROADMAP"
        "            };"
        "            g_map = new google.maps.Map(document.getElementById('map_canvas'), options);"
        "        }"
        "    </script>"
        "    <style type='text/css'>"
        "        body {"
        "            height: 100%;"
        "            margin: 0;"
        "        }"
        "        .map {"
        "            width: 100%;"
        "            height: 100%;"
        "        }"
        "    </style>"
        "</head>"
        "<body>"
        "    <div id='map_canvas' class='map'></div>"
        "</body>"
        "</html>";
// @formatter:on

static char const * const TAG_DELIMITER = "/";
static char const * const ID_MAP_RESULT = DEFAULT_ID_MAP_RESULT;

static char const * const ABOUT_BLANK_URL = "about:blank";

static char const * const WX_GOOGLE_MAP_DEFAULT_URL = "file://wxgooglemap/";
static wxWindowID   const WX_GOOGLE_MAP_WEBVIEW_ID  = 10001;

// ---------------
namespace __impl {
// ---------------

static std::vector<std::string> splitTokens(std::string const & source, std::string const & delimiter)
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

// ------------------
} // namespace __impl
// ------------------

wxBEGIN_EVENT_TABLE(wxGoogleMap, wxPanel)
    EVT_WEBVIEW_NAVIGATING(WX_GOOGLE_MAP_WEBVIEW_ID, wxGoogleMap::OnNavigating)
    EVT_WEBVIEW_NAVIGATED(WX_GOOGLE_MAP_WEBVIEW_ID, wxGoogleMap::OnNavigated)
    EVT_WEBVIEW_LOADED(WX_GOOGLE_MAP_WEBVIEW_ID, wxGoogleMap::OnLoaded)
    EVT_WEBVIEW_TITLE_CHANGED(WX_GOOGLE_MAP_WEBVIEW_ID, wxGoogleMap::OnTitleChanged)
    EVT_WEBVIEW_ERROR(WX_GOOGLE_MAP_WEBVIEW_ID, wxGoogleMap::OnError)
wxEND_EVENT_TABLE()

wxGoogleMap::wxGoogleMap() : wxPanel(), _callback(nullptr)
{
    Init();
}

wxGoogleMap::wxGoogleMap(wxWindow * parent,
                         wxWindowID winid,
                         wxPoint const & pos,
                         wxSize const & size,
                         long style,
                         wxString const & name)
        : wxPanel(parent, winid, pos, size, style, name), _callback(nullptr)
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

    _web = wxWebView::New(this, WX_GOOGLE_MAP_WEBVIEW_ID, ABOUT_BLANK_URL);
    _web->SetPage(wxString(DEFAULT_HTML), wxString(WX_GOOGLE_MAP_DEFAULT_URL));
    _web->EnableContextMenu(false);

    _sizer = new wxBoxSizer(wxVERTICAL);
    _sizer->Add(_web, wxSizerFlags().Expand().Proportion(1));

    SetSizer(_sizer);
    Centre();
}

void wxGoogleMap::OnNavigating(wxWebViewEvent & event)
{
    LogMessage(wxT("wxGoogleMap::OnNavigating(") + event.GetTarget() + wxT(",") + event.GetURL() + wxT(")"));
    if (_callback != nullptr && _callback->OnNavigating(event)) {
        return;
    }

    wxString const REQUEST_URL = event.GetURL().Lower();
    if (REQUEST_URL.StartsWith("request:")) {
        wxLogMessage(wxT("wxGoogleMap::OnNavigating(") + REQUEST_URL + wxT(")"));
        event.Veto();
    }
}

void wxGoogleMap::OnNavigated(wxWebViewEvent & event)
{
    LogMessage(wxT("wxGoogleMap::OnNavigated(") + event.GetTarget() + wxT(",") + event.GetURL() + wxT(")"));
    if (_callback != nullptr && _callback->OnNavigated(event)) {
        return;
    }
}

void wxGoogleMap::OnLoaded(wxWebViewEvent & event)
{
    LogMessage(wxT("wxGoogleMap::OnLoaded(") + event.GetTarget() + wxT(",") + event.GetURL() + wxT(")"));
    if (_callback != nullptr && _callback->OnLoaded(event)) {
        return;
    }

    wxString const REQUEST_URL = event.GetURL();
    if (REQUEST_URL.Lower() == ABOUT_BLANK_URL) {
        // EMPTY.
    } else {
        _first_load.store(true);
        JsLogMessage(wxT("wxGoogleMap::OnLoaded(") + event.GetURL() + wxT(")"));
    }
}

void wxGoogleMap::OnTitleChanged(wxWebViewEvent & event)
{
    LogMessage(wxT("wxGoogleMap::OnTitleChanged(") + event.GetTarget() + wxT(", ") + event.GetURL() + wxT(")"));
    if (_callback != nullptr && _callback->OnTitleChanged(event)) {
        return;
    }
}

void wxGoogleMap::OnError(wxWebViewEvent & event)
{
    LogMessage(wxT("wxGoogleMap::OnError(") + event.GetTarget() + wxT(", ") + event.GetURL() + wxT(")"));
    if (_callback != nullptr && _callback->OnError(event)) {
        return;
    }
}

void wxGoogleMap::Reload()
{
    _web->Reload(wxWEBVIEW_RELOAD_DEFAULT);
}

void wxGoogleMap::RunScript(wxString const & script)
{
    _web->RunScript(script);
}

void wxGoogleMap::RunUrl(wxString const & url)
{
    _web->LoadURL(url);
}

void wxGoogleMap::RunHtml(wxString const & html, wxString const & scheme)
{
    _web->SetPage(html, scheme);
}

void wxGoogleMap::RunFile(wxString const & path)
{
    _web->LoadURL(wxString(wxT("file://")) + path);
}

// -----------------
// User's operators.
// -----------------

void wxGoogleMap::ShowDebuggingPopup(bool enable)
{
    _web->RunScript(wxString::Format(wxT("javascript:showDebuggingPopup(%s);"), enable ? wxT("true") : wxT("false")));
}

wxString wxGoogleMap::GetDomResult()
{
    _web->SetFocus(); // IMPORTANT !!
    _web->RunScript(wxT("javascript:updateDom();"));
    //_web->Update();
    //_web->Refresh();

    return _web->GetSelectedText();
}

std::size_t wxGoogleMap::GetPathsCount()
{
    Json::Value root;
    Json::Reader reader;
    if (reader.parse(GetDomResult().ToStdString(), root)) {
        return root["paths"].size();
    }
    return 0;
}

// ---------------
// Static methods.
// ---------------

void wxGoogleMap::LogMessage(wxString const & message)
{
#if defined(ENABLE_WX_LOGGING)
    wxLogMessage(message);
#else
    std::cout << message.ToStdString() << std::endl;
#endif
}

void wxGoogleMap::JsLogMessage(wxString const & message)
{
    _web->RunScript(wxString::Format(wxT("javascript:appendLogMessage('%s');"), message));
}

void wxGoogleMap::Toast(wxString const & message)
{
    _web->RunScript(wxString::Format(wxT("javascript:toast('%s');"), message));
}

wxString wxGoogleMap::GetDomResult(wxString const & full_html)
{
    static wxChar const * REGEX_DIV_OPEN_PREFIX = wxT(R"(<[Dd][Ii][Vv][ ]*[Ii][Dd][ ]*=[ ]*["'])");
    static wxChar const * REGEX_DIV_OPEN_SUFFIX = wxT(R"(["'][^>]*>)");
    static wxChar const * REGEX_DIV_CLOSE       = wxT(R"(</[Dd][Ii][Vv]>)");

    wxRegEx  open_regex;
    wxRegEx close_regex;

    open_regex.Compile(
            wxString(REGEX_DIV_OPEN_PREFIX) +
            wxString(wxT(DEFAULT_ID_MAP_RESULT)) +
            wxString(REGEX_DIV_OPEN_SUFFIX));
    close_regex.Compile(wxString(REGEX_DIV_CLOSE));

    std::size_t  open_index(0),  open_length(0);
    std::size_t close_index(0), close_length(0);

    if (open_regex.Matches(full_html)) {
        open_regex.GetMatch(&open_index, &open_length, 0);
    }

    wxString const SUBSTR = full_html.SubString(open_index + open_length, full_html.Length() - 1);
    if (close_regex.Matches(SUBSTR)) {
        close_regex.GetMatch(&close_index, &close_length, 0);
    }

    return SUBSTR.SubString(0, close_index - 1).Trim();
}

