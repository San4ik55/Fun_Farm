#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include "player.h"
#include "map.h"
#include "utils.h"

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    srand(time(NULL));

    Player *player = NULL;
    int current_day = START_DAY;
    int current_hour = START_HOUR;
    char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN];
    loadItemNames(item_names);
    
    int choice;

    do {
        printf("\n=== МЕНЮ ===\n");
        printf("[0] Выход\n");
        printf("[1] Создать  персонажа\n");
        printf("[2] Карточка персонажа\n");
        printf("[3] Посмотреть на часы\n");
        printf("[4] Поработать\n");
        printf("[5] Посмотреть инвентарь\n");
        printf("[6] Положить предмет в слот\n");
        printf("[7] Выбросить предмет\n");
        printf("[8] Удалить дубликаты\n");
        printf("[9] Записать состояние в дневник\n");
        printf("[10] Оценка инвентаря у скупщика\n");
        printf("[11] Отправиться в лес\n");

        choice = safeInputIntRange("Ваш выбор: ", 0, 11);

        switch (choice) {
            case 0:
                printf("Выход из игры...\n");
                break;

            case 1:
                if (player != NULL) {
                    printf("Персонаж уже существует. Удалить его? (1 - Да, 0 - Нет): ");
                    int confirm = safeInputIntRange("", 0, 1);
                    if (confirm == 1) {
                        freePlayer(player);
                        player = NULL;
                    } else {
                        break;
                    }
                }
                player = createPlayer();
                if (player != NULL) {
                    player->inventory[1] = ITEM_WOOD;
                    player->inventory[2] = ITEM_STONE;
                    player->inventory[3] = ITEM_AXE;
                    printf("Персонаж успешно создан!\n");
                }
                break;

            case 2:
                if (player == NULL) {
                    printf("Ошибка: сначала создайте персонажа (пункт 1)!\n");
                } else {
                    printPlayerStats(player);
                    printTime(current_day, current_hour);
                }
                break;

            case 3:
                printTime(current_day, current_hour);
                break;

            case 4:
                if (player == NULL) { printf("Сначала создайте персонажа!\n"); break; }
                int hours = safeInputInt("Сколько часов работать? ");
                work(&current_day, &current_hour, hours);
                break;

            case 5:
                if (player == NULL) { printf("Сначала создайте персонажа!\n"); break; }
                printInv(player, item_names);
                break;

            case 6: {
                if (player == NULL) { printf("Сначала создайте персонажа!\n"); break; }
                int slot = safeInputIntRange("Введите номер слота (0-9): ", 0, 9);
                int id = safeInputIntRange("Введите ID предмета (0-9): ", 0, 9);
                putItem(player, slot, id, item_names);
                break;
            }

            case 7: {
                if (player == NULL) { printf("Сначала создайте персонажа!\n"); break; }
                int slot = safeInputIntRange("Введите номер слота для очистки (0-9): ", 0, 9);
                dropItem(player, slot);
                break;
            }

            case 8:
                if (player == NULL) { printf("Сначала создайте персонажа!\n"); break; }
                deleteDuplicates(player);
                break;

            case 9:
                if (player == NULL) { printf("Сначала создайте персонажа!\n"); break; }
                writeDiary(player->name, current_day, current_hour, player, item_names);
                break;

            case 10:
                if (player == NULL) { printf("Сначала создайте персонажа!\n"); break; }
                evaluateInventory(player, item_names);
                break;

            case 11: {
                if (player == NULL) { printf("Сначала создайте персонажа!\n"); break; }
                
                if (player->stamina <= 0) {
                    printf("У вас нет сил идти в лес! Отдохните.\n");
                    break;
                }

                GameMap game;
                game.map = NULL;
                game.is_cave = 0;
                game.distance = 0;

                int w = 8 + rand() % 23;
                int h = 8 + rand() % 23;
                generateMap(&game, w, h);
                fillMap(&game, player);

                int running = 1;
                int first_move = 1;

                while (running) {
                    system("cls");
                    
                    if (game.is_cave == 1) {
                        printf("=== ВЫ В ПЕЩЕРЕ ===\n");
                    } else {
                        printf("=== ВЫ В ЛЕСУ (Пройдено: %d) ===\n", game.distance);
                    }
                    printf("HP: %d/%d | Выносливость: %d/%d\n", player->hp, player->max_hp, player->stamina, player->max_stamina);
                    
                    if (first_move) {
                        printf("Размер карты: %dx%d\n", w, h);
                    }

                    printMap(&game);
                    printf("\nУправление: W-вверх, A-влево, S-вниз, D-вправо, Q-выход у костра.\n");
                    printf("Ваш ход: ");

                    char input_char;
                    scanf(" %c", &input_char);
                    
                    if (input_char >= 'a' && input_char <= 'z') input_char -= 32;

                    Direction dir = DIR_NONE;
                    if (input_char == 'W') dir = DIR_UP;
                    else if (input_char == 'S') dir = DIR_DOWN;
                    else if (input_char == 'A') dir = DIR_LEFT;
                    else if (input_char == 'D') dir = DIR_RIGHT;
                    else if (input_char == 'Q') dir = DIR_QUIT;

                    if (dir == DIR_QUIT) {
                        if (player->x == game.fire_x && player->y == game.fire_y) {
                            printf("\nВы отдохнули у костра и вернулись домой.\n");
                            running = 0;
                        } else {
                            printf("\nНужно отдохнуть у костра (^)!\n");
                            printf(">>>Enter>>> чтобы продолжить");
                            getchar(); getchar();
                        }
                    } else if (dir != DIR_NONE) {
                        movePlayer(&game, player, dir);
                    
                        if (player->stamina <= 0) {
                            printf("\nВы выбились из сил и вернулись домой!\n");
                            printf(">>>Enter>>> чтобы продолжить");
                            getchar(); getchar();
                            running = 0;
                        }
                    }
                    first_move = 0;
                }

                freeMap(&game);
                break;
            }
        }
    } while (choice != 0);

    if (player != NULL) {
        freePlayer(player);
    }

    return 0;
}