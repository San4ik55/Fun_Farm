#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
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

    initFoodCatalog();
    
    int choice;

    do {
        printf("\n=== МЕНЮ ===\n");
        printf("[0] Выход\n");
        printf("[1] Создать  персонажа\n");
        printf("[2] Карточка персонажа\n");
        printf("[3] Управление временем\n");
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
                    player->inventory[1] = ITEM_BERRY;
                    player->inventory[2] = ITEM_MEAT;
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

            case 3: {
                if (player == NULL) { printf("Сначала создайте персонажа!\n"); break; }
                
                int time_choice;
                do {
                    printf("\n--- УПРАВЛЕНИЕ ВРЕМЕНЕМ ---\n");
                    printf("[0] Назад\n");
                    printf("[1] Посмотреть на часы\n");
                    printf("[2] Подождать (Пропустить время)\n");
                    printf("[3] Лечь спать (Отдых)\n");
                    
                    time_choice = safeInputIntRange("Выбор: ", 0, 3);
                    
                    switch (time_choice) {
                        case 1:
                            printTime(current_day, current_hour);
                            break;
                        case 2: {
                            int hours = safeInputIntRange("Сколько часов ждать (1-8)? ", 1, 8);
                            if (current_hour + hours >= 26) {
                                printf("Нельзя ждать так долго! Наступит ночь, нужно спать.\n");
                            } else {
                                current_hour += hours;
                                while (current_hour >= 24) { current_hour -= 24; current_day++; }
                                printf("Вы подождали %d часов.\n", hours);
                            }
                            break;
                        }
                        case 3:
                            sleepTime(&current_day, &current_hour, player); 
                            break;
                    }
                } while (time_choice != 0);
                break;
            }

            case 4:
                if (player == NULL) { printf("Сначала создайте персонажа!\n"); break; }
                int hours = safeInputInt("Сколько часов работать? ");
                work(&current_day, &current_hour, player, hours);
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
                if (current_hour >= 2 && current_hour < 8) {
                    printf("Уже ночь (после 02:00)! Фермер должен спать.\n");
                    break;
                }

                GameMap game;
                game.map = NULL;
                game.is_cave = 0;
                game.distance = 0;
                game.steps_count = 0;
                int w = 8 + rand() % 23;
                int h = 8 + rand() % 23;
                generateMap(&game, w, h);
                fillMap(&game, player);

                int running = 1;
                int first_move = 1;

                while (running) {
                    system("cls");
                    
                   printf("HP: %d/%d | Выносливость: %d/%d | Сытость: %d/%d | Золото: %d | Карт пройдено: %d | Время: %02d:00\n",
       player->hp, player->max_hp, player->stamina, player->max_stamina,
       player->hunger, player->max_hunger, 
       player->gold, game.distance, current_hour);

                    if (game.is_cave == 1) printf("=== ВЫ В ПЕЩЕРЕ ===\n");
                    else printf("=== ВЫ В ЛЕСУ ===\n");

                    if (first_move) printf("Размер карты: %dx%d\n", w, h);

                    printMap(&game);
                    printf("\nУправление: W/A/S/D, Q-выход у костра.\nВаш ход: ");

                    char input_char = _getch();
                    if (input_char >= 'a' && input_char <= 'z') input_char -= 32;

                    Direction dir = DIR_NONE;
                    if (input_char == 'W') dir = DIR_UP;
                    else if (input_char == 'S') dir = DIR_DOWN;
                    else if (input_char == 'A') dir = DIR_LEFT;
                    else if (input_char == 'D') dir = DIR_RIGHT;
                    else if (input_char == 'Q') dir = DIR_QUIT;
                    else if (input_char == 'E') {
                        int ate = 0;
                        for (int i = 0; i < INV_SIZE; i++) {
                            int item_id = player->inventory[i];
                            
                            if (item_id == 10 || item_id == 11) { 
                                player->hunger += food_catalog[item_id].hunger_restore;
                                if (player->hunger > player->max_hunger) player->hunger = player->max_hunger;
                                
                                player->stamina += food_catalog[item_id].stamina_restore;
                                if (player->stamina > player->max_stamina) player->stamina = player->max_stamina;
                                
                                printf("\nВы съели %s! (Сытость +%d, Выносливость +%d)\n", 
                                       food_catalog[item_id].name, 
                                       food_catalog[item_id].hunger_restore, 
                                       food_catalog[item_id].stamina_restore);
                                       
                                player->inventory[i] = ITEM_EMPTY; 
                                ate = 1;
                                break; 
                            }
                        }
                        
                        if (!ate) {
                            printf("\nУ вас нет еды в рюкзаке (Ягод, Мяса и т.д.)!\n");
                            printf(">>>Enter>>> чтобы продолжить");
                            _getch(); _getch();
                        }
                        continue; 
                    }

                    if (dir == DIR_QUIT) {
                        if (player->x == game.fire_x && player->y == game.fire_y) {
                            printf("\nВы отдохнули у костра и вернулись домой.\n");
                            running = 0;
                        } else {
                            printf("\nНужно отдохнуть у костра (^)!\n");
                            printf(">>>Enter>>> чтобы продолжить");
                            _getch(); _getch();
                        }
                    } else if (dir != DIR_NONE) {
                         movePlayer(&game, player, dir, &current_day, &current_hour);
                        static int last_distance = 0;
                        if (game.distance > last_distance) {
                            current_hour++;
                            if (current_hour >= 24) { current_hour = 0; current_day++; }
                            last_distance = game.distance;
                        }
                        
                        if (player->stamina <= 0) {
                            printf("\nВы выбились из сил и упали без сознания!\n");
                            printf("Спасатели доставили вас домой, забрав половину золота.\n");
                            player->gold /= 2; 
                            player->stamina = 0;
                            printf(">>>Enter>>> чтобы продолжить");
                            _getch(); _getch();
                            running = 0;
                        }
                        else if (current_hour >= 2 && current_hour < 8) {
                            printf("\nНаступила глубокая ночь! Вы уснули на ходу.\n");
                            printf("Спасатели доставили вас домой, забрав половину золота.\n");
                            player->gold /= 2;
                            printf(">>>Enter>>> чтобы продолжить");
                            _getch(); _getch();
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