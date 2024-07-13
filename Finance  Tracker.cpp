//PROJECT: FINANCE REGISTER

#include <stdio.h>
#include <string.h>
#include <ctype.h>  

#define MAX_ENTRIES 50
#define FILENAME "entries.txt"

struct Entry {
    char date[11];        
    char description[50];  
    float amount;          
    char type;             
};

struct Entry entries[MAX_ENTRIES];
int entryCount = 0;

void saveEntriesToFile();
void addEntry();
void loadEntriesFromFile();
void checkEntries();
void checkBalance();
void deleteEntry();
void searchEntries();
void editEntry();
void printList();

int isNumeric(const char *str) {
    int dotCount = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            if (str[i] == '.' && dotCount == 0) {
                dotCount++;
            } else {
                return 0;
            }
        }
    }
    return 1;
}

float stringToFloat(const char *str) {
    float result = 0.0;
    float decimalMultiplier = 1.0;
    int decimalPointSeen = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '.') {
            decimalPointSeen = 1;
        } else if (isdigit(str[i])) {
            if (decimalPointSeen) {
                decimalMultiplier /= 10.0;
                result += (str[i] - '0') * decimalMultiplier;
            } else {
                result = result * 10.0 + (str[i] - '0');
            }
        }
    }

    return result;
}

void addEntry() {
    if (entryCount >= MAX_ENTRIES) {
        printf("You've reached the entry limit.\n");
        return;
    }

    struct Entry e;

    printf("Enter date as DD-MM-YYYY: ");
    scanf("%10s", e.date);

    printf("Enter description: ");
    scanf(" %[^\n]s", e.description);

    printf("Enter amount: ");
    char amountStr[20];
    scanf("%19s", amountStr);

    if (!isNumeric(amountStr)) {
        printf("Incorrect input. Please enter digits only.\n");
        return;
    }
    e.amount = stringToFloat(amountStr);

    printf("Enter type (I for Income, E for Expense): ");
    scanf(" %c", &e.type);

    if (e.type != 'I' && e.type != 'E') {
        printf("Incorrect input. Please enter 'I' or 'E'.\n");
        return;
    }

    entries[entryCount++] = e;

    FILE *file = fopen(FILENAME, "a");
    if (file != NULL) {
        fprintf(file, "%s;%s;%.2f;%c\n", e.date, e.description, e.amount, e.type);
        fclose(file);
    } else {
        printf("Error during file save.\n");
    }

    printf("Entry has been added.\n");
}

void loadEntriesFromFile() {
    FILE *file = fopen(FILENAME, "r");
    if (file != NULL) {
        while (fscanf(file, "%10[^;];%49[^;];%f;%c\n", entries[entryCount].date,
                      entries[entryCount].description, &entries[entryCount].amount,
                      &entries[entryCount].type) == 4) {
            entryCount++;
            if (entryCount >= MAX_ENTRIES) {
                printf("You've reached the entry limit.\n");
                break;
            }
        }
        fclose(file);
    } else {
        printf("No existing entries found.\n");
    }
}

void checkEntries() {
    if (entryCount == 0) {
        printf("No entries available.\n");
        return;
    }

    printf("\n%-12s %-20s %-10s %-10s\n", "Date", "Description", "Amount", "Type");
    printf("\n");

    for (int i = 0; i < entryCount; i++) {
        printf("%-12s %-20s %-10.2f %-10s\n", entries[i].date, entries[i].description,
               entries[i].amount, (entries[i].type == 'I') ? "Income" : "Expense");
    }
}

void checkBalance() {
    float balance = 0.0;
    float totalIncome = 0.0;
    float totalExpenses = 0.0;
    for (int i = 0; i < entryCount; i++) {
        if (entries[i].type == 'I') {
            totalIncome += entries[i].amount;
        } else if (entries[i].type == 'E') {
            totalExpenses += entries[i].amount;
        }
    }
    balance = totalIncome - totalExpenses;

    printf("\n");
    printf("            Current Balance       \n");
    printf("\n");
    printf("Total Income: %.2f\n", totalIncome);
    printf("Total Expenses: %.2f\n", totalExpenses);
    printf("Net Balance: %.2f\n", balance);
    printf("\n");
}

