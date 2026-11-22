#ifndef PLAYER_H
#define PLAYER_H

#define MAX_NAME_LEN 33
#define MAX_ITEMS 10
#define MAX_ITEM_NAME_LEN 32
#define INV_SIZE 10
#define START_DAY 1
#define START_HOUR 8
#define HOURS_IN_DAY 24
#define MAX_WORK_HOURS 16

#define ITEM_EMPTY 0
#define ITEM_WOOD 1
#define ITEM_STONE 2
#define ITEM_SEED 3
#define ITEM_AXE 4
#define ITEM_PICKAXE 5
#define ITEM_COPPER 6
#define ITEM_IRON 7
#define ITEM_GOLD 8
#define ITEM_COAL 

typedef enum {
    DIR_NONE = 0,
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT,
    DIR_QUIT
} Direction;

typedef struct {
    char name[MAX_NAME_LEN];
    int hp;     
    int max_hp;   
    int stamina;
    int max_stamina;
    int gold; 
    int x, y;
    int inventory[INV_SIZE];
} Player;

Player* createPlayer();
void freePlayer(Player *player);
void printPlayerStats(const Player *player);

Player* createPlayer();
void freePlayer(Player *player);
void printPlayerStats(const Player *player);

void printTime(int day, int hour);
void printInv(const Player *player, char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]);
void putItem(Player *player, int slot, int itemId, char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]);
void dropItem(Player *player, int slot);
void deleteDuplicates(Player *player);
void work(int *day, int *hour, int hoursWorked);

#endif 