#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Structure Definition ---
// This acts as our 'Database Record'
struct User {
    int id;             //Unique Customer ID
    char password[20];  //Authentication Key
    char name[50];      //Customer Name
    char pan[15];       //Customer PAN card Number
    char phone[15];     //Customer Phone Number
    float balance;      //Account Funds
};

// --- Function Prototypes ---
void setupDemoUsers();
void clearScreen();
void printHeader(char *title);
void createAccount();
void login();
void userMenu(struct User currentUser);
void updateBalance(int id, float newBalance);
void showUsers();

// --- Main Function ---
int main() {
    int choice;
    srand(time(0));

    // -------------------- //

    while (1) {
        clearScreen();
        printHeader("WELCOME TO STUDENT BANK SYSTEM");

        while (1) {
            clearScreen();
            printHeader("WELCOME TO STUDENT BANK SYSTEM");
            printf(" 1. Login to Existing Account\n");
            printf(" 2. Create New Account\n");
            printf(" 3. Exit\n");
            printf("====================================================\n");
            printf(" Enter Choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1: login();
                    break;
                case 2: createAccount();
                    break;
                case 3:
                    printf("\n Thank you for banking with us!\n");
                    exit(0);
                    break;
                case 0:
                    showUsers();
                    break;

                default: printf("\n Invalid Option! Press Enter to retry...");
                    getchar();
                    getchar();
            }
        }
        return 0;
    }
}


// --- Helper Functions ---

void clearScreen() {
    system("clear");
}

void printHeader(char *title) {
    printf("\n====================================================\n");
    printf("        %s\n", title);
    printf("====================================================\n");
}

void createAccount() {
    struct User u;
    FILE *fp;

    clearScreen();
    printHeader("OPEN NEW ACCOUNT");

    // Taking Inputs
    printf(" Enter Your Full Name: ");
    scanf(" %[^\n]s", u.name); // %[^\n]s reads string with spaces

    printf(" Enter PAN Number: ");
    scanf("%s", u.pan);

    printf(" Enter Phone Number: ");
    scanf("%s", u.phone);

    printf(" Create a Password: ");
    scanf("%s", u.password);

    // Generating a random 5-digit Customer ID
    u.id = 10000 + rand() % 90000;
    u.balance = 0.0; // Initial balance

    // Saving to File
    fp = fopen("bank_database.dat", "ab"); // Append Binary mode
    if (fp == NULL) {
        printf("\n Error accessing database!\n");
        return;
    }
    fwrite(&u, sizeof(struct User), 1, fp);
    fclose(fp);

    printf("\n [SUCCESS] Account Created Successfully!");
    printf("\n YOUR CUSTOMER ID: %d", u.id);
    printf("\n (Please remember this ID for login)");
    printf("\n\n Press Enter to return to Main Menu...");
    getchar(); getchar(); // Double getchar to hold screen
}

