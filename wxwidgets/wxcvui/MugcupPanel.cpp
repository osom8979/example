/**
 * @file   MugcupPanel.cpp
 * @brief  MugcupPanel class prototype.
 * @author zer0
 * @date   2016-08-04
 */

#include <MugcupPanel.hpp>
#include <wx/dcbuffer.h>

#include <cvui/cvui.h>

IMPLEMENT_CLASS(MugcupPanel, wxPanel);

BEGIN_EVENT_TABLE(MugcupPanel, wxPanel)
EVT_PAINT(MugcupPanel::onPaint)
END_EVENT_TABLE()

MugcupPanel::MugcupPanel(wxWindow      * parent
                         , wxWindowID      id
                         , wxPoint const & pos
                         , wxSize  const & size)
        : wxPanel(parent, id, pos, size)
{
    SetBackgroundColour(wxColour("black"));
}

MugcupPanel::~MugcupPanel()
{
}

// --------------
// Event methods.
// --------------

void MugcupPanel::onPaint(wxPaintEvent & event)
{
    wxBufferedPaintDC dc(this);
    dc.Clear();

    cv::Mat image(600, 480, CV_8UC3, cv::Scalar(49, 52, 49));
    dc.DrawBitmap(wxBitmap(wxImage(image.cols, image.rows, image.data, true)), 0, 0);
}

