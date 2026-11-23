#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "map.h"

void generateMap(GameMap *game, int width, int height) {
    game->width = width;
    game->height = height;
    game->map = (char**)malloc(height * sizeof(char*));
    for (int i = 0; i < height; i++) {
        game->map[i] = (char*)malloc(width * sizeof(char));
    }
}

void freeMap(GameMap *game) {
    if (game->map == NULL) return;
    for (int i = 0; i < game->height; i++) {
        free(game->map[i]);
    }
    free(game->map);
    game->map = NULL;
}

void fillMap(GameMap *game, Player *player) {
    for (int y = 0; y < game->height; y++) {
        for (int x = 0; x < game->width; x++) {
            if (y == 0 || y == game->height - 1 || x == 0 || x == game->width - 1) {
                game->map[y][x] = '#';
            } else {
                game->map[y][x] = '.';
            }
        }
    }

    player->x = game->width / 2;
    player->y = game->height / 2;
    game->map[player->y][player->x] = '@';

    int fire_placed = 0;
    while (!fire_placed) {
        int fx = rand() % (game->width - 2) + 1;
        int fy = rand() % (game->height - 2) + 1;
        if (game->map[fy][fx] == '.') {
            game->map[fy][fx] = '^';
            game->fire_x = fx;
            game->fire_y = fy;
            fire_placed = 1;
        }
    }

    int cave_placed = 0;
    while (!cave_placed) {
        int cx = rand() % (game->width - 2) + 1;
        int cy = rand() % (game->height - 2) + 1;
        if (game->map[cy][cx] == '.') {
            game->map[cy][cx] = 'O';
            cave_placed = 1;
        }
    }

    for (int y = 1; y < game->height - 1; y++) {
        for (int x = 1; x < game->width - 1; x++) {
            if (game->map[y][x] != '.') continue;
            int chance = rand() % 100;
            if (chance < 15) game->map[y][x] = 'T';
            else if (chance < 30) game->map[y][x] = 'R';
        }
    }
}

void generateCave(GameMap *game, Player *player) {
    freeMap(game);
    int w = 5 + rand() % 4;
    int h = 5 + rand() % 4;
    generateMap(game, w, h);
    game->is_cave = 1;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (y == 0 || y == h - 1 || x == 0 || x == w - 1) game->map[y][x] = '#';
            else game->map[y][x] = '.';
        }
    }

    player->x = w / 2;
    player->y = h / 2;
    game->map[player->y][player->x] = '@';

    int cave_exit = 0;
    while (!cave_exit) {
        int cx = rand() % (w - 2) + 1;
        int cy = rand() % (h - 2) + 1;
        if (game->map[cy][cx] == '.') {
            game->map[cy][cx] = 'O';
            cave_exit = 1;
        }
    }

    for (int y = 1; y < h - 1; y++) {
        for (int x = 1; x < w - 1; x++) {
            if (game->map[y][x] != '.') continue;
            if (rand() % 100 < 15) game->map[y][x] = 'R';
        }
    }
}

void printMap(const GameMap *game) {
    for (int y = 0; y < game->height; y++) {
        for (int x = 0; x < game->width; x++) {
            printf("%c", game->map[y][x]);
        }
        printf("\n");
    }
}

void movePlayer(GameMap *game, Player *player, Direction dir, int *current_day, int *current_hour) {
    int new_x = player->x;
    int new_y = player->y;

    switch (dir) {
        case DIR_UP: new_y--; break;
        case DIR_DOWN: new_y++; break;
        case DIR_LEFT: new_x--; break;
        case DIR_RIGHT: new_x++; break;
        default: return;
    }

    if (new_x < 0 || new_x >= game->width || new_y < 0 || new_y >= game->height) return;
    
    char target = game->map[new_y][new_x];
    if (target == '#' || target == 'T' || target == 'R') return;

    player->stamina -= 1;

    game->steps_count++;
    if (game->steps_count >= 5) {
        player->hunger -= 1;
        if (player->hunger < 0) player->hunger = 0;
        game->steps_count = 0;
    }

    if (player->hunger < 20) {
        player->hp -= 1;
        if (player->hp < 0) player->hp = 0;
        printf("\nВы голодны! Вы теряете здоровье.\n");
    }

    if (game->is_cave == 0 && target == 'O') {
        printf("\nВы спустились в пещеру!\n");
        generateCave(game, player);
        return;
    }

    if (game->is_cave == 1 && target == 'O') {
        printf("\nВы выбрались из пещеры!\n");
        freeMap(game);
        int w = 8 + rand() % 23;
        int h = 8 + rand() % 23;
        generateMap(game, w, h);
        game->is_cave = 0;
        fillMap(game, player);
        game->distance++;
        (*current_hour)++;
        if (*current_hour >= 24) {
            *current_hour = 0;
            (*current_day)++;
        }
        player->stamina -= 5;
        if (player->stamina < 0) player->stamina = 0;
        player->gold += GOLD_BASE_MULT * game->distance;
        return;
    }

    if (player->x == game->fire_x && player->y == game->fire_y) {
        game->map[player->y][player->x] = '^';
    } else {
        game->map[player->y][player->x] = '.';
    }

    player->y = new_y;
    player->x = new_x;
    game->map[player->y][player->x] = '@'; 
}