// =================================================================================
// Parkify: Smart Garage Management System
// A C-based console application for a Software Development Capstone Project.
// This code demonstrates core concepts including structs, file I/O, pointers,
// and a modular, menu-driven design based on the provided project documentation.
// =================================================================================

// --- Needed Header Files ---
#include <stdio.h>    // For standard I/O (printf, scanf, fopen, etc.)
#include <stdlib.h>   // For standard library functions (system, exit, etc.)
#include <string.h>   // For string manipulation (strcpy, strcmp, etc.)
#include <time.h>     // For getting current time for timestamps

// --- Global Constants & Macros ---
// Using #define for constants makes the code easier to read and modify.
#define MAX_USERS 100
#define MAX_CARS_PER_USER 3
#define MAX_NAME_LEN 50
#define MAX_ID_LEN 20
#define MAX_PLATE_LEN 15
#define MAX_MODEL_LEN 30
#define MAX_SERVICE_DESC_LEN 100
#define MAX_DATE_LEN 11       // YYYY-MM-DD
#define MAX_TIME_LEN 6        // HH:MM
#define MAX_STATUS_LEN 20
#define MAX_QUERY_LEN 200

// File names for data persistence
#define USERS_FILE "users.dat"
#define APPOINTMENTS_FILE "appointments.dat"
#define PARKING_FILE "parking.dat"
#define REFUELING_FILE "refueling.dat"
#define QUERIES_FILE "queries.dat"
#define INVOICES_FILE "invoices.dat"

// --- Data Structures (struct) ---
// Structs are used to group related variables into a single, logical unit.

typedef struct {
    char plateNumber[MAX_PLATE_LEN];
    char carModel[MAX_MODEL_LEN];
    char engineType[MAX_NAME_LEN];
    int year;
} Car;

typedef struct {
    char userID[MAX_ID_LEN];
    char password[MAX_ID_LEN]; // WARNING: Storing plain text is insecure!
    char fullName[MAX_NAME_LEN];
    int age;
    char email[MAX_NAME_LEN];
    char phone[MAX_ID_LEN];
    char address[100];
    Car cars[MAX_CARS_PER_USER];
    int numCars;
    int isAdmin; // 0 for customer, 1 for admin, 2 for mechanic
} User;

typedef struct {
    char partName[MAX_NAME_LEN];
    char partCode[MAX_ID_LEN];
    int quantity;
    double price;
    int isAvailable; // 1 if available, 0 if not
} Part;

typedef struct {
    char invoiceID[MAX_ID_LEN];
    char userID[MAX_ID_LEN];
    char serviceType[MAX_SERVICE_DESC_LEN];
    double totalCost;
    double discount;
    char date[MAX_DATE_LEN];
    char status[MAX_STATUS_LEN]; // "Paid", "Pending", "Cancelled"
} Invoice;

typedef struct {
    char serviceID[MAX_ID_LEN];
    char userID[MAX_ID_LEN];
    char carPlate[MAX_PLATE_LEN];
    char serviceType[MAX_SERVICE_DESC_LEN];
    char appointmentDate[MAX_DATE_LEN];
    char appointmentTime[MAX_TIME_LEN];
    char status[MAX_STATUS_LEN];
    double estimatedCost;
    char invoiceID[MAX_ID_LEN];
} ServiceAppointment;

typedef struct {
    char parkingID[MAX_ID_LEN];
    char carPlate[MAX_PLATE_LEN];
    char checkInTime[MAX_DATE_LEN + MAX_TIME_LEN + 2];
    char checkOutTime[MAX_DATE_LEN + MAX_TIME_LEN + 2];
    double parkingFee;
    char userID[MAX_ID_LEN];
} ParkingRecord;

// --- Global Variables & Pointers ---
// Global arrays act as our in-memory database.
User users[MAX_USERS];
int numUsers = 0;

// Sample parts inventory
Part partsInventory[] = {
    {"Engine Oil", "EO001", 50, 25.99, 1},
    {"Brake Pads", "BP002", 30, 89.99, 1},
    {"Air Filter", "AF003", 25, 15.99, 1},
    {"Spark Plugs", "SP004", 0, 12.99, 0}, // Out of stock
    {"Battery", "BT005", 15, 120.99, 1},
    {"Transmission Fluid", "TF006", 0, 35.99, 0}, // Out of stock
    {"Radiator", "RD007", 8, 250.99, 1},
    {"Tire Set", "TS008", 20, 400.99, 1}
};
int numParts = 8;

// This is a POINTER to a User struct. It will point to the specific user
// in the 'users' array who is currently logged in. It's NULL if no one is logged in.
User *loggedInUser = NULL;

// --- Function Prototypes ---
// Declaring functions before use is good practice in C.
void loadAllData();
void saveAllData();
void registerUser();
void loginUser();
void loggedInMenu();
void customerServicesMenu();
void requestParking();
void checkoutParking();
void requestRefueling();
void adminPanel();
void mechanicPanel();
void viewCustomerHistory();
void manageUserProfile();
void viewAvailableParts();
void orderParts();
void calculateEstimation();
void generateInvoice();
void confirmAppointment();
void viewParkingHistory();
void qnaSection();
void viewCarProfile();
void mechanicAssignment();
void serviceCalendarView();
void displayMainDashboard();
void displayCustomerDashboard();
void displayMechanicDashboard();
void displayAdminDashboard();
void drawProgressBar(int current, int max, int width);
void printDashboardHeader(const char* title);

// --- Utility Functions ---
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pressAnyKeyToContinue() {
    printf("\nPress Enter to continue...");
    // These two getchar() calls robustly handle leftover input from scanf
    while (getchar() != '\n');
    getchar();
}

// Safely reads a line of text, preventing buffer overflows.
void readLine(char *buffer, int size) {
    fgets(buffer, size, stdin);
    // fgets includes the newline character, so we remove it.
    buffer[strcspn(buffer, "\n")] = '\0';
}

void getCurrentDateTime(char *dateTimeBuffer, int bufferSize) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(dateTimeBuffer, bufferSize, "%04d-%02d-%02d %02d:%02d",
             tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min);
}

void generateUniqueId(char *id_buffer, const char *prefix, int count) {
    // A simple ID generator for demonstration.
    sprintf(id_buffer, "%s%ld%03d", prefix, time(NULL), count % 1000);
}

// --- File Handling Functions ---

