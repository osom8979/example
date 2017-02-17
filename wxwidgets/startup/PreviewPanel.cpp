/**
 * @file   PreviewPanel.cpp
 * @brief  PreviewPanel class prototype.
 * @author zer0
 * @date   2016-08-04
 */

#include <PreviewPanel.hpp>
#include <wx/dcbuffer.h>

IMPLEMENT_CLASS(PreviewPanel, wxPanel);

BEGIN_EVENT_TABLE(PreviewPanel, wxPanel)
EVT_PAINT(PreviewPanel::onPaint)
END_EVENT_TABLE()

PreviewPanel::PreviewPanel(wxWindow      * parent
                         , wxWindowID      id
                         , wxPoint const & pos
                         , wxSize  const & size)
        : wxPanel(parent, id, pos, size)
{
    SetBackgroundColour(wxColour("black"));
}

PreviewPanel::~PreviewPanel()
{
}

// --------------
// Event methods.
// --------------

void PreviewPanel::onPaint(wxPaintEvent & event)
{
    wxBufferedPaintDC dc(this);
    dc.Clear();
}

