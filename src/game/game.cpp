#include <unistd.h>
#include <cmath>

#include "game.hpp"
#include "menu.hpp"

#include "../physics/body.hpp"
#include "../physics/point.hpp"
#include "../physics/vector.hpp"
#include "../physics/collisions.hpp"
#include "../../etc/logs/logs.hpp"
#include "../entity/entity.hpp"

#define JUMPF (1/(1+m_exp(-0.18 * (cumulative - 8)))*5)
//#define JUMPF cumulative / (1 + cumulative)

double m_exp(double d)
{
	if (d >= 0)
		return exp(d);
	else
		return 1/exp(-d);
}



Game::Game()
{
	this->screen = Draw();
	screen.init();

}

Game::~Game()
{

	endwin();
}

void Game::run()
{
	Menu menu = Menu(screen.get_maxX(), screen.get_maxY());
	bool exit = false;
	menu.drawFirstMenu(this->screen);
	while (!exit) {

		menu.drawMenu(this->screen);
		int sel = menu.get_selected_option(this->screen);

		Credits credits;
		Settings settings;

		switch (sel)
		{


		case 0:
			{// New Game
			// Chiama la funzione start della classe game che si trova in game.cpp che non è statica
			this->start();
			// TODO: Implementing pause menu
			break;}
		case 1:
			{// Resume game
			this->resume();
			screen.getinput();
			break;}
		case 2:
			{// Settings
			settings.drawFirstSettings(this->screen);

			break;
			}
		case 3:
		{
			// chiama la funziona credits che si trova in credits.cpp
			credits = Credits();
			int dev = credits.drawCredits(this->screen);
			if (dev != -1) credits.openGithub(dev);

			break;
		}
		case 27:
			{
				if (exitGame() == true) exit = true;
			}
		}


	}
}


bool Game::exitGame(){
	// Esci dal gioco
	screen.clearScreen();
	screen.drawText(16, Draw::centerX("Are you sure you want to quit?"), "Are you sure you want to quit?");	nostd::string options[2] = {"Yes", "No"};
	int selected = 0;
	bool choose = false;
	// Create two button (yes or no) to quit the game
	while (!choose){


		for (int i = 0; i < 2; i++)
		{
			screen.drawSquareAround(options[i], 20, 65 + 15*i);
		}
		// Set the selected button to blue
		screen.attrOn(COLOR_PAIR(1));
		screen.drawText(20, 65 + 15*selected, options[selected]);
		screen.attrOff(COLOR_PAIR(1));

		// Wait for the user to press a key
		switch (screen.getinput())
		{
			case KEY_LEFT:
				if (selected == 1) selected = selected - 1;
				else if (selected == 0) selected = 1;
				break;
			case KEY_RIGHT:
				if (selected == 0) selected = selected + 1;
				else if (selected == 1) selected = 0;
				break;
			case 10:
				choose = true;
				break;
		}



	}
	if (selected == 0) return true;
}

