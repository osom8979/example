#include "tutorial.h"

int t07()
{
    wxTextFile file(wxT("tutorial.h"));

    file.Open();

    size_t line_count = file.GetLineCount();
    wxPrintf(wxT("Number of lines: %d\n"), (int) line_count);
    wxPrintf(wxT("First line: %s\n"), file.GetFirstLine().c_str());
    wxPrintf(wxT("Last line: %s\n"), file.GetLastLine().c_str());

    wxPuts(wxT("-------------------------------------"));

    wxString s;

    for (s = file.GetFirstLine(); !file.Eof(); s = file.GetNextLine()) {
        wxPuts(s);
    }

    file.Close();

    return EXIT_SUCCESS;
}
