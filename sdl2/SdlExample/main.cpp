
#include <stdio.h>
#include <stdlib.h>

#pragma comment (lib, "SDL2.lib")
#pragma comment (lib, "SDL2main.lib")
#pragma comment (lib, "SDL2_image.lib")
#pragma comment (lib, "SDL2_mixer.lib")
#pragma comment (lib, "SDL2_ttf.lib")

#include "sdl_example.h"

void printExampleList()
{
    printf("1. Hello SDL\n"); // In this tutorial we will be setting up the SDL library and creating our first window.
    printf("2. Getting an Image on the Screen\n"); // Now that we can get a window to appear, lets blit an image onto it.
    printf("3. Event Driven Programming\n"); // Here we'll start handling user input by allow the user to X out the window.
    printf("4. Key Presses\n"); // Here we'll learn to handle keyboard input.
    printf("5. Optimized Surface Loading and Soft Stretching\n"); // Now that we know how to load and blit surfaces, it's time to make our blits faster. We'll also take a smaller image and stretch it to fit the screen.
    printf("6. Extension Libraries and Loading Other Image Formats\n"); // Here we'll be using the SDL_image extension library to load png images.
    printf("7. Texture Loading and Rendering\n"); // A big new feature in SDL 2.0 is hardware accelerated texture based 2D rendering. Here we'll be loading an image to render it using textures.
    printf("8. Geometry Rendering\n"); // Another new feature in SDL 2.0 is hardware accelerated primitive rendering. Here we'll be using it to render some common shapes.
    printf("9. The Viewport\n"); // SDL 2.0 also lets you control where you render on the screen you using the viewport. We'll be using the viewport to create subscreens.
    printf("10. Color Keying\n"); // Here we'll use color keying to give texture transparent backgrounds.

    printf("11. Clip Rendering and Sprite Sheets\n"); // Using clip rendering, you can keep multiple images on one texture and render the part you need. We'll be using this to render individual sprites from a sprite sheet.
    printf("12. Color Modulation\n"); // We'll be altering the color of rendered textures using color modulation.
    printf("13. Alpha Blending\n"); // Here we'll be using SDL 2.0 new hardware accelerated alpha blending.
    printf("14. Animated Sprites and Vsync\n"); // Here we'll be using a sequence of sprites to animate them.
    printf("15. Rotation and Flipping\n"); // Here we'll be using SDL 2.0's new texture rotation and flipping.
    printf("16. True Type Fonts\n"); // Here we'll be rendering text from true type fonts using SDL_ttf.
    printf("17. Mouse Events\n"); // Here we'll learn to read mouse input using mouse events.
    printf("18. Key States\n"); // There's other ways to read the keys besides event polling. Here will get the current states of the keyboard using get states.
    printf("19. Gamepads and Joysticks\n"); // Here we'll learn to read input from a game controller.
    printf("20. Force Feedback\n"); // Another new feature for SDL 2.0 is rumble support using the SDL haptics. We'll make our controller rumble when a button is pressed.

    printf("21. Sound Effects and Music\n"); // Here we'll be using SDL_mixer to add music and sound to our SDL App.
    printf("22. Timing\n"); // Here we'll be using SDL's time capabilites.
    printf("23. Advanced Timers\n"); // Here we'll extend SDL time capabilities to make our own custom timer.
    printf("24. Calculating Frame Rate\n"); // Here we'll use the timers we built to measure frame rate.
    printf("25. Capping Frame Rate\n"); // If you need a constant frame rate when vsync isn't available, frame rate capping can be used as a fall back.
    printf("26. Motion\n"); // Here we'll be taking what we learned about render and handling input to make a dot move around the screen.
    printf("27. Collision Detection\n"); // Here we'll have two objects interact with each other using bounding box collision detection.
    printf("28. Per-pixel Collision Detection\n"); // Here we'll have two object collide using per-pixel collision detection.
    printf("29. Circular Collision Detection\n"); // Here we'll learn to detect collisions with circles and boxes.
    printf("30. Scrolling\n"); // Here we'll be implement a camera to scroll levels larger than the screen.

    printf("31. Scrolling Backgrounds\n"); // Here we'll using a scrolling background to give the illusion of an infinite level.
    printf("32. Text Input and Clipboard Handling\n"); // Here we'll using SDL 2.0's new way of handling text input and its new clip board handling feature.
    printf("33. File Reading and Writing\n"); // Here we'll using SDL's RWOps API to do binary file IO.
    printf("[DISABLE] 34. Audio Recording\n"); // SDL 2 is planned to have an audio recording feature. As of SDL 2.0.0, it is not yet implemented. This here is just a place holder until it is ready to go. Please do not e-mail me saying this link is broken. You'll just look silly.
    printf("35. Window Events\n"); // Here we'll be handling events from a resizable window.
    printf("36. Multiple Windows\n"); // A new feature in SDL is the ability to support more than one window. Here we'll make an application that has 3 windows.
    printf("37. Multiple Displays\n"); // Another new feature of SDL 2.0 is the ability to handle more than one physical display. Here we'll make our window jump from display to display.
    printf("38. Particle Engines\n"); // Here we'll use a simple particle effect to create a simple trail effect.
    printf("39. Tiling\n"); // Here we'll make a simple level using a tiling engine.

    printf("40. Texture Manipulation\n"); // Here we'll be directly accessing and manipulating a texture's pixels.
    printf("41. Bitmap Fonts\n"); // Here we'll be using a texture as a font using bitmap font techniques.
    printf("42. Texture Streaming\n"); // Here we'll be rendering from a streaming data source using texture streaming.
    printf("43. Render to Texture\n"); // Here we'll be taking a scene and rendering it to a texture.
    printf("44. Frame Independent Movement\n"); // Here we'll be making the dot move independent of the current frame rate.
    printf("45. Timer Callbacks\n"); // SDL has another timing mechanism called timer callbacks. Here we'll be setting a function to be called back after a certain amount of time.
    printf("46. Multithreading\n"); // Multithreading allows your program to do things simultaneously. Here we'll make things print to the console from outside our main thread.
    printf("47. Semaphores\n"); // A major issue in multithreaded applications is that you need to make sure that they don't try to access the same data at the same time. Semaphores are a way to make sure only a certain amount of threads are performing an action at the same time.
    printf("48. Atomic Operations\n"); // Atomic operations are another way to synchronize threads. Here we'll be redoing the previous tutorial with atomic counters.
    printf("49. Mutexes and Conditions\n"); // Mutexes and conditions are yet another way to synchronize threads. Here we'll be using the added benefit that they allow threads to communicate with each other.

    printf("60. TEST: Dual Render\n");
}

