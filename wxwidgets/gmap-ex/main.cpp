/**
 * @file   main.cpp
 * @brief  Program entry-point.
 * @author zer0
 * @date   2017-02-17
 */

#include <wx/wx.h>
#include <wx/menu.h>
#include <wxGoogleMap.h>

#define TITLE "wxGoogleMap"

class MainFrame : public wxFrame
{
public:
    enum {
        wxID_RELOAD = 99999,
        wxID_PATHS_COUNT,
        wxID_DOM_RESULT,
        wxID_TOAST,
        wxID_SHOW_DEBUG,
    };

private:
    wxMenuBar * _menu;
    wxMenu    * _menu_file;
    wxMenu    * _menu_map;

    wxBoxSizer  * _sizer;
    wxGoogleMap * _map;
    wxTextCtrl  * _text;
    wxLogTextCtrl * _log;

private:
    void InitMenu()
    {
        _menu = new wxMenuBar();
        _menu_file = new wxMenu();
        _menu_file->Append(wxID_OPEN, wxT("&Open"));
        _menu_file->AppendSeparator();
        _menu_file->Append(wxID_RELOAD, wxT("&Reload"));
        _menu_file->Append(wxID_EXIT, wxT("&Quit"));
        _menu->Append(_menu_file, wxT("&File"));
        _menu_map = new wxMenu();
        _menu_map->Append(wxID_PATHS_COUNT, wxT("Path Count"));
        _menu_map->Append(wxID_DOM_RESULT, wxT("Dom result"));
        _menu_file->AppendSeparator();
        _menu_map->Append(wxID_TOAST, wxT("Toast"));
        _menu_file->AppendSeparator();
        _menu_map->AppendCheckItem(wxID_SHOW_DEBUG, wxT("Show Debug Dialog"));
        _menu->Append(_menu_map, wxT("&Map"));
        SetMenuBar(_menu);

        Connect(wxID_OPEN  , wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnOpen));
        Connect(wxID_RELOAD, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnReload));
        Connect(wxID_EXIT  , wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnQuit));

        Connect(wxID_PATHS_COUNT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnPathsCount));
        Connect(wxID_DOM_RESULT , wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnDomResult));
        Connect(wxID_TOAST      , wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnToast));
        Connect(wxID_SHOW_DEBUG , wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnShowDebug));
    }

public:
    MainFrame() : wxFrame(NULL, wxID_ANY, wxT(TITLE), wxDefaultPosition, wxSize(800, 600))
    {
        InitMenu();

        _text = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
        _log  = new wxLogTextCtrl(_text);
        wxLog::SetActiveTarget(_log);

        _map = new wxGoogleMap(this, wxID_ANY);
        _sizer = new wxBoxSizer(wxVERTICAL);

        _sizer->Add( _map, wxSizerFlags().Expand().Proportion(4));
        _sizer->Add(_text, wxSizerFlags().Expand().Proportion(1));
        SetSizer(_sizer);

        Centre();
        Show();
    }

public:
    void OnOpen(wxCommandEvent & WXUNUSED(event))
    {
        wxFileDialog openFileDialog(this);
        if (openFileDialog.ShowModal() == wxID_OK){
            _map->RunFile(openFileDialog.GetPath());
        }
    }

    void OnReload(wxCommandEvent & WXUNUSED(event))
    {
        _map->Reload();
    }

    void OnQuit(wxCommandEvent & WXUNUSED(event))
    {
        Close(true);
    }

    void OnPathsCount(wxCommandEvent & WXUNUSED(event))
    {
        wxMessageBox(wxString::Format(wxT("Count is %d"), _map->GetPathsCount()), wxT("Paths Count"));
    }

    void OnDomResult(wxCommandEvent & WXUNUSED(event))
    {
        wxMessageBox(_map->GetDomResult(), wxT("Dom JSON Result"));
    }

    void OnToast(wxCommandEvent & event)
    {
        _map->Toast(wxT("Test toast message"));
    }

    void OnShowDebug(wxCommandEvent & event)
    {
        _map->ShowDebuggingPopup(event.IsChecked());
    }
};

class MapApp : public wxApp
{
private:
    MainFrame * _frame;

public:
    virtual bool OnInit() wxOVERRIDE
    {
        _frame = new MainFrame();
        return true;
    }
};

// IMPLEMENT_CLASS(Application, wxApp)
IMPLEMENT_APP(MapApp);

