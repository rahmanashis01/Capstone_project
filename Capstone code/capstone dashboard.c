#include <stdio.h>
#include <stdlib.h> // Needed for exit()
#include <string.h> // Needed for strcmp() to compare strings

// This structure holds all the information for a single user.
struct User {
    char name[100];
    char phone[20];
    char vehicleMake[50];
    char vehicleModel[50];
    char licensePlate[20];
};

// --- Function Declarations ---
void registerUser();
void loginUser(); // Our new function for logging in
void clearInputBuffer();

// This is the heart of our program
int main() {
    int choice;

    // This while(1) loop keeps the menu running until the user exits.
    while (1) {
        printf("\n--- Garage Management System ---\n");
        printf("1. Register a New User\n");
        printf("2. Login\n"); // New menu option
        printf("0. Exit\n");
        printf("----------------------------------\n");
        printf("Enter your choice: ");

        // Check if the input is a valid number
        if (scanf("%d", &choice) != 1) {
            printf("\nInvalid input! Please enter a number.\n");
            clearInputBuffer(); // Clear the faulty input
            continue; // Skip the rest of the loop and show the menu again
        }

        clearInputBuffer(); // Clean up the input after reading the number

        // The switch statement checks the user's choice.
        switch (choice) {
            case 1:
                registerUser(); // Call the registration function
                break;
            case 2:
                loginUser(); // Call the new login function
                break;
            case 0:
                printf("\nThank you for using the system. Goodbye!\n");
                exit(0); // This command stops the program.
                break;
            default:
                printf("\nInvalid choice! Please try again.\n");
                break;
        }
    }

    return 0;
}


// --- Function Definitions ---

/**
 * @brief Allows a new user to register and saves their info to a file.
 * (This function is the same as before)
 */
void registerUser() {
    struct User newUser;
    FILE *filePtr;

    filePtr = fopen("users.txt", "a");

    if (filePtr == NULL) {
        printf("Error! Could not open the file.\n");
        return;
    }

    printf("\n--- New User Registration ---\n");
    printf("Enter your full name: ");
    scanf(" %[^\n]", newUser.name);
    printf("Enter your phone number: ");
    scanf(" %[^\n]", newUser.phone);
    printf("Enter vehicle make (e.g., Toyota): ");
    scanf(" %[^\n]", newUser.vehicleMake);
    printf("Enter vehicle model (e.g., Corolla): ");
    scanf(" %[^\n]", newUser.vehicleModel);
    printf("Enter vehicle license plate: ");
    scanf(" %[^\n]", newUser.licensePlate);

    fprintf(filePtr, "%s,%s,%s,%s,%s\n",
            newUser.name,
            newUser.phone,
            newUser.vehicleMake,
            newUser.vehicleModel,
            newUser.licensePlate);

    fclose(filePtr);
    printf("\nRegistration successful! User has been saved.\n");
}

/**
 * @brief Logs a user in by checking their name and phone against the stored records.
 */
void loginUser() {
    char inputName[100];
    char inputPhone[20];
    int loginSuccess = 0; // A flag to check if login was successful. 0 = false, 1 = true.

    struct User fileUser; // A struct to hold the data we read from the file
    FILE *filePtr;

    // Open the users.txt file in "read" mode.
    // "r" means we can only read from the file, not write to it.
    filePtr = fopen("users.txt", "r");

    if (filePtr == NULL) {
        printf("\nNo users registered yet or file not found. Please register first.\n");
        return; // Exit the function
    }

    printf("\n--- User Login ---\n");
    printf("Enter your full name: ");
    scanf(" %[^\n]", inputName);
    printf("Enter your phone number: ");
    scanf(" %[^\n]", inputPhone);

    // Read the file line by line until the end (EOF - End Of File)
    while (fscanf(filePtr, "%99[^,],%19[^,],%49[^,],%49[^,],%19[^\n]\n",
                  fileUser.name,
                  fileUser.phone,
                  fileUser.vehicleMake,
                  fileUser.vehicleModel,
                  fileUser.licensePlate) != EOF) {

        // Compare the name and phone from the file with the user's input
        // strcmp() returns 0 if the strings are identical.
        if (strcmp(inputName, fileUser.name) == 0 && strcmp(inputPhone, fileUser.phone) == 0) {
            loginSuccess = 1; // We found a match!
            break; // Exit the loop since we found our user.
        }
    }

    fclose(filePtr); // Close the file

    // Check the flag to see if we found a match
    if (loginSuccess == 1) {
        printf("\nLogin successful! Welcome, %s.\n", inputName);
        // NOTE: Here is where we would show a new menu for a logged-in user.
        // We will add that in a future step.
    } else {
        printf("\nLogin failed! Incorrect name or phone number.\n");
    }
}


/**
 * @brief Cleans any leftover characters from the input buffer.
 */
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

