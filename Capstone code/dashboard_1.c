// =================================================================================
// Parkify: One Stop Solution to Your Car - Simple Dashboard Demo
// Created for 1-hour lab demonstration
// =================================================================================

#include <stdio.h>
#include <stdlib.h>

// Function prototypes
void showWelcome();
void showMainMenu();
void registerUser();
void loginUser();
void adminPanel();
void mechanicPanel();
void clearScreen();
void pauseScreen();

int main() {
    int choice;
    
    showWelcome();
    
    do {
        showMainMenu();
        printf("Enter your choice (1-5): ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                registerUser();
                break;
            case 2:
                loginUser();
                break;
            case 3:
                adminPanel();
                break;
            case 4:
                mechanicPanel();
                break;
            case 5:
                printf("\n🚗 Thank you for using Parkify! Drive safely! 🚗\n");
                exit(0);
                break;
            default:
                printf("❌ Invalid choice! Please try again.\n");
                pauseScreen();
        }
    } while(choice != 5);
    
    return 0;
}

void showWelcome() {
    clearScreen();
    printf("╔════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                                                            ║\n");
    printf("║              🚗 PARKIFY: ONE STOP SOLUTION TO YOUR CAR 🚗                ║\n");
    printf("║                                                                            ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════╝\n");
    printf("\n✨ Welcome to the Simple Dashboard Demo! ✨\n");
    pauseScreen();
}

void showMainMenu() {
    clearScreen();
    printf("┌────────────────────────────────────────────────────────────────────────────┐\n");
    printf("│                    PARKIFY - MAIN DASHBOARD                                │\n");
    printf("├────────────────────────────────────────────────────────────────────────────┤\n");
    printf("│  1. 📝 Register User                                                       │\n");
    printf("│  2. 🔐 Login User                                                          │\n");
    printf("│  3. 🔑 Admin Panel                                                         │\n");
    printf("│  4. 🔧 Mechanic Panel                                                      │\n");
    printf("│  5. 🚪 Exit                                                                │\n");
    printf("└────────────────────────────────────────────────────────────────────────────┘\n");
    printf("\n");
}

void registerUser() {
    clearScreen();
    printf("┌────────────────────────────────────────────────────────────────────────────┐\n");
    printf("│                           USER REGISTRATION                               │\n");
    printf("└────────────────────────────────────────────────────────────────────────────┘\n");
    printf("\n📝 Registration Process:\n");
    printf("• Enter your username\n");
    printf("• Create a secure password\n");
    printf("• Provide contact information\n");
    printf("• Choose account type (Customer/Admin/Mechanic)\n");
    printf("\n✅ Registration completed successfully!\n");
    printf("💡 You can now login with your credentials.\n");
    pauseScreen();
}

void loginUser() {
    clearScreen();
    printf("┌────────────────────────────────────────────────────────────────────────────┐\n");
    printf("│                              USER LOGIN                                   │\n");
    printf("└────────────────────────────────────────────────────────────────────────────┘\n");
    printf("\n🔐 Login Process:\n");
    printf("• Username: customer_demo\n");
    printf("• Password: ••••••••\n");
    printf("\n✅ Login successful! Welcome back!\n");
    printf("🎯 Redirecting to customer dashboard...\n");
    printf("\n📊 Customer Features Available:\n");
    printf("• 🚗 Manage your cars\n");
    printf("• 📅 Book service appointments\n");
    printf("• 🅿️ Parking management\n");
    printf("• 📋 View service history\n");
    pauseScreen();
}

void adminPanel() {
    clearScreen();
    printf("┌────────────────────────────────────────────────────────────────────────────┐\n");
    printf("│                              ADMIN PANEL                                  │\n");
    printf("└────────────────────────────────────────────────────────────────────────────┘\n");
    printf("\n🔑 Admin Dashboard Overview:\n");
    printf("═══════════════════════════════════════\n");
    printf("📊 System Statistics:\n");
    printf("• Total Users: 156\n");
    printf("• Active Services: 12\n");
    printf("• Daily Revenue: $1,250\n");
    printf("• Parking Spaces: 47/50 available\n");
    printf("\n🛠️ Admin Functions:\n");
    printf("• 👥 Manage all users\n");
    printf("• 📋 View customer history\n");
    printf("• 📦 Parts inventory control\n");
    printf("• 📈 Financial reports\n");
    printf("• ⚙️ System configuration\n");
    pauseScreen();
}

void mechanicPanel() {
    clearScreen();
    printf("┌────────────────────────────────────────────────────────────────────────────┐\n");
    printf("│                            MECHANIC PANEL                                 │\n");
    printf("└────────────────────────────────────────────────────────────────────────────┘\n");
    printf("\n🔧 Mechanic Dashboard Overview:\n");
    printf("════════════════════════════════════════\n");
    printf("📋 Today's Assignments:\n");
    printf("• Oil Change - Car ABC123 (30 min)\n");
    printf("• Brake Service - Car XYZ789 (1.5 hrs)\n");
    printf("• Engine Check - Car DEF456 (45 min)\n");
    printf("\n🛠️ Mechanic Tools:\n");
    printf("• 📋 View work assignments\n");
    printf("• 📦 Check parts availability\n");
    printf("• ✅ Update service status\n");
    printf("• 🔧 Equipment status monitor\n");
    printf("• ⏰ Time tracking system\n");
    pauseScreen();
}

void clearScreen() {
    system("clear");
}

void pauseScreen() {
    printf("\nPress Enter to continue...");
    getchar();
    getchar(); // Handle the newline from previous input
}

/*
================================================================================
SIMPLE PARKIFY DASHBOARD - LAB DEMONSTRATION VERSION
================================================================================
Features:
✅ Welcome screen with branding
✅ Main menu with 5 options
✅ Visual-only registration process
✅ Visual-only login demonstration
✅ Admin panel overview
✅ Mechanic panel overview
✅ Clean exit functionality

Perfect for 1-hour lab session!
Compile: gcc dashboard_1.c -o parkify_demo && ./parkify_demo
================================================================================
*/
