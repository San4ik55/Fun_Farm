#ifndef UTILS_H
#define UTILS_H

#include "player.h"

int safeInputInt(const char *prompt);
int safeInputIntRange(const char *prompt, int min, int max);
int safeInputString(const char *prompt, char *buffer, int max_len);

void loadItemNames(char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]);
void writeDiary(const char *farmer_name, int day, int hour, const Player *player, char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]);
void evaluateInventory(const Player *player, char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]);
void searchItemByName(const Player *player, char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]);

#endif 