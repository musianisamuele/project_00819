#include "enemy.hpp"
#include "coin.hpp"
#include "../../etc/nostd/vector.hpp"
#include "../maps/map.hpp"

#ifndef ENTITY_MANAGER
#define ENTITY_MANAGER

struct nemici {
  Enemy val;
  nemici* next;
};
typedef nemici* pnemici;

struct monete {
  Coin val;
  monete* next;
};
typedef monete* pmonete;

class Manager {
private:
  Map map;
  int current_chunk;
  nostd::vector<pnemici> Enemies;
  nostd::vector<pmonete> Coins;
  int Global_Entities;
  int Global_Enemies;
  int Global_Coins;

protected:
  //pnemici getAllEnemiesInChunk(int Chunk);
  //pmonete getAllCoinsInChunk(int Chunk);

public:

  Manager();
  ~Manager();

  void set_chunk(int Chunk);
  void add_enemy(int Chunk, Enemy enemy, phy::Point p)
  void add_coin(int Chunk, Coin coin, phy::Point p);

  void head_insert(int Chunk, Enemy enemy);
  void head_insert(int Chunk, Coin coin);
  void set_chunk(int Chunk, Map map);
  void collect_coin(int Chunk, Coin coin);
  void kill_entity(int Chunk, Enemy enemy);
  void move_enemies(int time);
  void print_enemy(); //la print enemy va chiamata dopo la chiamata del chunk

  }

};
#endif
