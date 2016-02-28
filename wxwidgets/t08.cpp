#include "tutorial.h"

int t08()
{
    wxDir dir(wxGetCwd());

    wxString file;

    bool cont = dir.GetFirst(&file, wxEmptyString, wxDIR_FILES | wxDIR_DIRS);

    while (cont) {
        wxPuts(file);
        cont = dir.GetNext(&file);
    }

    return EXIT_SUCCESS;
}
