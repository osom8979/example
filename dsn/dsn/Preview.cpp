/**
 * @file   Preview.cpp
 * @brief  Preview class implementation.
 * @author zer0
 * @date   2017-04-02
 */

#include <dsn/Preview.hpp>
#include <wx/dcbuffer.h>
#include <wx/image.h>

#ifndef WX_PRECOMP
#include <wx/dcmemory.h>
#include <wx/dcclient.h>
#endif

wxIMPLEMENT_CLASS(Preview, wxPanel);

wxBEGIN_EVENT_TABLE(Preview, wxPanel)
    EVT_PAINT(Preview::onPaint)
    EVT_ERASE_BACKGROUND(Preview::OnEraseBackground)
wxEND_EVENT_TABLE()

Preview::Preview(wxString const & image_path,
                 wxWindow * parent,
                 wxWindowID winid,
                 wxPoint const & pos,
                 wxSize const & size,
                 long style,
                 wxString const & name)
        : wxPanel(parent, winid, pos, size, style, name)
{
    SetBackgroundColour(wxColour(wxT("black")));

    wxImage image;
    if (image.LoadFile(image_path)) {
        _bitmap = wxBitmap(image);
    }
}

Preview::~Preview()
{
    // EMPTY.
}

void Preview::onPaint(wxPaintEvent & event)
{
    wxBufferedPaintDC dc(this);
    dc.Clear();

    if (_bitmap.IsOk()) {
        Preview::draw(dc, _bitmap, 0, 0);
    }
}

void Preview::OnEraseBackground(wxEraseEvent & event)
{
    if (event.GetDC() && _bitmap.IsOk()) {
        Preview::draw(*event.GetDC(), _bitmap, 0, 0);
    } else {
        wxClientDC dc(this);
        if (_bitmap.IsOk()) {
            Preview::draw(dc, _bitmap, 0, 0);
        }
    }
}

#if !defined(__WXGTK__) && wxUSE_PALETTE
# define USE_PALETTE_IN_SPLASH
#endif

void Preview::draw(wxDC & dc, wxBitmap const & bitmap, int WXUNUSED(x), int WXUNUSED(y))
{
    wxMemoryDC memdc;

#ifdef USE_PALETTE_IN_SPLASH
    bool is_high_color = (wxDisplayDepth() >= 16) ;
    if (bitmap.GetPalette() && is_high_color == false) {
        memdc.SetPalette(* bitmap.GetPalette());
    }
#endif

    memdc.SelectObjectAsSource(bitmap);
    dc.Blit(0, 0, bitmap.GetWidth(), bitmap.GetHeight(), &memdc, 0, 0, wxCOPY, true /* use mask */);
    memdc.SelectObject(wxNullBitmap);

#ifdef USE_PALETTE_IN_SPLASH
    if (bitmap.GetPalette() && is_high_color == false) {
        memdc.SetPalette(wxNullPalette);
    }
#endif
}

