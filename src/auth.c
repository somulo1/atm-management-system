#include <termios.h>
#include "header.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

char *USERS = "./data/users.txt";
int userId = 0;

void loginMenu(char a[50], char pass[50])
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
    scanf("%s", a);

    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    scanf("%s", pass);

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
};

const char *getPassword(struct User u)
{
    FILE *fp;
    struct User userChecker;

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    while (fscanf(fp, "%s %s", userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, u.name) == 0)
        {
            fclose(fp);
            char *buff = userChecker.password;
            return buff;
        }
    }

    fclose(fp);
    return "no user found";
}

int isValidName(const char *name) {
    if (strlen(name) == 0 || strlen(name) > 50) return 0;
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i]) && name[i] != ' ') return 0;
    }
    return 1;
}

int isValidPassword(const char *password) {
    if (strlen(password) < 8) return 0;
    int hasUpper = 0, hasLower = 0, hasDigit = 0, hasSpecial = 0;
    for (int i = 0; password[i] != '\0'; i++) {
        if (isupper(password[i])) hasUpper = 1;
        else if (islower(password[i])) hasLower = 1;
        else if (isdigit(password[i])) hasDigit = 1;
        else if (ispunct(password[i])) hasSpecial = 1;
    }
    return hasUpper && hasLower && hasDigit && hasSpecial;
}

int isValidDate(int day, int month, int year) {
    if (year < 1900 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    int daysInMonth;
    switch (month) {
        case 2:
            // Check for leap year
            daysInMonth = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ? 29 : 28;
            break;
        case 4: case 6: case 9: case 11:
            daysInMonth = 30;
            break;
        default:
            daysInMonth = 31;
    }
    return day >= 1 && day <= daysInMonth;
}

void registerUser() {
    char name[50], password[50];
    int day, month, year;
    FILE *file;
    int userExists = 0;

    printf("Enter your name: ");
    scanf("%s", name);

    if (!isValidName(name)) {
        printf("Invalid name. Please use only alphabetic characters and spaces, and ensure it is not empty or too long.\n");
        return;
    }

    // Check if user already exists
    file = fopen(USERS, "r");
    if (file == NULL) {
        perror("Unable to open users file");
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, name) != NULL) {
            userExists = 1;
            break;
        }
    }
    fclose(file);

    if (userExists) {
        printf("User with this name already exists.\n");
        return;
    }

    printf("Enter your password: ");
    scanf("%s", password);

    if (!isValidPassword(password)) {
        printf("Invalid password. It must be at least 8 characters long and contain at least one uppercase letter, one lowercase letter, one digit, and one special character.\n");
        return;
    }

    // Prompt for date of birth
    printf("Enter your date of birth (DD MM YYYY): ");
    scanf("%d %d %d", &day, &month, &year);

    if (!isValidDate(day, month, year)) {
        printf("Invalid date. Please enter a valid date in the format DD MM YYYY.\n");
        return;
    }

    // Save new user to file
    file = fopen(USERS, "a");
    if (file == NULL) {
        perror("Unable to open users file");
        return;
    }
    fprintf(file, "%d %s %s %02d/%02d/%04d\n", userId++, name, password, day, month, year);
    fclose(file);
    printf("Registration successful!\n");
}
