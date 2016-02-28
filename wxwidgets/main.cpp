/// @file main.cpp
/// @brief wxWidgets tutorials project entry-point.
/// @date 141107
/// @author zer0

#include "tutorial.h"

// ----------------------
// wxWidgets Application.
// ----------------------

class MyApp: public wxApp
{
public:
    static int tnum;
public:
    virtual bool OnInit()
    {
        switch (tnum) {
        case  9: t09(); break;
        case 10: t10(); break;
        case 11: t11(); break;
        case 12: t12(); break;
        case 13: t13(); break;
        case 14: t14(); break;
        case 15: t15(); break;
        case 16: t16(); break;
        case 17: t17(); break;
        case 18: t18(); break;
        case 19: t19(); break;
        case 20: t20(); break;
        case 21: t21(); break;
        case 22: t22(); break;
        case 23: t23(); break;
        case 24: t24(); break;
        case 25: t25(); break;
        case 26: t26(); break;
        case 27: t27(); break;
        case 28: t28(); break;
        case 29: t29(); break;
        case 30: t30(); break;
        case 31: t31(); break;
        case 32: t32(); break;
        case 33: t33(); break;
        case 34: t34(); break;
        case 35: t35(); break;
        case 36: t36(); break;
        case 37: t37(); break;
        case 38: t38(); break;
        case 39: t39(); break;
        case 40: t40(); break;
        case 41: t41(); break;
        case 42: t42(); break;
        case 43: t43(); break;
        case 44: t44(); break;
        case 45: t45(); break;
        case 46: t46(); break;
        case 47: t47(); break;
        case 48: t48(); break;
        case 49: t49(); break;
        case 50: t50(); break;
        case 51: t51(); break;
        case 52: t52(); break;
        default: break;
        }
        return true;
    }
};

int MyApp::tnum = 0;

//IMPLEMENT_APP(MyApp)
//wxIMPLEMENT_WX_THEME_SUPPORT
//wxIMPLEMENT_APP_NO_THEMES(MyApp)
//wxIMPLEMENT_WXWIN_MAIN
wxIMPLEMENT_APP_NO_MAIN(MyApp);

void printList()
{
    // wxWidgets helper classes:
    // http://zetcode.com/gui/wxwidgets/helperclasses/
    printf("01.Console\n");
    printf("02.wxString\n");
    printf("03.Shell\n");
    printf("04.Information\n");
    printf("05.Time & Date\n");
    printf("06.Create File\n");
    printf("07.Read File\n");
    printf("08.Directory\n");

    // First programs in wxWidgets
    // http://zetcode.com/gui/wxwidgets/firstprograms/
    printf("09.A simple application & icon\n");
    printf("10.A simple button\n");
    printf("11.Widgets communicate\n");

    // Menus and toolbars in wxWidgets
    // http://zetcode.com/gui/wxwidgets/menustoolbars/
    printf("12.Simple menu example\n");
    printf("13.Submenus\n");
    printf("14.A simple toolbar\n");
    printf("15.Toolbars\n");

    // Layout management
    // http://zetcode.com/gui/wxwidgets/layoutmanagement/
    printf("16.Absolute Positioning\n");
    printf("17.Using sizers\n");
    printf("18.wxBoxSizer: border\n");
    printf("19.wxBoxSizer: alignment\n");
    printf("20.Go To Class\n");
    printf("21.wxGridSizer\n");
    printf("22.wxFlexGridSizer\n");

    // Events
    // http://zetcode.com/gui/wxwidgets/events/
    printf("23.A simple event example\n");
    printf("24.Example using Connect()\n");
    printf("25.Event propagation\n");
    printf("26.Vetoing events\n");
    printf("27.Window identifiers\n");

    // Dialogs
    // http://zetcode.com/gui/wxwidgets/dialogs/
    printf("28.Message dialogs\n");
    printf("29.wxFileDialog\n");
    printf("30.wxFontDialog\n");
    printf("31.A custom dialog\n");

    // Widgets I
    // http://zetcode.com/gui/wxwidgets/widgets/
    printf("32.wxCheckBox\n");
    printf("33.wxBitmapButton\n");
    printf("34.wxToggleButton\n");
    printf("35.wxStaticLine\n");
    printf("36.wxStaticText\n");
    printf("37.wxSlider\n");

    // Widgets II
    // http://zetcode.com/gui/wxwidgets/widgetsII/
    printf("38.wxListBox\n");
    printf("39.wxNotebook\n");
    printf("40.wxScrolledWindow\n");

    // Drag & Drop
    // http://zetcode.com/gui/wxwidgets/dragdrop/
    printf("41.Drag and Drop\n");

    // Device Contexts
    // http://zetcode.com/gui/wxwidgets/gdi/
    printf("42.Simple line\n");
    printf("43.Drawing text\n");
    printf("44.Point\n");
    printf("45.Pen\n");
    printf("46.Regions\n");
    printf("47.Gradient\n");
    printf("48.Shapes\n");

    // Custom widgets
    // http://zetcode.com/gui/wxwidgets/customwidgets/
    printf("49.The Burning Widget\n");

    // Tetris
    // http://zetcode.com/gui/wxwidgets/thetetrisgame/
    printf("50.The Tetris\n");

    // wxAUI
    printf("51.wxAUI example 01\n");
    printf("52.wxAUI example 02\n");
}

int scanNumber()
{
    int buffer;
    printf("Select wxWidgets tutorials: ");
    scanf("%d", &buffer);
    return buffer;
}

int run(int number, int argc, char ** argv)
{
    switch (number) {
    // Console tutorials.
    case  1: return t01(); break;
    case  2: return t02(); break;
    case  3: return t03(); break;
    case  4: return t04(); break;
    case  5: return t05(); break;
    case  6: return t06(); break;
    case  7: return t07(); break;
    case  8: return t08(); break;
    default:
        break;
    }

    // wxWidget tutorials.
    MyApp::tnum = number;
    wxEntry(argc, argv);
    return EXIT_SUCCESS;
}

int main(int argc, char ** argv)
{
    ::wxInitialize(argc, argv);

    int select = 0;

    if (argc >= 2) {
        select = atoi(argv[1]);
    } else {
        printList();
        select = scanNumber();
    }

    return run(select, argc, argv);
}
