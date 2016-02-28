#include "tutorial.h"

// wxNotebook following style flags:
// wxNB_LEFT
// wxNB_RIGHT
// wxNB_TOP
// wxNB_BOTTOM

class MyFrameT39: public wxFrame
{
public:
    MyFrameT39(const wxString& title);

    void OnQuit(wxCommandEvent& event);
};

class MyGrid: public wxGrid
{
public:
    MyGrid(wxNotebook *parent);
};

MyFrameT39::MyFrameT39(const wxString& title) :
        wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(400, 350))
{
    wxNotebook *nb = new wxNotebook(this, -1, wxPoint(-1, -1), wxSize(-1, -1), wxNB_BOTTOM);

    wxMenuBar *menubar = new wxMenuBar;
    wxMenu *file = new wxMenu;
    file->Append(wxID_EXIT, wxT("Quit"), wxT(""));
    menubar->Append(file, wxT("&File"));
    SetMenuBar(menubar);

    Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyFrameT39::OnQuit));

    MyGrid *grid1 = new MyGrid(nb);
    MyGrid *grid2 = new MyGrid(nb);
    MyGrid *grid3 = new MyGrid(nb);

    nb->AddPage(grid1, wxT("Sheet1"));
    nb->AddPage(grid2, wxT("Sheet2"));
    nb->AddPage(grid3, wxT("Sheet3"));

    CreateStatusBar();
    Center();
}

void MyFrameT39::OnQuit(wxCommandEvent& event)
{
    Close(true);
}

MyGrid::MyGrid(wxNotebook * parent) :
        wxGrid(parent, wxID_ANY)
{
    CreateGrid(30, 30);
    SetRowLabelSize(50);
    SetColLabelSize(25);
    SetRowLabelAlignment(wxALIGN_RIGHT, wxALIGN_CENTRE);
    SetLabelFont(wxFont(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    for (int i = 0; i < 30; i++) {
        this->SetRowSize(i, 25);
    }
}

int t39()
{
    MyFrameT39 * frame = new MyFrameT39(wxT("MyFrame"));
    frame->Show(true);
    return EXIT_SUCCESS;
}

