// A Simple scanw example

#include <ncurses.h>
#include <string.h>

int main(int argc, char ** argv)
{
    char mesg[] = "Enter a string: ";
    char str[80] = { 0, };
    int row, col;

    initscr();
    getmaxyx(stdscr, row, col);

    int move_row = row / 2;
    int move_col = (col - strlen(mesg)) / 2;

    mvprintw(move_row, move_col, "%s", mesg); // print the message at the center of the screen.
    getstr(str);
    mvprintw(LINES - 2, 0, "You Entered: %s", str);
    getch();
    endwin();

    return 0;
}