void saveUsers() {
    // 'w' mode: write mode. It creates the file if it doesn't exist,
    // or TRUNCATES (empties) it if it does. This is good for saving the
    // entire state, as it prevents duplicate entries from previous saves.
    FILE *file = fopen(USERS_FILE, "w");
    if (file == NULL) {
        perror("Error opening users file for writing");
        return;
    }
    // Loop through the in-memory array and write each user to the file.
    for (int i = 0; i < numUsers; i++) {
        // fprintf is like printf, but it writes to a file.
        fprintf(file, "%s %s %s %d %s %s %s %d %d\n", 
                users[i].userID, users[i].password, users[i].fullName, 
                users[i].age, users[i].email, users[i].phone, users[i].address,
                users[i].isAdmin, users[i].numCars);
        for (int j = 0; j < users[i].numCars; j++) {
            fprintf(file, "%s %s %s %d\n", 
                    users[i].cars[j].plateNumber, users[i].cars[j].carModel,
                    users[i].cars[j].engineType, users[i].cars[j].year);
        }
    }
    fclose(file); // Always close a file when you're done.
}

void loadUsers() {
    // 'r' mode: read mode. The file must exist.
    FILE *file = fopen(USERS_FILE, "r");
    if (file == NULL) {
        // This is not necessarily an error on first run, so we just return.
        return;
    }
    // Loop until we reach the end of the file (EOF) or the array is full.
    // The check `fscanf(...) == 9` ensures we read a full, valid user record.
    // If it returns a different number, the line is malformed or we're at EOF.
    numUsers = 0;
    while (numUsers < MAX_USERS &&
           fscanf(file, "%s %s %s %d %s %s %s %d %d", 
                  users[numUsers].userID, users[numUsers].password, 
                  users[numUsers].fullName, &users[numUsers].age,
                  users[numUsers].email, users[numUsers].phone, users[numUsers].address,
                  &users[numUsers].isAdmin, &users[numUsers].numCars) == 9) {

        for (int j = 0; j < users[numUsers].numCars; j++) {
            // Check the return value of fscanf to ensure we read car data correctly.
            if (fscanf(file, "%s %s %s %d", 
                      users[numUsers].cars[j].plateNumber, 
                      users[numUsers].cars[j].carModel,
                      users[numUsers].cars[j].engineType,
                      &users[numUsers].cars[j].year) != 4) {
                 printf("Error reading car data for user %s.\n", users[numUsers].userID);
                 break;
            }
        }
        numUsers++;
    }
    fclose(file);
}

// NOTE: In a full application, you would have load/save functions for every data type
// (Appointments, Parking, etc.) following the same pattern as loadUsers/saveUsers.
// For brevity, this example focuses on persisting user data.

void loadAllData() {
    loadUsers();
    // In a full app, you'd call:
    // loadAppointments();
    // loadParkingRecords();
    // etc.
}

void saveAllData() {
    saveUsers();
    // In a full app, you'd call:
    // saveAppointments();
    // saveParkingRecords();
    // etc.
}


// --- Main Application Logic ---

void registerUser() {
    clearScreen();
    printf("--- Parkify: User Registration ---\n");

    if (numUsers >= MAX_USERS) {
        printf("System user limit reached. Cannot register.\n");
        pressAnyKeyToContinue();
        return;
    }

    User newUser; // A temporary user struct on the stack.

    printf("Enter desired User ID: ");
    readLine(newUser.userID, sizeof(newUser.userID));

    // Check if user ID already exists
    for (int i = 0; i < numUsers; i++) {
        if (strcmp(users[i].userID, newUser.userID) == 0) {
            printf("\nError: User ID '%s' is already taken.\n", newUser.userID);
            pressAnyKeyToContinue();
            return;
        }
    }

    printf("Enter password: ");
    readLine(newUser.password, sizeof(newUser.password));
    printf("Enter your full name (use '_' for spaces): ");
    readLine(newUser.fullName, sizeof(newUser.fullName));
    printf("Enter your age: ");
    scanf("%d", &newUser.age);
    while (getchar() != '\n'); // Consume newline
    
    printf("Enter your email: ");
    readLine(newUser.email, sizeof(newUser.email));
    printf("Enter your phone number: ");
    readLine(newUser.phone, sizeof(newUser.phone));
    printf("Enter your address (use '_' for spaces): ");
    readLine(newUser.address, sizeof(newUser.address));

    newUser.numCars = 0;
    newUser.isAdmin = 0; // New users are customers by default

    // Add this new user to our global array
    users[numUsers] = newUser;
    numUsers++;

    printf("\nRegistration successful! Please log in to continue.\n");
    saveUsers(); // Save immediately to file
    pressAnyKeyToContinue();
}

void loginUser() {
    clearScreen();
    printf("--- Parkify: User Login ---\n");

    if (loggedInUser != NULL) {
        printf("You are already logged in as %s.\n", loggedInUser->fullName);
        pressAnyKeyToContinue();
        return;
    }

    char userID[MAX_ID_LEN];
    char password[MAX_ID_LEN];
    printf("Enter User ID: ");
    readLine(userID, sizeof(userID));
    printf("Enter Password: ");
    readLine(password, sizeof(password));

    // Search for the user in our in-memory database
    for (int i = 0; i < numUsers; i++) {
        if (strcmp(users[i].userID, userID) == 0 && strcmp(users[i].password, password) == 0) {
            // Login success!
            // Make the global pointer 'loggedInUser' point to this user's memory location in the array.
            loggedInUser = &users[i];
            clearScreen();
            printf("Login successful! Welcome, %s.\n", loggedInUser->fullName);
            pressAnyKeyToContinue();
            loggedInMenu(); // Proceed to the logged-in menu
            return; // Return after the loggedInMenu is exited
        }
    }

    printf("\nInvalid User ID or Password.\n");
    pressAnyKeyToContinue();
}

// This is the main menu for a user who has successfully logged in.
void loggedInMenu() {
    int choice;
    do {
        clearScreen();
        printf("--- Parkify Main Menu ---\n");
        printf("Logged in as: %s\n\n", loggedInUser->fullName);
        printf("1. Access Garage Services (Appointments, Invoices, etc.)\n");
        printf("2. Parking Services\n");
        printf("3. Request Refueling\n");
        printf("4. Q&A Section\n");
        printf("5. Dashboard View\n");
        printf("6. Logout\n");

        if(loggedInUser->isAdmin == 1) {
            printf("9. ** ADMIN PANEL **\n");
        }
        if(loggedInUser->isAdmin == 2) {
            printf("8. ** MECHANIC PANEL **\n");
        }

        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n');

        switch (choice) {
            case 1:
                customerServicesMenu();
                break;
            case 2:
                // Parking Menu
                requestParking(); // For simplicity, we can have a dedicated parking menu later.
                break;
            case 3:
                requestRefueling();
                break;
            case 4:
                qnaSection();
                break;
            case 5:
                // Dashboard based on user type
                if (loggedInUser->isAdmin == 1) {
                    displayAdminDashboard();
                } else if (loggedInUser->isAdmin == 2) {
                    displayMechanicDashboard();
                } else {
                    displayCustomerDashboard();
                }
                break;
            case 6:
                // Logout: set the global pointer back to NULL
                loggedInUser = NULL;
                printf("\nYou have been logged out.\n");
                pressAnyKeyToContinue();
                return; // Exit this menu and go back to the main login/register menu
            case 8:
                if (loggedInUser->isAdmin == 2) {
                    mechanicPanel();
                } else {
                     printf("Invalid choice.\n");
                     pressAnyKeyToContinue();
                }
                break;
            case 9:
                if (loggedInUser->isAdmin == 1) {
                    adminPanel();
                } else {
                     printf("Invalid choice.\n");
                     pressAnyKeyToContinue();
                }
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                pressAnyKeyToContinue();
                break;
        }

    } while (choice != 6);
}

