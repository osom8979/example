#include "tutorial.h"

int t04()
{
    wxPuts(wxGetHomeDir());
    wxPuts(wxGetOsDescription());
    wxPuts(wxGetUserName());
    wxPuts(wxGetFullHostName());

    wxMemorySize mem = wxGetFreeMemory();

    wxPrintf(wxT("Memory: %s\n"), mem.ToString());
    return EXIT_SUCCESS;
}
