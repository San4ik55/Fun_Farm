#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"
#include "utils.h"

Player* createPlayer() {
    Player *p = (Player*)malloc(sizeof(Player));
    if (p == NULL) {
        printf("Ошибка выделения памяти под игрока!\n");
        return NULL;
    }

    p->hp = 100;
    p->max_hp = 100;
    p->stamina = 100;
    p->max_stamina = 100;
    p->gold = 0;
    p->x = 0;
    p->y = 0;

    for (int i = 0; i < INV_SIZE; i++) p->inventory[i] = ITEM_EMPTY;

    while (1) {
        int res = safeInputString("Введите имя фермера: ", p->name, MAX_NAME_LEN);
        if (res == 1) {
            if (strlen(p->name) == 0) {
                strcpy(p->name, "Лох");
                printf("Значит будешь Лох!\n");
            }
            break;
        } else {
            printf("Ошибка: имя слишком длинное!\n");
        }
    }
    
    return p;
}

void freePlayer(Player *player) {
    if (player != NULL) {
        printf("Память структуры по адресу %p освобождена\n", (void*)player);
        free(player);
    }
}

void printPlayerStats(const Player *player) {
    if (player == NULL) {
        printf("Персонаж еще не создан!\n");
        return;
    }
    printf("\n--- Карточка персонажа ---\n");
    printf("Имя: %s\n", player->name);
    printf("HP: %d/%d\n", player->hp, player->max_hp);
    printf("Выносливость: %d/%d\n", player->stamina, player->max_stamina);
    printf("Золото: %d\n", player->gold);
    printf("--------------------------\n");
}

void printTime(int day, int hour) {
    printf("Текущее время: День %d, %d Часов\n", day, hour);
}

void printInv(const Player *player, char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]) {
    printf("---Инвентарь---\n");
    for (int i = 0; i < INV_SIZE; i++) {
        int id = player->inventory[i];
        printf("Слот %d: [%d] (%s)\n", i, id, item_names[id]);
    }
}

void putItem(Player *player, int slot, int itemId, char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]) {
    if (slot < 0 || slot >= INV_SIZE) {
        printf("Ошибка! Слот должен быть от 0 до %d!\n", INV_SIZE - 1);
        return;
    }
    if (itemId < 0 || itemId > 9) {
        printf("Ошибка! ID предмета должен быть от 0 до 9!\n");
        return;
    }
    player->inventory[slot] = itemId;
    printf("В слот %d положен предмет: %s\n", slot, item_names[itemId]);
}

void dropItem(Player *player, int slot) {
    if (slot < 0 || slot >= INV_SIZE) {
        printf("Ошибка! Слот должен быть от 0 до %d!\n", INV_SIZE - 1);
        return;
    }
    player->inventory[slot] = ITEM_EMPTY;
    printf("Слот %d теперь пуст!\n", slot);
}

void deleteDuplicates(Player *player) {
    printf("Дубликаты удалены!\n");
    for (int i = 0; i < INV_SIZE; i++) {
        if (player->inventory[i] == ITEM_EMPTY) continue;
        for (int j = i + 1; j < INV_SIZE; j++) {
            if(player->inventory[j] == player->inventory[i]) {
                player->inventory[j] = ITEM_EMPTY;
            }
        }
    }
}

void work(int *day, int *hour, Player *player, int hoursWorked) {

    if (hoursWorked < 0) {
        printf("Ошибка! Работать в минус нельзя! А так хотелось...\n");
        return;
    }

    if (hoursWorked > MAX_WORK_HOURS) {
        printf("Не надорвешься? Нельзя работать больше %d часов!\n", MAX_WORK_HOURS);
        return;
    }

    if (*hour + hoursWorked >= 26) {
        printf("Уже поздно! Фермер должен спать. Идите отдыхать.\n");
        return;
    }

    if (player->stamina <= 0) {
        printf("У вас нет сил работать! Отдохните.\n");
        return;
    }

    *hour = *hour + hoursWorked;

    while (*hour >= HOURS_IN_DAY) {
        *hour = *hour - HOURS_IN_DAY;
        (*day)++;
    }


    player->stamina -= hoursWorked * 2;
    if (player->stamina < 0) player->stamina = 0; 

    printf("Вы работали целых %d часов! Уже день %d, %d часов\n", hoursWorked, *day, *hour);
    printf("Выносливость: %d/%d\n", player->stamina, player->max_stamina);
}