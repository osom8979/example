/**
 * @file   MainFrame.hpp
 * @brief  wxWidgets main frame.
 * @author zer0
 * @date   2016-08-04
 */

#ifndef __INCLUDE_LIBMUGCUP__WXBARISTA_MAINFRAME_HPP__
#define __INCLUDE_LIBMUGCUP__WXBARISTA_MAINFRAME_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <wx/wx.h>
#include <wx/aui/aui.h>
#include <wxbarista/PreviewPanel.hpp>

/**
 * List of control id of wxWidgets.
 *
 * @author zer0
 * @date   2016-08-04
 */
enum IdTable
{
    ID_MAINFRAME = 10000, ///< MainFrame ID.
    ID_PREVIEWPANEL,
    ID_MENU__FILE_OPEN,
    ID_MENU__FILE_EXIT
};

/**
 * wxWidgets main frame.
 *
 * @author zer0
 * @date   2016-08-04
 */
class MainFrame : public wxFrame
{
public:
    wxDECLARE_EVENT_TABLE();

private:
    wxAuiManager _aui_manager;
    PreviewPanel * _preview;

public:
    MainFrame(const wxString& title);
    ~MainFrame();

private:
    void createMenu();

// EVENT methods.
protected:
    /** Window 'CLOSE' EVENT notification method. */
    void onClose(wxCloseEvent & event);
    void onDestroy(wxWindowDestroyEvent & event);
    void onKeyDown(wxKeyEvent & event);

// MENU events.
protected:
    void onMenuFileOpen(wxCommandEvent & event);
    void onMenuFileExit(wxCommandEvent & event);

public:
    /** showing the exit program question message box. */
    static int showExitMessageBox(wxWindow * parent);
};

#endif // __INCLUDE_LIBMUGCUP__WXBARISTA_MAINFRAME_HPP__

