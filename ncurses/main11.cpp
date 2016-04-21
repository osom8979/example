// Access the menu with mouse !!!

#include <ncurses.h>
#include <string.h>

/*
 * Name            Description
 * ---------------------------------------------------------------------
 * BUTTON1_PRESSED          mouse button 1 down
 * BUTTON1_RELEASED         mouse button 1 up
 * BUTTON1_CLICKED          mouse button 1 clicked
 * BUTTON1_DOUBLE_CLICKED   mouse button 1 double clicked
 * BUTTON1_TRIPLE_CLICKED   mouse button 1 triple clicked
 * BUTTON2_PRESSED          mouse button 2 down
 * BUTTON2_RELEASED         mouse button 2 up
 * BUTTON2_CLICKED          mouse button 2 clicked
 * BUTTON2_DOUBLE_CLICKED   mouse button 2 double clicked
 * BUTTON2_TRIPLE_CLICKED   mouse button 2 triple clicked
 * BUTTON3_PRESSED          mouse button 3 down
 * BUTTON3_RELEASED         mouse button 3 up
 * BUTTON3_CLICKED          mouse button 3 clicked
 * BUTTON3_DOUBLE_CLICKED   mouse button 3 double clicked
 * BUTTON3_TRIPLE_CLICKED   mouse button 3 triple clicked
 * BUTTON4_PRESSED          mouse button 4 down
 * BUTTON4_RELEASED         mouse button 4 up
 * BUTTON4_CLICKED          mouse button 4 clicked
 * BUTTON4_DOUBLE_CLICKED   mouse button 4 double clicked
 * BUTTON4_TRIPLE_CLICKED   mouse button 4 triple clicked
 * BUTTON_SHIFT             shift was down during button state change
 * BUTTON_CTRL              control was down during button state change
 * BUTTON_ALT               alt was down during button state change
 * ALL_MOUSE_EVENTS         report all button state changes
 * REPORT_MOUSE_POSITION    report mouse movement
 */

#define WIDTH 30
#define HEIGHT 10

int startx = 0;
int starty = 0;

char const * choices[] = {
    "Choice 1",
    "Choice 2",
    "Choice 3",
    "Choice 4",
    "Exit",
};

int n_choices = sizeof(choices) / sizeof(char *);

void print_menu(WINDOW * menu_win, int highlight);
void report_choice(int mouse_x, int mouse_y, int * p_choice);

int main(int argc, char ** argv)
{
    int c, choice = 0;

    WINDOW * menu_win;
    MEVENT event;

    // Initialize curses.
    initscr();
    clear();
    noecho();
    cbreak();

    // Try to put the window in the middle of screen.
    startx = (80 - WIDTH)  / 2;
    starty = (24 - HEIGHT) / 2;

    attron(A_REVERSE);
    mvprintw(23, 1, "Click on Exit to quit (Works best in a virtual console)");
    refresh();
    attroff(A_REVERSE);

    // Print the menu for the first time.
    menu_win = newwin(HEIGHT, WIDTH, starty, startx);
    print_menu(menu_win, 1);

    // Get all the mouse events.
    mousemask(ALL_MOUSE_EVENTS, NULL);

    while (1) {
        c = wgetch(menu_win);
        switch (c) {
        case KEY_MOUSE:
            if (getmouse(&event) == OK) {
                // When the user clicks left mouse button.
                if (event.bstate & BUTTON1_PRESSED) {
                    report_choice(event.x + 1, event.y + 1, &choice);
                    if(choice == -1) {
                        //Exit chosen
                        goto end;
                    }
                    mvprintw(22, 1, "Choice made is : %d String Chosen is \"%10s\"", choice, choices[choice - 1]);
                    refresh();
                }
            }
            print_menu(menu_win, choice);
            break;
        }
    }

end:
    endwin();
    return 0;
}

void print_menu(WINDOW *menu_win, int highlight)
{
    int x, y, i;
    x = 2;
    y = 2;

    box(menu_win, 0, 0);

    for (i = 0; i < n_choices; ++i) {
        if (highlight == i + 1) {
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y, x, "%s", choices[i]);
            wattroff(menu_win, A_REVERSE);
        } else {
            mvwprintw(menu_win, y, x, "%s", choices[i]);
        }
        ++y;
    }

    wrefresh(menu_win);
}

// Report the choice according to mouse position.
void report_choice(int mouse_x, int mouse_y, int *p_choice)
{
    int i,j, choice;

    i = startx + 2;
    j = starty + 3;

    for (choice = 0; choice < n_choices; ++choice) {
        if (mouse_y == j + choice && mouse_x >= i && mouse_x <= i + strlen(choices[choice])) {
            if (choice == n_choices - 1) {
                *p_choice = -1;
            } else {
                *p_choice = choice + 1;
            }
            break;
        }
    }
}

