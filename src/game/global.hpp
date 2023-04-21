#ifndef GAME_GLOBAL
#define GAME_GLOBAL

#include <string>

#define MAX_VOLUME_LEVEL 20
#define MIN_VOLUME_LEVEL 0

#define MAX_SENSITIVITY_LEVEL 20
#define MIN_SENSITIVITY_LEVEL 0

#include "../../etc/nostd/string.hpp"



extern int SETTINGS_VOLUME_LEVEL;
extern int SETTINGS_SENSITIVITY_LEVEL;
extern std::string SETTINGS_CONTROL_KEYS[8];
extern bool SETTINGS_CALIBRATED;
extern int SETTINGS_PRESSURE_CALIBRATION;

#endif
