#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>
#include <windows.h>  // For Windows console colors
#include <conio.h>    // For hidden password input

#define USERS_FILE "user_data.txt"
#define PARTS_FILE "inventory.txt"
#define NOT_AVAILABLE_FILE "not_available.txt"
#define ORDERS_FILE "orders.txt"
#define DISCOUNTS_FILE "discounts.txt"
#define MECHANICS_FILE "mechanics.txt"
#define APPOINTMENTS_FILE "appointments.txt"
#define CAR_PROFILES_FILE "car_profiles.txt"
#define VEHICLE_HISTORY_FILE "vehicle_history.txt"
#define PROGRESS_FILE "progress.txt"
#define DEADLINES_FILE "deadlines.txt"
#define SERVICE_CALENDAR_FILE "service_calendar.txt"
#define MAINTENANCE_REMINDERS_FILE "maintenance_reminders.txt"
#define QNA_FILE "qna.txt"
#define CAR_PARKING_FILE "car_parking.txt"
#define VAT_RATE 0.15f
#define CARS_FILE "cars.txt"
#define ADMIN_SECRET_CODE "ADMIN2024@SECURE"
#define CONSOLE_WIDTH 80
#define MAX_LOGIN_ATTEMPTS 3
#define LOCKOUT_TIME 120 // 2 minutes in seconds

// Console Color Codes
#define COLOR_BLUE 9
#define COLOR_GREEN 10
#define COLOR_CYAN 11
#define COLOR_RED 12
#define COLOR_MAGENTA 13
#define COLOR_YELLOW 14
#define COLOR_WHITE 15
#define COLOR_DEFAULT 7

// Global variables for login attempts
int admin_failed_attempts = 0;
int user_failed_attempts = 0;
time_t admin_lockout_time = 0;
time_t user_lockout_time = 0;

// Function prototypes
void set_console_color(int color);
void reset_console_color();
void display_current_time();
void display_ascii_logo();
void center_print(const char* text);
void center_prompt(const char* text);
void get_hidden_password(char *password, int max_length);
void print_separator();
void clear_screen();
// Simple bordered box functions
void print_top_border();
void print_bottom_border();
void print_side_border_line(const char* text);
void print_empty_border_line();
void print_simple_box(const char* title, const char* content[], int content_count);
void clear_screen_with_simple_border();
void register_user();
int login_user(char logged_in_user[30], char logged_in_role[20]);
int admin_login(char logged_in_user[30], char logged_in_role[20]);
int admin_password_recovery();
int validate_phone(const char* phone);
int validate_email(const char* email);
int validate_username(const char* username, const char* name);
int is_account_locked(time_t lockout_time);
void lock_account(time_t* lockout_time);
void add_part();
void delete_part();
void view_parts();
void view_not_available_parts();
void update_profile(const char *logged_in_username);
void order_parts(const char *username);
void view_order_history(const char *username);
float calculate_estimation(const char *username);
void generate_invoice(const char *actor_username, const char *actor_role);
void add_car();
void view_cars();
void delete_car();
void modify_car();
void admin_panel(const char *admin_username);
void view_all_users();
void view_all_orders();
void system_statistics();
void daily_dashboard();
void main_menu();
void manage_discounts();
void add_discount();
void view_discounts();
void delete_discount();
float apply_promo_code(float total_amount);
void initialize_default_discounts();
void manage_appointments();
void add_mechanic();
void view_mechanics();
void assign_mechanic_to_order();
void view_appointments();
void delete_mechanic();
// Mechanic Dashboard Functions
int mechanic_login(char logged_in_user[30], char logged_in_role[20]);
void mechanic_dashboard(const char *mechanic_username);
void view_car_profile();
void add_car_profile(const char *car_number);
void view_vehicle_history();
void add_vehicle_history();
void update_progress();
void view_progress();
void set_deadline();
void view_assigned_cars(const char *mechanic_username);
void view_service_calendar();
void add_service_event();
void set_maintenance_reminder();
void view_maintenance_reminders();
void customer_view_maintenance_reminders();
void manage_qna();
void add_qna();
void view_qna();
void customer_view_qna();
void add_car_parking(const char *username);
void view_car_parking();
void initialize_default_qna();

/**
 * Set console color
 */
