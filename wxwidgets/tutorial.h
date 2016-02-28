#ifndef __INCLUDE_WXWIDGET__TUTORIAL_H__
#define __INCLUDE_WXWIDGET__TUTORIAL_H__

#include <wx/wx.h>
#include <wx/string.h>
#include <wx/utils.h>
#include <wx/file.h>
#include <wx/textfile.h>
#include <wx/dir.h>
#include <wx/filefn.h>

#include <iostream>

#ifndef NAMESPACE_ANONYMOUS
#define NAMESPACE_ANONYMOUS
#define NAMESPACE_ANONYMOUS_OPEN namespace NAMESPACE_ANONYMOUS {
#define NAMESPACE_ANONYMOUS_CLOSE }
#endif

// wxWidgets helper classes:
// http://zetcode.com/gui/wxwidgets/helperclasses/
int t01();
int t02();
int t03();
int t04();
int t05();
int t06();
int t07();
int t08();

#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/wxprec.h>

// First programs in wxWidgets
// http://zetcode.com/gui/wxwidgets/firstprograms/
int t09();
int t10();
int t11();

#include <wx/menu.h>

// Menus and toolbars in wxWidgets
// http://zetcode.com/gui/wxwidgets/menustoolbars/
int t12();
int t13();
int t14();
int t15();

// Layout management
// http://zetcode.com/gui/wxwidgets/layoutmanagement/
int t16();
int t17();
int t18();
int t19();
int t20();
int t21();
int t22();

// Events
// http://zetcode.com/gui/wxwidgets/events/
int t23();
int t24();
int t25();
int t26();
int t27();

#include <wx/fontdlg.h>

// Dialogs
// http://zetcode.com/gui/wxwidgets/dialogs/
int t28();
int t29();
int t30();
int t31();

#include <wx/slider.h>
#include <wx/tglbtn.h>
#include <wx/stattext.h>
#include <wx/statline.h>

// Widgets I
// http://zetcode.com/gui/wxwidgets/widgets/
int t32();
int t33();
int t34();
int t35();
int t36();
int t37();

#include <wx/listbox.h>
#include <wx/textdlg.h>
#include <wx/notebook.h>
#include <wx/grid.h>

// Widgets II
// http://zetcode.com/gui/wxwidgets/widgetsII/
int t38();
int t39();
int t40();

#include <wx/dnd.h>
#include <wx/treectrl.h>
#include <wx/dirctrl.h>
#include <wx/dir.h>
#include <wx/splitter.h>
#include <wx/listctrl.h>

// Drag & Drop
// http://zetcode.com/gui/wxwidgets/dragdrop/
int t41();

#include <stdlib.h>
#include <time.h>

// Device Contexts
// http://zetcode.com/gui/wxwidgets/gdi/
int t42();
int t43();
int t44();
int t45();
int t46();
int t47();
int t48();

// Custom widgets
// http://zetcode.com/gui/wxwidgets/customwidgets/
int t49();

#include <stdlib.h>
#include <algorithm>

// Tetris
// http://zetcode.com/gui/wxwidgets/thetetrisgame/
int t50();

// wxAUI
int t51();
int t52();

#endif // __INCLUDE_WXWIDGET__TUTORIAL_H__
