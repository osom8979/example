/**
 * @file   MainFrame.cpp
 * @brief  wxWidgets main frame implementation.
 * @author zer0
 * @date   2017-04-26
 */

#include <MainFrame.hpp>

#include <wx/stattext.h>
#include <wx/statline.h>

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, wxT("Layouts"))
{
    int const BORDER_IN_PIXELS = 6;
    int const RADIO_HORIZONTAL_SPACE  = 20;
    int const BUTTON_HORIZONTAL_SPACE =  6;

    wxBoxSizer * sizer = new wxBoxSizer(wxVERTICAL);

    wxFont const TITLE_FONT(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    wxStaticText * title = new wxStaticText(this, wxID_ANY, wxT("The SaveZone DisplayBoard Controller"));
    title->SetFont(TITLE_FONT);
    sizer->Add(title, wxSizerFlags().Border(wxALL, BORDER_IN_PIXELS));
    sizer->Add(new wxStaticLine(this, wxID_ANY), wxSizerFlags().Expand().Border(wxALL, BORDER_IN_PIXELS));

    wxBoxSizer * radios = new wxBoxSizer(wxHORIZONTAL);
    radios->Add(new wxRadioButton(this, wxID_ANY, wxT("Enable")));
    radios->AddSpacer(RADIO_HORIZONTAL_SPACE);
    radios->Add(new wxRadioButton(this, wxID_ANY, wxT("Disable")));
    sizer->Add(radios, wxSizerFlags().Center().Border(wxALL, BORDER_IN_PIXELS));

    int const GRID_COLS = 2;
    int const GRID_VGAP = 8;
    int const GRID_HGAP = 6;

    wxPanel * property_panel = new wxPanel(this, wxID_ANY);
    {
        wxBoxSizer * hbox = new wxBoxSizer(wxHORIZONTAL);
        wxFlexGridSizer * grid = new wxFlexGridSizer(GRID_COLS, GRID_VGAP, GRID_HGAP);
        grid->Add(new wxStaticText(property_panel, wxID_ANY, wxT("Server IP:")));
        grid->Add(new   wxTextCtrl(property_panel, wxID_ANY), wxSizerFlags().Proportion(1).Expand());
        grid->Add(new wxStaticText(property_panel, wxID_ANY, wxT("Server PORT:")));
        grid->Add(new   wxTextCtrl(property_panel, wxID_ANY), wxSizerFlags().Proportion(1).Expand());
        grid->Add(new wxStaticText(property_panel, wxID_ANY, wxT("Direction:")));

        wxString items[] = {wxT("Left"), wxT("Right")};
        wxComboBox * combo = new wxComboBox(property_panel, wxID_ANY, items[0], wxDefaultPosition, wxDefaultSize, 2, items);
        combo->SetEditable(false);
        grid->Add(combo, wxSizerFlags().Proportion(1).Expand());

        grid->AddGrowableRow(2, 1);
        grid->AddGrowableCol(1, 1);

        hbox->Add(grid, wxSizerFlags().Proportion(1).Border(wxALL, BORDER_IN_PIXELS));//1, wxALL | wxEXPAND, 15);
        property_panel->SetSizer(hbox);
    }
    sizer->Add(property_panel, wxSizerFlags().Expand().Border(wxALL, BORDER_IN_PIXELS));
    sizer->AddStretchSpacer(1);
    sizer->Add(new wxStaticLine(this, wxID_ANY), wxSizerFlags().Expand().Border(wxALL, BORDER_IN_PIXELS));

    wxBoxSizer * buttons = new wxBoxSizer(wxHORIZONTAL);
    wxButton   * cancel  = new wxButton(this, wxID_ANY, wxT("Cancel"));
    wxButton   * apply   = new wxButton(this, wxID_ANY, wxT("Apply"));
    wxButton   * ok      = new wxButton(this, wxID_ANY, wxT("Ok"));
    buttons->Add(cancel);
    buttons->AddSpacer(BUTTON_HORIZONTAL_SPACE);
    buttons->Add(apply);
    buttons->AddSpacer(BUTTON_HORIZONTAL_SPACE);
    buttons->Add(ok);
    sizer->Add(buttons, wxSizerFlags().Right().Bottom().Border(wxALL, BORDER_IN_PIXELS));

    SetSizer(sizer);
    Centre();
}

MainFrame::~MainFrame()
{
    // EMPTY.
}