int scanExampleNumber()
{
    int buffer;
    printf("Select SDL Sample: ");
    scanf("%d", &buffer);
    return buffer;
}

int runExample(int number)
{
    int exit_code = 0;

    switch (number) {
    case 1:  exit_code = sdl1Main(); break;
    case 2:  exit_code = sdl2Main(); break;
    case 3:  exit_code = sdl3Main(); break;
    case 4:  exit_code = sdl4Main(); break;
    case 5:  exit_code = sdl5Main(); break;
    case 6:  exit_code = sdl6Main(); break;
    case 7:  exit_code = sdl7Main(); break;
    case 8:  exit_code = sdl8Main(); break;
    case 9:  exit_code = sdl9Main(); break;
    case 10: exit_code = sdl10Main(); break;
    case 11: exit_code = sdl11Main(); break;
    case 12: exit_code = sdl12Main(); break;
    case 13: exit_code = sdl13Main(); break;
    case 14: exit_code = sdl14Main(); break;
    case 15: exit_code = sdl15Main(); break;
    case 16: exit_code = sdl16Main(); break;
    case 17: exit_code = sdl17Main(); break;
    case 18: exit_code = sdl18Main(); break;
    case 19: exit_code = sdl19Main(); break;
    case 20: exit_code = sdl20Main(); break;
    case 21: exit_code = sdl21Main(); break;
    case 22: exit_code = sdl22Main(); break;
    case 23: exit_code = sdl23Main(); break;
    case 24: exit_code = sdl24Main(); break;
    case 25: exit_code = sdl25Main(); break;
    case 26: exit_code = sdl26Main(); break;
    case 27: exit_code = sdl27Main(); break;
    case 28: exit_code = sdl28Main(); break;
    case 29: exit_code = sdl29Main(); break;
    case 30: exit_code = sdl30Main(); break;
    case 31: exit_code = sdl31Main(); break;
    case 32: exit_code = sdl32Main(); break;
    case 33: exit_code = sdl33Main(); break;
    case 34: exit_code = sdl34Main(); break;
    case 35: exit_code = sdl35Main(); break;
    case 36: exit_code = sdl36Main(); break;
    case 37: exit_code = sdl37Main(); break;
    case 38: exit_code = sdl38Main(); break;
    case 39: exit_code = sdl39Main(); break;
    case 40: exit_code = sdl40Main(); break;
    case 41: exit_code = sdl41Main(); break;
    case 42: exit_code = sdl42Main(); break;
    case 43: exit_code = sdl43Main(); break;
    case 44: exit_code = sdl44Main(); break;
    case 45: exit_code = sdl45Main(); break;
    case 46: exit_code = sdl46Main(); break;
    case 47: exit_code = sdl47Main(); break;
    case 48: exit_code = sdl48Main(); break;
    case 49: exit_code = sdl49Main(); break;
//------------------------------------------
    case 60: exit_code = sdl60Main(); break;
    case 0:
    default:
        exit_code = 1;
        break;
    }

    return exit_code;
}

int main(int argc, char *args[])
{
    int select_number = 0;

    if (argc >= 2) {
        select_number = atoi(args[1]);
    } else {
        printExampleList();
        select_number = scanExampleNumber();
    }

    return runExample(select_number);
}
