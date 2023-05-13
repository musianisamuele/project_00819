#include <unistd.h>
#include <cmath>

#include "game.hpp"
#include "menu.hpp"
#include "file.hpp"

#include "save.hpp"
#include "../physics/collisions.hpp"
#include "../../etc/logs/logs.hpp"

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
	File::initSettings();
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
			
			break;}
		case 1: 
			{// Resume game
			
			this->resume();
			break;
			}
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
				break;
			}
		}
		

	}
}


bool Game::exitGame(){
	// Esci dal gioco
	screen.clearScreen();
	screen.drawText(16, Draw::centerX("Are you sure you want to quit?"), "Are you sure you want to quit?");	
	nostd::string options[2] = {"Yes", "No"};
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
	return selected == 0;
}

void Game::start()
{
	// clear the screen and draw the border
	setDifficulty();
	this->map = Map();
	this->player = phy::Body();
	this->player.set_position(phy::Point(40, 20));
	this->player.set_acceleration(phy::Vector(1, -90));
	this->current_chunk = 0;
	play();

}

void Game::play(){

	screen.drawMap(this->map, 0);

	
	screen.drawPlayer(player.get_position());
	screen.refreshScreen();
	screen.nodel(true);
	deb::debug(player.get_position(), "player position");
	stats();
	

	bool exit = false;
	screen.nodel(true);
	int cumulative = 0;
	int count_not_key = 0;
	int which_key = 0;
	while (!exit){
		updateStats();
		bool right; 
		int input = screen.getinput();

		if (input == SETTINGS_CONTROL_KEYS[3]) // jump right

		{
			which_key = 1;
			cumulative++;
			count_not_key = 0;
		} 
		else if (input == SETTINGS_CONTROL_KEYS[2]) // jump left
		{
			which_key = 2;
			cumulative++;
			count_not_key = 0;
		}
		else if (input == SETTINGS_CONTROL_KEYS[4]){ // jump vertical
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
					{
						player.set_velocity(phy::Vector(JUMPF, 55));
						jump++;
					}

				else if (cumulative > 1 && which_key == 2 && map.get_chunk(current_chunk).is_there_a_platform(player.get_position() - phy::Point(0, 1)))
					{
						player.set_velocity(phy::Vector(JUMPF, 125));
						jump++;
					}

				else if (cumulative > 1 && which_key == 3 && map.get_chunk(current_chunk).is_there_a_platform(player.get_position() - phy::Point(0, 1)))
					{
						player.set_velocity(phy::Vector(JUMPF, 90));
						jump++;
					}
				cumulative = 0;
			}

		
		
			if (input == SETTINGS_CONTROL_KEYS[0]) // move player left
			{
				if(map.get_chunk(current_chunk).is_there_a_platform(player.get_position() - phy::Point(0, 1)))
					player.set_position(player.get_position() - phy::Point(1, 0));
			}
			else if (input == SETTINGS_CONTROL_KEYS[1]) // move player right
			{
				if(map.get_chunk(current_chunk).is_there_a_platform(player.get_position() - phy::Point(0, 1)))
					player.set_position(player.get_position() + phy::Point(1, 0));
			}

#ifdef USE_HACK
			case (KEY_UP):
				if (fly) player.set_position(player.get_position() + phy::Point(0, 1));
				break;
			case (KEY_LEFT):
				if (fly) player.set_position(player.get_position() + phy::Point(-1, 0));
				break;
			case (KEY_DOWN):
				if (fly) player.set_position(player.get_position() + phy::Point(0, -1));
				break;
			case (KEY_RIGHT):
				if (fly) player.set_position(player.get_position() + phy::Point(1, 0));
				break;
				
			case ((int) 'h'):
				screen.nodel(false);
				hack();
				screen.nodel(true);
				break;
#endif
			else if (input == 27)
			{
				bool quitGamepley = pauseGame(); // se true esci dal gioco
				if (quitGamepley == true) exit = true;
			}
			else
			{
				// default case
			}
					}
		// player.update(0.05);
#ifdef USE_HACK
		if (!fly) phy::updateWithCollisions(player, 0.15, map.get_chunk(current_chunk));
#else
		phy::updateWithCollisions(player, 0.15, map.get_chunk(current_chunk));
#endif
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
		screen.drawMap(map, current_chunk);
		screen.drawText(2, 1, nostd::to_string(current_chunk));
		screen.drawText(1, 1, nostd::to_string(player.get_position().get_xPosition()));
		screen.drawText(1, 5, nostd::to_string(player.get_position().get_yPosition()));
		screen.drawText(1, 140, nostd::to_string(JUMPF));
		//screen.drawText(2, 140, nostd::to_string(1+pow(1.1, - cumulative/50)));
		screen.drawText(3, 140, nostd::to_string(cumulative));
		napms(5);
	
	}
	screen.nodel(false);
	stats_scr.clearwithoutbox();
	stats_scr.refreshScreen();
	stats_scr.deleteWin();	
	screen.eraseScreen();
}

