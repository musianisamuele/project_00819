#include "game.hpp"
#include "menu.hpp"
#include <unistd.h>
#include "game.hpp"

WINDOW *win;

Game::Game()
{
	this->screen = Draw();
	this->screen.init();

}

Game::~Game()
{
	wgetch(win);
	endwin();
}

void Game::run()
{
	Menu menu = Menu(screen.get_maxX(), screen.get_maxY());
	bool exit = false;
	while (!exit) {

		menu.drawMenu();
		int x = menu.get_selected_option();
		menu.isSelected(x);
	}
}

void Game::start()
{
	// clear the screen and draw the border
	screen.clearScreen();
}

void Game::resume()
{
	screen.clearScreen();
	std::string s = "Load your game from a saved file";
	screen.centerX(s, 3, 75 - (s.length()/2) );
}
