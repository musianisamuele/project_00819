#include <ncurses.h>
#include <iostream>
#include <chrono>
#include <thread>
#include "../../../point.hpp"
#include "../../../vector.hpp"
#include "../../../body.hpp"

//const char PROTAGONIST = 'T';

void drawChar(phy::Point p) 
{
	int x_max;
	int y_max;
	getmaxyx(stdscr, y_max, x_max);

	mvprintw(y_max - p.get_yPosition(), p.get_xPosition(), "@");
}

void drawProtagonist() 
{
	int ch;

	bool EXIT = false;

	int x = 0;
	int y = 0;

	phy::Body b;
	b.set_position(phy::Point(0, 0));
	b.set_velocity(phy::Vector(4, 60));
	b.set_acceleration(phy::Vector(0.4, -90));

	int tmp;

	while(!EXIT)
	{
		b.update(0.15);
		//b.set_position(phy::Point(x, y));

		drawChar(b.get_position());

		refresh();
		//sleep(0.8);
		std::this_thread::sleep_for(std::chrono::milliseconds(20));

		// tmp++;
		//Double jmp :)
		// if(tmp % 60 == 0)
		// 	b.set_velocity(phy::Vector(4, 55));

		clear();

		/*
		ch = getchar();
		switch (ch)
		{
			case 'w':
				if(y > 0) y--;
				break;

			case 's':
				if(y < LINES - 2) y++;
				break;

			case 'a':
				if(x > 0) x--;
				break;

			case 'd':
				if(x < COLS - 1) x++;
				break;

			case 'q':
				if(x > 0 && y > 0) 
				{
					x--;
					y--;
				}
				break;

			case 'g':
				b.set_velocity(phy::Vector(1, 0));
				break;
				
			case 'e':
				if(x < COLS - 1 && y > 0) 
				{
					x++;
					y--;
				}
				break;

			case KEY_F(1):
				EXIT = true;
				break;

			default:
				break;
		}
		*/
	}
}

int main()
{
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);

	drawProtagonist();

	endwin();

	return 0;
}