void Game::resume()
{	
	bool deleted = false; 
	do {
	screen.clearScreen();
	deleted = false;
	nostd::vector<nostd::string> savedMaps = File::getNames();
	nostd::vector<nostd::string> savedDate = File::getLastSaves();
	if (savedMaps.size() == 0) {
		screen.drawText(5, (Draw::centerX("No saved maps")), "No saved maps");
		screen.refreshScreen();
		screen.getinput();
	}
	else {
		int selected = 0;
		bool choose = false;
		bool exit = false;
		
		while (!choose && !exit){
			screen.drawText(3, (Draw::centerX("Load your game from a saved file")), "Load your game from a saved file");
			screen.drawText(7, (Draw::centerX("Press enter to play")), "Press enter to play");
			screen.drawText(9, (Draw::centerX("Press 'r' to remove a saved game")), "Press 'r' to remove a saved game");
			for (int i = 0; i < savedMaps.size(); i++)
			{
				screen.drawSquareAround(savedMaps[i] + " " + savedDate[i], 13 + 4*i, screen.centerX(savedMaps[i] + " " + savedDate[i]));
			}

			screen.attrOn(COLOR_PAIR(1));
			screen.drawText(13 + 4*selected, screen.centerX(savedMaps[selected] + " " + savedDate[selected]), savedMaps[selected] + " " + savedDate[selected]);
			screen.attrOff(COLOR_PAIR(1));

			switch (screen.getinput())
			{
				case KEY_UP:
					if (selected == 0) selected = savedMaps.size() - 1;
					else selected = selected - 1;
					break;
				case KEY_DOWN:
					if (selected == savedMaps.size() - 1) selected = 0;
					else selected = selected + 1;
					break;
				case 10:
					choose = true;
					break;
				case 'r':
					File::deleteSave(savedMaps[selected]);
					deleted = true;
					exit = true;
					break;
				case 27:
					exit = true;
					break;
				default:
					break;
			}
			screen.eraseScreen();
		}
		if (choose) {
			this->map = File::getMap(savedMaps[selected]);
			this->current_chunk = File::getChunk(savedMaps[selected]);
			deb::debug(savedMaps[selected]);
			this->player.set_position(File::getPoint(savedMaps[selected]));
			deb::debug(File::getPoint(savedMaps[selected]));
			play();
		}
	}
	} while (deleted);
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

void Game::stats()
{
	int posY, posX;
	screen.size(posY, posX, 44, 150);

	this->stats_scr = screen.newWindow(3, 150, posY - 2, posX);
	updateStats();
}

void Game::updateStats(){
	this->stats_scr.clearwithoutbox();

	this->stats_scr.drawRectagle(1, 0 , 3, 149);

	this->stats_scr.drawText(2, 2, "Lives: " );
	for (int i = 0; i < this->heart; i++)
	{
		this->stats_scr.drawText(2, 10 + i*2, "♥");
	}
	this->stats_scr.drawText(2, 50, "Level: " + nostd::to_string(this->current_chunk));
	this->stats_scr.drawText(2, 70, "Jump: " + nostd::to_string(this->jump));
	this->stats_scr.drawText(2, 90, "Coins: " + nostd::to_string(this->coins));
	this->stats_scr.refreshScreen();
}


bool Game::pauseGame()
{
	screen.nodel(false);

	bool resumed = false;
	bool exit = false;
	int posY, posX;
	screen.size(posY, posX, 46, 150);
	
	Draw pause = screen.newWindow(46, 60, posY, 90 + posX);
	while(!resumed) {

		pause.clearwithoutbox();
		pause.drawBox();
		pause.drawText(3, 30 - pause.center("Game Paused"),  "Game Paused");
		nostd::string options[4] = {"Resume", "Settings", "Save", "Exit"};
		int selected = 0;
		bool choose = false;
		
		stats_scr.redraw();
		screen.redraw();
		screen.noOutRefresh();
		stats_scr.noOutRefresh();
		pause.noOutRefresh();
		Screen::update();



		while (!choose){	
			for (int i = 0; i < 4; i++)
			{
				pause.drawSquareAround(options[i], 20 + 4*i, 30 - (options[i].length() / 2));
			}
			pause.attrOn(COLOR_PAIR(1));
			pause.drawText(20 + 4*selected, 30 - (options[selected].length() / 2), options[selected]);
			pause.attrOff(COLOR_PAIR(1));
			switch (pause.getinput())
			{
				case KEY_UP:
					if (selected == 0) selected = 3;
					else selected = selected - 1;
					break;
				case KEY_DOWN:
					if (selected == 3) selected = 0;
					else selected = selected + 1;
					break;
				case 10:
					choose = true;
					break;
				default: 
					break;
			}
		}
		pause.refreshScreen();
		Save save = Save();
		switch (selected)
		{
			case 0:
				screen.nodel(true);
				pause.deleteWin();

				resumed = true;
				break;

			case 1:{
				Settings settings = Settings();
				int posX, posY;
				screen.size(posY, posX, 46, 150);
				Draw settings_scr = screen.newWindow(46, 150, posY, posX);
				settings.drawFirstSettings(settings_scr);
				settings_scr.eraseScreen();
				settings_scr.deleteWin();
				


				break;
			}
			case 2:
			{
				int posY, posX;
				screen.size(posY, posX, 46, 150);
				
				Draw save_scr = screen.newWindow(46, 150, posY, posX);

				save.saveNewGame(save_scr, map, current_chunk, player.get_position());
				save_scr.eraseScreen();
				save_scr.deleteWin();
				break;
			}
				
			case 3: 
				{
				pause.clearScreen();
				pause.deleteWin();
				screen.redraw();
				stats_scr.redraw();	
				screen.noOutRefresh();
				stats_scr.noOutRefresh();
				Screen::update();

				
				save.quitGame(screen, map, current_chunk, player.get_position());

				resumed = true; 
				exit = true; 
				break;
				}	
			default:
				break;
			
		}
		} pause.deleteWin(); return exit;
}

#ifdef USE_HACK

void Game::hack(){
	int posX, posY;
	screen.size(posY, posX, 20, 50);
	Draw hack = screen.newWindow(20, 50, posY, posX);
	hack.attrOn(COLOR_PAIR(2));
	hack.drawBox();
	hack.drawText(2, 25 - hack.center("Hack Menu"), "Hack Menu");
	hack.drawText(4, 25 - hack.center("Only use for development purposes!") , "Only use for development purposes!");
	hack.drawText(7, 7, "1. Add life");
	hack.drawText(7, 25, "6. Remove life");
	hack.drawText(9, 7, "2. Add coin");
	hack.drawText(9, 25, "7. Remove coin");
	hack.drawText(11, 7, "3. Add jump");
	hack.drawText(11, 25, "8. Remove jump");
	hack.drawText(13, 7, "4. Add level");
	hack.drawText(13, 25, "9. Remove level");
	hack.drawText(15, 7, "5. Enable fly");
	hack.drawText(15, 25, "0. Disable fly");
	hack.refreshScreen();
	int x = hack.getinput();
	switch (x){
		case '1':
			this->heart++;
			break;
		case '2':
			this->coins++;
			break;
		case '3':
			this->jump++;
			break;
		case '4':
			this->current_chunk++;
			break;
		case '5':
			fly = true;
			break;
		case '6':
			this->heart--;
			break;
		case '7':
			this->coins--;
			break;
		case '8':
			this->jump--;
			break;
		case '9':
			this->current_chunk--;
			break;
		case '0':
			fly = false;
			break;
		case 's':  // set all custom 
		{

			hack.eraseScreen();
			hack.drawText(2, 25 - hack.center("Super secret menu"), "Super secret menu");
			hack.drawText(4, 25 - hack.center("WARNING: this menu is for skilled developers!"), "WARNING: this menu is for skilled developers!");
			nostd::string options[4] = {"1. Set life", "2. Set coins", "3. Set jump", "4. Set level"};
			for (int i = 0; i < 4; i++)
			{
				hack.drawText(7 + 2*i, 7, options[i]);
			}
			hack.refreshScreen();
			
			int secret = hack.getinput();
			switch (secret){
				case '1':
				{
					hack.eraseScreen();
					hack.drawText(2, 25 - hack.center("Set life"), "Set life");
					int custom = setCustom(hack);
					if (custom != -1) this->heart = custom;
					break;
				}
				case '2':
				{
					hack.eraseScreen();
					hack.drawText(2, 25 - hack.center("Set coins"), "Set coins");
					int custom = setCustom(hack);
					if (custom != -1) this->coins = custom;
					break;
				}
				case '3':
				{
					hack.eraseScreen();
					hack.drawText(2, 25 - hack.center("Set jump"), "Set jump");
					int custom = setCustom(hack);
					if (custom != -1) this->jump = custom;
					break;
				}
				case '4':
				{
					hack.eraseScreen();
					hack.drawText(2, 25 - hack.center("Set level"), "Set level");
					int custom = setCustom(hack);
					if (custom != -1) this->current_chunk = custom;
					break;
				}
				default:
					break;
			}
			break;
		}
		default:
			break;
	}
	hack.attrOff(COLOR_PAIR(2));
	hack.eraseScreen();
	hack.deleteWin();
}

int Game::setCustom(Draw hack){
	int a = hack.getinput();
	nostd::string set_chunk;
	while (a != 10  && a != 27){ 
		// controlla se l'input è un numero
		if (a >= 48 && a <= 57) set_chunk = set_chunk + nostd::to_string(char(a));
		// se è backspace cancella l'ultimo carattere
		else if (a == 127) set_chunk = set_chunk.substr(0, set_chunk.length() - 1);
		hack.clearLine(4, 0);
		hack.drawText(4, 25 - hack.center(set_chunk), set_chunk);
		a = hack.getinput();
	}
	if (a == 27) return -1;
	else return stoi(set_chunk);
}
#endif
