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
    inventory[8] = ITEM_GOLD;
    inventory[9] = ITEM_COAL;
 

    // Главный цикл игры
    int choice;
    do {
        printf("\n=== МЕНЮ ===\n");
        printf("[0] Выход\n");
        printf("[1] Посмотреть на часы\n");
        printf("[2] Промотать время (Поработать)\n");
        printf("[3] Посмотреть инвентарь\n");
        printf("[4] Положить предмет в слот\n");
        printf("[5] Выбросить предмет\n");

        choice = safeInputInt("Ваш выбор: ");

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
                int slot = safeInputInt("Введите номер слота (0-9): ");
                int id = safeInputInt("Введите ID предмета (0-9): ");
                putItem(inventory, slot, id);
                break;
            }
            case 5: {
                int slot = safeInputInt("Введите номер слота для очистки (0-9): ");
                dropItem(inventory, slot);
                break;
            }
            
        }
    } while (choice != 0);

    return 0;
}

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

void work(int *day, int *hour, int hoursWorked) {
    if (hoursWorked < 0) {
        printf("Ошибка! Часы не могут быть отрицательными!\n");
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