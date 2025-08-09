#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define FILE_PATH "cars.txt"

void add_car() {
    FILE *f = fopen(FILE_PATH, "a");
    if (!f) {
        printf("\nError opening file for appending.\n");

        perror("Error opening file for appending");

        return;
    }

    char number[20], time[20];

    printf("Parkify: One stop Solution to your car\n");
    printf("Enter Car Number: ");
    scanf("%s", number);
    printf("Enter Entry Time: ");
    scanf("%s", time);

    fprintf(f, "%s %s\n", number, time);
    fclose(f);
    printf("Car added successfully to %s\n", FILE_PATH);
}

void view_cars() {
    FILE *f = fopen(FILE_PATH, "r");
    if (!f) {
        printf("\nFile not found! No entries to show.\n");
        return;
    }

    char line[100];
    printf("\nCar Entries in %s\n", FILE_PATH);
    while (fgets(line, sizeof(line), f)) {
        printf("%s", line);
    }
    fclose(f);
}

void delete_car() {
    FILE *f = fopen(FILE_PATH, "r");
    if (!f) {
        printf("\nFile not found.\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        printf("Error creating temporary file.\n");
        fclose(f);
        return;
    }

    char number[20], time[20];
    char target[20];
    int found = 0;

    printf("Enter Car Number to Delete: ");
    scanf("%s", target);

    while (fscanf(f, "%s %s", number, time) == 2) {
        if (strcmp(number, target) != 0) {
            fprintf(temp, "%s %s\n", number, time);
        } else {
            found = 1;
        }
    }

    fclose(f);
    fclose(temp);
    remove(FILE_PATH);
    rename("temp.txt", FILE_PATH);

    if (found)
        printf("Car deleted successfully.\n");
    else
        printf("Car not found.\n");
}

void modify_car() {
    FILE *f = fopen(FILE_PATH, "r");
    if (!f) {
        printf("\nFile not found.\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        printf("Error creating temporary file.\n");
        fclose(f);
        return;
    }

    char number[20], time[20];
    char target[20];
    int found = 0;

    printf("Enter Car Number to Modify: ");
    scanf("%s", target);

    while (fscanf(f, "%s %s", number, time) == 2) {
        if (strcmp(number, target) == 0) {
            printf("Enter New Entry Time: ");
            scanf("%s", time);
            found = 1;
        }
        fprintf(temp, "%s %s\n", number, time);
    }

    fclose(f);
    fclose(temp);
    remove(FILE_PATH);
    rename("temp.txt", FILE_PATH);

    if (found)
        printf("Car modified successfully.\n");
    else
        printf("Car not found.\n");
}

int main() {
    int choice;
    while (1) {
        printf("\nParkify: One stop Solution to your car\n");
        printf("1. Add Car\n");
        printf("2. View Cars\n");
        printf("3. Delete Car\n");
        printf("4. Modify Car\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: add_car(); break;
            case 2: view_cars(); break;
            case 3: delete_car(); break;
            case 4: modify_car(); break;
            case 5: printf("Exiting...\n"); return 0;
            default: printf("Invalid choice. Please try again.\n");
        }
    }
}
