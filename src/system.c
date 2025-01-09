#include "header.h"
#include <ctype.h> // Add this line
const char *RECORDS = "./data/records.txt";

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r->id,
                  &r->userId,
                  name,
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}

void saveAccountToFile(FILE *ptr, struct User *u, struct Record *r)
{
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
            r->id,
            u->id,
            u->name,
            r->accountNbr,
            r->deposit.month,
            r->deposit.day,
            r->deposit.year,
            r->country,
            r->phone,
            r->amount,
            r->accountType);
}

void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
    {
        system("clear");
        mainMenu(u);
    }
    else
    {
        system("clear");
        exit(1);
    }
}

void success(struct User u)
{
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

int isValidAccountNumber(int accountNbr) {
    // Convert account number to string to check length
    char accountNbrStr[12]; // Buffer to hold account number as a string
    sprintf(accountNbrStr, "%d", accountNbr); // Convert integer to string

    // Check if the length of the account number is exactly 9 digits
    if (strlen(accountNbrStr) != 9) {
        printf("Invalid account number. Please enter a number with exactly 9 digits.\n");
        return 0; // Invalid account number
    }

    // Check if the account number is within the valid range
    if (accountNbr < 100000000 || accountNbr > 999999999) {
        printf("Invalid account number. Please enter a number with exactly 9 digits.\n");
        return 0; // Invalid account number
    }

    return 1; // Valid account number
}

int isValidName(const char *name) {
    if (strlen(name) == 0 || strlen(name) > 50) return 0;
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i]) && name[i] != ' ') return 0;
    }
    return 1;
}

int isValidDate(int day, int month, int year) {
    if (year < 2024) {
        fprintf(stderr, "Invalid year: %d\n", year);
        return 0; // Invalid year
    }
    if (month < 1 || month > 12) {
        fprintf(stderr, "Invalid month: %d\n", month);
        return 0; // Invalid month
    }
    int daysInMonth;
    switch (month) {
        case 2:
            daysInMonth = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ? 29 : 28;
            break;
        case 4: case 6: case 9: case 11:
            daysInMonth = 30;
            break;
        default:
            daysInMonth = 31;
    }
    if (day < 1 || day > daysInMonth) {
        fprintf(stderr, "Invalid day: %d\n", day);
        return 0; // Invalid day
    }
    return 1; // Valid date
}

void checkAllAccounts(struct User u) {
    char userName[100];
    struct Record r;

    FILE *pf = fopen(RECORDS, "r");
    if (pf == NULL) {
        perror("Unable to open records file");
        return;
    }

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (getAccountFromFile(pf, userName, &r)) {
        if (strcmp(userName, u.name) == 0) {
            printf("_____________________\n");
            printf("\nAccount number: %d\nDeposit Date: %d/%d/%d \nCountry: %s \nPhone number: %d \nAmount deposited: $%.2f \nType Of Account: %s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
        }
    }
    fclose(pf);
    success(u);
}

void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    char accountNbrStr[12]; // Buffer to hold account number as a string
    FILE *pf = fopen(RECORDS, "a+");

    while (1) { // Loop until valid input is received
        system("clear");
        printf("\t\t\t===== New record(%s) =====\n", u.name);

        // Read date
        printf("\nEnter today's date (mm/dd/yyyy):");
        if(scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year) != 3) {
            printf("Invalid date format. Please enter in mm/dd/yyyy format.\n");
            while(getchar() != '\n'); // clear the buffer
            continue;
        }

        // Read account number
        printf("\nEnter the account number (9 digits):");
        scanf("%s", accountNbrStr); // Use fgets to capture the full input
        accountNbrStr[strcspn(accountNbrStr, "\n")] = '\0'; // Remove newline
        r.accountNbr = atoi(accountNbrStr); // Convert string to integer

        // Validate account number
        if (!isValidAccountNumber(r.accountNbr)) {
            continue; // Prompt for account number again
        }

        // Check if account number already exists
        int accountExists = 0; // Flag to track if account exists
        while (getAccountFromFile(pf, userName, &cr)) {
            if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr) {
                printf("✖ This Account already exists for this user\n\n");
                accountExists = 1; // Set flag to indicate account exists
                break; // Exit the loop
            }
        }

        if (accountExists) {
            continue; // Prompt for account details again
        }

        // Read country
        printf("\nEnter the country:");
        scanf("%s", r.country);
        r.country[strcspn(r.country, "\n")] = '\0'; // Remove newline
        

        // Read phone number
        printf("\nEnter the phone number:");
        if(scanf("%d", &r.phone) != 1) {
            printf("Invalid phone number. Please enter a valid number.\n");
            while(getchar() != '\n'); // clear the buffer
            continue; // Prompt for phone number again
        }
        while (getchar() != '\n');  // Consume the leftover newline after scanf
        if (r.phone <= 0) {
            printf("Invalid phone number. Please enter a valid number.\n");
            continue; // Prompt for phone number again
        }

        // Read deposit amount
        printf("\nEnter amount to deposit: $");
        if(scanf("%lf", &r.amount) != 1) {
            printf("Invalid amount. Please enter a valid amount.\n");
            while(getchar() != '\n'); // clear the buffer
            continue; // Prompt for amount again
        }
        while (getchar() != '\n');  // Consume the leftover newline after scanf
        if (r.amount <= 0) {
            printf("Invalid amount. Please enter a positive number.\n");
            continue; // Prompt for amount again
        }
        
        // Set next available ID
        int NextId = 0;
        while (fscanf(pf,"%d %d %s %d %d/%d/%d %s %d %lf %s",
        &r.id,
        &u.id, 
        r.name, 
        &r.accountNbr, 
        &r.deposit.month, 
        &r.deposit.day, 
        &r.deposit.year, 
        r.country,
        &r.phone, 
        &r.amount,
        r.accountType) !=EOF)
        {
            if (r.id >= NextId) 
            {
                NextId = r.id+1 ;
            }
        }

        // Account type selection
        printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
        fgets(r.accountType, sizeof(r.accountType), stdin);
        r.accountType[strcspn(r.accountType, "\n")] = '\0'; // Remove newline

        // Save the new account to the file
        saveAccountToFile(pf, &u, &r);
        fprintf(pf, "\n\n%d %d %s %d %d/%d/%d %s %d %.2lf %s",NextId,
                                                                u.id, 
                                                                u.name, 
                                                                r.accountNbr, 
                                                                r.deposit.month, r.deposit.day, r.deposit.year, 
                                                                r.country, 
                                                                r.phone, 
                                                                r.amount,
                                                                r.accountType  );

        fclose(pf);
        success(u);
        break; // Exit the loop after successful account creation
    }
}
