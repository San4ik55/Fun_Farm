#include <stdio.h>
#include <string.h>

// Константы
#define INV_SIZE 10
#define START_DAY 1
#define START_HOUR 8
#define HOURS_IN_DAY 24
#define MAX_WORK_HOURS 16
#define MAX_NAME_LEN 33

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
void deleteDuplicates(int inventory[]);
int safeInputIntRange(const char *prompt, int min, int max);
int safeInputString(const char *prompt, char *buffer, int max_len);


int main() {
    // Игровое время
    int current_day = START_DAY;
    int current_hour = START_HOUR;
    // Инвентарь
    int inventory[INV_SIZE] = {0};

    inventory[0] = ITEM_EMPTY;
    inventory[1] = ITEM_WOOD;
    inventory[2] = ITEM_STONE;
    inventory[3] = ITEM_SEED;
    inventory[4] = ITEM_AXE;
    inventory[5] = ITEM_PICKAXE;
    inventory[6] = ITEM_COPPER;
    inventory[7] = ITEM_IRON;
    inventory[8] = ITEM_STONE;
    inventory[9] = ITEM_COAL;

 // Имя Фермера 
 char farmer_name[MAX_NAME_LEN];
 while (1) {
     int result = safeInputString("Введи свое имя Фермер!", farmer_name, MAX_NAME_LEN);
     if (result == 1) {
     if (strlen(farmer_name) == 0) {
        strcpy(farmer_name, "Лох");
        printf("Значит будешь Лох!\n");
     }
     break;
    } else {
        printf("Ошибка! Имя не должно быть длинее %d символов!\n", MAX_NAME_LEN - 1);
    }
}   

    // Главный цикл игры
    int choice;
    do {
        printf("\n=== Здраствуй, %s! ===\n", farmer_name);
        printf("=== МЕНЮ ===\n");
        printf("[0] Выход\n");
        printf("[1] Посмотреть на часы\n");
        printf("[2] Поработать\n");
        printf("[3] Посмотреть инвентарь\n");
        printf("[4] Положить предмет в слот\n");
        printf("[5] Выбросить предмет\n");
        printf("[6] Удалить дубликаты\n");

        choice = safeInputIntRange("Ваш выбор: ", 0, 6);

        switch (choice) {
            case 0:
                printf("Выход из игры...\n");
                break;
            case 1:
                printTime(current_day, current_hour);
                break;
            case 2: {
                int hours = safeInputInt("Сколько часов работать? ");
                work(&current_day, &current_hour, hours);
                break;
            }
            case 3:
                printInv(inventory);
                break;
            case 4: {
                int slot = safeInputIntRange("Введите номер слота (0-9): ", 0, 9);
                int id = safeInputIntRange("Введите ID предмета (0-9): ", 0, 9);
                putItem(inventory, slot, id);
                break;
            }
            case 5: {
                int slot = safeInputIntRange("Введите номер слота для очистки (0-9): ", 0, 9);
                dropItem(inventory, slot);
                break;
            }
            case 6:
            deleteDuplicates(inventory);
            break;
        }
    } while (choice != 0);

    return 0;
}

// Защита от дурака
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
// Функции
void printTime(int day, int hour) {
    printf("Текущее время: День %d, %d Часов\n", day, hour);
}
void printInv(const int inventory[]) {
    printf("---Инвентарь---\n");
    for (int i = 0; i < INV_SIZE; i++) {
        int id = inventory[i];
        printf("Слот %d: [%d] (%s)\n", i, id, ITEM_NAMES[id]);
    }
}
void work(int *day, int *hour, int hoursWorked) {
    if (hoursWorked < 0) {
        printf("Ошибка! Работать в минус нельзя! А так хотелось...\n");
        return;

    }
    if (hoursWorked > MAX_WORK_HOURS ) {
        printf("Не надорвешься? Нельзя работать больше %d часов!\n", MAX_WORK_HOURS);
        return;
    }

    *hour = *hour + hoursWorked;
    
    while (*hour >= HOURS_IN_DAY) {
        *hour = *hour - HOURS_IN_DAY;
        (*day)++;
    }

    printf("Вы работали целых %d часов! Уже день %d, %d часов\n",
         hoursWorked, *day, *hour);
}

void putItem(int inventory[], int slot, int itemId) {
    if (slot < 0 || slot >= INV_SIZE) {
        printf("Ошибка! Слот должен быть от 0 до %d!\n", INV_SIZE - 1);
        return;
    }

    if (itemId < 0 || itemId >9) {
        printf("Ошибка! ID предмета должен быть от 0 до 9!\n");
        return;
    }

    inventory[slot] = itemId;
    printf("В слот %d положен предмет: %s\n", slot, ITEM_NAMES[itemId]);
}

void dropItem(int inventory[], int slot) {
     if (slot < 0 || slot >= INV_SIZE) {
        printf("Ошибка! Слот должен быть от 0 до %d!\n", INV_SIZE - 1);
        return;
    }

    inventory[slot] = ITEM_EMPTY;
    printf("Слот %d теперь пуст!\n", slot);
}

void deleteDuplicates( int inventory[]) {
    printf("Дубликаты удалены!\n");

    for (int i = 0; i < INV_SIZE; i++) {
        if (inventory[i] ==  ITEM_EMPTY) {
            continue;
        }

        for (int j = i + 1; j < INV_SIZE; j++) {
            if(inventory[j] == inventory[i]) {
                inventory[j] = ITEM_EMPTY;
            }
        }
    }

}  