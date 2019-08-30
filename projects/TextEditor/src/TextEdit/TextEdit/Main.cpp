#define PDC_DLL_BUILD 1

#include "curses.h"
#include <stdlib.h>
#include <time.h>

int main(void)
{
	WINDOW* main_window = nullptr;
	int num_cols = 0;
	int num_rows = 0;

	//SETUP
	//initialize our window
	main_window = initscr();

	//resize our window
	resize_term(5000, 5000);
	getmaxyx(main_window, num_rows, num_cols);
	resize_term(num_rows - 1, num_cols - 1);
	getmaxyx(main_window, num_rows, num_cols);

	//turn keyboard echo
	noecho();

	//turn on keypad input
	keypad(main_window, TRUE);

	//hide cursor
	curs_set(FALSE);

	//MAIN PROGRAM LOGIC GOES HERE

	for (int i = 0; i < num_cols; i++)
	{
		//top row
		mvaddch(0, i, ACS_BLOCK);

		//bottom row
		mvaddch(num_rows - 1, i, ACS_BLOCK);
	}

	for (int i = 0; i < num_rows; i++)
	{
		//left column
		mvaddch(i, 0, ACS_BBSS);

		//right column
		mvaddch(i, num_cols - 1, ACS_BLOCK);
	}

	//refresh tells curses to draw everything

	//END OF PROGRAM LOGIC GOES HERE

	//pause for user input
	char input = getch();

	//end curses mode
	endwin();
}