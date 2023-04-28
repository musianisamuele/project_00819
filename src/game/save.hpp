#include <ncurses.h>
#include "../draw/draw.hpp"
#include "file.hpp"

#ifndef SAVE_HPP
#define SAVE_HPP

class Save {
    private: 
        bool alreadySaved; // per controllare se esiste già il nome del file di salvataggio
        std::string nome;
    public: 
        void saveNewGame(Draw screen, Map map, int chunk, phy::Point);
        void saveGame(Draw saved);

        void quitGame(Draw screen, Map map, int chunk, phy::Point);
};



#endif