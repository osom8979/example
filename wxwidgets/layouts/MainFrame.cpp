/**
 * @file   MainFrame.cpp
 * @brief  wxWidgets main frame implementation.
 * @author zer0
 * @date   2017-04-26
 */

#include <MainFrame.hpp>

MainFrame::MainFrame(wxString const & title) : wxFrame(nullptr, wxID_ANY, title)
{
    Centre();
}

MainFrame::~MainFrame()
{
    // EMPTY.
}

