#include <string>
#include <ncurses.h>
#include "../draw/draw.hpp"
#include "credits.hpp"
#include "game.hpp"

#ifndef MENU_MENU
#define MENU_MENU

#define NUMBER_OF_OPTIONS 4



class Menu
{
	private:
		int posY;
		int posX;
		std::string options[NUMBER_OF_OPTIONS] = {"New Game", "Resume Game from saved file", "Help", "Credits"};
		Draw screen;
	public:

		Menu(int x, int y);

		void drawMenu(); // Disegna il menu

		bool isSelected(int selection);

		int get_selected_option();

};

#endif
