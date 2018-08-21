/**
 * @file   MainFrame.hpp
 * @brief  MainFrame class prototype.
 * @author zer0
 * @date   2017-04-01
 */

#ifndef __INCLUDE_DSN__MAINFRAME_HPP__
#define __INCLUDE_DSN__MAINFRAME_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <wx/wx.h>
#include <wx/utils.h>
#include <wx/process.h>

#include <dsn/Config.hpp>
#include <dsn/Preview.hpp>

/**
 * MainFrame class prototype.
 *
 * @author zer0
 * @date   2017-04-01
 */
class MainFrame : public wxFrame
{
public:
    wxDECLARE_NO_COPY_CLASS(MainFrame);
    wxDECLARE_EVENT_TABLE();

private:
    Config _config;

private:
    Preview * _panel;
    wxPanel * _btns;

    wxButton * _ok;
    wxButton * _cancel;

private:
    wxProcess _process;
    wxExecuteEnv _envs;
    long _process_id;

public:
    MainFrame();
    virtual ~MainFrame();

private:
    void initConfig();
    void initMenu();
    void initSplash();

// Events.
protected:
    void onClose(wxCloseEvent & event);
    void onDestroy(wxWindowDestroyEvent & event);
    void onKeyDown(wxKeyEvent & event);

protected:
    void onClickOk(wxCommandEvent & event);
    void onClickCancel(wxCommandEvent & event);
    void onMenuFileExit(wxCommandEvent & event);
    void onEndProcess(wxProcessEvent & event);

    void onProgressSet(wxCommandEvent & event);
    void onProgressRun(wxCommandEvent & event);
    void onProgressEnd(wxCommandEvent & event);
    void onProgressErr(wxCommandEvent & event);

public:
    void postProgressSet(int type, wxString const & msg);
    void postProgressRun(int type, wxString const & msg, int progress);
    void postProgressEnd(int type, wxString const & msg);
    void postProgressErr(int type, wxString const & msg, int error);

public:
    /** showing the exit program question message box. */
    int showExitMessageBox();
};

#endif // __INCLUDE_DSN__MAINFRAME_HPP__

