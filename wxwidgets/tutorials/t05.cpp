#include "tutorial.h"

int t05()
{
    {
        wxDateTime now = wxDateTime::Now();

        wxString date1 = now.Format();
        wxString date2 = now.Format(wxT("%X"));
        wxString date3 = now.Format(wxT("%x"));

        wxPuts(date1);
        wxPuts(date2);
        wxPuts(date3);

        // // Assert failure: failed in CallStrftime():strftime() failed.
        // wxPrintf(wxT("   Tokyo: %s\n"), now.Format(wxT("%a %T"), wxDateTime::GMT9).c_str());
        // wxPrintf(wxT("  Moscow: %s\n"), now.Format(wxT("%a %T"), wxDateTime::MSD).c_str());
        // wxPrintf(wxT("Budapest: %s\n"), now.Format(wxT("%a %T"), wxDateTime::CEST).c_str());
        // wxPrintf(wxT("  London: %s\n"), now.Format(wxT("%a %T"), wxDateTime::WEST).c_str());
        // wxPrintf(wxT("New York: %s\n"), now.Format(wxT("%a %T"), wxDateTime::EDT).c_str());

        wxPrintf(wxT("Yokyo : %s\n"), now.Format(wxT("%a %X"), wxDateTime::GMT9).c_str());
        wxPrintf(wxT("Moscow : %s\n"), now.Format(wxT("%a %X"), wxDateTime::MSD).c_str());
        wxPrintf(wxT("Budapest : %s\n"), now.Format(wxT("%a %X"), wxDateTime::CEST).c_str());
        wxPrintf(wxT("London : %s\n"), now.Format(wxT("%a %X"), wxDateTime::WEST).c_str());
        wxPrintf(wxT("New York : %s\n"), now.Format(wxT("%a %X"), wxDateTime::EDT).c_str());

        wxString date4 = now.Format(wxT("%B %d %Y"));
        wxPuts(date4);

        wxDateSpan span(0, 1);
        wxDateTime then = now.Add(span);

        wxString date5 = then.Format(wxT("%B %d %Y"));
        wxPuts(date5);
    }

    return EXIT_SUCCESS;
}
