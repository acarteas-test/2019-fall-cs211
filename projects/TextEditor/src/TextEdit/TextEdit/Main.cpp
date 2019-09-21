#define PDC_DLL_BUILD 1

#include "curses.h"
#include <stdlib.h>
#include <time.h>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void PrintTabs(const char* tabs[], WINDOW* winptr);

void PrintError(string error, WINDOW* winptr);

void PrintFile(vector<char> lines, WINDOW* winptr);

void CheckFile(string file, WINDOW* winptr);


int main()
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
	
	//Turning color
	
	if (has_colors() == FALSE)
	{
		endwin();
		printf("The Terminal does not support color");
		exit(EXIT_FAILURE);
	}
	start_color();
	
	//Picking color for columns
	init_pair(1, COLOR_BLUE, COLOR_WHITE);
	attron(COLOR_PAIR(1));

	//Setting up boarders
	mvaddch(1, 2, ACS_BSSB);						//inner corner boarders top left around
	mvaddch(1, num_cols - 3, ACS_BBSS);  
	mvaddch(num_rows - 2, num_cols - 3, ACS_SBBS);
	mvaddch(num_rows - 2, 2, ACS_SSBB);
	//ACS_SBSB left and right inner boarder
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
		mvaddch(i+1, 0, ACS_BLOCK);
		mvaddch(i+1, 1, ACS_BLOCK);

		//right column
		mvaddch(i+1, num_cols - 1, ACS_BLOCK);
		mvaddch(i + 1, num_cols - 2, ACS_BLOCK);

	}

	//turring off color for borders
	attroff(COLOR_PAIR(1));
	
	//coloring text
	init_pair(2, COLOR_CYAN, COLOR_BLACK);
	attron(COLOR_PAIR(2));
	attron(A_BOLD | A_BLINK);

	//Text windows

	string file = "File";				//string declaration
	string edit = "Edit";
	string options = "Options";
	string tools = "Tools";
	string help = "Help";
	const char* filechar = file.c_str(); //conversion into const char* so mvprintw works
	const char* editchar = edit.c_str();
	const char* optionschar = options.c_str();
	const char* toolschar = tools.c_str();
	const char* helpchar = help.c_str();
	
	static const char* tabs[5] = {filechar, editchar, optionschar, toolschar, helpchar};

	PrintTabs(tabs, main_window);
	
	//now playing with files
	
	typedef vector<char> Lines;
	Lines lines;
	
	//check to see if it is open (can be made to a function)
	CheckFile("Text.txt", main_window);
	
	//refresh tells curses to draw everything

	//END OF PROGRAM LOGIC GOES HERE

	//pause for user input
	char input = getch();

	//end curses mode
	endwin();
}

//is suppose to make my life easier by printing the top tabs (but its more work than its worth)
void PrintTabs(const char* tabs[], WINDOW* winptr)
{
	//Color for the text
	init_pair(2, COLOR_CYAN, COLOR_BLACK);
	attron(COLOR_PAIR(2));
	attron(A_BOLD | A_BLINK);

	int pos = 3;
	for (int i = 0; i < 5; i++)
	{
		if(i == 0)
		mvwprintw(winptr, 0, 2, "|");
		mvwprintw(winptr, 0, pos, tabs[i]);
		printw(" | ");
		pos = pos + strlen(tabs[i]) + 3;
	}
	attroff(COLOR_PAIR(2));
	attroff(A_BOLD | A_BLINK);
}

void PrintError(string error, WINDOW* winptr)
{
	if (error == "color/support")
		mvwprintw(winptr, 15, 15, "The Terminal Does Not Support Color");
	else if (error == "file/open")
		mvwprintw(winptr, 15, 15, "The file could not open");

}

void PrintFile(vector<char> lines, WINDOW* winptr)
{
	typedef vector<char> Lines;
	int maxlength = 205;
	int loop_counter = 0;
	int counter_len = 3;
	int counter_row = 3;

	for (Lines::const_iterator i = lines.begin(); i != lines.end(); ++i)
	{
		if (loop_counter < maxlength)//looking at the wrong thing
		{
			mvaddch(counter_row, counter_len, *i);
			counter_len++;
			loop_counter++;
		}
		else
		{
			counter_row++;
			counter_len = 3;
			loop_counter = 0;
			mvaddch(counter_row, counter_len, *i);
			i--;
		}
		
	}
}

void CheckFile(string file, WINDOW* winptr)
{ // use getline to push into a temp place holder then push in into the vector
	typedef vector<char> Doc;
	char ch;
	Doc doc;
	string line;
	ifstream myfile;
	myfile.open(file);

	if (myfile.is_open())
	{
		while (!myfile.eof())
		{
			getline(myfile, line);
			for (int i = 0; i < line.length(); i++)
			{
				doc.push_back(line.at(i));
			}
		}
	}
	else
	{
		PrintError("file/open", winptr);
	}

	myfile.close();
	PrintFile(doc, winptr);
}