void set_console_color(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

/**
 * Reset console color to default
 */
void reset_console_color() {
    set_console_color(COLOR_DEFAULT);
}

/**
 * Clear screen function with blue background
 */
void clear_screen() {
    system("cls"); // For Windows
    set_console_color(COLOR_BLUE);
}

/**
 * Hidden password input function
 */
void get_hidden_password(char *password, int max_length) {
    int i = 0;
    char ch;

    while (i < max_length - 1) {
        ch = _getch(); // Get character without displaying it

        if (ch == '\r' || ch == '\n') { // Enter key pressed
            break;
        } else if (ch == '\b' && i > 0) { // Backspace pressed
            printf("\b \b"); // Erase the asterisk
            i--;
        } else if (ch != '\b') { // Normal character
            password[i] = ch;
            printf("*"); // Display asterisk
            i++;
        }
    }
    password[i] = '\0'; // Null terminate
    printf("\n");
}

/**
 * Center align text in console
 */
void center_print(const char* text) {
    set_console_color(COLOR_CYAN);
    int len = strlen(text);
    int padding = (CONSOLE_WIDTH - len) / 2;
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s\n", text);
    set_console_color(COLOR_BLUE);
}

/**
 * Center align input prompt (without newline)
 */
void center_prompt(const char* text) {
    set_console_color(COLOR_YELLOW);
    int len = strlen(text);
    int padding = (CONSOLE_WIDTH - len) / 2;
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s", text);
    set_console_color(COLOR_BLUE);
}

/**
 * Print decorative separator line
 */
void print_separator() {
    set_console_color(COLOR_CYAN);
    for (int i = 0; i < CONSOLE_WIDTH; i++) printf("=");
    printf("\n");
    set_console_color(COLOR_BLUE);
}

// ========== SIMPLE BORDERED BOX SYSTEM ==========

/**
 * Print top border using equals signs
 */
void print_top_border() {
    set_console_color(COLOR_CYAN);
    printf("================================================================================\n");
    reset_console_color();
}

/**
 * Print bottom border using equals signs
 */
void print_bottom_border() {
    set_console_color(COLOR_CYAN);
    printf("================================================================================\n");
    reset_console_color();
}

/**
 * Print a line with side borders
 */
void print_side_border_line(const char* text) {
    set_console_color(COLOR_CYAN);
    printf("|| ");
    set_console_color(COLOR_WHITE);

    int text_len = strlen(text);
    int padding = (CONSOLE_WIDTH - 6 - text_len) / 2; // Account for "|| " and " ||"

    // Left padding
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s", text);

    // Right padding
    int remaining = CONSOLE_WIDTH - 6 - text_len - padding;
    for (int i = 0; i < remaining; i++) printf(" ");

    set_console_color(COLOR_CYAN);
    printf(" ||\n");
    reset_console_color();
}

/**
 * Print empty line with side borders
 */
void print_empty_border_line() {
    set_console_color(COLOR_CYAN);
    printf("||");
    for (int i = 0; i < CONSOLE_WIDTH - 4; i++) printf(" ");
    printf("||\n");
    reset_console_color();
}

/**
 * Print a complete simple box with title and content
 */
void print_simple_box(const char* title, const char* content[], int content_count) {
    print_top_border();
    print_empty_border_line();

    // Print title
    set_console_color(COLOR_CYAN);
    printf("|| ");
    set_console_color(COLOR_YELLOW);

    int title_len = strlen(title);
    int padding = (CONSOLE_WIDTH - 6 - title_len) / 2;

    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s", title);

    int remaining = CONSOLE_WIDTH - 6 - title_len - padding;
    for (int i = 0; i < remaining; i++) printf(" ");

    set_console_color(COLOR_CYAN);
    printf(" ||\n");
    reset_console_color();

    print_empty_border_line();

    // Print separator line
    set_console_color(COLOR_CYAN);
    printf("|| ");
    set_console_color(COLOR_MAGENTA);
    for (int i = 0; i < CONSOLE_WIDTH - 6; i++) printf("-");
    set_console_color(COLOR_CYAN);
    printf(" ||\n");
    reset_console_color();

    print_empty_border_line();

    // Print content
    for (int i = 0; i < content_count; i++) {
        print_side_border_line(content[i]);
        if (i < content_count - 1) print_empty_border_line();
    }

    print_empty_border_line();
    print_bottom_border();
}

/**
 * Clear screen and start simple bordered mode
 */
void clear_screen_with_simple_border() {
    clear_screen();
    printf("\n");
}

// ========== END SIMPLE BORDERED BOX SYSTEM ==========

/**
 * Display current time with decorative elements
 */
void display_current_time() {
    time_t now = time(NULL);
    struct tm *local = localtime(&now);

    set_console_color(COLOR_YELLOW);
    printf("\n");
    center_print("╔═══════════════════════════════════════════════════════════════╗");

    char time_display[100];
    sprintf(time_display, "║  ⏰ CURRENT TIME: %02d:%02d:%02d  📅 DATE: %04d-%02d-%02d  ║",
            local->tm_hour, local->tm_min, local->tm_sec,
            local->tm_year + 1900, local->tm_mon + 1, local->tm_mday);
    center_print(time_display);

    center_print("╚═══════════════════════════════════════════════════════════════╝");
    set_console_color(COLOR_BLUE);
}

/**
 * Display ASCII art logo with center alignment
 */
void display_ascii_logo() {
    clear_screen();
    printf("\n\n");

    set_console_color(COLOR_YELLOW);
    center_print("  ____   _    ____  _  __ ___ _______   __");
    center_print(" |  _ \\ / \\  |  _ \\| |/ /|_ _|  ___\\ \\ / /");
    center_print(" | |_) / _ \\ | |_) | ' /  | || |_   \\ V / ");
    center_print(" |  __/ ___ \\|  _ <| . \\  | ||  _|   | |  ");
    center_print(" |_| /_/   \\_\\_| \\_\\_|\\_\\|___|_|     |_|  ");

    printf("\n");
    set_console_color(COLOR_MAGENTA);
    center_print("*** SMART GARAGE MANAGEMENT SYSTEM ***");

    set_console_color(COLOR_CYAN);
    print_separator();
    center_print("Your One-Stop Solution for Car Parts & Garage Management");
    print_separator();

    set_console_color(COLOR_BLUE);
    printf("\n");
}

/**
 * Validate phone number (must start with 01 and be 11 digits)
 */
int validate_phone(const char* phone) {
    if (strlen(phone) != 11) return 0;
    if (phone[0] != '0' || phone[1] != '1') return 0;
    for (int i = 0; i < 11; i++) {
        if (!isdigit(phone[i])) return 0;
    }
    return 1;
}

/**
 * Validate email domain
 */
int validate_email(const char* email) {
    const char* valid_domains[] = {
        "@gmail.com", "@hotmail.com", "@outlook.com",
        "@protonmail.com", "@diu.edu.bd", "@yahoo.com"
    };
    int num_domains = sizeof(valid_domains) / sizeof(valid_domains[0]);

    for (int i = 0; i < num_domains; i++) {
        if (strstr(email, valid_domains[i]) != NULL) {
            return 1;
        }
    }
    return 0;
}

/**
 * Validate username format (name_id)
 */
int validate_username(const char* username, const char* name) {
    // Extract first name from full name for username validation
    char first_name[50];
    sscanf(name, "%s", first_name); // Gets first word only

    char expected_format[100];
    sprintf(expected_format, "%s_", first_name);

    // Check if username starts with first name followed by underscore
    if (strncmp(username, expected_format, strlen(expected_format)) != 0) {
        return 0;
    }

    // Check if there's an ID after underscore
    const char* id_part = username + strlen(expected_format);
    if (strlen(id_part) == 0) return 0;

    // ID part should contain at least some characters/numbers
    return 1;
}

/**
 * Check if account is locked
 */
int is_account_locked(time_t lockout_time) {
    if (lockout_time == 0) return 0;

    time_t current_time = time(NULL);
    if (current_time - lockout_time < LOCKOUT_TIME) {
        int remaining = LOCKOUT_TIME - (current_time - lockout_time);
        char lock_msg[100];
        sprintf(lock_msg, "[X] Account locked! Try again in %d seconds.", remaining);
        center_print(lock_msg);
        return 1;
    }
    return 0;
}

/**
 * Lock account for specified time
 */
void lock_account(time_t* lockout_time) {
    *lockout_time = time(NULL);
    center_print("[!] SECURITY ALERT: Account locked for 2 minutes!");
    center_print("Too many failed login attempts detected.");
}

/**
 * Admin password recovery
 */
int admin_password_recovery() {
    clear_screen();
    center_print("[#] ADMIN PASSWORD RECOVERY");
    center_print("Security Questions for Password Recovery");
    print_separator();

    char answer[100];

    printf("\n");
    center_print("Security Question: What is the default admin username?");
    printf("%*s", (CONSOLE_WIDTH-25)/2, "");
    printf("Answer: ");
    scanf("%s", answer);

    if (strcmp(answer, "admin") != 0) {
        center_print("[X] Incorrect answer!");
        return 0;
    }

    center_print("Security Question: What is the system name? (SmartGarage)");
    printf("%*s", (CONSOLE_WIDTH-25)/2, "");
    printf("Answer: ");
    scanf("%s", answer);

    if (strcmp(answer, "SmartGarage") != 0) {
        center_print("[X] Incorrect answer!");
        return 0;
    }

    printf("\n");
    center_print("[+] Security questions verified!");
    center_print("[#] Default Password: admin123");
    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
    return 1;
}

/**
 * Customer registration (No inventory manager option)
 */
void register_user() {
    clear_screen();
    display_ascii_logo();
    center_print("[+] CUSTOMER REGISTRATION");
    print_separator();

    FILE *f = fopen(USERS_FILE, "a");
    char username[30], password[30];
    char name[50], email[50], phone[20];

    if (!f) {
        center_print("[X] Error opening users file.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    printf("\n");
    center_print("Please fill in your details:");
    printf("\n");

    // Get name first for username validation
    center_prompt("Full Name: ");
    getchar(); // Clear any leftover newline character
    fgets(name, sizeof(name), stdin);
    // Remove newline character if present
    if (name[strlen(name) - 1] == '\n') {
        name[strlen(name) - 1] = '\0';
    }

    // Validate and get username
    while (1) {
        char prompt[100];
        char first_name[50];
        sscanf(name, "%s", first_name); // Extract first name for prompt
        sprintf(prompt, "Username (format: %s_id): ", first_name);
        center_prompt(prompt);
        scanf("%s", username);

        if (validate_username(username, name)) {
            break;
        } else {
            center_print("[X] Invalid username format! Use: firstname_id");
        }
    }

    center_prompt("Password: ");
    get_hidden_password(password, sizeof(password));

    // Validate and get email
    while (1) {
        center_prompt("Email: ");
        scanf("%s", email);

        if (validate_email(email)) {
            break;
        } else {
            center_print("[X] Invalid email domain!");
            center_print("Allowed: gmail.com, hotmail.com, outlook.com, protonmail.com, diu.edu.bd, yahoo.com");
        }
    }

    // Validate and get phone
    while (1) {
        center_prompt("Phone (01XXXXXXXXX): ");
        scanf("%s", phone);

        if (validate_phone(phone)) {
            break;
        } else {
            center_print("[X] Invalid phone number!");
            center_print("Must start with 01 and be exactly 11 digits.");
        }
    }

    // All registrations are customers by default
    fprintf(f, "Customer %s %s %s %s %s\n", username, password, name, email, phone);
    fclose(f);

    printf("\n");
    center_print("[+] Registration successful! You are registered as a Customer.");
    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

/**
 * Regular user login (Customer only)
 */
int login_user(char logged_in_user[30], char logged_in_role[20]) {
    if (is_account_locked(user_lockout_time)) {
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return 0;
    }

    clear_screen();
    display_ascii_logo();
    center_print("[#] USER LOGIN");
    print_separator();

    FILE *f = fopen(USERS_FILE, "r");
    char username[30], password[30], role[20];
    char name[50], email[50], phone[20];
    char input_username[30], input_password[30];
    int found = 0;

    if (!f) {
        center_print("[X] No users registered yet.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return 0;
    }

    printf("\n");
    center_prompt("Username: ");
    scanf("%s", input_username);

    center_prompt("Password: ");
    get_hidden_password(input_password, sizeof(input_password));

    while (fscanf(f, "%s %s %s %s %s %s", role, username, password, name, email, phone) == 6) {
        if (strcmp(username, input_username) == 0 && strcmp(password, input_password) == 0) {
            if (strcmp(role, "Customer") == 0) { // Only customers can login here
                printf("\n");
                center_print("[+] Login Successful!");
                printf("\n");
                char welcome_msg[100];
                sprintf(welcome_msg, "Welcome, %s (%s)", name, role);
                center_print(welcome_msg);
                printf("\n");

                strcpy(logged_in_user, username);
                strcpy(logged_in_role, role);
                found = 1;
                user_failed_attempts = 0; // Reset on successful login
                break;
            }
        }
    }
    fclose(f);

    if (!found) {
        user_failed_attempts++;
        center_print("[X] Login failed. Invalid credentials.");

        if (user_failed_attempts >= MAX_LOGIN_ATTEMPTS) {
            lock_account(&user_lockout_time);
            user_failed_attempts = 0;
        } else {
            char attempts_msg[100];
            sprintf(attempts_msg, "[!] Warning: %d/%d attempts remaining",
                   MAX_LOGIN_ATTEMPTS - user_failed_attempts, MAX_LOGIN_ATTEMPTS);
            center_print(attempts_msg);
        }

        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return 0;
    }

    center_print("Press any key to continue...");
    getchar(); getchar();
    return 1;
}

/**
 * Secure admin login
 */
int admin_login(char logged_in_user[30], char logged_in_role[20]) {
    if (is_account_locked(admin_lockout_time)) {
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return 0;
    }

    clear_screen();
    display_ascii_logo();
    center_print("[#] ADMIN SECURE LOGIN");
    center_print("[!] AUTHORIZED PERSONNEL ONLY [!]");
    print_separator();

    char username[30], password[30];

    printf("\n");
    center_prompt("Admin Username: ");
    scanf("%s", username);

    center_prompt("Admin Password: ");
    get_hidden_password(password, sizeof(password));

    // Check default admin credentials
    if (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0) {
        strcpy(logged_in_user, "admin");
        strcpy(logged_in_role, "Admin");
        admin_failed_attempts = 0; // Reset on successful login

        center_print("[*] ADMIN ACCESS GRANTED");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return 1;
    }

    admin_failed_attempts++;
    center_print("[X] Invalid admin credentials.");

    if (admin_failed_attempts >= MAX_LOGIN_ATTEMPTS) {
        lock_account(&admin_lockout_time);
        admin_failed_attempts = 0;

        printf("\n");
        center_print("[?] Forgot Password? Try Password Recovery");
        center_print("Press 'R' for Recovery or any other key to continue...");

        char choice = getchar();
        getchar(); // consume newline

        if (choice == 'R' || choice == 'r') {
            if (admin_password_recovery()) {
                admin_lockout_time = 0; // Reset lockout if recovery successful
            }
        }
    } else {
        char attempts_msg[100];
        sprintf(attempts_msg, "[!] Warning: %d/%d attempts remaining",
               MAX_LOGIN_ATTEMPTS - admin_failed_attempts, MAX_LOGIN_ATTEMPTS);
        center_print(attempts_msg);
    }

    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
    return 0;
}

/**
 * Mechanic login function
 */
int mechanic_login(char logged_in_user[30], char logged_in_role[20]) {
    clear_screen();
    display_ascii_logo();
    center_print("[#] MECHANIC LOGIN");
    print_separator();

    // Check if mechanics are locked out
    if (is_account_locked(user_lockout_time)) {
        center_print("[X] Account temporarily locked due to failed attempts.");
        center_print("Please try again later.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return 0;
    }

    char username[30];
    printf("\n");
    center_prompt("Username: ");
    scanf("%s", username);

    // Check if mechanic exists in mechanics.txt
    FILE *f = fopen(MECHANICS_FILE, "r");
    if (!f) {
        center_print("[X] No mechanics registered in system.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return 0;
    }

    char line[200], stored_name[50], stored_username[30];
    int age;
    char phone[20];
    int found = 0;

    while (fgets(line, sizeof(line), f)) {
        sscanf(line, "%s %s %d %s", stored_name, stored_username, &age, phone);
        if (strcmp(stored_username, username) == 0) {
            found = 1;
            break;
        }
    }
    fclose(f);

    if (found) {
        strcpy(logged_in_user, username);  // Store username instead of name
        strcpy(logged_in_role, "Mechanic");

        user_failed_attempts = 0; // Reset attempts on success

        center_print("[+] Login successful!");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return 1;
    } else {
        user_failed_attempts++;

        center_print("[X] Username not found in system.");
        center_print("Please contact admin to register as a mechanic.");

        if (user_failed_attempts >= MAX_LOGIN_ATTEMPTS) {
            lock_account(&user_lockout_time);
            center_print("[!] Account locked due to multiple failed attempts.");
        }

        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return 0;
    }
}

/**
 * Daily dashboard for admin
 */
void daily_dashboard() {
    clear_screen();
    display_ascii_logo();
    center_print("[*] DAILY DASHBOARD");
    print_separator();

    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    char today_date[20];
    sprintf(today_date, "%04d-%02d-%02d",
            local->tm_year + 1900, local->tm_mon + 1, local->tm_mday);

    char dashboard_msg[100];
    sprintf(dashboard_msg, "[*] Today's Report: %s", today_date);
    center_print(dashboard_msg);
    printf("\n");

    // Count today's orders
    FILE *f = fopen(ORDERS_FILE, "r");
    int today_orders = 0;
    float today_revenue = 0;
    char order_parts[100][50];
    int part_count = 0;

    if (f) {
        char user[30], part[50];
        int quantity;
        float total;

        while (fscanf(f, "%s %s %d %f", user, part, &quantity, &total) == 4) {
            // For demo purposes, we'll count all orders as "today's"
            // In real implementation, you'd compare with actual order dates
            today_orders++;
            today_revenue += total;

            // Store unique parts ordered today
            int part_exists = 0;
            for (int i = 0; i < part_count; i++) {
                if (strcmp(order_parts[i], part) == 0) {
                    part_exists = 1;
                    break;
                }
            }
            if (!part_exists && part_count < 100) {
                strcpy(order_parts[part_count], part);
                part_count++;
            }
        }
        fclose(f);
    }

    // Display today's statistics
    sprintf(dashboard_msg, "[*] Orders Placed Today: %d", today_orders);
    center_print(dashboard_msg);

    sprintf(dashboard_msg, "[$] Today's Revenue: $%.2f", today_revenue);
    center_print(dashboard_msg);

    sprintf(dashboard_msg, "[#] Different Parts Ordered: %d", part_count);
    center_print(dashboard_msg);

    if (part_count > 0) {
        printf("\n");
        center_print("[*] POPULAR PARTS TODAY:");
        print_separator();
        for (int i = 0; i < part_count && i < 10; i++) { // Show top 10
            printf("%*s", (CONSOLE_WIDTH-20)/2, "");
            printf("* %s\n", order_parts[i]);
        }
    }

    // Quick stats
    printf("\n");
    print_separator();
    center_print("[*] QUICK SYSTEM STATUS");

    // Count total users
    f = fopen(USERS_FILE, "r");
    int total_customers = 0;
    if (f) {
        char role[20], username[30], password[30], name[50], email[50], phone[20];
        while (fscanf(f, "%s %s %s %s %s %s", role, username, password, name, email, phone) == 6) {
            if (strcmp(role, "Customer") == 0) total_customers++;
        }
        fclose(f);
    }

    sprintf(dashboard_msg, "[#] Total Customers: %d", total_customers);
    center_print(dashboard_msg);

    // Count cars in garage
    f = fopen(CARS_FILE, "r");
    int cars_count = 0;
    if (f) {
        char line[100];
        while (fgets(line, sizeof(line), f)) {
            cars_count++;
        }
        fclose(f);
    }

    sprintf(dashboard_msg, "[*] Cars in Garage: %d", cars_count);
    center_print(dashboard_msg);

    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

/**
 * Add part to inventory
 */
void add_part() {
    clear_screen();
    display_ascii_logo();
    center_print("[+] ADD NEW PART");
    print_separator();

    FILE *f = fopen(PARTS_FILE, "a");
    char part[50], spec[50];
    float price;

    if (!f) {
        center_print("[X] Error opening inventory file.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    printf("\n");
    center_prompt("Part Name: ");
    scanf("%s", part);

    center_prompt("Specifications: ");
    scanf("%s", spec);

    center_prompt("Price: $");
    scanf("%f", &price);

    fprintf(f, "%s %s %.2f\n", part, spec, price);
    fclose(f);

    printf("\n");
    center_print("[+] Part added successfully!");
    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

/**
 * Delete part from inventory
 */
void delete_part() {
    clear_screen();
    display_ascii_logo();
    center_print("[-] DELETE PART");
    print_separator();

    FILE *f = fopen(PARTS_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    char part[50], spec[50], target[50];
    float price;
    int found = 0;

    if (!f || !temp) {
        center_print("[X] Error opening files.");
        if (f) fclose(f);
        if (temp) fclose(temp);
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    printf("\n");
    center_prompt("Enter part name to delete: ");
    scanf("%s", target);

    while (fscanf(f, "%s %s %f", part, spec, &price) == 3) {
        if (strcmp(part, target) != 0) {
            fprintf(temp, "%s %s %.2f\n", part, spec, price);
        } else {
            found = 1;
        }
    }

    fclose(f);
    fclose(temp);
    remove(PARTS_FILE);
    rename("temp.txt", PARTS_FILE);

    printf("\n");
    if (found)
        center_print("[+] Part deleted successfully!");
    else
        center_print("[X] Part not found.");

    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

/**
 * View all parts in inventory
 */
void view_parts() {
    clear_screen();
    display_ascii_logo();
    center_print("[*] AVAILABLE PARTS INVENTORY");
    print_separator();

    FILE *f = fopen(PARTS_FILE, "r");
    char part[50], spec[50];
    float price;

    if (!f) {
        center_print("[-] No parts found in inventory.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    printf("\n");
    printf("%*s", (CONSOLE_WIDTH-40)/2, "");
    printf("%-15s %-20s %s\n", "PART NAME", "SPECIFICATIONS", "PRICE ($)");
    printf("%*s", (CONSOLE_WIDTH-40)/2, "");
    for (int i = 0; i < 40; i++) printf("-");
    printf("\n");

    while (fscanf(f, "%s %s %f", part, spec, &price) == 3) {
        printf("%*s", (CONSOLE_WIDTH-40)/2, "");
        printf("%-15s %-20s $%.2f\n", part, spec, price);
    }
    fclose(f);

    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

/**
 * View not available parts
 */
void view_not_available_parts() {
    clear_screen();
    display_ascii_logo();
    center_print("[X] OUT OF STOCK / UPCOMING PARTS");
    print_separator();

    FILE *f = fopen(NOT_AVAILABLE_FILE, "r");
    char part[50], spec[50], date[20];

    if (!f) {
        center_print("[+] All parts are currently available!");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    printf("\n");
    printf("%*s", (CONSOLE_WIDTH-50)/2, "");
    printf("%-15s %-20s %s\n", "PART NAME", "SPECIFICATIONS", "RESTOCK DATE");
    printf("%*s", (CONSOLE_WIDTH-50)/2, "");
    for (int i = 0; i < 50; i++) printf("-");
    printf("\n");

    while (fscanf(f, "%s %s %s", part, spec, date) == 3) {
        printf("%*s", (CONSOLE_WIDTH-50)/2, "");
        printf("%-15s %-20s %s\n", part, spec, date);
    }
    fclose(f);

    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

/**
 * Update user profile
 */
void update_profile(const char *logged_in_username) {
    clear_screen();
    display_ascii_logo();
    center_print("[*] UPDATE PROFILE");
    print_separator();

    FILE *f = fopen(USERS_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    char username[30], password[30], role[20];
    char name[50], email[50], phone[20];
    int found = 0;

    if (!f || !temp) {
        center_print("[X] Error opening files.");
        if (f) fclose(f);
        if (temp) fclose(temp);
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    while (fscanf(f, "%s %s %s %s %s %s", role, username, password, name, email, phone) == 6) {
        if (strcmp(username, logged_in_username) == 0) {
            char update_msg[50];
            sprintf(update_msg, "Updating profile for: %s", logged_in_username);
            center_print(update_msg);
            printf("\n");

            printf("%*s", (CONSOLE_WIDTH-20)/2, "");
            printf("New Name: ");
            scanf("%s", name);

            // Validate and get email
            while (1) {
                printf("%*s", (CONSOLE_WIDTH-20)/2, "");
                printf("New Email: ");
                scanf("%s", email);

                if (validate_email(email)) {
                    break;
                } else {
                    center_print("[X] Invalid email domain!");
                }
            }

            // Validate and get phone
            while (1) {
                printf("%*s", (CONSOLE_WIDTH-30)/2, "");
                printf("New Phone (01XXXXXXXXX): ");
                scanf("%s", phone);

                if (validate_phone(phone)) {
                    break;
                } else {
                    center_print("[X] Invalid phone number format!");
                }
            }

            found = 1;
        }
        fprintf(temp, "%s %s %s %s %s %s\n", role, username, password, name, email, phone);
    }

    fclose(f);
    fclose(temp);
    remove(USERS_FILE);
    rename("temp.txt", USERS_FILE);

    printf("\n");
    if (found)
        center_print("[+] Profile updated successfully!");
    else
        center_print("[X] Username not found.");

    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

/**
 * Order parts (Customer only)
 */
void order_parts(const char *username) {
    clear_screen();
    display_ascii_logo();
    center_print("[*] PLACE YOUR ORDER");
    print_separator();

    FILE *f_parts = fopen(PARTS_FILE, "r");
    if (!f_parts) {
        center_print("[X] Error opening parts file.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    // Read and display all parts with numbers
    char parts[100][50], specs[100][50];
    float prices[100];
    int total_parts = 0;

    printf("\n");
    center_print("[*] AVAILABLE PARTS:");
    printf("\n");

    while (fscanf(f_parts, "%s %s %f", parts[total_parts], specs[total_parts], &prices[total_parts]) == 3) {
        char part_line[150];
        sprintf(part_line, "%d.  [%s] %s - $%.2f", total_parts + 1, specs[total_parts], parts[total_parts], prices[total_parts]);
        center_print(part_line);
        total_parts++;
        if (total_parts >= 100) break; // Safety limit
    }
    fclose(f_parts);

    if (total_parts == 0) {
        center_print("[X] No parts available in inventory.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    printf("\n");
    int choice, quantity;
    char prompt[100];
    sprintf(prompt, "Select part number (1-%d): ", total_parts);
    center_prompt(prompt);
    scanf("%d", &choice);

    if (choice < 1 || choice > total_parts) {
        center_print("[X] Invalid part selection.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    choice--; // Convert to 0-based index

    center_prompt("Quantity: ");
    scanf("%d", &quantity);

    if (quantity <= 0) {
        center_print("[X] Invalid quantity.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    float total_price = prices[choice] * quantity;

    // Show order summary
    printf("\n");
    center_print("[*] ORDER SUMMARY:");
    char summary[150];
    sprintf(summary, "Part: %s | Quantity: %d | Unit Price: $%.2f", parts[choice], quantity, prices[choice]);
    center_print(summary);
    sprintf(summary, "Total Amount: $%.2f", total_price);
    center_print(summary);

    // Apply promo code
    float final_price = apply_promo_code(total_price);

    // Payment options
    printf("\n");
    center_print("[*] PAYMENT OPTIONS:");
    center_print("1.  [C]  Cash Payment");
    center_print("2.  [O]  Paid Online");
    printf("\n");

    int payment_choice;
    center_prompt("Select payment method (1-2): ");
    scanf("%d", &payment_choice);

    char payment_method[20];
    if (payment_choice == 1) {
        strcpy(payment_method, "Cash");
    } else if (payment_choice == 2) {
        strcpy(payment_method, "Online");
    } else {
        center_print("[X] Invalid payment option. Defaulting to Cash.");
        strcpy(payment_method, "Cash");
    }

    // Get current date and time
    time_t now = time(0);
    char *date_time = ctime(&now);
    date_time[strlen(date_time) - 1] = '\0'; // Remove newline

    // Save order to file with payment info and date/time
    FILE *f_orders = fopen(ORDERS_FILE, "a");
    if (!f_orders) {
        center_print("[X] Error saving order.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    fprintf(f_orders, "%s %s %d %.2f %s %s\n", username, parts[choice], quantity, final_price, payment_method, date_time);
    fclose(f_orders);

    printf("\n");
    center_print("[+] Order placed successfully!");
    char payment_msg[100];
    sprintf(payment_msg, "[*] Payment Method: %s", payment_method);
    center_print(payment_msg);
    sprintf(payment_msg, "[*] Final Amount: $%.2f", final_price);
    center_print(payment_msg);

    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

/**
 * View Order History
 */
void view_order_history(const char *username) {
    clear_screen();
    display_ascii_logo();
    center_print("[H] ORDER HISTORY");
    print_separator();

    printf("\n");
    center_print("1.  [*]  View All Orders");
    center_print("2.  [F]  Filter by Date");
    center_print("3.  [<]  Back to Menu");
    printf("\n");

    int choice;
    center_prompt("Select option (1-3): ");
    scanf("%d", &choice);

    FILE *f_orders = fopen(ORDERS_FILE, "r");
    if (!f_orders) {
        center_print("[X] Error opening orders file or no orders found.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    char user[30], part[50], payment[20], date_time[100];
    int quantity, order_count = 0;
    float price;

    if (choice == 1) {
        // View all orders
        clear_screen();
        display_ascii_logo();
        center_print("[H] YOUR ORDER HISTORY");
        print_separator();
        printf("\n");

        // Table headers
        printf("%*s", (CONSOLE_WIDTH-95)/2, "");
        printf("%-4s %-15s %-8s %-10s %-12s %-10s %-15s %-12s\n", "No.", "Item Name", "Quantity", "Unit Price", "Total Cost", "Payment", "Customer Name", "Current Date");
        printf("%*s", (CONSOLE_WIDTH-95)/2, "");
        for (int i = 0; i < 95; i++) printf("-");
        printf("\n");

        // Get current date
        time_t now = time(0);
        char current_date[15];
        strftime(current_date, sizeof(current_date), "%Y-%m-%d", localtime(&now));

        // Get customer's full name from user file
        char customer_full_name[50] = "Unknown";
        FILE *f_users = fopen(USERS_FILE, "r");
        if (f_users) {
            char role[20], stored_username[30], password[30], name[50], email[50], phone[20];
            while (fscanf(f_users, "%s %s %s %s %s %s", role, stored_username, password, name, email, phone) == 6) {
                if (strcmp(stored_username, username) == 0) {
                    strcpy(customer_full_name, name);
                    break;
                }
            }
            fclose(f_users);
        }

        while (fscanf(f_orders, "%s %s %d %f %s %[^\n]", user, part, &quantity, &price, payment, date_time) == 6) {
            if (strcmp(user, username) == 0) {
                order_count++;
                float unit_price = price / quantity; // Calculate unit price
                printf("%*s", (CONSOLE_WIDTH-95)/2, "");
                printf("%-4d %-15s %-8d $%-9.2f $%-11.2f %-10s %-15s %-12s\n",
                       order_count, part, quantity, unit_price, price, payment, customer_full_name, current_date);
            }
        }

        if (order_count == 0) {
            center_print("[X] No orders found for your account.");
        }

    } else if (choice == 2) {
        // Filter by date
        char search_date[20];
        printf("\n");
        center_prompt("Enter date to search (format: Mon Aug 09 or just Aug 09): ");
        getchar(); // Clear buffer
        fgets(search_date, sizeof(search_date), stdin);
        search_date[strcspn(search_date, "\n")] = 0; // Remove newline

        clear_screen();
        display_ascii_logo();
        center_print("[F] FILTERED ORDER HISTORY");
        print_separator();
        printf("\n");
        char filter_msg[100];
        sprintf(filter_msg, "Showing orders for: %s", search_date);
        center_print(filter_msg);
        printf("\n");

        // Table headers
        printf("%*s", (CONSOLE_WIDTH-95)/2, "");
        printf("%-4s %-15s %-8s %-10s %-12s %-10s %-15s %-12s\n", "No.", "Item Name", "Quantity", "Unit Price", "Total Cost", "Payment", "Customer Name", "Current Date");
        printf("%*s", (CONSOLE_WIDTH-95)/2, "");
        for (int i = 0; i < 95; i++) printf("-");
        printf("\n");

        // Get current date
        time_t now = time(0);
        char current_date[15];
        strftime(current_date, sizeof(current_date), "%Y-%m-%d", localtime(&now));

        // Get customer's full name from user file
        char customer_full_name[50] = "Unknown";
        FILE *f_users = fopen(USERS_FILE, "r");
        if (f_users) {
            char role[20], stored_username[30], password[30], name[50], email[50], phone[20];
            while (fscanf(f_users, "%s %s %s %s %s %s", role, stored_username, password, name, email, phone) == 6) {
                if (strcmp(stored_username, username) == 0) {
                    strcpy(customer_full_name, name);
                    break;
                }
            }
            fclose(f_users);
        }

        while (fscanf(f_orders, "%s %s %d %f %s %[^\n]", user, part, &quantity, &price, payment, date_time) == 6) {
            if (strcmp(user, username) == 0 && strstr(date_time, search_date) != NULL) {
                order_count++;
                float unit_price = price / quantity; // Calculate unit price
                printf("%*s", (CONSOLE_WIDTH-95)/2, "");
                printf("%-4d %-15s %-8d $%-9.2f $%-11.2f %-10s %-15s %-12s\n",
                       order_count, part, quantity, unit_price, price, payment, customer_full_name, current_date);
            }
        }

        if (order_count == 0) {
            center_print("[X] No orders found for the specified date.");
        }

    } else if (choice == 3) {
        fclose(f_orders);
        return;
    } else {
        center_print("[X] Invalid choice.");
    }

    fclose(f_orders);
    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

/**
 * Calculate order estimation
 */
float calculate_estimation(const char *username) {
    clear_screen();
    display_ascii_logo();
    center_print("[$] ORDER ESTIMATION");
    print_separator();

    FILE *orders = fopen(ORDERS_FILE, "r");
    char user[30], part[50], payment[20], date_time[100];
    int quantity;
    float total, sum = 0;

    if (!orders) {
        center_print("[-] No orders found.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return 0;
    }

    printf("\n");
    center_print("[*] YOUR ORDERS:");
    printf("\n");
    printf("%*s", (CONSOLE_WIDTH-60)/2, "");
    printf("%-15s %-10s %-8s %s\n", "PART", "QUANTITY", "PAYMENT", "TOTAL ($)");
    printf("%*s", (CONSOLE_WIDTH-60)/2, "");
    for (int i = 0; i < 60; i++) printf("-");
    printf("\n");

    // Try new format first
    while (fscanf(orders, "%s %s %d %f %s %[^\n]", user, part, &quantity, &total, payment, date_time) == 6) {
        if (strcmp(user, username) == 0) {
            printf("%*s", (CONSOLE_WIDTH-60)/2, "");
            printf("%-15s %-10d %-8s $%.2f\n", part, quantity, payment, total);
            sum += total;
        }
    }

    // If no new format found, try old format
    if (sum == 0) {
        fseek(orders, 0, SEEK_SET);
        while (fscanf(orders, "%s %s %d %f", user, part, &quantity, &total) == 4) {
            if (strcmp(user, username) == 0) {
                printf("%*s", (CONSOLE_WIDTH-60)/2, "");
                printf("%-15s %-10d %-8s $%.2f\n", part, quantity, "Cash", total);
                sum += total;
            }
        }
    }
    fclose(orders);

    printf("\n");
    char total_msg[50];
    sprintf(total_msg, "[$] TOTAL ESTIMATION: $%.2f", sum);
    center_print(total_msg);

    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
    return sum;
}

/**
 * Generate invoice
 */
void generate_invoice(const char *actor_username, const char *actor_role) {
    clear_screen();
    display_ascii_logo();
    center_print("[*] GENERATE INVOICE");
    print_separator();

    char target_username[30];
    char cust_name[50] = "N/A", cust_email[50] = "N/A", cust_phone[20] = "N/A";
    int user_found = 0;

    if (strcmp(actor_role, "Admin") == 0) {
        printf("\n");
        center_prompt("Customer username for invoice: ");
        scanf("%s", target_username);
    } else {
        strcpy(target_username, actor_username);
    }

    FILE *f_users = fopen(USERS_FILE, "r");
    if (f_users) {
        char role[20], username[30], password[30], name[50], email[50], phone[20];
        while (fscanf(f_users, "%s %s %s %s %s %s", role, username, password, name, email, phone) == 6) {
            if (strcmp(username, target_username) == 0) {
                strcpy(cust_name, name);
                strcpy(cust_email, email);
                strcpy(cust_phone, phone);
                user_found = 1;
                break;
            }
        }
        fclose(f_users);
    }

    if (!user_found) {
        center_print("[X] Customer not found.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    printf("\n");
    print_separator();
    center_print("[*] OFFICIAL INVOICE");
    print_separator();

    char customer_info[100];
    sprintf(customer_info, "Customer: %s", cust_name);
    center_print(customer_info);

    sprintf(customer_info, "Email: %s", cust_email);
    center_print(customer_info);

    sprintf(customer_info, "Phone: %s", cust_phone);
    center_print(customer_info);

    print_separator();

    // Show order details without clearing screen
    FILE *orders = fopen(ORDERS_FILE, "r");
    char user[30], part[50];
    int quantity;
    float total, sum = 0;

    if (orders) {
        printf("\n");
        center_print("[*] ORDER DETAILS:");
        printf("\n");
        printf("%*s", (CONSOLE_WIDTH-40)/2, "");
        printf("%-15s %-10s %s\n", "PART", "QUANTITY", "TOTAL ($)");
        printf("%*s", (CONSOLE_WIDTH-40)/2, "");
        for (int i = 0; i < 40; i++) printf("-");
        printf("\n");

        while (fscanf(orders, "%s %s %d %f", user, part, &quantity, &total) == 4) {
            if (strcmp(user, target_username) == 0) {
                printf("%*s", (CONSOLE_WIDTH-40)/2, "");
                printf("%-15s %-10d $%.2f\n", part, quantity, total);
                sum += total;
            }
        }
        fclose(orders);
    }

    float vat = sum * VAT_RATE;

    printf("\n");
    print_separator();
    char vat_msg[50], final_msg[50];
    sprintf(vat_msg, "VAT (%.0f%%): $%.2f", VAT_RATE * 100, vat);
    sprintf(final_msg, "TOTAL WITH VAT: $%.2f", sum + vat);

    center_print(vat_msg);
    center_print(final_msg);
    print_separator();

    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

/**
 * Car management functions
 */
void add_car() {
    clear_screen();
    display_ascii_logo();
    center_print("[*] ADD CAR TO GARAGE");
    print_separator();

    FILE *f = fopen(CARS_FILE, "a");
    if (!f) {
        center_print("[X] Error opening cars file.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    char number[20], time[20];

    printf("\n");
    center_prompt("Car Number: ");
    scanf("%s", number);

    center_prompt("Entry Time: ");
    scanf("%s", time);

    fprintf(f, "%s %s\n", number, time);
    fclose(f);

    printf("\n");
    center_print("[+] Car added to garage successfully!");
    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

void view_cars() {
    clear_screen();
    display_ascii_logo();
    center_print("[*] CARS IN GARAGE");
    print_separator();

    FILE *f = fopen(CARS_FILE, "r");
    if (!f) {
        center_print("[P] No cars in garage.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    char line[100];
    printf("\n");
    printf("%*s", (CONSOLE_WIDTH-30)/2, "");
    printf("%-15s %s\n", "CAR NUMBER", "ENTRY TIME");
    printf("%*s", (CONSOLE_WIDTH-30)/2, "");
    for (int i = 0; i < 30; i++) printf("-");
    printf("\n");

    while (fgets(line, sizeof(line), f)) {
        printf("%*s", (CONSOLE_WIDTH-30)/2, "");
        printf("%s", line);
    }
    fclose(f);

    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

void delete_car() {
    clear_screen();
    display_ascii_logo();
    center_print("[-] REMOVE CAR FROM GARAGE");
    print_separator();

    FILE *f = fopen(CARS_FILE, "r");
    if (!f) {
        center_print("[P] No cars in garage.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    FILE *temp = fopen("temp_cars.txt", "w");
    if (!temp) {
        center_print("[X] Error creating temporary file.");
        fclose(f);
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    char number[20], time[20], target[20];
    int found = 0;

    printf("\n");
    center_prompt("Car number to remove: ");
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
    remove(CARS_FILE);
    rename("temp_cars.txt", CARS_FILE);

    printf("\n");
    if (found)
        center_print("[+] Car removed from garage successfully!");
    else
        center_print("[X] Car not found.");

    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

void modify_car() {
    clear_screen();
    display_ascii_logo();
    center_print("[*] MODIFY CAR ENTRY");
    print_separator();

    FILE *f = fopen(CARS_FILE, "r");
    if (!f) {
        center_print("[P] No cars in garage.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    FILE *temp = fopen("temp_cars.txt", "w");
    if (!temp) {
        center_print("[X] Error creating temporary file.");
        fclose(f);
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    char number[20], time[20], target[20];
    int found = 0;

    printf("\n");
    center_prompt("Car number to modify: ");
    scanf("%s", target);

    while (fscanf(f, "%s %s", number, time) == 2) {
        if (strcmp(number, target) == 0) {
            center_prompt("New entry time: ");
            scanf("%s", time);
            found = 1;
        }
        fprintf(temp, "%s %s\n", number, time);
    }

    fclose(f);
    fclose(temp);
    remove(CARS_FILE);
    rename("temp_cars.txt", CARS_FILE);

    printf("\n");
    if (found)
        center_print("[+] Car entry modified successfully!");
    else
        center_print("[X] Car not found.");

    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

/**
 * Admin panel functions
 */
void view_all_users() {
    clear_screen();
    display_ascii_logo();
    center_print("[#] ALL REGISTERED USERS");
    print_separator();

    FILE *f = fopen(USERS_FILE, "r");
    if (!f) {
        center_print("[-] No users found.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    char role[20], username[30], password[30], name[50], email[50], phone[20];
    printf("\n");
    printf("%*s", (CONSOLE_WIDTH-70)/2, "");
    printf("%-12s %-15s %-20s %-15s %s\n", "ROLE", "USERNAME", "NAME", "EMAIL", "PHONE");
    printf("%*s", (CONSOLE_WIDTH-70)/2, "");
    for (int i = 0; i < 70; i++) printf("-");
    printf("\n");

    while (fscanf(f, "%s %s %s %s %s %s", role, username, password, name, email, phone) == 6) {
        printf("%*s", (CONSOLE_WIDTH-70)/2, "");
        printf("%-12s %-15s %-20s %-15s %s\n", role, username, name, email, phone);
    }
    fclose(f);

    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

void view_all_orders() {
    clear_screen();
    display_ascii_logo();
    center_print("[*] ALL CUSTOMER ORDERS");
    print_separator();

    FILE *f = fopen(ORDERS_FILE, "r");
    if (!f) {
        center_print("[-] No orders found.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    char user[30], part[50], payment[20], date_time[100];
    int quantity;
    float total;

    printf("\n");
    printf("%*s", (CONSOLE_WIDTH-80)/2, "");
    printf("%-12s %-12s %-4s %-8s %-8s %s\n", "CUSTOMER", "PART", "QTY", "TOTAL($)", "PAYMENT", "DATE");
    printf("%*s", (CONSOLE_WIDTH-80)/2, "");
    for (int i = 0; i < 80; i++) printf("-");
    printf("\n");

    // Try new format first
    while (fscanf(f, "%s %s %d %f %s %[^\n]", user, part, &quantity, &total, payment, date_time) == 6) {
        printf("%*s", (CONSOLE_WIDTH-80)/2, "");
        printf("%-12s %-12s %-4d $%-7.2f %-8s %s\n", user, part, quantity, total, payment, date_time);
    }

    // If no new format found, try old format
    long pos = ftell(f);
    if (pos == 0) { // File pointer hasn't moved, no new format orders
        fseek(f, 0, SEEK_SET);
        while (fscanf(f, "%s %s %d %f", user, part, &quantity, &total) == 4) {
            printf("%*s", (CONSOLE_WIDTH-80)/2, "");
            printf("%-12s %-12s %-4d $%-7.2f %-8s %s\n", user, part, quantity, total, "Cash", "Legacy Order");
        }
    }

    fclose(f);

    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

void system_statistics() {
    clear_screen();
    display_ascii_logo();
    center_print("[*] SYSTEM STATISTICS");
    print_separator();

    // Count users
    FILE *f = fopen(USERS_FILE, "r");
    int total_users = 0, customers = 0, admins = 0;
    if (f) {
        char role[20], username[30], password[30], name[50], email[50], phone[20];
        while (fscanf(f, "%s %s %s %s %s %s", role, username, password, name, email, phone) == 6) {
            total_users++;
            if (strcmp(role, "Customer") == 0) customers++;
            else if (strcmp(role, "Admin") == 0) admins++;
        }
        fclose(f);
    }

    // Count parts
    f = fopen(PARTS_FILE, "r");
    int total_parts = 0;
    if (f) {
        char part[50], spec[50];
        float price;
        while (fscanf(f, "%s %s %f", part, spec, &price) == 3) {
            total_parts++;
        }
        fclose(f);
    }

    // Count orders and payment statistics
    f = fopen(ORDERS_FILE, "r");
    int total_orders = 0, cash_payments = 0, online_payments = 0;
    float total_revenue = 0, cash_revenue = 0, online_revenue = 0;
    if (f) {
        char user[30], part[50], payment[20], date_time[100];
        int quantity;
        float total;

        // Try new format first (with payment and date)
        while (fscanf(f, "%s %s %d %f %s %[^\n]", user, part, &quantity, &total, payment, date_time) == 6) {
            total_orders++;
            total_revenue += total;

            if (strcmp(payment, "Cash") == 0) {
                cash_payments++;
                cash_revenue += total;
            } else if (strcmp(payment, "Online") == 0) {
                online_payments++;
                online_revenue += total;
            }
        }

        // If no new format orders found, try old format
        if (total_orders == 0) {
            fseek(f, 0, SEEK_SET); // Reset file pointer
            while (fscanf(f, "%s %s %d %f", user, part, &quantity, &total) == 4) {
                total_orders++;
                total_revenue += total;
                cash_payments++; // Old orders default to cash
                cash_revenue += total;
            }
        }
        fclose(f);
    }

    // Count cars
    f = fopen(CARS_FILE, "r");
    int total_cars = 0;
    if (f) {
        char line[100];
        while (fgets(line, sizeof(line), f)) {
            total_cars++;
        }
        fclose(f);
    }

    printf("\n");
    char stat_msg[100];

    sprintf(stat_msg, "[#] Total Users: %d", total_users);
    center_print(stat_msg);
    sprintf(stat_msg, "   * Customers: %d", customers);
    center_print(stat_msg);
    sprintf(stat_msg, "   * Admins: %d", admins);
    center_print(stat_msg);

    printf("\n");
    sprintf(stat_msg, "[*] Total Parts in Inventory: %d", total_parts);
    center_print(stat_msg);
    sprintf(stat_msg, "[*] Total Orders: %d", total_orders);
    center_print(stat_msg);
    sprintf(stat_msg, "[$] Total Revenue: $%.2f", total_revenue);
    center_print(stat_msg);

    printf("\n");
    center_print("[*] PAYMENT STATISTICS:");
    sprintf(stat_msg, "   * Cash Payments: %d (Revenue: $%.2f)", cash_payments, cash_revenue);
    center_print(stat_msg);
    sprintf(stat_msg, "   * Online Payments: %d (Revenue: $%.2f)", online_payments, online_revenue);
    center_print(stat_msg);

    sprintf(stat_msg, "[*] Cars in Garage: %d", total_cars);
    center_print(stat_msg);

    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

void admin_panel(const char *admin_username) {
    int choice;

    while (1) {
        clear_screen_with_simple_border();
        display_ascii_logo();

        char welcome_msg[100];
        sprintf(welcome_msg, "🔐 Welcome, Administrator: %s", admin_username);

        const char* admin_content[] = {
            welcome_msg,
            "System Administration Dashboard - Full Access",
            "",
            "1.   [*]  Daily Dashboard",
            "2.   [#]  View All Users",
            "3.   [*]  Manage Inventory",
            "4.   [*]  View All Orders",
            "5.   [*]  Generate Customer Invoice",
            "6.   [*]  Car Management",
            "7.   [%]  Manage Discounts",
            "8.   [*]  Appointment Management",
            "9.   [X]  View Out of Stock Parts",
            "10.  [*]  View Car Parking Records",
            "11.  [*]  System Statistics",
            "12.  [X]  Return to Main Menu"
        };

        print_simple_box("🛠️ ADMIN CONTROL PANEL 🛠️", admin_content, 15);

        printf("\n");
        center_prompt("🔹 Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: daily_dashboard(); break;
            case 2: view_all_users(); break;
            case 3: {
                int inv_choice;
                clear_screen();
                display_ascii_logo();
                center_print("[*] INVENTORY MANAGEMENT");
                print_separator();
                center_print("1.  [+]  Add Part");
                center_print("2.  [-]  Delete Part");
                center_print("3.  [*]  View Parts");
                center_print("4.  [<]  Back");
                printf("\n");
                center_prompt("Enter choice: ");
                scanf("%d", &inv_choice);

                switch (inv_choice) {
                    case 1: add_part(); break;
                    case 2: delete_part(); break;
                    case 3: view_parts(); break;
                    case 4: break;
                    default:
                        center_print("[X] Invalid choice.");
                        printf("\n");
                        center_print("Press any key to continue...");
                        getchar(); getchar();
                }
                break;
            }
            case 4: view_all_orders(); break;
            case 5: generate_invoice(admin_username, "Admin"); break;
            case 6: {
                int car_choice;
                clear_screen();
                display_ascii_logo();
                center_print("[*] CAR MANAGEMENT");
                print_separator();
                center_print("1.  [+]  Add Car");
                center_print("2.  [*]  View Cars");
                center_print("3.  [-]  Remove Car");
                center_print("4.  [*]  Modify Car");
                center_print("5.  [<]  Back");
                printf("\n");
                center_prompt("Enter choice: ");
                scanf("%d", &car_choice);

                switch (car_choice) {
                    case 1: add_car(); break;
                    case 2: view_cars(); break;
                    case 3: delete_car(); break;
                    case 4: modify_car(); break;
                    case 5: break;
                    default:
                        center_print("[X] Invalid choice.");
                        printf("\n");
                        center_print("Press any key to continue...");
                        getchar(); getchar();
                }
                break;
            }
            case 7: manage_discounts(); break;
            case 8: manage_appointments(); break;
            case 9: view_not_available_parts(); break;
            case 10: view_car_parking(); break;
            case 11: system_statistics(); break;
            case 12:
                center_print("[*] Admin logged out successfully.");
                printf("\n");
                center_print("[*] Thank you for using Smart Garage Management System!");
                center_print("Goodbye!");
                printf("\n");
                center_print("Press any key to continue...");
                getchar(); getchar();
                return;
            default:
                center_print("[X] Invalid choice.");
                printf("\n");
                center_print("Press any key to continue...");
                getchar(); getchar();
        }
    }
}

/**
 * Initialize default discount codes
 */
void initialize_default_discounts() {
    FILE *f = fopen(DISCOUNTS_FILE, "r");
    if (f) {
        fclose(f);
        return; // File exists, don't overwrite
    }

    f = fopen(DISCOUNTS_FILE, "w");
    if (f) {
        fprintf(f, "OFF10 10.0\n");
        fprintf(f, "OFF25 25.0\n");
        fprintf(f, "OFF50 50.0\n");
        fprintf(f, "SAVE15 15.0\n");
        fprintf(f, "WELCOME20 20.0\n");
        fclose(f);
    }
}

/**
 * Apply promo code and return discounted amount
 */
float apply_promo_code(float total_amount) {
    char promo_code[20];

    printf("\n");
    center_prompt("Enter promo code (or 'SKIP' to skip): ");
    scanf("%s", promo_code);

    if (strcmp(promo_code, "SKIP") == 0 || strcmp(promo_code, "skip") == 0) {
        return total_amount;
    }

    FILE *f = fopen(DISCOUNTS_FILE, "r");
    if (!f) {
        center_print("[X] Discount system not available.");
        return total_amount;
    }

    char code[20];
    float discount_percent;
    int found = 0;

    while (fscanf(f, "%s %f", code, &discount_percent) == 2) {
        if (strcmp(code, promo_code) == 0) {
            found = 1;
            break;
        }
    }
    fclose(f);

    if (found) {
        float discount_amount = total_amount * (discount_percent / 100.0f);
        float final_amount = total_amount - discount_amount;

        char discount_msg[100];
        sprintf(discount_msg, "[*] Promo Code Applied: %s (%.0f%% OFF)", promo_code, discount_percent);
        center_print(discount_msg);

        sprintf(discount_msg, "[*] Discount Amount: $%.2f", discount_amount);
        center_print(discount_msg);

        sprintf(discount_msg, "[$] Final Amount: $%.2f", final_amount);
        center_print(discount_msg);

        return final_amount;
    } else {
        center_print("[X] Invalid promo code! No discount applied.");
        return total_amount;
    }
}

/**
 * Add new discount code
 */
void add_discount() {
    clear_screen();
    display_ascii_logo();
    center_print("[+] ADD NEW DISCOUNT CODE");
    print_separator();

    FILE *f = fopen(DISCOUNTS_FILE, "a");
    if (!f) {
        center_print("[X] Error opening discounts file.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    char code[20];
    float percent;

    printf("\n");
    center_prompt("Promo Code: ");
    scanf("%s", code);

    center_prompt("Discount Percentage (0-100): ");
    scanf("%f", &percent);

    if (percent < 0 || percent > 100) {
        center_print("[X] Invalid discount percentage! Must be 0-100.");
        fclose(f);
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    fprintf(f, "%s %.1f\n", code, percent);
    fclose(f);

    printf("\n");
    center_print("[+] Discount code added successfully!");
    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

/**
 * View all discount codes
 */
void view_discounts() {
    clear_screen();
    display_ascii_logo();
    center_print("[*] AVAILABLE DISCOUNT CODES");
    print_separator();

    FILE *f = fopen(DISCOUNTS_FILE, "r");
    if (!f) {
        center_print("[-] No discount codes found.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    char code[20];
    float percent;

    printf("\n");
    printf("%*s", (CONSOLE_WIDTH-30)/2, "");
    printf("%-15s %s\n", "PROMO CODE", "DISCOUNT (%)");
    printf("%*s", (CONSOLE_WIDTH-30)/2, "");
    for (int i = 0; i < 30; i++) printf("-");
    printf("\n");

    while (fscanf(f, "%s %f", code, &percent) == 2) {
        printf("%*s", (CONSOLE_WIDTH-30)/2, "");
        printf("%-15s %.1f%%\n", code, percent);
    }
    fclose(f);

    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

/**
 * Delete discount code
 */
void delete_discount() {
    clear_screen();
    display_ascii_logo();
    center_print("[-] DELETE DISCOUNT CODE");
    print_separator();

    FILE *f = fopen(DISCOUNTS_FILE, "r");
    if (!f) {
        center_print("[-] No discount codes found.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    FILE *temp = fopen("temp_discounts.txt", "w");
    if (!temp) {
        center_print("[X] Error creating temporary file.");
        fclose(f);
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    char target_code[20], code[20];
    float percent;
    int found = 0;

    printf("\n");
    center_prompt("Enter promo code to delete: ");
    scanf("%s", target_code);

    while (fscanf(f, "%s %f", code, &percent) == 2) {
        if (strcmp(code, target_code) != 0) {
            fprintf(temp, "%s %.1f\n", code, percent);
        } else {
            found = 1;
        }
    }

    fclose(f);
    fclose(temp);
    remove(DISCOUNTS_FILE);
    rename("temp_discounts.txt", DISCOUNTS_FILE);

    printf("\n");
    if (found)
        center_print("[+] Discount code deleted successfully!");
    else
        center_print("[X] Discount code not found.");

    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

/**
 * Manage discount codes (Admin only)
 */
void manage_discounts() {
    int choice;

    while (1) {
        clear_screen();
        display_ascii_logo();
        center_print("[%] DISCOUNT MANAGEMENT");
        print_separator();

        center_print("1.  [+]  Add Discount Code");
        center_print("2.  [*]  View All Discounts");
        center_print("3.  [-]  Delete Discount Code");
        center_print("4.  [<]  Back to Admin Panel");

        printf("\n");
        printf("%*s", (CONSOLE_WIDTH-20)/2, "");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: add_discount(); break;
            case 2: view_discounts(); break;
            case 3: delete_discount(); break;
            case 4: return;
            default:
                center_print("[X] Invalid choice.");
                printf("\n");
                center_print("Press any key to continue...");
                getchar(); getchar();
        }
    }
}

/**
 * Add new mechanic
 */
void add_mechanic() {
    clear_screen();
    display_ascii_logo();
    center_print("[+] ADD NEW MECHANIC");
    print_separator();

    FILE *f = fopen(MECHANICS_FILE, "a");
    if (!f) {
        center_print("[X] Error opening mechanics file.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    char name[50], phone[20], username[30];
    int age;

    printf("\n");
    printf("%*s", (CONSOLE_WIDTH-25)/2, "");
    printf("Mechanic Name: ");
    scanf("%s", name);

    printf("%*s", (CONSOLE_WIDTH-25)/2, "");
    printf("Username: ");
    scanf("%s", username);

    printf("%*s", (CONSOLE_WIDTH-20)/2, "");
    printf("Age: ");
    scanf("%d", &age);

    // Validate and get phone
    while (1) {
        printf("%*s", (CONSOLE_WIDTH-30)/2, "");
        printf("Phone (01XXXXXXXXX): ");
        scanf("%s", phone);

        if (validate_phone(phone)) {
            break;
        } else {
            center_print("[X] Invalid phone number!");
            center_print("Must start with 01 and be exactly 11 digits.");
        }
    }

    fprintf(f, "%s %s %d %s\n", name, username, age, phone);
    fclose(f);

    printf("\n");
    center_print("[+] Mechanic added successfully!");
    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

/**
 * View all mechanics
 */
void view_mechanics() {
    clear_screen();
    display_ascii_logo();
    center_print("[*] AVAILABLE MECHANICS");
    print_separator();

    FILE *f = fopen(MECHANICS_FILE, "r");
    if (!f) {
        center_print("[-] No mechanics found.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    char name[50], phone[20], username[30];
    int age;

    printf("\n");
    printf("%*s", (CONSOLE_WIDTH-60)/2, "");
    printf("%-20s %-15s %-5s %s\n", "MECHANIC NAME", "USERNAME", "AGE", "PHONE");
    printf("%*s", (CONSOLE_WIDTH-60)/2, "");
    for (int i = 0; i < 60; i++) printf("-");
    printf("\n");

    while (fscanf(f, "%s %s %d %s", name, username, &age, phone) == 4) {
        printf("%*s", (CONSOLE_WIDTH-60)/2, "");
        printf("%-20s %-15s %-5d %s\n", name, username, age, phone);
    }
    fclose(f);

    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

/**
 * Delete mechanic
 */
void delete_mechanic() {
    clear_screen();
    display_ascii_logo();
    center_print("[-] DELETE MECHANIC");
    print_separator();

    FILE *f = fopen(MECHANICS_FILE, "r");
    if (!f) {
        center_print("[-] No mechanics found.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    FILE *temp = fopen("temp_mechanics.txt", "w");
    if (!temp) {
        center_print("[X] Error creating temporary file.");
        fclose(f);
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    char target_name[50], name[50], phone[20];
    int age, found = 0;

    printf("\n");
    printf("%*s", (CONSOLE_WIDTH-30)/2, "");
    printf("Enter mechanic name to delete: ");
    scanf("%s", target_name);

    while (fscanf(f, "%s %d %s", name, &age, phone) == 3) {
        if (strcmp(name, target_name) != 0) {
            fprintf(temp, "%s %d %s\n", name, age, phone);
        } else {
            found = 1;
        }
    }

    fclose(f);
    fclose(temp);
    remove(MECHANICS_FILE);
    rename("temp_mechanics.txt", MECHANICS_FILE);

    printf("\n");
    if (found)
        center_print("[+] Mechanic deleted successfully!");
    else
        center_print("[X] Mechanic not found.");

    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

/**
 * Assign mechanic to customer order
 */
void assign_mechanic_to_order() {
    clear_screen();
    display_ascii_logo();
    center_print("[*] ASSIGN MECHANIC TO ORDER");
    print_separator();

    // First show all orders
    FILE *orders = fopen(ORDERS_FILE, "r");
    if (!orders) {
        center_print("[-] No orders found.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    printf("\n");
    center_print("[*] AVAILABLE ORDERS:");
    printf("\n");
    printf("%*s", (CONSOLE_WIDTH-60)/2, "");
    printf("%-15s %-15s %-10s %s\n", "CUSTOMER", "PART", "QUANTITY", "TOTAL ($)");
    printf("%*s", (CONSOLE_WIDTH-60)/2, "");
    for (int i = 0; i < 60; i++) printf("-");
    printf("\n");

    char user[30], part[50];
    int quantity;
    float total;
    int order_count = 0;

    while (fscanf(orders, "%s %s %d %f", user, part, &quantity, &total) == 4) {
        order_count++;
        printf("%*s", (CONSOLE_WIDTH-60)/2, "");
        printf("%-15s %-15s %-10d $%.2f\n", user, part, quantity, total);
    }
    fclose(orders);

    if (order_count == 0) {
        center_print("[-] No orders available for assignment.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    // Show available mechanics
    printf("\n");
    center_print("[*] AVAILABLE MECHANICS:");

    FILE *mechanics_display = fopen(MECHANICS_FILE, "r");
    if (mechanics_display) {
        char mech_name_display[50], mech_username_display[30], mech_phone_display[20];
        int mech_age_display;

        printf("\n");
        printf("%*s", (CONSOLE_WIDTH-60)/2, "");
        printf("%-20s %-15s %-5s %s\n", "MECHANIC NAME", "USERNAME", "AGE", "PHONE");
        printf("%*s", (CONSOLE_WIDTH-60)/2, "");
        for (int i = 0; i < 60; i++) printf("-");
        printf("\n");

        while (fscanf(mechanics_display, "%s %s %d %s", mech_name_display, mech_username_display, &mech_age_display, mech_phone_display) == 4) {
            printf("%*s", (CONSOLE_WIDTH-60)/2, "");
            printf("%-20s %-15s %-5d %s\n", mech_name_display, mech_username_display, mech_age_display, mech_phone_display);
        }
        fclose(mechanics_display);
    }

    // Get assignment details
    char customer[30], part_name[50], mechanic_username[30];
    printf("\n");
    printf("%*s", (CONSOLE_WIDTH-25)/2, "");
    printf("Customer username: ");
    scanf("%s", customer);

    printf("%*s", (CONSOLE_WIDTH-20)/2, "");
    printf("Part name: ");
    scanf("%s", part_name);

    printf("%*s", (CONSOLE_WIDTH-25)/2, "");
    printf("Mechanic username: ");
    scanf("%s", mechanic_username);

    // Verify mechanic exists
    FILE *mechanics = fopen(MECHANICS_FILE, "r");
    if (!mechanics) {
        center_print("[X] No mechanics available.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    char mech_name[50], mech_username[30], mech_phone[20];
    int mech_age, mechanic_found = 0;

    while (fscanf(mechanics, "%s %s %d %s", mech_name, mech_username, &mech_age, mech_phone) == 4) {
        if (strcmp(mech_username, mechanic_username) == 0) {
            mechanic_found = 1;
            break;
        }
    }
    fclose(mechanics);

    if (!mechanic_found) {
        center_print("[X] Mechanic not found!");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    // Verify order exists
    orders = fopen(ORDERS_FILE, "r");
    int order_found = 0;

    while (fscanf(orders, "%s %s %d %f", user, part, &quantity, &total) == 4) {
        if (strcmp(user, customer) == 0 && strcmp(part, part_name) == 0) {
            order_found = 1;
            break;
        }
    }
    fclose(orders);

    if (!order_found) {
        center_print("[X] Order not found!");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    // Save assignment
    FILE *appointments = fopen(APPOINTMENTS_FILE, "a");
    if (!appointments) {
        center_print("[X] Error opening appointments file.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    // Get current date/time for appointment
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    char date[20];
    sprintf(date, "%04d-%02d-%02d",
            local->tm_year + 1900, local->tm_mon + 1, local->tm_mday);

    fprintf(appointments, "%s %s %s %s\n", customer, part_name, mechanic_username, date);
    fclose(appointments);

    printf("\n");
    center_print("[+] Mechanic assigned successfully!");
    char assignment_msg[100];
    sprintf(assignment_msg, "[*] %s assigned to %s's %s order", mechanic_username, customer, part_name);
    center_print(assignment_msg);

    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

/**
 * View all appointments/assignments
 */
void view_appointments() {
    clear_screen();
    display_ascii_logo();
    center_print("[*] MECHANIC ASSIGNMENTS");
    print_separator();

    FILE *f = fopen(APPOINTMENTS_FILE, "r");
    if (!f) {
        center_print("[-] No assignments found.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    char customer[30], part[50], mechanic[50], date[20];

    printf("\n");
    printf("%*s", (CONSOLE_WIDTH-70)/2, "");
    printf("%-15s %-15s %-20s %s\n", "CUSTOMER", "PART", "MECHANIC", "DATE");
    printf("%*s", (CONSOLE_WIDTH-70)/2, "");
    for (int i = 0; i < 70; i++) printf("-");
    printf("\n");

    while (fscanf(f, "%s %s %s %s", customer, part, mechanic, date) == 4) {
        printf("%*s", (CONSOLE_WIDTH-70)/2, "");
        printf("%-15s %-15s %-20s %s\n", customer, part, mechanic, date);
    }
    fclose(f);

    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

/**
 * Manage appointments (Admin only)
 */
void manage_appointments() {
    int choice;

    while (1) {
        clear_screen();
        display_ascii_logo();
        center_print("[*] APPOINTMENT MANAGEMENT");
        print_separator();

        center_print("1.  [+]  Add Mechanic");
        center_print("2.  [*]  View All Mechanics");
        center_print("3.  [-]  Delete Mechanic");
        center_print("4.  [*]  Assign Mechanic to Order");
        center_print("5.  [*]  View All Assignments");
        center_print("6.  [<]  Back to Admin Panel");

        printf("\n");
        printf("%*s", (CONSOLE_WIDTH-20)/2, "");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: add_mechanic(); break;
            case 2: view_mechanics(); break;
            case 3: delete_mechanic(); break;
            case 4: assign_mechanic_to_order(); break;
            case 5: view_appointments(); break;
            case 6: return;
            default:
                center_print("[X] Invalid choice.");
                printf("\n");
                center_print("Press any key to continue...");
                getchar(); getchar();
        }
    }
}

/**
 * Mechanic Dashboard
 */
void mechanic_dashboard(const char *mechanic_username) {
    int choice;

    while (1) {
        clear_screen();
        display_ascii_logo();
        char welcome_msg[100];
        sprintf(welcome_msg, "Welcome, %s (Mechanic)", mechanic_username);
        center_print(welcome_msg);
        center_print("[*] MECHANIC DASHBOARD");
        print_separator();

        center_print("1.  [*]  View My Assigned Cars");
        center_print("2.  [*]  Car Profile Management");
        center_print("3.  [*]  Vehicle History");
        center_print("4.  [*]  Update Progress");
        center_print("5.  [*]  Set/View Deadlines");
        center_print("6.  [*]  Service Calendar");
        center_print("7.  [*]  Maintenance Reminders");
        center_print("8.  [*]  Manage Q&A");
        center_print("9.  [X]  Logout");

        printf("\n");
        printf("%*s", (CONSOLE_WIDTH-20)/2, "");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: view_assigned_cars(mechanic_username); break;
            case 2: view_car_profile(); break;
            case 3: view_vehicle_history(); break;
            case 4: update_progress(); break;
            case 5: {
                int deadline_choice;
                clear_screen();
                display_ascii_logo();
                center_print("[*] DEADLINE MANAGEMENT");
                print_separator();
                center_print("1.  [+]  Set New Deadline");
                center_print("2.  [*]  View All Deadlines");
                center_print("3.  [<]  Back");
                printf("\n");
                printf("%*s", (CONSOLE_WIDTH-20)/2, "");
                printf("Enter choice: ");
                scanf("%d", &deadline_choice);

                switch (deadline_choice) {
                    case 1: set_deadline(); break;
                    case 2: view_progress(); break; // This will show deadlines too
                    case 3: break;
                    default:
                        center_print("[X] Invalid choice.");
                        printf("\n");
                        center_print("Press any key to continue...");
                        getchar(); getchar();
                }
                break;
            }
            case 6: view_service_calendar(); break;
            case 7: {
                int reminder_choice;
                clear_screen();
                display_ascii_logo();
                center_print("[*] MAINTENANCE REMINDERS");
                print_separator();
                center_print("1.  [+]  Set Maintenance Reminder");
                center_print("2.  [*]  View All Reminders");
                center_print("3.  [<]  Back");
                printf("\n");
                printf("%*s", (CONSOLE_WIDTH-20)/2, "");
                printf("Enter choice: ");
                scanf("%d", &reminder_choice);

                switch (reminder_choice) {
                    case 1: set_maintenance_reminder(); break;
                    case 2: view_maintenance_reminders(); break;
                    case 3: break;
                    default:
                        center_print("[X] Invalid choice.");
                        printf("\n");
                        center_print("Press any key to continue...");
                        getchar(); getchar();
                }
                break;
            }
            case 8: manage_qna(); break;
            case 9:
                center_print("[*] Logged out successfully.");
                printf("\n");
                center_print("[*] Thank you for using Smart Garage Management System!");
                center_print("Goodbye!");
                printf("\n");
                center_print("Press any key to continue...");
                getchar(); getchar();
                return;
            default:
                center_print("[X] Invalid choice. Please try again.");
                printf("\n");
                center_print("Press any key to continue...");
                getchar(); getchar();
        }
    }
}

/**
 * Main menu system
 */
void main_menu() {
    char logged_in_user[30], logged_in_role[20];
    int choice, logged_in = 0;

    while (1) {
        if (!logged_in) {
            clear_screen_with_simple_border();
            display_ascii_logo();

            // Create main menu with simple bordered box
            const char* main_menu_content[] = {
                "🚗 Welcome to Smart Garage Management System 🚗",
                "",
                "1.  [+]  Register (Customer)",
                "2.  [#]  User Login",
                "3.  [#]  Admin Login",
                "4.  [#]  Mechanic Login",
                "5.  [X]  Exit System"
            };

            print_simple_box("🔧 MAIN MENU - PARKIFY GARAGE SYSTEM 🔧", main_menu_content, 7);

            printf("\n");
            center_prompt("🔹 Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    register_user();
                    break;
                case 2:
                    if (login_user(logged_in_user, logged_in_role)) {
                        logged_in = 1;
                    }
                    break;
                case 3:
                    if (admin_login(logged_in_user, logged_in_role)) {
                        admin_panel(logged_in_user);
                    }
                    break;
                case 4:
                    if (mechanic_login(logged_in_user, logged_in_role)) {
                        mechanic_dashboard(logged_in_user);
                    }
                    break;
                case 5:
                    clear_screen_with_simple_border();
                    display_ascii_logo();

                    const char* goodbye_content[] = {
                        "🚗 Thank you for using Smart Garage Management System! 🚗",
                        "",
                        "💯 Your trusted partner for automotive excellence 💯",
                        "",
                        "🔧 Drive Safe, Service Smart! 🔧",
                        "",
                        "Have a wonderful day!"
                    };

                    print_simple_box("👋 GOODBYE FROM PARKIFY GARAGE 👋", goodbye_content, 7);

                    printf("\n");
                    center_prompt("🔹 Press any key to exit...");
                    getchar(); getchar();
                    return;
                default:
                    center_print("[X] Invalid choice. Please try again.");
                    printf("\n");
                    center_print("Press any key to continue...");
                    getchar(); getchar();
            }
        } else {
            // Customer menu
            clear_screen_with_simple_border();
            display_ascii_logo();

            char welcome_msg[100];
            sprintf(welcome_msg, "🎯 Welcome, %s (%s)", logged_in_user, logged_in_role);

            const char* customer_content[] = {
                welcome_msg,
                "",
                "1.   [*]  View Parts",
                "2.   [*]  Update Profile",
                "3.   [*]  Appointment Options",
                "4.   [$]  Calculate Estimation",
                "5.   [*]  Generate Invoice",
                "6.   [*]  Car Management",
                "7.   [*]  My Service Progress",
                "8.   [?]  Troubleshooting Q&A",
                "9.   [*]  Maintenance Reminders",
                "10.  [H]  Order History",
                "11.  [X]  Logout"
            };

            print_simple_box("🚗 CUSTOMER DASHBOARD 🚗", customer_content, 13);

            printf("\n");
            center_prompt("Enter choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1: view_parts(); break;
                case 2: update_profile(logged_in_user); break;
                case 3: {
                    // Appointment Options submenu
                    int appointment_choice;
                    while (1) {
                        clear_screen();
                        display_ascii_logo();
                        center_print("[*] APPOINTMENT OPTIONS");
                        print_separator();
                        center_print("1.  [*]  Order Parts");
                        center_print("2.  [*]  Car Parking");
                        center_print("3.  [<]  Back to Main Menu");

                        printf("\n");
                        center_prompt("Enter choice: ");
                        scanf("%d", &appointment_choice);

                        switch (appointment_choice) {
                            case 1:
                                order_parts(logged_in_user);
                                break;
                            case 2:
                                add_car_parking(logged_in_user);
                                break;
                            case 3:
                                goto exit_appointment_menu;
                            default:
                                center_print("[X] Invalid choice.");
                                printf("\n");
                                center_print("Press any key to continue...");
                                getchar(); getchar();
                        }
                    }
                    exit_appointment_menu:
                    break;
                }
                case 4: calculate_estimation(logged_in_user); break;
                case 5: generate_invoice(logged_in_user, logged_in_role); break;
                case 6: {
                    int car_choice;
                    clear_screen();
                    display_ascii_logo();
                    center_print("[*] CAR MANAGEMENT");
                    print_separator();
                    center_print("1.  [+]  Add Car");
                    center_print("2.  [*]  View Cars");
                    center_print("3.  [-]  Remove Car");
                    center_print("4.  [*]  Modify Car");
                    center_print("5.  [*]  View Parking Data");
                    center_print("6.  [<]  Back");
                    printf("\n");
                    center_prompt("Enter choice: ");
                    scanf("%d", &car_choice);

                    switch (car_choice) {
                        case 1: add_car(); break;
                        case 2: view_cars(); break;
                        case 3: delete_car(); break;
                        case 4: modify_car(); break;
                        case 5: view_car_parking(); break;
                        case 6: break;
                        default:
                            center_print("[X] Invalid choice.");
                            printf("\n");
                            center_print("Press any key to continue...");
                            getchar(); getchar();
                    }
                    break;
                }
                case 7: {
                    // Customer view progress
                    char car_number[20];
                    printf("\n");
                    printf("%*s", (CONSOLE_WIDTH-25)/2, "");
                    printf("Enter your car number: ");
                    scanf("%s", car_number);

                    // Show progress
                    clear_screen();
                    display_ascii_logo();
                    center_print("[*] MY SERVICE PROGRESS");
                    print_separator();

                    FILE *f = fopen(PROGRESS_FILE, "r");
                    if (f) {
                        char line[300], stored_car[20], date[20], percentage[10], description[100];
                        int found = 0;

                        printf("\n");
                        center_print("=== SERVICE PROGRESS ===");
                        while (fgets(line, sizeof(line), f)) {
                            sscanf(line, "%s %s %s %s", stored_car, date, percentage, description);
                            if (strcmp(stored_car, car_number) == 0) {
                                printf("%*s", (CONSOLE_WIDTH-50)/2, "");
                                printf("%s - %s: %s\n", date, percentage, description);
                                found = 1;
                            }
                        }

                        if (!found) {
                            center_print("[!] No service records found for this car.");
                        }
                        fclose(f);
                    } else {
                        center_print("[!] No progress data available.");
                    }

                    // Show deadlines
                    printf("\n");
                    center_print("=== SERVICE DEADLINES ===");
                    f = fopen(DEADLINES_FILE, "r");
                    if (f) {
                        char line[200], stored_car[20], deadline[20], service[50];
                        int found = 0;

                        while (fgets(line, sizeof(line), f)) {
                            sscanf(line, "%s %s %s", stored_car, deadline, service);
                            if (strcmp(stored_car, car_number) == 0) {
                                printf("%*s", (CONSOLE_WIDTH-40)/2, "");
                                printf("%s: %s\n", service, deadline);
                                found = 1;
                            }
                        }

                        if (!found) {
                            center_print("[!] No deadlines set for this car.");
                        }
                        fclose(f);
                    }

                    printf("\n");
                    center_print("Press any key to continue...");
                    getchar(); getchar();
                    break;
                }
                case 8:
                    customer_view_qna();
                    break;
                case 9:
                    customer_view_maintenance_reminders();
                    break;
                case 10:
                    view_order_history(logged_in_user);
                    break;
                case 11:
                    logged_in = 0;
                    center_print("[*] Logged out successfully.");
                    printf("\n");
                    center_print("[*] Thank you for using Smart Garage Management System!");
                    center_print("Goodbye!");
                    printf("\n");
                    center_print("Press any key to continue...");
                    getchar(); getchar();
                    break;
                default:
                    center_print("[X] Invalid choice. Please try again.");
                    printf("\n");
                    center_print("Press any key to continue...");
                    getchar(); getchar();
            }
        }
    }
}

// ==================== MECHANIC DASHBOARD FUNCTIONS ====================

/**
 * View cars assigned to specific mechanic
 */
void view_assigned_cars(const char *mechanic_username) {
    clear_screen();
    display_ascii_logo();
    center_print("[*] MY ASSIGNED CARS");
    print_separator();

    FILE *f = fopen(APPOINTMENTS_FILE, "r");
    if (!f) {
        center_print("[!] No assignments found.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    char line[200], assigned_mechanic[50], car_number[20], order_id[20];
    int found = 0;

    printf("\n");
    printf("%*s", (CONSOLE_WIDTH-50)/2, "");
    printf("%-15s %-15s %s\n", "ORDER ID", "CAR NUMBER", "ASSIGNED TO");
    printf("%*s", (CONSOLE_WIDTH-50)/2, "");
    for (int i = 0; i < 50; i++) printf("-");
    printf("\n");

    while (fgets(line, sizeof(line), f)) {
        sscanf(line, "%s %s %s", order_id, car_number, assigned_mechanic);
        if (strcmp(assigned_mechanic, mechanic_username) == 0) {
            printf("%*s", (CONSOLE_WIDTH-50)/2, "");
            printf("%-15s %-15s %s\n", order_id, car_number, assigned_mechanic);
            found = 1;
        }
    }

    if (!found) {
        center_print("[!] No cars assigned to you.");
    }

    fclose(f);
    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

/**
 * View and manage car profiles
 */
void view_car_profile() {
    clear_screen();
    display_ascii_logo();
    center_print("[*] CAR PROFILE MANAGEMENT");
    print_separator();

    int choice;
    center_print("1.  [*]  View Car Profile");
    center_print("2.  [+]  Add Car Profile");
    center_print("3.  [<]  Back");

    printf("\n");
    printf("%*s", (CONSOLE_WIDTH-20)/2, "");
    printf("Enter choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1: {
            char car_number[20];
            printf("\n");
            printf("%*s", (CONSOLE_WIDTH-25)/2, "");
            printf("Enter Car Number: ");
            scanf("%s", car_number);

            FILE *f = fopen(CAR_PROFILES_FILE, "r");
            if (!f) {
                center_print("[!] No car profiles found.");
                printf("\n");
                center_print("Press any key to continue...");
                getchar(); getchar();
                return;
            }

            char line[300], stored_car[20], owner[50], model[50], engine[50];
            int found = 0;

            while (fgets(line, sizeof(line), f)) {
                sscanf(line, "%s %s %s %s", stored_car, owner, model, engine);
                if (strcmp(stored_car, car_number) == 0) {
                    printf("\n");
                    center_print("=== CAR PROFILE ===");
                    printf("%*s", (CONSOLE_WIDTH-40)/2, "");
                    printf("Car Number: %s\n", stored_car);
                    printf("%*s", (CONSOLE_WIDTH-40)/2, "");
                    printf("Owner: %s\n", owner);
                    printf("%*s", (CONSOLE_WIDTH-40)/2, "");
                    printf("Model: %s\n", model);
                    printf("%*s", (CONSOLE_WIDTH-40)/2, "");
                    printf("Engine: %s\n", engine);
                    found = 1;
                    break;
                }
            }

            if (!found) {
                center_print("[!] Car profile not found.");
            }

            fclose(f);
            break;
        }
        case 2: {
            char car_number[20];
            printf("\n");
            printf("%*s", (CONSOLE_WIDTH-25)/2, "");
            printf("Enter Car Number: ");
            scanf("%s", car_number);
            add_car_profile(car_number);
            break;
        }
        case 3:
            return;
        default:
            center_print("[X] Invalid choice.");
    }

    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

/**
 * Add car profile
 */
void add_car_profile(const char *car_number) {
    FILE *f = fopen(CAR_PROFILES_FILE, "a");
    if (!f) {
        center_print("[X] Error opening car profiles file.");
        return;
    }

    char owner[50], model[50], engine[50];

    printf("\n");
    printf("%*s", (CONSOLE_WIDTH-25)/2, "");
    printf("Owner Name: ");
    scanf("%s", owner);

    printf("%*s", (CONSOLE_WIDTH-20)/2, "");
    printf("Car Model: ");
    scanf("%s", model);

    printf("%*s", (CONSOLE_WIDTH-25)/2, "");
    printf("Engine Number: ");
    scanf("%s", engine);

    fprintf(f, "%s %s %s %s\n", car_number, owner, model, engine);
    fclose(f);

    center_print("[+] Car profile added successfully!");
}

/**
 * View vehicle service history
 */
void view_vehicle_history() {
    clear_screen();
    display_ascii_logo();
    center_print("[*] VEHICLE SERVICE HISTORY");
    print_separator();

    char car_number[20];
    printf("\n");
    printf("%*s", (CONSOLE_WIDTH-25)/2, "");
    printf("Enter Car Number: ");
    scanf("%s", car_number);

    FILE *f = fopen(VEHICLE_HISTORY_FILE, "r");
    if (!f) {
        center_print("[!] No service history found.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    char line[300], stored_car[20], date[20], service[100], problem[100];
    int found = 0;

    printf("\n");
    center_print("=== SERVICE HISTORY ===");
    printf("%*s", (CONSOLE_WIDTH-60)/2, "");
    printf("%-12s %-20s %s\n", "DATE", "SERVICE", "PROBLEM");
    printf("%*s", (CONSOLE_WIDTH-60)/2, "");
    for (int i = 0; i < 60; i++) printf("-");
    printf("\n");

    while (fgets(line, sizeof(line), f)) {
        sscanf(line, "%s %s %s %s", stored_car, date, service, problem);
        if (strcmp(stored_car, car_number) == 0) {
            printf("%*s", (CONSOLE_WIDTH-60)/2, "");
            printf("%-12s %-20s %s\n", date, service, problem);
            found = 1;
        }
    }

    if (!found) {
        center_print("[!] No service history for this car.");
    }

    fclose(f);
    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

/**
 * Update service progress
 */
void update_progress() {
    clear_screen();
    display_ascii_logo();
    center_print("[*] UPDATE SERVICE PROGRESS");
    print_separator();

    char car_number[20], progress[100];
    int percentage;

    printf("\n");
    printf("%*s", (CONSOLE_WIDTH-25)/2, "");
    printf("Car Number: ");
    scanf("%s", car_number);

    printf("%*s", (CONSOLE_WIDTH-30)/2, "");
    printf("Progress Percentage (0-100): ");
    scanf("%d", &percentage);

    printf("%*s", (CONSOLE_WIDTH-25)/2, "");
    printf("Progress Description: ");
    getchar(); // consume newline
    fgets(progress, sizeof(progress), stdin);
    progress[strcspn(progress, "\n")] = '\0'; // remove newline

    FILE *f = fopen(PROGRESS_FILE, "a");
    if (!f) {
        center_print("[X] Error opening progress file.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    char date[20];
    sprintf(date, "%04d-%02d-%02d", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday);

    fprintf(f, "%s %s %d%% %s\n", car_number, date, percentage, progress);
    fclose(f);

    center_print("[+] Progress updated successfully!");
    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

/**
 * View progress and deadlines
 */
void view_progress() {
    clear_screen();
    display_ascii_logo();
    center_print("[*] SERVICE PROGRESS & DEADLINES");
    print_separator();

    char car_number[20];
    printf("\n");
    printf("%*s", (CONSOLE_WIDTH-25)/2, "");
    printf("Enter Car Number: ");
    scanf("%s", car_number);

    // Show Progress
    printf("\n");
    center_print("=== PROGRESS HISTORY ===");
    FILE *f = fopen(PROGRESS_FILE, "r");
    if (f) {
        char line[300], stored_car[20], date[20], percentage[10], description[100];
        int found = 0;

        while (fgets(line, sizeof(line), f)) {
            sscanf(line, "%s %s %s %s", stored_car, date, percentage, description);
            if (strcmp(stored_car, car_number) == 0) {
                printf("%*s", (CONSOLE_WIDTH-50)/2, "");
                printf("%s - %s: %s\n", date, percentage, description);
                found = 1;
            }
        }

        if (!found) {
            center_print("[!] No progress records found.");
        }
        fclose(f);
    }

    // Show Deadlines
    printf("\n");
    center_print("=== DEADLINES ===");
    f = fopen(DEADLINES_FILE, "r");
    if (f) {
        char line[200], stored_car[20], deadline[20], service[50];
        int found = 0;

        while (fgets(line, sizeof(line), f)) {
            sscanf(line, "%s %s %s", stored_car, deadline, service);
            if (strcmp(stored_car, car_number) == 0) {
                printf("%*s", (CONSOLE_WIDTH-40)/2, "");
                printf("%s: %s\n", service, deadline);
                found = 1;
            }
        }

        if (!found) {
            center_print("[!] No deadlines set.");
        }
        fclose(f);
    }

    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

/**
 * Set service deadline
 */
void set_deadline() {
    clear_screen();
    display_ascii_logo();
    center_print("[*] SET SERVICE DEADLINE");
    print_separator();

    char car_number[20], deadline[20], service[50];

    printf("\n");
    printf("%*s", (CONSOLE_WIDTH-25)/2, "");
    printf("Car Number: ");
    scanf("%s", car_number);

    printf("%*s", (CONSOLE_WIDTH-25)/2, "");
    printf("Service Type: ");
    scanf("%s", service);

    printf("%*s", (CONSOLE_WIDTH-35)/2, "");
    printf("Deadline (YYYY-MM-DD): ");
    scanf("%s", deadline);

    FILE *f = fopen(DEADLINES_FILE, "a");
    if (!f) {
        center_print("[X] Error opening deadlines file.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    fprintf(f, "%s %s %s\n", car_number, deadline, service);
    fclose(f);

    center_print("[+] Deadline set successfully!");
    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

/**
 * View service calendar
 */
void view_service_calendar() {
    clear_screen();
    display_ascii_logo();
    center_print("[*] SERVICE CALENDAR");
    print_separator();

    FILE *f = fopen(SERVICE_CALENDAR_FILE, "r");
    if (!f) {
        center_print("[!] No service events found.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    char line[200], date[20], car[20], service[50];

    printf("\n");
    printf("%*s", (CONSOLE_WIDTH-50)/2, "");
    printf("%-12s %-15s %s\n", "DATE", "CAR", "SERVICE");
    printf("%*s", (CONSOLE_WIDTH-50)/2, "");
    for (int i = 0; i < 50; i++) printf("-");
    printf("\n");

    while (fgets(line, sizeof(line), f)) {
        sscanf(line, "%s %s %s", date, car, service);
        printf("%*s", (CONSOLE_WIDTH-50)/2, "");
        printf("%-12s %-15s %s\n", date, car, service);
    }

    fclose(f);
    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

/**
 * Set maintenance reminder
 */
void set_maintenance_reminder() {
    clear_screen();
    display_ascii_logo();
    center_print("[*] SET MAINTENANCE REMINDER");
    print_separator();

    char car_number[20], reminder_date[20], service_type[50];

    printf("\n");
    printf("%*s", (CONSOLE_WIDTH-25)/2, "");
    printf("Car Number: ");
    scanf("%s", car_number);

    printf("%*s", (CONSOLE_WIDTH-25)/2, "");
    printf("Service Type: ");
    scanf("%s", service_type);

    printf("%*s", (CONSOLE_WIDTH-35)/2, "");
    printf("Reminder Date (YYYY-MM-DD): ");
    scanf("%s", reminder_date);

    FILE *f = fopen(MAINTENANCE_REMINDERS_FILE, "a");
    if (!f) {
        center_print("[X] Error opening reminders file.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    fprintf(f, "%s %s %s\n", car_number, reminder_date, service_type);
    fclose(f);

    center_print("[+] Maintenance reminder set successfully!");
    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

/**
 * View maintenance reminders
 */
void view_maintenance_reminders() {
    clear_screen();
    display_ascii_logo();
    center_print("[*] MAINTENANCE REMINDERS");
    print_separator();

    FILE *f = fopen(MAINTENANCE_REMINDERS_FILE, "r");
    if (!f) {
        center_print("[!] No reminders found.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    char line[200], car[20], date[20], service[50];

    printf("\n");
    printf("%*s", (CONSOLE_WIDTH-50)/2, "");
    printf("%-15s %-12s %s\n", "CAR", "DATE", "SERVICE");
    printf("%*s", (CONSOLE_WIDTH-50)/2, "");
    for (int i = 0; i < 50; i++) printf("-");
    printf("\n");

    while (fgets(line, sizeof(line), f)) {
        sscanf(line, "%s %s %s", car, date, service);
        printf("%*s", (CONSOLE_WIDTH-50)/2, "");
        printf("%-15s %-12s %s\n", car, date, service);
    }

    fclose(f);
    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

/**
 * Customer view maintenance reminders (filtered for alerts)
 */
void customer_view_maintenance_reminders() {
    clear_screen();
    display_ascii_logo();
    center_print("[*] MAINTENANCE ALERTS & REMINDERS");
    print_separator();

    FILE *f = fopen(MAINTENANCE_REMINDERS_FILE, "r");
    if (!f) {
        center_print("[*] No maintenance reminders at this time.");
        center_print("Your vehicle is up to date!");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    char line[200], car[20], date[20], service[50];
    int reminder_count = 0;

    // Get current date for comparison
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    char today[20];
    sprintf(today, "%04d-%02d-%02d", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday);

    printf("\n");
    center_print("UPCOMING MAINTENANCE SCHEDULE");
    printf("\n");
    printf("%*s", (CONSOLE_WIDTH-60)/2, "");
    printf("%-15s %-12s %s\n", "CAR", "DATE", "SERVICE");
    printf("%*s", (CONSOLE_WIDTH-60)/2, "");
    for (int i = 0; i < 60; i++) printf("=");
    printf("\n");

    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%s %s %[^\n]", car, date, service) == 3) {
            printf("%*s", (CONSOLE_WIDTH-60)/2, "");
            printf("%-15s %-12s %s\n", car, date, service);
            reminder_count++;
        }
    }

    if (reminder_count == 0) {
        printf("%*s", (CONSOLE_WIDTH-40)/2, "");
        printf("No scheduled maintenance found.\n");
        printf("%*s", (CONSOLE_WIDTH-40)/2, "");
        printf("Your vehicle is up to date!\n");
    } else {
        printf("\n");
        center_print("TIP: Regular maintenance keeps your car running smoothly!");
        center_print("Contact us to schedule your service appointment.");
    }

    fclose(f);
    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

/**
 * Manage Q&A system
 */
void manage_qna() {
    int choice;

    while (1) {
        clear_screen();
        display_ascii_logo();
        center_print("[*] Q&A MANAGEMENT");
        print_separator();

        center_print("1.  [+]  Add Q&A");
        center_print("2.  [*]  View All Q&A");
        center_print("3.  [<]  Back");

        printf("\n");
        printf("%*s", (CONSOLE_WIDTH-20)/2, "");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: add_qna(); break;
            case 2: view_qna(); break;
            case 3: return;
            default:
                center_print("[X] Invalid choice.");
                printf("\n");
                center_print("Press any key to continue...");
                getchar(); getchar();
        }
    }
}

/**
 * Add Q&A entry
 */
void add_qna() {
    clear_screen();
    display_ascii_logo();
    center_print("[+] ADD Q&A ENTRY");
    print_separator();

    char question[200], answer[300];

    printf("\n");
    printf("%*s", (CONSOLE_WIDTH-15)/2, "");
    printf("Question: ");
    getchar(); // consume newline
    fgets(question, sizeof(question), stdin);
    question[strcspn(question, "\n")] = '\0';

    printf("\n");
    printf("%*s", (CONSOLE_WIDTH-15)/2, "");
    printf("Answer: ");
    fgets(answer, sizeof(answer), stdin);
    answer[strcspn(answer, "\n")] = '\0';

    FILE *f = fopen(QNA_FILE, "a");
    if (!f) {
        center_print("[X] Error opening Q&A file.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    fprintf(f, "Q: %s\nA: %s\n---\n", question, answer);
    fclose(f);

    center_print("[+] Q&A added successfully!");
    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

/**
 * View all Q&A entries
 */
void view_qna() {
    clear_screen();
    display_ascii_logo();
    center_print("[*] Q&A DATABASE");
    print_separator();

    FILE *f = fopen(QNA_FILE, "r");
    if (!f) {
        center_print("[!] No Q&A entries found.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    char line[500];
    printf("\n");

    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "Q: ") == line) {
            printf("%*s", (CONSOLE_WIDTH-60)/2, "");
            printf("[?] %s", line + 3);
        } else if (strstr(line, "A: ") == line) {
            printf("%*s", (CONSOLE_WIDTH-60)/2, "");
            printf("[>] %s", line + 3);
        } else if (strstr(line, "---") == line) {
            printf("\n");
        }
    }

    fclose(f);
    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

/**
 * Customer view Q&A (for customer dashboard)
 */
void customer_view_qna() {
    clear_screen();
    display_ascii_logo();
    center_print("[*] TROUBLESHOOTING Q&A");
    print_separator();

    FILE *f = fopen(QNA_FILE, "r");
    if (!f) {
        center_print("[!] No Q&A available.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    char line[500];
    int question_count = 0;

    // Count questions first
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "Q: ") == line) {
            question_count++;
        }
    }
    rewind(f);

    if (question_count == 0) {
        center_print("[!] No questions available.");
        fclose(f);
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    // Display questions with numbers
    printf("\n");
    center_print("Available Questions:");
    printf("\n");

    int current_q = 1;
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "Q: ") == line) {
            printf("%*s", (CONSOLE_WIDTH-70)/2, "");
            printf("%d. %s", current_q, line + 3);
            current_q++;
        }
    }

    // Let customer select a question
    int choice;
    printf("\n");
    char prompt[50];
    sprintf(prompt, "Select question (1-%d): ", question_count);
    center_prompt(prompt);
    scanf("%d", &choice);

    if (choice < 1 || choice > question_count) {
        center_print("[X] Invalid choice.");
        fclose(f);
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    // Show selected Q&A
    rewind(f);
    current_q = 1;
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "Q: ") == line) {
            if (current_q == choice) {
                printf("\n");
                center_print("=== SELECTED Q&A ===");
                printf("%*s", (CONSOLE_WIDTH-60)/2, "");
                printf("[?] %s", line + 3);

                // Get the answer
                if (fgets(line, sizeof(line), f) && strstr(line, "A: ") == line) {
                    printf("%*s", (CONSOLE_WIDTH-60)/2, "");
                    printf("[>] %s", line + 3);
                }
                break;
            }
            current_q++;
        }
    }

    fclose(f);
    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

// ==================== CAR PARKING FUNCTIONS ====================

/**
 * Add car parking entry
 */
void add_car_parking(const char *username) {
    clear_screen();
    display_ascii_logo();
    center_print("[+] CAR PARKING REGISTRATION");
    print_separator();

    char car_number[20], entry_time[20], manufacturer[50];

    printf("\n");
    center_prompt("Car Number: ");
    scanf("%s", car_number);

    center_prompt("Entry Time (HH:MM): ");
    scanf("%s", entry_time);

    center_prompt("Car Manufacturer Company: ");
    // Clear the input buffer first
    while (getchar() != '\n');
    // Now read the manufacturer name
    fgets(manufacturer, sizeof(manufacturer), stdin);
    // Remove the trailing newline character
    manufacturer[strcspn(manufacturer, "\n")] = '\0';

    // Get current date
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    char date[20];
    sprintf(date, "%04d-%02d-%02d", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday);

    FILE *f = fopen(CAR_PARKING_FILE, "a");
    if (!f) {
        center_print("[X] Error opening car parking file.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    fprintf(f, "%s %s %s %s %s\n", username, car_number, date, entry_time, manufacturer);
    fclose(f);

    printf("\n");
    center_print("[+] Car parking registered successfully!");
    printf("\n");
    char parking_msg[100];
    sprintf(parking_msg, "[*] %s parked at %s on %s", car_number, entry_time, date);
    center_print(parking_msg);

    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

/**
 * View car parking records
 */
void view_car_parking() {
    clear_screen();
    display_ascii_logo();
    center_print("[*] CAR PARKING RECORDS");
    print_separator();

    FILE *f = fopen(CAR_PARKING_FILE, "r");
    if (!f) {
        center_print("[!] No parking records found.");
        printf("\n");
        center_print("Press any key to continue...");
        getchar(); getchar();
        return;
    }

    char line[300], username[30], car_number[20], date[20], entry_time[20], manufacturer[50];

    printf("\n");
    printf("%*s", (CONSOLE_WIDTH-80)/2, "");
    printf("%-12s %-12s %-12s %-8s %s\n", "CUSTOMER", "CAR NUMBER", "DATE", "TIME", "MANUFACTURER");
    printf("%*s", (CONSOLE_WIDTH-80)/2, "");
    for (int i = 0; i < 80; i++) printf("-");
    printf("\n");

    while (fgets(line, sizeof(line), f)) {
        sscanf(line, "%s %s %s %s %s", username, car_number, date, entry_time, manufacturer);
        printf("%*s", (CONSOLE_WIDTH-80)/2, "");
        printf("%-12s %-12s %-12s %-8s %s\n", username, car_number, date, entry_time, manufacturer);
    }

    fclose(f);
    printf("\n");
    center_print("Press any key to continue...");
    getchar(); getchar();
}

/**
 * Initialize default Q&A entries
 */
void initialize_default_qna() {
    FILE *f = fopen(QNA_FILE, "r");
    if (f) {
        fclose(f);
        return; // File exists, don't overwrite
    }

    f = fopen(QNA_FILE, "w");
    if (f) {
        fprintf(f, "Q: My car engine is overheating. What should I do?\n");
        fprintf(f, "A: Turn off the AC, turn on the heater, pull over safely, turn off the engine, check coolant level (when cool), and call for help if needed.\n\n");

        fprintf(f, "Q: My car won't start. What are the possible causes?\n");
        fprintf(f, "A: Check battery connections, try jump-starting, check fuel level, ensure the gear is in Park/Neutral, and verify the starter motor.\n\n");

        fprintf(f, "Q: Why are my brakes making squeaking noises?\n");
        fprintf(f, "A: Usually indicates worn brake pads. Have them inspected immediately as this is a safety issue. May also be due to dust or moisture.\n\n");

        fprintf(f, "Q: How often should I change my car's oil?\n");
        fprintf(f, "A: Generally every 3,000-7,500 miles depending on oil type. Synthetic oil lasts longer. Check your owner's manual for specific recommendations.\n\n");

        fprintf(f, "Q: My car is pulling to one side while driving. What's wrong?\n");
        fprintf(f, "A: Could be tire pressure imbalance, wheel alignment issues, or brake problems. Check tire pressure first, then visit a mechanic.\n\n");

        fprintf(f, "Q: What should I do if my car battery dies?\n");
        fprintf(f, "A: Use jumper cables with another car, call roadside assistance, or use a portable battery pack. Replace old batteries preventively.\n\n");

        fprintf(f, "Q: How can I improve my car's fuel efficiency?\n");
        fprintf(f, "A: Maintain proper tire pressure, regular oil changes, remove excess weight, drive smoothly, and keep up with regular maintenance.\n\n");

        fprintf(f, "Q: My check engine light is on. Is it serious?\n");
        fprintf(f, "A: It varies. If blinking, stop driving immediately. If steady, get it diagnosed soon. Could range from loose gas cap to serious engine issues.\n\n");

        fclose(f);
    }
}

/**
 * Main function
 */
int main() {
    // Create default admin user (no need to store in file)
    // Default credentials: username=admin, password=admin123

    // Initialize default discount codes
    initialize_default_discounts();

    // Initialize default Q&A entries
    initialize_default_qna();

    main_menu();
    return 0;
}
