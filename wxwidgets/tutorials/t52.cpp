#include "tutorial.h"
#include "manager.h"

class MyFrameT52: public wxFrame
{
public:
    MyFrameT52(wxWindow* parent) :
            wxFrame(parent, -1, wxT("wxAUI Test"), wxDefaultPosition, wxSize(800, 600),
            wxDEFAULT_FRAME_STYLE)
    {
        // notify wxAUI which frame to use
        // m_mgr.SetManagedWindow(this);
        m_mgr.SetFrame(this);

        // create several text controls
        wxTextCtrl* text1 = new wxTextCtrl(this, -1, wxT("Pane 1 - sample text"), wxDefaultPosition,
                wxSize(200, 150),
                wxNO_BORDER | wxTE_MULTILINE);

        wxTextCtrl* text2 = new wxTextCtrl(this, -1, wxT("Pane 2 - sample text"), wxDefaultPosition,
                wxSize(200, 150),
                wxNO_BORDER | wxTE_MULTILINE);

        wxTextCtrl* text3 = new wxTextCtrl(this, -1, wxT("Main content window"), wxDefaultPosition,
                wxSize(200, 150),
                wxNO_BORDER | wxTE_MULTILINE);

        // add the panes to the manager
        m_mgr.AddPane(text1, wxLEFT, wxT("Pane Number One"));
        m_mgr.AddPane(text2, wxBOTTOM, wxT("Pane Number Two"));
        m_mgr.AddPane(text3, wxCENTER);

        // tell the manager to "commit" all the changes just made
        m_mgr.Update();
    }

    ~MyFrameT52()
    {
        // deinitialize the frame manager
        m_mgr.UnInit();
    }

private:
    wxFrameManager m_mgr;
};

int t52()
{
    MyFrameT52 * frame = new MyFrameT52(NULL);
    // SetTopWindow(frame);
    frame->Show();
    return EXIT_SUCCESS;
}

