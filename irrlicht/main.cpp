/// @file main.cpp
/// @brief irrlicht demo entry point.
/// @author zer0
/// @date 141025

#include "tutorial.h"

void printList()
{
    printf("01.HelloWorld\n");
    printf("02.Quake3Map\n");
    printf("03.CustomSceneNode\n");
    printf("04.Movement\n");
    printf("05.UserInterface\n");
    printf("06.2DGraphics\n");
    printf("07.Collision\n");
    printf("08.SpecialFX\n");
    printf("09.Meshviewer\n");
    printf("10.Shaders\n");
    printf("11.PerPixelLighting\n");
    printf("12.TerrainRendering\n");
    printf("13.RenderToTexture\n");
    printf("14.Win32Window\n");
    printf("15.LoadIrrFile\n");
    printf("16.Quake3MapShader\n");
    printf("17.HelloWorld_Mobile\n");
    printf("18.SplitScreen\n");
    printf("19.MouseAndJoystick\n");
    printf("20.ManagedLights\n");
    printf("21.Quake3Explorer\n");
    printf("22.MaterialViewer\n");
    printf("23.SMeshHandling\n");
    printf("24.CursorControl\n");
    printf("25.XmlHandling\n");
    printf("26.OcclusionQuery\n");
}

int scanNumber()
{
    int buffer;
    printf("Select SDL Sample: ");
    scanf("%d", &buffer);
    return buffer;
}

int run(int number)
{
    switch (number) {
    case  1:  return t01(); break;
    case  2:  return t02(); break;
    case  3:  return t03(); break;
    case  4:  return t04(); break;
    case  5:  return t05(); break;
    case  6:  return t06(); break;
    case  7:  return t07(); break;
    case  8:  return t08(); break;
    case  9:  return t09(); break;
    case 10:  return t10(); break;
    case 11:  return t11(); break;
    case 12:  return t12(); break;
    case 13:  return t13(); break;
    case 14:  return t14(); break;
    case 15:  return t15(); break;
    case 16:  return t16(); break;
    case 17:  return t17(); break;
    case 18:  return t18(); break;
    case 19:  return t19(); break;
    case 20:  return t20(); break;
    case 21:  return t21(); break;
    case 22:  return t22(); break;
    case 23:  return t23(); break;
    case 24:  return t24(); break;
    case 25:  return t25(); break;
    case 26:  return t26(); break;
    }
}

int main(int argc, char ** args)
{
    int select = 0;

    if (argc >= 2) {
        select = atoi(args[1]);
    } else {
        printList();
        select = scanNumber();
    }

    return run(select);
}
