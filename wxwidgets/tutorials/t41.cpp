#include "tutorial.h"

// We deal with the following objects:
// Data object
// Data source
// Data target

class MyFrameT41: public wxFrame
{
public:
    MyFrameT41(const wxString& title);

    void OnSelect(wxCommandEvent& event);
    void OnDragInit(wxListEvent& event);

    wxGenericDirCtrl *m_gdir;
    wxListCtrl *m_lc1;
    wxListCtrl *m_lc2;
};

class MyTextDropTarget: public wxTextDropTarget
{
public:
    MyTextDropTarget(wxListCtrl *owner);

    virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data);

    wxListCtrl *m_owner;
};

MyFrameT41::MyFrameT41(const wxString& title) :
        wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(300, 200))
{
    wxSplitterWindow *spl1 = new wxSplitterWindow(this, -1);
    wxSplitterWindow *spl2 = new wxSplitterWindow(spl1, -1);
    m_gdir = new wxGenericDirCtrl(spl1, -1, wxT("/home/"), wxPoint(-1, -1), wxSize(-1, -1),
            wxDIRCTRL_DIR_ONLY);

    m_lc1 = new wxListCtrl(spl2, -1, wxPoint(-1, -1), wxSize(-1, -1), wxLC_LIST);
    m_lc2 = new wxListCtrl(spl2, -1, wxPoint(-1, -1), wxSize(-1, -1), wxLC_LIST);

    MyTextDropTarget *mdt = new MyTextDropTarget(m_lc2);
    m_lc2->SetDropTarget(mdt);

    Connect(m_lc1->GetId(), wxEVT_COMMAND_LIST_BEGIN_DRAG,
            wxListEventHandler(MyFrameT41::OnDragInit));

    wxTreeCtrl *tree = m_gdir->GetTreeCtrl();

    spl2->SplitHorizontally(m_lc1, m_lc2);
    spl1->SplitVertically(m_gdir, spl2);

    Connect(tree->GetId(), wxEVT_COMMAND_TREE_SEL_CHANGED,
            wxCommandEventHandler(MyFrameT41::OnSelect));

    Center();
}

MyTextDropTarget::MyTextDropTarget(wxListCtrl *owner)
{
    m_owner = owner;
}

bool MyTextDropTarget::OnDropText(wxCoord x, wxCoord y, const wxString& data)
{
    m_owner->InsertItem(0, data);
    return true;
}

void MyFrameT41::OnSelect(wxCommandEvent& event)
{
    wxString filename;
    wxString path = m_gdir->GetPath();
    wxDir dir(path);

    bool cont = dir.GetFirst(&filename, wxEmptyString, wxDIR_FILES);

    int i = 0;

    m_lc1->ClearAll();
    m_lc2->ClearAll();

    while (cont) {
        m_lc1->InsertItem(i, filename);
        cont = dir.GetNext(&filename);
        i++;
    }
}

void MyFrameT41::OnDragInit(wxListEvent& event)
{
    wxString text = m_lc1->GetItemText(event.GetIndex());

    wxTextDataObject tdo(text);
    wxDropSource tds(tdo, m_lc1);
    tds.DoDragDrop(wxDrag_CopyOnly);
}

int t41()
{
    MyFrameT41 * frame = new MyFrameT41(wxT("MyFrame"));
    frame->Show(true);
    return EXIT_SUCCESS;
}