// This menu shows the 21 Functional Requirements as service options
void customerServicesMenu() {
    int choice;
    do {
        clearScreen();
        printf("--- Garage Services ---\n");
        printf("This menu represents the functional requirements from your document.\n\n");

        // We group related FRs for a cleaner menu.
        printf("--- Profile Management ---\n");
        printf("1. Manage Your Profile and Cars\n");
        printf("2. View User Profile\n\n");

        printf("--- Parts and Orders ---\n");
        printf("3. View Available Parts\n");
        printf("4. Order Parts\n\n");

        printf("--- Booking and Tracking (FR10, FR14, FR15, FR17) ---\n");
        printf("5. Book a New Service Appointment\n");
        printf("6. Confirm Appointment Booking\n");
        printf("7. Track Service Progress\n");
        printf("8. View Service Calendar (Admin/Mechanic Feature)\n\n");

        printf("--- Billing and Invoices (FR7, FR8, FR9, FR18, FR19) ---\n");
        printf("9. Calculate Cost Estimation\n");
        printf("10. Generate Invoice\n");
        printf("11. View Your Invoices\n\n");

        printf("--- History and Reports (FR3, FR12, FR13, FR20) ---\n");
        printf("12. View Annual Maintenance Reminders\n");
        printf("13. View Parking History\n\n");

        printf("--- Support (FR22) ---\n");
        printf("14. Submit a Query (Q&A)\n\n");

        printf("15. Back to Main Menu\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n');

        // This switch would call the respective functions. We'll mock a few.
        switch (choice) {
            case 1:
                manageUserProfile();
                break;
            case 2:
                printf("\n=== User Profile ===\n");
                printf("User ID: %s\n", loggedInUser->userID);
                printf("Name: %s\n", loggedInUser->fullName);
                printf("Age: %d\n", loggedInUser->age);
                printf("Email: %s\n", loggedInUser->email);
                printf("Phone: %s\n", loggedInUser->phone);
                printf("Address: %s\n", loggedInUser->address);
                printf("Number of Cars: %d\n", loggedInUser->numCars);
                pressAnyKeyToContinue();
                break;
            case 3:
                viewAvailableParts();
                break;
            case 4:
                orderParts();
                break;
            case 5:
                printf("\n(Function to Book Service Appointment would run here...)\n");
                pressAnyKeyToContinue();
                break;
            case 6:
                confirmAppointment();
                break;
            case 7:
                printf("\n(Function to Track Service Progress would run here...)\n");
                pressAnyKeyToContinue();
                break;
            case 8:
                serviceCalendarView();
                break;
            case 9:
                calculateEstimation();
                break;
            case 10:
                generateInvoice();
                break;
            case 11:
                printf("\n(Function to View Invoices would run here...)\n");
                pressAnyKeyToContinue();
                break;
            case 12:
                printf("\n(Function to View Maintenance Reminders would run here...)\n");
                pressAnyKeyToContinue();
                break;
            case 13:
                viewParkingHistory();
                break;
            case 14:
                qnaSection();
                break;
            case 15:
                return;
            default:
                printf("\nInvalid choice. Please try again.\n");
                pressAnyKeyToContinue();
                break;
        }
    } while (choice != 15);
}

// Implementation for Parking Feature
void requestParking() {
    clearScreen();
    printf("--- Parking Services ---\n");

    if (loggedInUser->numCars == 0) {
        printf("You must register a car in your profile first.\n");
        pressAnyKeyToContinue();
        return;
    }

    printf("Your Registered Cars:\n");
    for (int i = 0; i < loggedInUser->numCars; i++) {
        printf("%d. Plate: %s, Model: %s\n", i + 1, loggedInUser->cars[i].plateNumber, loggedInUser->cars[i].carModel);
    }
    printf("Select car for parking (number): ");
    int carChoice;
    scanf("%d", &carChoice);
    while(getchar()!='\n');

    if (carChoice < 1 || carChoice > loggedInUser->numCars) {
        printf("Invalid selection.\n");
        pressAnyKeyToContinue();
        return;
    }
    // Using a pointer to make the code cleaner
    Car *selectedCar = &loggedInUser->cars[carChoice - 1];

    ParkingRecord newRecord;
    char dateTimeStr[50];
    getCurrentDateTime(dateTimeStr, sizeof(dateTimeStr));
    strcpy(newRecord.checkInTime, dateTimeStr);
    strcpy(newRecord.carPlate, selectedCar->plateNumber);

    // Save this record to a file
    FILE *file = fopen(PARKING_FILE, "a"); // 'a' mode: append. Adds to the end of the file. Good for logs.
    if (file == NULL) {
        perror("Error opening parking file");
        pressAnyKeyToContinue();
        return;
    }
    fprintf(file, "USER:%s PLATE:%s CHECK-IN:%s\n", loggedInUser->userID, newRecord.carPlate, newRecord.checkInTime);
    fclose(file);

    printf("\nCar '%s' checked in at %s.\n", newRecord.carPlate, newRecord.checkInTime);
    printf("The check-out time and fee will be recorded upon exit.\n");
    pressAnyKeyToContinue();
}

void requestRefueling() {
    clearScreen();
    printf("--- Refueling Services ---\n");
     if (loggedInUser->numCars == 0) {
        printf("You must register a car in your profile first.\n");
        pressAnyKeyToContinue();
        return;
    }
    printf("This feature is under construction. Thank you for your interest!\n");
    // Full implementation would be similar to requestParking()
    pressAnyKeyToContinue();
}

// === ADMIN PANEL FUNCTIONS ===
void adminPanel() {
    int choice;
    do {
        clearScreen();
        printf("=== ADMIN PANEL ===\n");
        printf("Welcome, Administrator!\n\n");
        printf("1. View Customer History\n");
        printf("2. View All Users\n");
        printf("3. View Parking Records\n");
        printf("4. System Statistics\n");
        printf("5. Admin Dashboard\n");
        printf("6. Back to Main Menu\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n');

        switch (choice) {
            case 1:
                viewCustomerHistory();
                break;
            case 2:
                clearScreen();
                printf("=== ALL REGISTERED USERS ===\n");
                for (int i = 0; i < numUsers; i++) {
                    printf("User %d: ID=%s, Name=%s, Age=%d, Cars=%d, Type=%s\n", 
                           i+1, users[i].userID, users[i].fullName, users[i].age, 
                           users[i].numCars, 
                           users[i].isAdmin == 1 ? "Admin" : users[i].isAdmin == 2 ? "Mechanic" : "Customer");
                }
                pressAnyKeyToContinue();
                break;
            case 3:
                viewParkingHistory();
                break;
            case 4:
                clearScreen();
                printf("=== SYSTEM STATISTICS ===\n");
                printf("Total Users: %d\n", numUsers);
                printf("Total Parts in Inventory: %d\n", numParts);
                int availableParts = 0;
                for (int i = 0; i < numParts; i++) {
                    if (partsInventory[i].isAvailable) availableParts++;
                }
                printf("Available Parts: %d\n", availableParts);
                printf("Out of Stock Parts: %d\n", numParts - availableParts);
                pressAnyKeyToContinue();
                break;
            case 5:
                displayAdminDashboard();
                break;
            case 6:
                return;
            default:
                printf("Invalid choice.\n");
                pressAnyKeyToContinue();
        }
    } while (choice != 6);
}

void viewCustomerHistory() {
    clearScreen();
    printf("=== CUSTOMER HISTORY ===\n");
    
    char userID[MAX_ID_LEN];
    printf("Enter User ID to view history (or 'all' for all users): ");
    readLine(userID, sizeof(userID));
    
    if (strcmp(userID, "all") == 0) {
        printf("\n--- ALL CUSTOMER ACTIVITIES ---\n");
        for (int i = 0; i < numUsers; i++) {
            if (users[i].isAdmin == 0) {
                printf("Customer: %s (%s)\n", users[i].fullName, users[i].userID);
                printf("  Cars: %d registered\n", users[i].numCars);
                for (int j = 0; j < users[i].numCars; j++) {
                    printf("    - %s (%s)\n", users[i].cars[j].carModel, users[i].cars[j].plateNumber);
                }
                printf("\n");
            }
        }
    } else {
        // Find specific user
        int found = 0;
        for (int i = 0; i < numUsers; i++) {
            if (strcmp(users[i].userID, userID) == 0) {
                found = 1;
                printf("\n--- CUSTOMER DETAILS ---\n");
                printf("Name: %s\n", users[i].fullName);
                printf("Email: %s\n", users[i].email);
                printf("Phone: %s\n", users[i].phone);
                printf("Cars Registered: %d\n", users[i].numCars);
                for (int j = 0; j < users[i].numCars; j++) {
                    printf("  Car %d: %s - %s\n", j+1, users[i].cars[j].plateNumber, users[i].cars[j].carModel);
                }
                break;
            }
        }
        if (!found) {
            printf("User not found!\n");
        }
    }
    pressAnyKeyToContinue();
}

// === MECHANIC PANEL FUNCTIONS ===
void mechanicPanel() {
    int choice;
    do {
        clearScreen();
        printf("=== MECHANIC PANEL ===\n");
        printf("Welcome, %s!\n\n", loggedInUser->fullName);
        printf("1. View Car Profiles\n");
        printf("2. Mechanic Assignment\n");
        printf("3. Service Calendar View\n");
        printf("4. Mechanic Dashboard\n");
        printf("5. Back to Main Menu\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n');

        switch (choice) {
            case 1:
                viewCarProfile();
                break;
            case 2:
                mechanicAssignment();
                break;
            case 3:
                serviceCalendarView();
                break;
            case 4:
                displayMechanicDashboard();
                break;
            case 5:
                return;
            default:
                printf("Invalid choice.\n");
                pressAnyKeyToContinue();
        }
    } while (choice != 5);
}

void viewCarProfile() {
    clearScreen();
    printf("=== CAR PROFILES ===\n");
    
    int totalCars = 0;
    for (int i = 0; i < numUsers; i++) {
        if (users[i].numCars > 0) {
            printf("\nOwner: %s (%s)\n", users[i].fullName, users[i].userID);
            for (int j = 0; j < users[i].numCars; j++) {
                printf("  Car %d:\n", j+1);
                printf("    Plate Number: %s\n", users[i].cars[j].plateNumber);
                printf("    Model: %s\n", users[i].cars[j].carModel);
                printf("    Engine Type: %s\n", users[i].cars[j].engineType);
                printf("    Year: %d\n", users[i].cars[j].year);
                totalCars++;
            }
        }
    }
    printf("\nTotal Cars in System: %d\n", totalCars);
    pressAnyKeyToContinue();
}

void mechanicAssignment() {
    clearScreen();
    printf("=== MECHANIC ASSIGNMENT ===\n");
    printf("Current Assignments:\n");
    printf("1. Oil Change - Car ABC123 - Assigned to: John Mechanic\n");
    printf("2. Brake Repair - Car XYZ789 - Assigned to: Mike Mechanic\n");
    printf("3. Engine Diagnostics - Car DEF456 - Assigned to: %s\n", loggedInUser->fullName);
    printf("\nThis is a sample assignment view.\n");
    pressAnyKeyToContinue();
}

void serviceCalendarView() {
    clearScreen();
    printf("=== SERVICE CALENDAR ===\n");
    printf("Today's Schedule:\n");
    printf("09:00 - Oil Change (Car: ABC123)\n");
    printf("11:00 - Brake Inspection (Car: XYZ789)\n");
    printf("14:00 - Engine Diagnostics (Car: DEF456)\n");
    printf("16:00 - Tire Replacement (Car: GHI101)\n");
    printf("\nUpcoming Appointments:\n");
    printf("Tomorrow: 3 appointments scheduled\n");
    printf("This Week: 15 appointments scheduled\n");
    pressAnyKeyToContinue();
}

// === USER PROFILE FUNCTIONS ===
void manageUserProfile() {
    int choice;
    do {
        clearScreen();
        printf("=== MANAGE PROFILE ===\n");
        printf("1. Add New Car\n");
        printf("2. View Your Cars\n");
        printf("3. Update Contact Information\n");
        printf("4. Back to Services Menu\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n');

        switch (choice) {
            case 1:
                if (loggedInUser->numCars >= MAX_CARS_PER_USER) {
                    printf("Maximum cars limit reached!\n");
                    pressAnyKeyToContinue();
                    break;
                }
                printf("Enter car plate number: ");
                readLine(loggedInUser->cars[loggedInUser->numCars].plateNumber, 
                        sizeof(loggedInUser->cars[loggedInUser->numCars].plateNumber));
                printf("Enter car model: ");
                readLine(loggedInUser->cars[loggedInUser->numCars].carModel, 
                        sizeof(loggedInUser->cars[loggedInUser->numCars].carModel));
                printf("Enter engine type: ");
                readLine(loggedInUser->cars[loggedInUser->numCars].engineType, 
                        sizeof(loggedInUser->cars[loggedInUser->numCars].engineType));
                printf("Enter car year: ");
                scanf("%d", &loggedInUser->cars[loggedInUser->numCars].year);
                while (getchar() != '\n');
                loggedInUser->numCars++;
                printf("Car added successfully!\n");
                saveUsers();
                pressAnyKeyToContinue();
                break;
            case 2:
                printf("\n=== YOUR CARS ===\n");
                for (int i = 0; i < loggedInUser->numCars; i++) {
                    printf("Car %d:\n", i+1);
                    printf("  Plate: %s\n", loggedInUser->cars[i].plateNumber);
                    printf("  Model: %s\n", loggedInUser->cars[i].carModel);
                    printf("  Engine: %s\n", loggedInUser->cars[i].engineType);
                    printf("  Year: %d\n\n", loggedInUser->cars[i].year);
                }
                pressAnyKeyToContinue();
                break;
            case 3:
                printf("Update Email: ");
                readLine(loggedInUser->email, sizeof(loggedInUser->email));
                printf("Update Phone: ");
                readLine(loggedInUser->phone, sizeof(loggedInUser->phone));
                printf("Contact information updated!\n");
                saveUsers();
                pressAnyKeyToContinue();
                break;
            case 4:
                return;
            default:
                printf("Invalid choice.\n");
                pressAnyKeyToContinue();
        }
    } while (choice != 4);
}

// === PARTS MANAGEMENT FUNCTIONS ===
void viewAvailableParts() {
    clearScreen();
    printf("=== AVAILABLE PARTS ===\n");
    printf("%-15s %-10s %-10s %-10s %s\n", "Part Name", "Code", "Quantity", "Price", "Status");
    printf("-----------------------------------------------------------\n");
    
    for (int i = 0; i < numParts; i++) {
        printf("%-15s %-10s %-10d $%-9.2f %s\n", 
               partsInventory[i].partName,
               partsInventory[i].partCode,
               partsInventory[i].quantity,
               partsInventory[i].price,
               partsInventory[i].isAvailable ? "Available" : "Out of Stock");
    }
    pressAnyKeyToContinue();
}

void orderParts() {
    clearScreen();
    printf("=== ORDER PARTS ===\n");
    
    char partName[MAX_NAME_LEN];
    printf("Enter part name you want to order: ");
    readLine(partName, sizeof(partName));
    
    int found = 0;
    for (int i = 0; i < numParts; i++) {
        if (strstr(partsInventory[i].partName, partName) != NULL) {
            found = 1;
            if (partsInventory[i].isAvailable && partsInventory[i].quantity > 0) {
                printf("\nPart Found: %s\n", partsInventory[i].partName);
                printf("Price: $%.2f\n", partsInventory[i].price);
                printf("Available Quantity: %d\n", partsInventory[i].quantity);
                
                int orderQty;
                printf("Enter quantity to order: ");
                scanf("%d", &orderQty);
                while (getchar() != '\n');
                
                if (orderQty <= partsInventory[i].quantity) {
                    partsInventory[i].quantity -= orderQty;
                    if (partsInventory[i].quantity == 0) {
                        partsInventory[i].isAvailable = 0;
                    }
                    printf("\nOrder placed successfully!\n");
                    printf("Total Cost: $%.2f\n", orderQty * partsInventory[i].price);
                } else {
                    printf("Insufficient stock!\n");
                }
            } else {
                printf("\nPart '%s' is currently out of stock!\n", partsInventory[i].partName);
                printf("We apologize for the inconvenience. Please check back later.\n");
            }
            break;
        }
    }
    
    if (!found) {
        printf("\nPart '%s' not found in our inventory!\n", partName);
        printf("Available parts: Engine Oil, Brake Pads, Air Filter, Spark Plugs, Battery, etc.\n");
    }
    
    pressAnyKeyToContinue();
}

void calculateEstimation() {
    clearScreen();
    printf("=== COST ESTIMATION ===\n");
    
    printf("Select service type:\n");
    printf("1. Oil Change ($50)\n");
    printf("2. Brake Service ($150)\n");
    printf("3. Engine Diagnostics ($120)\n");
    printf("4. Tire Replacement ($300)\n");
    printf("5. Full Service ($400)\n");
    printf("Enter choice: ");
    
    int choice;
    scanf("%d", &choice);
    while (getchar() != '\n');
    
    double baseCost = 0;
    char serviceName[100];
    
    switch (choice) {
        case 1: baseCost = 50; strcpy(serviceName, "Oil Change"); break;
        case 2: baseCost = 150; strcpy(serviceName, "Brake Service"); break;
        case 3: baseCost = 120; strcpy(serviceName, "Engine Diagnostics"); break;
        case 4: baseCost = 300; strcpy(serviceName, "Tire Replacement"); break;
        case 5: baseCost = 400; strcpy(serviceName, "Full Service"); break;
        default:
            printf("Invalid choice!\n");
            pressAnyKeyToContinue();
            return;
    }
    
    printf("\n=== ESTIMATION DETAILS ===\n");
    printf("Service: %s\n", serviceName);
    printf("Base Cost: $%.2f\n", baseCost);
    
    double discount = 0;
    if (loggedInUser->numCars > 2) {
        discount = baseCost * 0.1; // 10% discount for multiple cars
        printf("Multi-car Discount (10%%): -$%.2f\n", discount);
    }
    
    double tax = (baseCost - discount) * 0.08; // 8% tax
    double totalCost = baseCost - discount + tax;
    
    printf("Tax (8%%): +$%.2f\n", tax);
    printf("------------------------\n");
    printf("Total Estimated Cost: $%.2f\n", totalCost);
    
    pressAnyKeyToContinue();
}

void generateInvoice() {
    clearScreen();
    printf("=== GENERATE INVOICE ===\n");
    
    char serviceType[MAX_SERVICE_DESC_LEN];
    double cost;
    
    printf("Enter service description: ");
    readLine(serviceType, sizeof(serviceType));
    printf("Enter service cost: $");
    scanf("%lf", &cost);
    while (getchar() != '\n');
    
    char invoiceID[MAX_ID_LEN];
    generateUniqueId(invoiceID, "INV", numUsers);
    
    char currentDate[MAX_DATE_LEN];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(currentDate, sizeof(currentDate), "%04d-%02d-%02d",
             tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    
    printf("\n=== INVOICE ===\n");
    printf("Invoice ID: %s\n", invoiceID);
    printf("Date: %s\n", currentDate);
    printf("Customer: %s\n", loggedInUser->fullName);
    printf("Service: %s\n", serviceType);
    printf("Amount: $%.2f\n", cost);
    printf("Status: Generated\n");
    
    // Save to file
    FILE *file = fopen(INVOICES_FILE, "a");
    if (file != NULL) {
        fprintf(file, "%s %s %s %.2f %s\n", invoiceID, loggedInUser->userID, serviceType, cost, currentDate);
        fclose(file);
        printf("\nInvoice saved successfully!\n");
    }
    
    pressAnyKeyToContinue();
}

void confirmAppointment() {
    clearScreen();
    printf("=== CONFIRM APPOINTMENT ===\n");
    
    char serviceType[MAX_SERVICE_DESC_LEN];
    char appointmentDate[MAX_DATE_LEN];
    char appointmentTime[MAX_TIME_LEN];
    
    printf("Enter service type: ");
    readLine(serviceType, sizeof(serviceType));
    printf("Enter preferred date (YYYY-MM-DD): ");
    readLine(appointmentDate, sizeof(appointmentDate));
    printf("Enter preferred time (HH:MM): ");
    readLine(appointmentTime, sizeof(appointmentTime));
    
    char appointmentID[MAX_ID_LEN];
    generateUniqueId(appointmentID, "APT", numUsers);
    
    printf("\n=== APPOINTMENT CONFIRMED ===\n");
    printf("Appointment ID: %s\n", appointmentID);
    printf("Customer: %s\n", loggedInUser->fullName);
    printf("Service: %s\n", serviceType);
    printf("Date: %s\n", appointmentDate);
    printf("Time: %s\n", appointmentTime);
    printf("Status: Confirmed\n");
    
    // Save to file
    FILE *file = fopen(APPOINTMENTS_FILE, "a");
    if (file != NULL) {
        fprintf(file, "%s %s %s %s %s Confirmed\n", appointmentID, loggedInUser->userID, serviceType, appointmentDate, appointmentTime);
        fclose(file);
        printf("\nAppointment saved successfully!\n");
    }
    
    pressAnyKeyToContinue();
}

void viewParkingHistory() {
    clearScreen();
    printf("=== PARKING HISTORY ===\n");
    
    FILE *file = fopen(PARKING_FILE, "r");
    if (file == NULL) {
        printf("No parking history found.\n");
        pressAnyKeyToContinue();
        return;
    }
    
    char line[200];
    printf("%-15s %-15s %-20s\n", "User ID", "Car Plate", "Check-in Time");
    printf("------------------------------------------------\n");
    
    while (fgets(line, sizeof(line), file) != NULL) {
        char userID[MAX_ID_LEN], plate[MAX_PLATE_LEN], checkIn[50];
        if (sscanf(line, "USER:%s PLATE:%s CHECK-IN:%[^\n]", userID, plate, checkIn) == 3) {
            printf("%-15s %-15s %-20s\n", userID, plate, checkIn);
        }
    }
    
    fclose(file);
    pressAnyKeyToContinue();
}

// === Q&A SECTION ===
void qnaSection() {
    int choice;
    do {
        clearScreen();
        printf("=== CAR MAINTENANCE Q&A ===\n");
        printf("Select a topic:\n\n");
        printf("1. Engine Problems\n");
        printf("2. Brake Issues\n");
        printf("3. Battery Problems\n");
        printf("4. Tire Maintenance\n");
        printf("5. Oil Change\n");
        printf("6. General Maintenance\n");
        printf("7. Back to Main Menu\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n');

        clearScreen();
        switch (choice) {
            case 1:
                printf("=== ENGINE PROBLEMS ===\n");
                printf("Q: My car engine is making strange noises. What should I do?\n");
                printf("A: Stop driving immediately and check oil level. If oil is low, add oil.\n");
                printf("   If noise persists, contact a mechanic as it could be serious engine damage.\n\n");
                
                printf("Q: Engine overheating - what are the causes?\n");
                printf("A: Common causes include low coolant, faulty thermostat, radiator issues,\n");
                printf("   or water pump failure. Check coolant level first, then visit mechanic.\n\n");
                
                printf("Q: Car won't start - engine problems?\n");
                printf("A: Could be dead battery, faulty starter, fuel system issues, or\n");
                printf("   ignition problems. Try jump-starting first, then check with mechanic.\n");
                break;
                
            case 2:
                printf("=== BRAKE ISSUES ===\n");
                printf("Q: Brakes are squeaking - is this dangerous?\n");
                printf("A: Squeaking usually means brake pads are worn. Replace them soon.\n");
                printf("   If you hear grinding, stop driving immediately - metal on metal damage.\n\n");
                
                printf("Q: Brake pedal feels spongy - what's wrong?\n");
                printf("A: This indicates air in brake lines or brake fluid leak.\n");
                printf("   This is dangerous - have brakes inspected immediately.\n\n");
                
                printf("Q: How often should I replace brake pads?\n");
                printf("A: Typically every 25,000-70,000 miles depending on driving habits.\n");
                printf("   City driving wears them faster than highway driving.\n");
                break;
                
            case 3:
                printf("=== BATTERY PROBLEMS ===\n");
                printf("Q: How long do car batteries last?\n");
                printf("A: Most car batteries last 3-5 years. Replace if car is slow to start\n");
                printf("   or if battery is over 3 years old and showing signs of weakness.\n\n");
                
                printf("Q: Battery keeps dying - what's the cause?\n");
                printf("A: Could be alternator not charging, parasitic drain (lights left on),\n");
                printf("   old battery, or electrical system problems. Test alternator first.\n\n");
                
                printf("Q: Can I jump-start my car safely?\n");
                printf("A: Yes, but follow proper procedure: red to positive, black to ground,\n");
                printf("   start helper car first, then try starting your car.\n");
                break;
                
            case 4:
                printf("=== TIRE MAINTENANCE ===\n");
                printf("Q: How often should I check tire pressure?\n");
                printf("A: Check monthly and before long trips. Proper pressure improves\n");
                printf("   fuel economy and tire life. Check when tires are cold.\n\n");
                
                printf("Q: When should I replace my tires?\n");
                printf("A: Replace when tread depth is 2/32\" or less. Use penny test:\n");
                printf("   if you can see Lincoln's head, replace tires.\n\n");
                
                printf("Q: Why do I need tire rotation?\n");
                printf("A: Rotation ensures even wear, extending tire life. Rotate every\n");
                printf("   5,000-8,000 miles or with every other oil change.\n");
                break;
                
            case 5:
                printf("=== OIL CHANGE ===\n");
                printf("Q: How often should I change my oil?\n");
                printf("A: Most modern cars: every 7,500-10,000 miles with synthetic oil.\n");
                printf("   Conventional oil: every 3,000-5,000 miles. Check owner's manual.\n\n");
                
                printf("Q: What happens if I don't change oil regularly?\n");
                printf("A: Engine damage, reduced performance, voided warranty, and expensive\n");
                printf("   repairs. Oil lubricates and cleans engine components.\n\n");
                
                printf("Q: Can I change oil myself?\n");
                printf("A: Yes, if you have proper tools and disposal method for used oil.\n");
                printf("   Ensure correct oil type and filter. Many prefer professional service.\n");
                break;
                
            case 6:
                printf("=== GENERAL MAINTENANCE ===\n");
                printf("Q: What's included in regular maintenance?\n");
                printf("A: Oil changes, filter replacements, fluid checks, brake inspection,\n");
                printf("   tire rotation, battery test, and following manufacturer's schedule.\n\n");
                
                printf("Q: How can I extend my car's life?\n");
                printf("A: Regular maintenance, gentle driving, proper warm-up, quality fuel,\n");
                printf("   address problems early, and keep it clean inside and out.\n\n");
                
                printf("Q: What warning signs should I never ignore?\n");
                printf("A: Check engine light, unusual noises, fluid leaks, strange smells,\n");
                printf("   vibrations, or any change in normal operation.\n");
                break;
                
            case 7:
                return;
                
            default:
                printf("Invalid choice. Please try again.\n");
                pressAnyKeyToContinue();
                continue;
        }
        pressAnyKeyToContinue();
    } while (choice != 7);
}

// === DASHBOARD FUNCTIONS ===
void printDashboardHeader(const char* title) {
    printf("\n");
    for (int i = 0; i < 60; i++) printf("=");
    printf("\n");
    int padding = (60 - strlen(title)) / 2;
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s\n", title);
    for (int i = 0; i < 60; i++) printf("=");
    printf("\n");
}

void drawProgressBar(int current, int max, int width) {
    int filled = (current * width) / max;
    printf("[");
    for (int i = 0; i < width; i++) {
        if (i < filled) {
            printf("#");
        } else {
            printf("-");
        }
    }
    printf("] %d/%d", current, max);
}

void displayMainDashboard() {
    clearScreen();
    printDashboardHeader("PARKIFY GARAGE DASHBOARD");
    
    // Current Date and Time
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("Date: %04d-%02d-%02d | Time: %02d:%02d\n", 
           tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min);
    
    printf("\n=== QUICK STATISTICS ===\n");
    printf("Total Customers: %d | Parked Cars: 12 | Active Services: 8\n", numUsers);
    printf("Today's Revenue: $1,250 | Appointments: 15 | Mechanics: 4\n");
    
    printf("\n=== CAPACITY INDICATORS ===\n");
    printf("Parking Spaces: ");
    drawProgressBar(12, 50, 20);
    printf(" (24%% Full)\n");
    
    printf("Service Bays:   ");
    drawProgressBar(8, 10, 20);
    printf(" (80%% Busy)\n");
    
    printf("Parts Stock:    ");
    drawProgressBar(6, 8, 20);
    printf(" (75%% Available)\n");
    
    printf("\n=== RECENT ACTIVITY ===\n");
    printf("• Customer checked in car ABC123\n");
    printf("• Oil change completed for XYZ789\n");
    printf("• New appointment booked\n");
    printf("• Parts order received\n");
    
    printf("\n=== SYSTEM STATUS ===\n");
    printf("Database: Online | Backup: Up to date | Network: Connected\n");
    
    pressAnyKeyToContinue();
}

void displayCustomerDashboard() {
    clearScreen();
    printDashboardHeader("CUSTOMER DASHBOARD");
    
    printf("Welcome back, %s!\n", loggedInUser->fullName);
    
    printf("\n=== YOUR ACCOUNT SUMMARY ===\n");
    printf("Registered Cars: %d | Total Visits: 18\n", loggedInUser->numCars);
    printf("Total Spent: $850 | Loyalty Points: 1,250\n");
    
    printf("\n=== CURRENT SERVICES ===\n");
    if (loggedInUser->numCars > 0) {
        printf("• Car %s - Available for service\n", loggedInUser->cars[0].plateNumber);
    } else {
        printf("• No cars registered yet\n");
    }
    
    printf("\n=== UPCOMING APPOINTMENTS ===\n");
    printf("• 2025-07-28 14:00 - Brake Service\n");
    printf("• 2025-07-30 10:30 - Tire Rotation\n");
    
    printf("\n=== QUICK ACTIONS ===\n");
    printf("1. Book Service | 2. Check Parking | 3. View Invoices\n");
    printf("4. Order Parts  | 5. Contact Support | 6. Pay Bills\n");
    
    pressAnyKeyToContinue();
}

void displayMechanicDashboard() {
    clearScreen();
    printDashboardHeader("MECHANIC DASHBOARD");
    
    printf("Welcome, %s!\n", loggedInUser->fullName);
    
    printf("\n=== TODAY'S WORKLOAD ===\n");
    printf("Completed Jobs: 5 | In Progress: 3 | Pending: 7\n");
    
    printf("\n=== CURRENT ASSIGNMENTS ===\n");
    printf("• Bay 1: Engine Diagnostics (Car DEF456) - 45 min remaining\n");
    printf("• Bay 2: Brake Repair (Car GHI789) - 1h 20m remaining\n");
    printf("• Bay 3: Oil Change (Car JKL012) - 15 min remaining\n");
    
    printf("\n=== TOOLS & EQUIPMENT STATUS ===\n");
    printf("Hydraulic Lift 1: Available\n");
    printf("Hydraulic Lift 2: In Use (Bay 2)\n");
    printf("Diagnostic Scanner: Available\n");
    printf("Tire Changer: Available\n");
    
    printf("\n=== PARTS NEEDED TODAY ===\n");
    printf("• Brake Pads (2 sets) - Available\n");
    printf("• Engine Oil (5 liters) - Available\n");
    printf("• Air Filter (3 units) - Low Stock\n");
    printf("• Spark Plugs (1 set) - Out of Stock\n");
    
    pressAnyKeyToContinue();
}

void displayAdminDashboard() {
    clearScreen();
    printDashboardHeader("ADMIN DASHBOARD");
    
    printf("Welcome, Administrator!\n");
    
    printf("\n=== BUSINESS OVERVIEW ===\n");
    printf("Daily Revenue: $1,250 | Monthly: $28,500 | YTD: $185,000\n");
    printf("Total Customers: %d | Cars Serviced: 1,250 | Avg Rating: 4.8/5\n", numUsers);
    
    printf("\n=== DEPARTMENT STATUS ===\n");
    printf("Service Department: ");
    drawProgressBar(8, 10, 15);
    printf(" (80%% Capacity)\n");
    
    printf("Parking Management: ");
    drawProgressBar(12, 50, 15);
    printf(" (24%% Occupied)\n");
    
    printf("Parts Inventory:    ");
    drawProgressBar(6, 8, 15);
    printf(" (75%% Stocked)\n");
    
    printf("Staff on Duty:      ");
    drawProgressBar(8, 12, 15);
    printf(" (67%% Staffed)\n");
    
    printf("\n=== FINANCIAL SUMMARY ===\n");
    printf("• Service Revenue: $950 (76%%)\n");
    printf("• Parking Revenue: $200 (16%%)\n");
    printf("• Parts Revenue: $100 (8%%)\n");
    printf("• Operating Costs: $400\n");
    printf("• Net Profit: $850\n");
    
    printf("\n=== ALERTS & NOTIFICATIONS ===\n");
    printf("• Spark Plugs out of stock - Order required\n");
    printf("• Service Bay 4 - Equipment maintenance due\n");
    printf("• 5 customer feedback forms pending review\n");
    printf("• Monthly report generation scheduled\n");
    
    pressAnyKeyToContinue();
}


int main() {
    // 1. Load all data from files into memory at the start.
    loadAllData();

    // 2. Create default admin and mechanic if no users exist (for first-time use).
    if (numUsers == 0) {
        printf("No users found. Creating default admin and mechanic...\n");
        
        // Create admin user
        strcpy(users[0].userID, "admin");
        strcpy(users[0].password, "admin");
        strcpy(users[0].fullName, "Admin_User");
        strcpy(users[0].email, "admin@parkify.com");
        strcpy(users[0].phone, "555-0001");
        strcpy(users[0].address, "Admin_Office");
        users[0].age = 99;
        users[0].isAdmin = 1;
        users[0].numCars = 0;
        
        // Create mechanic user
        strcpy(users[1].userID, "mechanic");
        strcpy(users[1].password, "mechanic");
        strcpy(users[1].fullName, "Chief_Mechanic");
        strcpy(users[1].email, "mechanic@parkify.com");
        strcpy(users[1].phone, "555-0002");
        strcpy(users[1].address, "Workshop_Area");
        users[1].age = 35;
        users[1].isAdmin = 2; // 2 for mechanic
        users[1].numCars = 0;
        
        numUsers = 2;
        saveUsers(); // Save these new users to the file.
        printf("Default users created:\n");
        printf("Admin - UserID: admin, Password: admin\n");
        printf("Mechanic - UserID: mechanic, Password: mechanic\n");
        pressAnyKeyToContinue();
    }


    int choice;
    do {
        clearScreen();
        printf("===================================================\n");
        printf("    Welcome to Parkify Smart Garage System\n");
        printf("===================================================\n\n");
        printf("1. Register a new account\n");
        printf("2. Login to your account\n");
        printf("3. Exit Program\n\n");
        printf("Please enter your choice (1-3): ");

        // Read user input
        if (scanf("%d", &choice) != 1) {
            choice = 0; // Set to invalid choice if input is not a number
        }
        while (getchar() != '\n'); // Clear input buffer

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                loginUser(); // This function will handle the menu for logged-in users
                break;
            case 3:
                // Before exiting, save all data from memory back to files.
                saveAllData();
                clearScreen();
                printf("\nThank you for using Parkify. Goodbye!\n");
                break;
            default:
                printf("\nInvalid input. Please enter a number between 1 and 3.\n");
                pressAnyKeyToContinue();
                break;
        }
    } while (choice != 3);

    return 0;
}


/*
================================================================================
SIDE NOTES: C CONCEPTS EXPLAINED
================================================================================

1.  File Handling:
    ----------------
    -   `fopen(const char *filename, const char *mode)`: This is the function to open a file.
        -   `"r"` (read): Opens a file for reading. The file MUST exist.
        -   `"w"` (write): Opens a file for writing. If the file exists, its contents are DESTROYED. If it doesn't exist, a new file is created. This is what we use in `saveUsers()` to write a fresh copy of all user data.
        -   `"a"` (append): Opens a file for writing, but appends new data to the END of the file. If the file doesn't exist, it's created. This is useful for log files, like in `requestParking()`.
    -   `fprintf()` / `fscanf()`: These are for reading/writing FORMATTED text. They are perfect for saving and loading `struct` data where you know the exact layout (e.g., "string string integer").
    -   `fputs()` / `fgets()`: These are for reading/writing entire LINES of text (strings). `fgets()` is safer than `scanf()` for user input because you specify the maximum buffer size, preventing overflows.
    -   `fputc()` / `fgetc()`: These are for reading/writing a SINGLE character. They are less common for this type of application but are the building blocks of the other functions.
    -   `perror()`: A very useful function for debugging file errors. When a function like `fopen()` fails, it sets a global error number (`errno`). `perror("Your message")` prints your custom message followed by a system-generated, human-readable error description (e.g., "Permission denied").

2.  Pointers (`*`):
    ----------------
    -   A pointer is a variable that stores the MEMORY ADDRESS of another variable.
    -   In this code, `User *loggedInUser;` declares a pointer named `loggedInUser` that can hold the address of a `User` struct.
    -   When a user logs in successfully, we do `loggedInUser = &users[i];`. The `&` operator gets the memory address of the user's struct in the `users` array.
    -   Now, `loggedInUser` doesn't hold a copy of the user; it holds the address of the ORIGINAL user data.
    -   When we want to access a member of the struct through the pointer, we use the arrow operator `->`, like `loggedInUser->fullName`. This is shorthand for `(*loggedInUser).fullName`. Using pointers is more efficient than passing large structs by value (copying them) to functions.

3.  `!EOF` and File Reading Loops:
    ---------------------------------
    -   `EOF` stands for End-Of-File. It's a special value indicating that there's no more data to read.
    -   A common but slightly risky way to read a file is `while (!feof(file))`. The problem is that the EOF flag is only set AFTER you try to read past the end of the file.
    -   A much better and safer way is to check the return value of the read function itself.
        -   `fgets()` returns `NULL` on failure or when it reaches the end of the file. So, `while (fgets(...) != NULL)` is a robust loop.
        -   `fscanf()` returns the number of items it successfully read. So, `while (fscanf(...) == 6)` is a great way to loop, as it ensures you're reading a full, valid record each time. If it reaches the end of the file or finds a malformed line, it will return a number other than 6, and the loop will correctly terminate. This is the method used in `loadUsers()`.

4.  Buffers:
    ----------
    -   A buffer is simply a block of memory used for temporary storage. In C, we most often create buffers using character arrays.
    -   Example: `char lineBuffer[256];` creates a buffer that can hold 255 characters plus the null terminator (`\0`).
    -   When you use `fgets(lineBuffer, 256, stdin)`, you are telling the function to read a line of input from the keyboard (`stdin`) and store it temporarily in `lineBuffer`, but to stop after 255 characters to prevent overflowing the buffer and corrupting memory. This is a fundamental concept for safe programming in C.
*/