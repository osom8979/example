/**
 * @file   MainFrame.cpp
 * @brief  MainFrame class implementation.
 * @author zer0
 * @date   2017-04-01
 */

#include <dsn/MainFrame.hpp>
#include <dsn/Downloader.hpp>
#include <wx/sstream.h>

static bool isDebugMode()
{
#if defined(NDEBUG)
    return false;
#else
    return true;
#endif
}

enum {
    DSN_ID_MAINFRAME = 9999,

    DSN_ID_PANEL_CANVAS,
    DSN_ID_PANEL_BTNS,
    DSN_ID_BUTTON_OK,
    DSN_ID_BUTTON_CANCEL,

    DSN_ID_MENU_FILE_EXIT,

    DSN_ID_PROCESS,
};

static int const DEFAULT_BORDER_PIXEL = 5;

wxDEFINE_EVENT(PROGRESS_SET, wxCommandEvent);
wxDEFINE_EVENT(PROGRESS_RUN, wxCommandEvent);
wxDEFINE_EVENT(PROGRESS_END, wxCommandEvent);
wxDEFINE_EVENT(PROGRESS_ERR, wxCommandEvent);

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_CLOSE(MainFrame::onClose)
    EVT_WINDOW_DESTROY(MainFrame::onDestroy)
    EVT_KEY_DOWN(MainFrame::onKeyDown)

    EVT_BUTTON(DSN_ID_BUTTON_OK, MainFrame::onClickOk)
    EVT_BUTTON(DSN_ID_BUTTON_CANCEL, MainFrame::onClickCancel)
    EVT_MENU(DSN_ID_MENU_FILE_EXIT, MainFrame::onMenuFileExit)

    EVT_END_PROCESS(DSN_ID_PROCESS, MainFrame::onEndProcess)

    EVT_COMMAND(DSN_ID_MAINFRAME, PROGRESS_SET, MainFrame::onProgressSet)
    EVT_COMMAND(DSN_ID_MAINFRAME, PROGRESS_RUN, MainFrame::onProgressRun)
    EVT_COMMAND(DSN_ID_MAINFRAME, PROGRESS_END, MainFrame::onProgressEnd)
    EVT_COMMAND(DSN_ID_MAINFRAME, PROGRESS_ERR, MainFrame::onProgressErr)
wxEND_EVENT_TABLE()

MainFrame::MainFrame() : wxFrame(), _process(this, DSN_ID_PROCESS), _process_id(0)
{
    initConfig();

    wxString name = _config.get_app_name();
    int width  = _config.get_app_width();
    int height = _config.get_app_height();

    wxString type  = _config.get_view_type().Lower();
    wxString image = _config.env_view_image();

    //long const STYLE = wxDEFAULT_FRAME_STYLE;
    long const STYLE = wxFRAME_NO_TASKBAR;

    wxFrame::Init();
    wxFrame::Create(nullptr, DSN_ID_MAINFRAME, name, wxDefaultPosition, wxSize(width, height), STYLE, wxFrameNameStr);

    initMenu();
    //initSplash();

    _panel  = new Preview(image, this, DSN_ID_PANEL_CANVAS);
    _btns   = new wxPanel(this, DSN_ID_PANEL_BTNS);
    _ok     = new wxButton(_btns, DSN_ID_BUTTON_OK, wxT("OK"));
    _cancel = new wxButton(_btns, DSN_ID_BUTTON_CANCEL, wxT("CANCEL"));
    _panel->SetBackgroundColour(wxColor(wxT("Gray")));

    wxBoxSizer * btns_sizer = new wxBoxSizer(wxHORIZONTAL);
    btns_sizer->Add(_ok);
    btns_sizer->Add(DEFAULT_BORDER_PIXEL, 0);
    btns_sizer->Add(_cancel);
    _btns->SetSizer(btns_sizer);

    wxBoxSizer * sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(_panel, wxSizerFlags().Expand().Proportion(1));
    sizer->Add(_btns , wxSizerFlags().CenterHorizontal().Border(wxALL, DEFAULT_BORDER_PIXEL));
    SetSizer(sizer);

    Center();
    Show();
}

MainFrame::~MainFrame()
{
    // EMPTY.
}

void MainFrame::initConfig()
{
    wxString const CONFIG_PATH = Config::getDefaultPath();
    if (Config::exists(CONFIG_PATH)) {
        _config.read(CONFIG_PATH);
    } else {
        _config.setDefault();
        _config.save(CONFIG_PATH, isDebugMode());
    }

    if (_config.get_app_debug()) {
        Config::print();
    }
}

