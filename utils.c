#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"


int safeInputInt(const char *prompt) {
    int value;
    char ch;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1) {
            while ((ch = getchar()) != '\n' && ch != EOF);
            return value;
        } else {
            printf("Ошибка! Это не число!\n");
            while ((ch = getchar()) != '\n' && ch != EOF);
        }
    }
}

int safeInputIntRange(const char *prompt, int min, int max) {
    int value;
    while(1) {
        value = safeInputInt(prompt);
        if (value >= min && value <= max) {
            return value;
        }
        printf("Ошибка! Выберите число от %d до %d!\n", min, max);
    }
}

int safeInputString(const char *prompt, char *buffer, int max_len) {
    printf("%s", prompt);
    if (fgets(buffer, max_len, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            return 1;
        } else {
            int ch;
            while((ch = getchar()) != '\n' && ch != EOF);
            return 0;
        }
    }
    return 0;
}

void loadItemNames(char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]) {
    FILE *file = fopen("items.txt", "r");
    if (file == NULL) {
        printf("Внимание: файл items.txt не найден. Используются стандартные названия.\n");
        strcpy(item_names[0], "Пусто");
        strcpy(item_names[1], "Дерево");
        strcpy(item_names[2], "Камень");
        strcpy(item_names[3], "Семена");
        strcpy(item_names[4], "Топор");
        strcpy(item_names[5], "Кирка");
        strcpy(item_names[6], "Медь");
        strcpy(item_names[7], "Железо");
        strcpy(item_names[8], "Золото");
        strcpy(item_names[9], "Уголь");
        return;
    }
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (fgets(item_names[i], MAX_ITEM_NAME_LEN, file) == NULL) {
            strcpy(item_names[i], "Неизвестно");
        } else {
            item_names[i][strcspn(item_names[i], "\n")] = '\0';
        }
    }
    fclose(file);
}

void writeDiary(const char *farmer_name, int day, int hour, const Player *player, char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]) {
    FILE *file = fopen("diary.txt", "a");
    if (file == NULL) {
        printf("Ошибка: не удалось открыть diary.txt для записи!\n");
        return;
    }

    printf("\n--- Запись в дневник ---\n");
    printf("Фермер: %s\n", farmer_name);
    printf("Время: День %d, %02d:00\n", day, hour);
    printf("Рюкзак: ");
    
    for (int i = 0; i < INV_SIZE; i++) {
        if (player->inventory[i] != ITEM_EMPTY) {
            printf("%s ", item_names[player->inventory[i]]);
        }
    }
    printf("\n");

    fprintf(file, "=== День %d, %02d:00 ===\n", day, hour);
    fprintf(file, "Фермер: %s\n", farmer_name);
    fprintf(file, "Рюкзак: ");
    for (int i = 0; i < INV_SIZE; i++) {
        if (player->inventory[i] != ITEM_EMPTY) {
            fprintf(file, "%s ", item_names[player->inventory[i]]);
        }
    }
    fprintf(file, "\n\n");

    fclose(file);
    printf("Запись успешно добавлена в дневник\n");
}

void evaluateInventory(const Player *player, char item_names[MAX_ITEMS][MAX_ITEM_NAME_LEN]) {
    printf("Оценка рюкзака у скупщика\n");

    FILE *inputFile = fopen("input.txt", "r");
    if (inputFile == NULL) {
        printf("Ошибка! файл input.txt не найден!\n");
        return;
    }

    int prices[MAX_ITEMS] = {0};
    char item_name_buffer[MAX_ITEM_NAME_LEN];
    int price_buffer;

    while (fscanf(inputFile, "%s %d", item_name_buffer, &price_buffer) == 2) {
        for (int i = 0; i < MAX_ITEMS; i++) {
            if (strcmp(item_names[i], item_name_buffer) == 0) {
                prices[i] = price_buffer;
                break;
            }
        }
    }
    fclose(inputFile);

    FILE *outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        printf("Ошибка: не удалось создать output.txt!\n");
        return;
    }

    int total_cost = 0;
    int max_cost = -1;
    int max_id = -1;

    printf("--- Ведомость ---\n");
    fprintf(outputFile, "--- Ведомость ---\n");

    for (int i = 0; i < INV_SIZE; i++) {
        int id = player->inventory[i];
        if (id != ITEM_EMPTY) {
            int cost = prices[id];
            total_cost += cost;

            printf("Слот %d: %s - %d монет\n", i, item_names[id], cost);
            fprintf(outputFile, "Слот %d: %s - %d монет\n", i, item_names[id], cost);

            if (cost > max_cost) {
                max_cost = cost;
                max_id = id;
            }
        }
    }

    printf("----------------------\n");
    printf("Общая стоимость: %d монет\n", total_cost);
    fprintf(outputFile, "----------------------\n");
    fprintf(outputFile, "Общая стоимость: %d монет\n", total_cost);

    if (max_id != -1) {
        printf("Самый дорогой предмет: %s (%d монет)\n", item_names[max_id], max_cost);
        fprintf(outputFile, "Самый дорогой предмет: %s (%d монет)\n", item_names[max_id], max_cost);
    }

    fclose(outputFile);
    printf("Результат сохранен в output.txt\n");
}