#include <termios.h>
#include "header.h"

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

void registerUser() {
    char name[50], password[50];
    FILE *file;
    int userExists = 0;

    printf("Enter your name: ");
    scanf("%s", name);

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

    // Save new user to file
    file = fopen(USERS, "a");
    if (file == NULL) {
        perror("Unable to open users file");
        return;
    }
    fprintf(file, "%d %s %s\n", userId++, name, password);
    fclose(file);
    printf("Registration successful!\n");
}
