#include <stdio.h>

// Константы
#define INV_SIZE 10
#define START_DAY 1
#define START_HOUR 8
#define HOURS_IN_DAY 24

// ID предметов
#define ITEM_EMPTY 0
#define ITEM_WOOD 1
#define ITEM_STONE 2
#define ITEM_SEED 3
#define ITEM_AXE 4
#define ITEM_PICKAXE 5
#define ITEM_COPPER 6
#define ITEM_IRON 7
#define ITEM_GOLD 8
#define ITEM_COAL 9

// Имена предметов
const char *ITEM_NAMES[] = {
    "Пусто", "Дерево", "Камень", "Семена", "Топор", "Кирка", "Медь", "Железо", "Золото", "Уголь"
};

// Прототипы функций 
void printTime(int day, int hour);
void printInv(const int inventory[]);
void putItem(int inventory[], int slot, int itemId);
void dropItem(int inventory[], int slot);
void work(int *day, int *hour, int hoursWorked);
int safeInputInt(const char *prompt);
void removeDuplicates(int inventory[]);