void MainFrame::initMenu()
{
    wxMenuBar * menu = new wxMenuBar();
    wxMenu * file = new wxMenu();
    file->Append(DSN_ID_MENU_FILE_EXIT, wxT("E&xit"));
    menu->Append(file, wxT("&File"));
    SetMenuBar(menu);
}

void MainFrame::initSplash()
{
    // splash screen must not be used as parent by the other windows because it
    // is going to disappear soon, indicate it by giving it this special style
    SetExtraStyle(GetExtraStyle() | wxWS_EX_TRANSIENT);

#if defined(__WXGTK20__)
    gtk_window_set_type_hint(GTK_WINDOW(m_widget), GDK_WINDOW_TYPE_HINT_SPLASHSCREEN);
#endif
}

// --------------
// Event methods.
// --------------

void MainFrame::onClose(wxCloseEvent & event)
{
    if (event.CanVeto()) {
        if (showExitMessageBox() != wxID_YES) {
            event.Veto();
            return;
        }
    }

    // you may also do: event.Skip();
    // since the default event handler does call Destroy(), too
    Destroy();
}

void MainFrame::onDestroy(wxWindowDestroyEvent & event)
{
    DestroyChildren();
}

void MainFrame::onKeyDown(wxKeyEvent & event)
{
    switch (event.GetKeyCode()) {
    case WXK_ESCAPE:
        Close(true);
        break;

    default:
        break;
    }
}

void MainFrame::onClickOk(wxCommandEvent & event)
{
    // wxString const EXE_FILE = _config.env_exec_file();
    // _process_id = wxExecute(EXE_FILE, wxEXEC_ASYNC, &_process, &_envs);
    // if (_process_id != 0) {
    //     Hide();
    // } else if (_config.get_app_debug()) {
    //     wxLogMessage(wxT("Execute process(%s) failure."), EXE_FILE);
    // }

    Downloader down(this, 0, _config.get_update_base(), _config.get_update_ver_xml());
    if (down.connect()) {
        wxStringOutputStream sos;
        if (down.request(sos)) {
            wxLogMessage(wxT("Request success: %s"), sos.GetString());
        } else {
            wxLogMessage(wxT("Request error."));
        }
    } else {
        wxLogMessage(wxT("Connect error."));
    }
}

void MainFrame::onClickCancel(wxCommandEvent & event)
{
    if (showExitMessageBox() == wxID_YES) {
        Close(true);
    }
}

void MainFrame::onMenuFileExit(wxCommandEvent & event)
{
    if (showExitMessageBox() == wxID_YES) {
        Close(true);
    }
}

void MainFrame::onEndProcess(wxProcessEvent & event)
{
    if (event.GetExitCode() == 0) {
        Destroy();
    }

    Show();
    Raise();
    SetFocus();
}

void MainFrame::onProgressSet(wxCommandEvent & event)
{
    // EMPTY.
}

void MainFrame::onProgressRun(wxCommandEvent & event)
{
    // EMPTY.
}

void MainFrame::onProgressEnd(wxCommandEvent & event)
{
    // EMPTY.
}

void MainFrame::onProgressErr(wxCommandEvent & event)
{
    // EMPTY.
}

void MainFrame::postProgressSet(int type, wxString const & msg)
{
    wxCommandEvent event(PROGRESS_SET, DSN_ID_MAINFRAME);
    event.SetInt(type);
    event.SetString(msg);
    wxPostEvent(this, event);
}

void MainFrame::postProgressRun(int type, wxString const & msg, int progress)
{
    wxCommandEvent event(PROGRESS_RUN, DSN_ID_MAINFRAME);
    event.SetInt(type);
    event.SetString(msg);
    event.SetExtraLong(progress);
    wxPostEvent(this, event);
}

void MainFrame::postProgressEnd(int type, wxString const & msg)
{
    wxCommandEvent event(PROGRESS_END, DSN_ID_MAINFRAME);
    event.SetInt(type);
    event.SetString(msg);
    wxPostEvent(this, event);
}

void MainFrame::postProgressErr(int type, wxString const & msg, int error)
{
    wxCommandEvent event(PROGRESS_ERR, DSN_ID_MAINFRAME);
    event.SetInt(type);
    event.SetString(msg);
    event.SetExtraLong(error);
    wxPostEvent(this, event);
}

int MainFrame::showExitMessageBox()
{
    wxString const MESSAGE = wxT("Are you sure to quit?");
    wxString const CAPTION = wxT("Question");
    long const STYLE = wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION;

    wxMessageDialog dialog(this, MESSAGE, CAPTION, STYLE);
    int const RESULT = dialog.ShowModal();
    dialog.Destroy();
    return RESULT;
}