void deleteEntry() {
    if (entryCount == 0) {
        printf("No entries available to delete.\n");
        return;
    }
    int index;
    printf("Enter the index of the entry to delete (1 to %d): ", entryCount);
    scanf("%d", &index);

    if (index < 1 || index > entryCount) {
        printf("Invalid index.\n");
        return;
    }

    for (int i = index - 1; i < entryCount - 1; i++) {
        entries[i] = entries[i + 1];
    }
    entryCount--;
    saveEntriesToFile();
    printf("Entry deleted successfully.\n");
}

void saveEntriesToFile() {
    FILE *file = fopen(FILENAME, "w");
    if (file != NULL) {
        for (int i = 0; i < entryCount; i++) {
            fprintf(file, "%s;%s;%.2f;%c\n", entries[i].date, entries[i].description,
                    entries[i].amount, entries[i].type);
        }
        fclose(file);
    } else {
        printf("Error saving entries to file.\n");
    }
}

void searchEntries() {
    if (entryCount == 0) {
        printf("No entries available.\n");
        return;
    }

    char searchDesc[50];
    printf("Enter description to search: ");
    scanf(" %[^\n]s", searchDesc);

    printf("\n%-12s %-20s %-10s %-10s\n", "Date", "Description", "Amount", "Type");
    printf("\n");

    for (int i = 0; i < entryCount; i++) {
        if (strstr(entries[i].description, searchDesc) != NULL) {
            printf("%-12s %-20s %-10.2f %-10s\n", entries[i].date, entries[i].description,
                   entries[i].amount, (entries[i].type == 'I') ? "Income" : "Expense");
        }
    }
}
void editEntry() {
    if (entryCount == 0) {
        printf("No entries available to edit.\n");
        return;
    }

    int index;
    printf("Enter the index of the entry to edit (1 to %d): ", entryCount);
    scanf("%d", &index);

    if (index < 1 || index > entryCount) {
        printf("Invalid index.\n");
        return;
    }

    struct Entry *e = &entries[index - 1];

    printf("Editing entry at index %d\n", index);

    printf("Enter new date (DD-MM-YYYY) [%s]: ", e->date);
    char newDate[11];
    scanf("%s", newDate);
    if (strcmp(newDate, "") != 0) {
        strcpy(e->date, newDate);
    }

    printf("Enter new description [%s]: ", e->description);
    char newDescription[50];
    scanf(" %[^\n]s", newDescription);
    if (strcmp(newDescription, "") != 0) {
        strcpy(e->description, newDescription);
    }

    printf("Enter new amount [%.2f]: ", e->amount);
    char amountStr[20];
    scanf("%19s", amountStr);

    if (!isNumeric(amountStr)) {
        printf("Invalid input. Please enter digits only.\n");
        return;
    }
    e->amount = stringToFloat(amountStr);

    printf("Enter new type (I for Income, E for Expense) [%c]: ", e->type);
    char newType;
    scanf(" %c", &newType);

    if (newType == 'I' || newType == 'E') {
        e->type = newType;
    } else {
        printf("Invalid input. Retaining original value.\n");
    }
    saveEntriesToFile();
    printf("Entry has been edit successfully.\n");
}

void printList() {
    printf("\n \t Financial Diary\n");
    printf("1. Add Entry\n");
    printf("2. Check Entries\n");
    printf("3. Check Balance\n");
    printf("4. Delete Entry\n");
    printf("5. Search Entries\n");
    printf("6. Edit Entry\n");
    printf("7. Close\n");
    printf("Select an option: ");
}
int main() {
    int choice;
    loadEntriesFromFile();

    while (1) {
        printList();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addEntry();
                break;
            case 2:
                checkEntries();
                break;
            case 3:
                checkBalance();
                break;
            case 4:
                deleteEntry();
                break;
            case 5:
                searchEntries();
                break;
            case 6:
                editEntry();
                break;
            case 7:
                printf("Closing the application.\n");
                saveEntriesToFile();
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

