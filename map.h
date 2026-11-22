#ifndef MAP_H
#define MAP_H

#include "player.h"

typedef struct {
    char **map;
    int width;
    int height;
    int fire_x;
    int fire_y;
    int is_cave;
    int distance;
} GameMap;

void generateMap(GameMap *game, int width, int height);
void fillMap(GameMap *game, Player *player);
void generateCave(GameMap *game, Player *player);
void freeMap(GameMap *game);
void printMap(const GameMap *game);
void movePlayer(GameMap *game, Player *player, Direction dir);

#endif