#include <stdio.h>

#define CONSOLE_WIDTH 80

void center_print(const char* text) {
    int len = strlen(text);
    int padding = (CONSOLE_WIDTH - len) / 2;
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s\n", text);
}

int main() {
    printf("=== MENU ALIGNMENT TEST ===\n\n");
    
    printf("BEFORE (misaligned):\n");
    printf("1. [+] Register (Customer)\n");
    printf("2.      [#]     User Login\n");
    printf("3.      [#]     Admin Login\n");
    printf("4. [#]     Mechanic Login\n");
    printf("5.      [X]     Exit System\n\n");
    
    printf("AFTER (perfectly aligned):\n");
    center_print("1.  [+]  Register (Customer)");
    center_print("2.  [#]  User Login");
    center_print("3.  [#]  Admin Login");
    center_print("4.  [#]  Mechanic Login");
    center_print("5.  [X]  Exit System");
    
    return 0;
}
