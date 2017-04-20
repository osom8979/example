/**
 * @file   PreviewPanel.cpp
 * @brief  PreviewPanel class prototype.
 * @author zer0
 * @date   2016-08-04
 */

#include <PreviewPanel.hpp>
#include <wx/dcbuffer.h>

#include <iostream>

enum {
    WIDGET_ID_ADD_BUTTON = 1000,
    WIDGET_ID_REMOVE_BUTTON,
};

IMPLEMENT_CLASS(PreviewPanel, wxPanel);

BEGIN_EVENT_TABLE(PreviewPanel, wxPanel)
    EVT_PAINT(PreviewPanel::onPaint)
    EVT_BUTTON(WIDGET_ID_ADD_BUTTON, PreviewPanel::onClickAdd)
    EVT_BUTTON(WIDGET_ID_REMOVE_BUTTON, PreviewPanel::onClickRemove)
END_EVENT_TABLE()

PreviewPanel::PreviewPanel(wxWindow      * parent
                         , wxWindowID      id
                         , wxPoint const & pos
                         , wxSize  const & size)
        : wxPanel(parent, id, pos, size)
{
    _vsizer = new wxBoxSizer(wxVERTICAL);
    _hsizer = new wxBoxSizer(wxHORIZONTAL);
    _add    = new wxButton(this, WIDGET_ID_ADD_BUTTON, wxT("Add"));
    _remove = new wxButton(this, WIDGET_ID_REMOVE_BUTTON, wxT("Remove"));
    _hsizer->Add(_add);
    _hsizer->Add(_remove);
    _vsizer->Add(_hsizer);
    SetSizer(_vsizer);
}

PreviewPanel::~PreviewPanel()
{
    // EMPTY.
}

// --------------
// Event methods.
// --------------

void PreviewPanel::onPaint(wxPaintEvent & event)
{
    wxBufferedPaintDC dc(this);
    dc.Clear();
}

void PreviewPanel::onClickAdd(wxCommandEvent & event)
{
    _vsizer->Add(new wxButton(this, wxID_ANY, wxT("New")));
    _vsizer->Layout();
    //Fit();
}

void PreviewPanel::onClickRemove(wxCommandEvent & event)
{
    int const LAST_INDEX = static_cast<int>(_vsizer->GetItemCount() - 1);
    std::cout << "LAST INDEX: " << LAST_INDEX << std::endl;

    auto * item = _vsizer->GetItem(LAST_INDEX);
    auto * button = item->GetWindow();
    std::cout << std::boolalpha << "Button label: " << button->GetLabel() << std::endl;

    _vsizer->Hide(LAST_INDEX);
    bool detach_result = _vsizer->Detach(LAST_INDEX);
    std::cout << std::boolalpha << "Detach result: " << detach_result << std::endl;

    _vsizer->Layout();
    //Fit();
}

