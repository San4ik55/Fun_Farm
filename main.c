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
    // Стартовое состояние
    printTime(current_day, current_hour);
    printInv(inventory);
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
int safeInputint(const char *prompt) {
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