void login() {
    int inputID, found = 0;
    char inputPass[20];
    struct User u;
    FILE *fp;

    clearScreen();
    printHeader("SECURE LOGIN");

    printf(" Enter Customer ID: ");
    scanf("%d", &inputID);

    printf(" Enter Password: ");
    scanf("%s", inputPass);

    fp = fopen("bank_database.dat", "rb"); // Read Binary mode
    if (fp == NULL) {
        printf("\n [ERROR] No accounts found in system.\n Press Enter...");
        getchar(); getchar();
        return;
    }

    // Search logic
    while (fread(&u, sizeof(struct User), 1, fp)) {
        if (u.id == inputID && strcmp(u.password, inputPass) == 0) {
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (found) {
        printf("\n [SUCCESS] Login Verified. Redirecting...");
        userMenu(u); // Pass the found user to the dashboard
    } else {
        printf("\n [FAILED] Invalid ID or Password.\n Press Enter to retry...");
        getchar(); getchar();
    }
}

// Function to update the file record
void userMenu(struct User currentUser) {
    int choice;
    float amount;

    while(1) {
        clearScreen();

        // --- DASHBOARD DISPLAY START ---
        printf("\n====================================================\n");
        printf("                  CUSTOMER DASHBOARD                \n");
        printf("====================================================\n");
        printf("  [ User Profile ]\n");
        printf("  Name       : %s\n", currentUser.name);
        printf("  Account No : %d\n", currentUser.id);
        printf("  Phone      : %s\n", currentUser.phone);
        printf("\n");
        printf("  [ CURRENT BALANCE ]\n");
        printf("  Rs. %.2f\n", currentUser.balance);
        printf("====================================================\n");
        // --- DASHBOARD DISPLAY END ---

        printf("  1. Deposit Money\n");
        printf("  2. Withdraw Money\n");
        printf("  3. Account Details (Print)\n");
        printf("  4. Logout\n");
        printf("----------------------------------------------------\n");
        printf("  Select Operation: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: // Deposit
                printf("\n  -- DEPOSIT SCREEN --");
                printf("\n  Enter Amount to Deposit: Rs.");
                scanf("%f", &amount);
                if (amount > 0) {
                    currentUser.balance += amount;
                    updateBalance(currentUser.id, currentUser.balance);
                    printf("\n  [SUCCESS] Rs.%.2f added to your account.", amount);
                } else {
                    printf("\n  [ERROR] Invalid amount.");
                }
                break;

            case 2: // Withdraw
                printf("\n  -- WITHDRAWAL SCREEN --");
                printf("\n  Enter Amount to Withdraw: Rs.");
                scanf("%f", &amount);
                if (amount > 0 && amount <= currentUser.balance) {
                    currentUser.balance -= amount;
                    updateBalance(currentUser.id, currentUser.balance);
                    printf("\n  [SUCCESS] Rs.%.2f withdrawn from account.", amount);
                    printf("\n  Remaining Balance: Rs.%.2f", currentUser.balance);
                } else {
                    printf("\n  [ERROR] Insufficient funds (Rs.%.2f only).", currentUser.balance);
                }
                break;

            case 3: // Account Details (Optional Extra)
                printf("\n  -- FULL DETAILS --");
                printf("\n  User: %s", currentUser.name);
                printf("\n  PAN Card: %s", currentUser.pan);
                printf("\n  Contact: %s", currentUser.phone);
                printf("\n  Balance: %.2f",currentUser.balance);
                printf("\n  Status: Active");
                break;

            case 4: // Logout
                printf("\n  Logging out...");
                return; // Returns to main menu

            default: printf("\n  Invalid Choice.");
        }

        // Pause so the user can see the "Success" message before the dashboard reloads
        if (choice != 4) {
            printf("\n\n  Press Enter to return to Dashboard...");
            getchar(); getchar();
        }
    }
}
void updateBalance(int id, float newBalance) {
    FILE *fp = fopen("bank_database.dat", "rb+"); // Read/Write mode
    struct User u;

    if (fp == NULL) return;

    while (fread(&u, sizeof(struct User), 1, fp)) {
        if (u.id == id) {
            u.balance = newBalance;
            // Move file pointer back one struct size to overwrite
            fseek(fp, -sizeof(struct User), SEEK_CUR);
            fwrite(&u, sizeof(struct User), 1, fp);
            break;
        }
    }
    fclose(fp);
}

void showUsers() {
    // 'rb' (read binary) is safer here since we are only reading, not writing
    FILE *fp = fopen("bank_database.dat", "rb");
    struct User u;

    if (fp == NULL) {
        printf("\n [ERROR] No database found or unable to open file.\n");
        printf("\n Press Enter to return to Main Menu...");
        getchar(); getchar();
        return;
    }

    clearScreen();
    printHeader("SYSTEM DATABASE (ADMIN VIEW)");

    // fread reads one 'struct User' at a time from the file until it reaches the end
    while (fread(&u, sizeof(struct User), 1, fp)) {
        printf(" ID       : %d\n", u.id);
        printf(" Name     : %s\n", u.name);
        printf(" Phone    : %s\n", u.phone);
        printf(" PAN      : %s\n", u.pan);
        printf(" Balance  : Rs. %.2f\n", u.balance);
        printf(" Password : %s\n", u.password);
        printf("----------------------------------------------------\n");
    }

    fclose(fp);

    // Pause so the user can read the list before the main menu clears the screen
    printf("\n End of database. Press Enter to return to Main Menu...");
    getchar(); getchar();
}


