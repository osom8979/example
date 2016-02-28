#include "tutorial.h"

int t02()
{
    { // ex01.
        wxString str1 = wxT("Linux");
        wxString str2 = wxT("Operating");
        wxString str3 = wxT("System");
        wxString str = str1 + wxT(" ") + str2 + wxT(" ") + str3;
        wxPuts(str);
    }

    { // ex02.
        int flowers = 21;
        wxString str;
        str.Printf(wxT("There are %d red roses."), flowers);
        wxPuts(str);
    }

    { // ex03.
        wxString str = wxT("The history of my life");
        if (str.Contains(wxT("history"))) {
            wxPuts(wxT("Contains!"));
        }
        if (!str.Contains(wxT("plain"))) {
            wxPuts(wxT("Does not contain!"));
        }
    }

    { // ex04.
        wxString str = wxT("The history of my life");
        wxPrintf(wxT("The string has %d characters\n"), (int) str.Len());
    }

    { // ex05.
        wxString str = wxT("The history of my life");
        wxPuts(str.MakeLower());
        wxPuts(str.MakeUpper());
    }

    return EXIT_SUCCESS;
}
