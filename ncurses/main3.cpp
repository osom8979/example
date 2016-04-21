// A Simple printw example

#include <ncurses.h>
#include <string.h>

int main(int argc, char ** argv)
{
    char mesg[] = "Just a string";
    int row, col;

    /* WINDOW * w = */ initscr();

    getmaxyx(stdscr, row, col); // get the number of rows and columns.

    int move_row = row / 2;
    int move_col = (col - strlen(mesg)) / 2;

    mvprintw(move_row, move_col, "%s", mesg); // print the message at the center of the screen.
    mvprintw(row - 2, 0, "This screen has %d rows and %d columns\n", row, col);

    printw("Try resizing your window(if possible) and then run this program again.");

    refresh();
    getch();
    endwin();

    return 0;
}