void Game::start()
{
	// clear the screen and draw the border
	setDifficulty();
	Map map = Map();
	screen.drawMap(map, 0);

	// Creare un oggetto body, chiamo getposition su body. Passo il punto che mi ritorna alla drawPlayer e la drawPlayer disegna il player in quella posizione
	phy::Body player = phy::Body();
	player.set_position(phy::Point(40, 20));
	player.set_acceleration(phy::Vector(1, -90));

	//inizialize manager
	Manager manager = Manager(map);
	//time for moving
	int time = 0;

	// Implementare che con KEY_LEFT, KEY_RIGHT si sposta il giocatore utilizzando il metodo setPosition di body e poi disegnare il giocatore in quella posizione con drawPlayer
	bool exit = false;
	screen.nodel(true);
	 int cumulative = 0;
	 int count_not_key = 0;
	int current_chunk = 0;
	int which_key = 0;
	while (!exit){
		bool right;
		int input = screen.getinput();

		if (input == (int) 'f')

		{
			which_key = 1;
			cumulative++;
			count_not_key = 0;
		}
		else if (input == (int) 'a')
		{
			which_key = 2;
			cumulative++;
			count_not_key = 0;
		}
		else if (input == 'v'){
			which_key = 3;
			cumulative++;
			count_not_key = 0;

		}
		else
		{
			//deb::debug((int)cumulative, "cumulative");
			//deb::debug((double) (JUMPF), "JUMPF");
			count_not_key++;
			if(count_not_key > 30)
			{
				if (cumulative > 1 && which_key == 1 && map.get_chunk(current_chunk).is_there_a_platform(player.get_position() - phy::Point(0, 1)))
					player.set_velocity(phy::Vector(JUMPF, 55));

				else if (cumulative > 1 && which_key == 2 && map.get_chunk(current_chunk).is_there_a_platform(player.get_position() - phy::Point(0, 1)))
					player.set_velocity(phy::Vector(JUMPF, 125));

				else if (cumulative > 1 && which_key == 3 && map.get_chunk(current_chunk).is_there_a_platform(player.get_position() - phy::Point(0, 1)))
					player.set_velocity(phy::Vector(JUMPF, 90));
				cumulative = 0;
			}



		switch(input)
		{
			case ((int) 's'): // move player left
				if(map.get_chunk(current_chunk).is_there_a_platform(player.get_position() - phy::Point(0, 1)))
					player.set_position(player.get_position() - phy::Point(1, 0));
				break;

			case ((int) 'd'): // move player right
				if(map.get_chunk(current_chunk).is_there_a_platform(player.get_position() - phy::Point(0, 1)))
					player.set_position(player.get_position() + phy::Point(1, 0));
				break;
			case 27:
				exit = true;
				break;
			default:
				break;
		}
		}
		// player.update(0.05);
		phy::updateWithCollisions(player, 0.15, map.get_chunk(current_chunk));
		screen.eraseScreen();
		if (player.get_position().get_yPosition() < 0)
		{
			current_chunk--;
			player.set_position(player.get_position() + phy::Point(0, 42));
		}
		else if (player.get_position().get_yPosition() >= 42)
		{
			current_chunk++;
			player.set_position(player.get_position() - phy::Point(0, 42));
		}
		screen.drawPlayer(player.get_position());

		//qui vengono chiamate draw dei nemici e monete
		manager.set_chunk(current_chunk, map);

		//debugging
		//manager.print_enemy_list();

		manager.print_entity(screen);
		if(time == 1001) time = 0;
		manager.move_enemies(time);

		screen.drawMap(map, current_chunk);
		screen.drawText(2, 1, std::to_string(current_chunk));
		screen.drawText(1, 1, std::to_string(player.get_position().get_xPosition()));
		screen.drawText(1, 5, std::to_string(player.get_position().get_yPosition()));
		screen.drawText(1, 140, std::to_string(JUMPF));
		//screen.drawText(2, 140, std::to_string(1+pow(1.1, - cumulative/50)));
		screen.drawText(3, 140, std::to_string(cumulative));
		napms(5);

	}
	screen.nodel(false);
	screen.clearScreen();
}

void Game::resume()
{
	screen.clearScreen();
	screen.drawText(3, (Draw::centerX("Load your game from a saved file")), "Load your game from a saved file");
}

int Game::setDifficulty()
{
	screen.clearScreen();
	screen.drawText(3, (Draw::centerX("Select the difficulty")), "Select the difficulty");
	nostd::string options[3] = {"Easy", "Medium", "Hard"};
	int selected = 0;
	bool choose = false;
	while (!choose){
		for (int i = 0; i < 3; i++)
		{
			screen.drawSquareAround(options[i], 20, 58 + 15*i);
		}
		screen.attrOn(COLOR_PAIR(1));
		screen.drawText(20, 58 + 15*selected, options[selected]);
		screen.attrOff(COLOR_PAIR(1));

		switch (screen.getinput())
		{
			case KEY_LEFT:
				if (selected == 0) selected = 2;
				else selected = selected - 1;
				break;
			case KEY_RIGHT:
				if (selected == 2) selected = 0;
				else selected = selected + 1;
				break;
			case 10:
				choose = true;
				break;
		}
	}
	return selected;
}
