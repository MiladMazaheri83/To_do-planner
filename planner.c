#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <direct.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <windows.h>
#include <time.h>


typedef struct Task
{
    char *taskname;
    int n;
    long day;
    char *priority;
    struct Task *next;

} task;

int remove_directory(const char *path)
{
    WIN32_FIND_DATA findFileData;
    HANDLE hFind;
    char searchPath[MAX_PATH];

    // Create a search path for all files and subdirectories
    snprintf(searchPath, sizeof(searchPath), "%s\\*", path);

    // Start finding the first file/directory
    hFind = FindFirstFile(searchPath, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        // No files or directories found
        return RemoveDirectory(path) != 0 ? 0 : -1;
    }

    do
    {
        // Skip "." and ".." directories
        if (strcmp(findFileData.cFileName, ".") != 0 &&
            strcmp(findFileData.cFileName, "..") != 0)
        {
            // Create the full path to the file/directory
            char filePath[MAX_PATH];
            snprintf(filePath, sizeof(filePath), "%s\\%s", path, findFileData.cFileName);

            // Recursively remove subdirectories and files
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if (remove_directory(filePath) != 0)
                {
                    FindClose(hFind);
                    return -1;
                }
            }
            else
            {
                if (remove(filePath) != 0)
                {
                    FindClose(hFind);
                    return -1;
                }
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);

    // Remove the empty directory
    return RemoveDirectory(path) != 0 ? 0 : -1;
}

int sign_up();
int log_in();
int making_boards_and_lists();
int view(int *s, char buffer[100]);
void free2DArray(char **arr, int rows);
int Delete();
int get_back();
int time_calculate(char* f_time);
int new_task();

int main()
{
    // system("color 70");
    char x;
    char buffer[100];
    printf("are you already have an account?\nif yes press 1, and if no\npress 2 and make an account :) ");
    while (1)
    {
        x = getch();
        if (x == 50)
        {
            while (1)
            {
                if (sign_up() == 0)
                {
                    break;
                }
                system("cls");
                printf("This acount exist! please try another name...\n");
            }
            break;
        }
        if (x == 49)
        {
            while (1)
            {
                if (log_in() == 0)
                {
                    break;
                }
                system("cls");
                printf("there is no such an account");
            }
            break;
        }
        printf("\nyou entered wrong key, please try again ~>");
    }

    while (1)
    {
        printf("1. view Boards\n2. Create New Boards\n3. Delete Board\n");
        int c = getch();
        int s;
        system("cls");
        switch (c)
        {
        case '1':
        {
            printf("Available Boards: \n");
            view(&s, buffer);
            system("cls");
            if (s == 1)
            {
                system("cls");
                printf("THERE IS NO BOARD!");
                Sleep(1000);
                system("cls");
                s = 0;
                break;
            }
            int z;
            while (1)
            {
                printf("Board: <%s> \n", buffer);
                printf("1. View List\n2. Create New list\n3. Delete List\n4. Go Back\n");
                int x = getch();
                system("cls");
                switch (x)
                {
                case '1':
                {
                    printf(" Lists in <%s> Board:", buffer);
                    view(&s, buffer);
                    system("cls");
                    if (s == 1)
                    {
                        system("cls");
                        printf("THERE IS NO LIST!");
                        Sleep(1000);
                        system("cls");
                        s = 0;
                    }
                    continue;
                }
                case '2':
                {
                    system("cls");
                    printf("Add New List to <%s>:\n", buffer);
                    printf("What's the name of the new List? ");
                    making_boards_and_lists();
                    continue;
                }
                case '3':
                {
                    printf("Delete list from <%s>:\n", buffer);
                    printf("Which List Do you want to Delete?\n");
                    Delete();
                    continue;
                }
                case '4':
                {
                    z = 1;
                    system("cls");
                    get_back();
                }
                default:
                    break;
                }
                if (z)
                {
                    break;
                }
                system("cls");
            }
            z = 0;
            break;
        }
        case '2':
        {
            printf("whats the name of the new Board?");
            making_boards_and_lists();
            break;
        }
        case '3':
        {
            Delete();
        }
        default:
            break;
        }
        system("cls");
    }
    return 0;
}

int sign_up()
{
    char username[100];
    char passkey[100];
    char filepath[200];
    printf("\nplease enter your username:\n");
    fgets(username, 100, stdin);
    int num = strlen(username);
    if (username[num - 1] == '\n')
    {
        username[num - 1] = '\0';
    }

    if (mkdir(username) == 0)
    {
        printf("hello <%s> now make sure you have a strong password :)\n", username);
    }
    else
    {
        return -1;
    }
    if (chdir(username) != 0)
    {
        perror("Error changing directory");
        return 1;
    }

    printf("Please enter your password :\n");
    fgets(passkey, 100, stdin);
    num = strlen(passkey);
    if (passkey[num - 1] == '\n')
    {
        passkey[num - 1] = '\0';
    }

    snprintf(filepath, sizeof(filepath), "password.csv");

    FILE *password = fopen(filepath, "wb");

    if (password == NULL)
    {
        perror("Error creating file");
        return 1;
    }
    fprintf(password, "%s", passkey);
    fclose(password);
    system("cls");
    return 0;
}

int log_in()
{

    char username[100];
    char passkey[100];
    char filepath[200];
    char *key = NULL;
    FILE *password;
    while (1)
    {
        printf("\nplease enter your username:\n");
        fgets(username, 100, stdin);
        int num = strlen(username);
        if (username[num - 1] == '\n')
        {
            username[num - 1] = '\0';
        }

        if (chdir(username) != 0)
        {
            printf("there is no such an account");
            return 1;
        }
        else
        {
            printf("hello <%s>, Im so happy to see you :)\n", username);
            break;
        }
    }
    snprintf(filepath, sizeof(filepath), "password.csv");

    password = fopen(filepath, "r");
    if (password == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    fseek(password, 0, SEEK_END);
    long length = ftell(password);
    fseek(password, 0, SEEK_SET);

    key = (char *)malloc(length + 1); // +1 for null-termination
    if (key == NULL)
    {
        perror("Memory allocation error");
        fclose(password);
        return 1;
    }

    fread(key, 1, length, password);
    key[length] = '\0'; // Ensure null-termination
    while (1)
    {
        printf("%s Please enter your password:\n", username);
        fgets(passkey, 100, stdin);
        int num = strlen(passkey);
        if (passkey[num - 1] == '\n')
        {
            passkey[num - 1] = '\0';
        }
        system("cls");

        int u = strcmp(key, passkey);
        if (u == 0)
        {
            printf("Welcome, %s!\n", username);
            break;
        }
        else
        {
            printf("Incorrect password.\n");
        }
    }

    free(key);
    fclose(password);
    return 0;
}

int making_boards_and_lists()
{
    char name[100];
    char filepath[200];
    scanf("%s", name);
    if (mkdir(name) == 0)
    {
        printf("%s sucsessfuly added!", name);
        Sleep(1000);
        system("cls");
    }
    else
    {
        printf("already excist.\n");
        Sleep(1000);
        return 0;
    }
    snprintf(filepath, sizeof(filepath), "name.csv");
    FILE *table = fopen(filepath, "ab");
    if (table == NULL)
    {
        perror("Error creating file");
        return 1;
    }
    fprintf(table, "%s,", name);
    fclose(table);
}

int view(int *s, char buffer[100])
{
    char line[256];
    int length = 0;
    int i = 0;

    // Get the current working directory
    char currentDir[FILENAME_MAX];
    GetCurrentDirectory(FILENAME_MAX, currentDir);

    // Build the full path to the file
    char filepath[FILENAME_MAX];
    snprintf(filepath, sizeof(filepath), "%s\\name.csv", currentDir);

    FILE *name = fopen(filepath, "rb");
    if (name == NULL)
    {
        printf("THERE IS NOTHING!");
        // Sleep(10000);
        *s = 1;
        return 0;
    }

    while (fgets(line, sizeof(line), name) != NULL)
    {
        char *s = strtok(line, ",");
        while (s != NULL)
        {
            i++;
            printf("\n%d. %s  ", i, s);
            s = strtok(NULL, ",");
            length++;
        }
    }
    printf("\n~> ");
    fclose(name);
    char **list = malloc(sizeof(int) * length);
    for (int i = 0; i < length; i++)
    {
        list[i] = malloc(sizeof(char) * 100);
    }
    i = 0;
    name = fopen(filepath, "rb");
    while (fgets(line, sizeof(line), name) != NULL)
    {
        char *s = strtok(line, ",");
        while (s != NULL)
        {
            list[i] = s;
            s = strtok(NULL, ",");
            i++;
        }
    }
    int n;
    while (1)
    {
        if (scanf("%d", &n) == 1 && n >= 1 && n <= i)
        {
            break;
        }
        else
        {
            printf("\nyour input is unfortunately wrong please enter again (:");
            printf("\n~> ");
            while (getchar() != '\n')
                ;
        }
    }

    if (chdir(list[n - 1]) != 0)
    {
        printf("there is no such an account");
    }
    else
    {
        printf("<%s> is now open\n", list[n - 1]);
        strcpy(buffer, list[n - 1]);
    }
    fclose(name);
    free2DArray(list, length);
    return 1;
}

void free2DArray(char **arr, int rows)
{
    for (int i = 0; i < rows; i++)
    {
        free(arr[i]);
    }
    free(arr);
}

int Delete()
{
    char line[256];
    int length = 0;
    int i = 0;

    // Get the current working directory
    char currentDir[FILENAME_MAX];
    GetCurrentDirectory(FILENAME_MAX, currentDir);

    // Build the full path to the file
    char filepath[FILENAME_MAX];
    snprintf(filepath, sizeof(filepath), "%s\\name.csv", currentDir);

    FILE *name = fopen(filepath, "rb");
    if (name == NULL)
    {
        printf("There is Nothing to Delete!");
        Sleep(3000);
        system("cls");
        return 1;
    }

    while (fgets(line, sizeof(line), name) != NULL)
    {
        char *s = strtok(line, ",");
        while (s != NULL)
        {
            i++;
            printf("\n%d. %s  ", i, s);
            s = strtok(NULL, ",");
            length++;
        }
    }
    printf("\n~> ");
    char **list = malloc(sizeof(int) * length);
    for (int i = 0; i < length; i++)
    {
        list[i] = malloc(sizeof(char) * 100);
    }
    i = 0;
    fseek(name, 0, SEEK_SET);
    while (fgets(line, sizeof(line), name) != NULL)
    {
        char *s = strtok(line, ",");
        while (s != NULL)
        {
            list[i] = s;
            s = strtok(NULL, ",");
            i++;
        }
    }
    fclose(name);
    int n;
    while (1)
    {
        if (scanf("%d", &n) == 1 && n >= 1 && n <= i)
        {
            break;
        }
        else
        {
            printf("\nyour input is unfortunately wrong please enter again (:");
            printf("\n~> ");
            while (getchar() != '\n')
                ;
        }
    }
    if (remove_directory(list[n - 1]) == 0)
    {
        printf("\n%s is sucsessfully Deleted!", list[n - 1]);
        Sleep(3000);
        system("cls");
    }
    name = fopen(filepath, "w");
    fclose(name);
    name = fopen(filepath, "a");
    for (i = 0; i < length; i++)
    {
        if (i != (n - 1))
        {
            fprintf(name, "%s,", list[i]);
        }
    }
    fclose(name);
    length--;
    if (length == 0)
    {
        DeleteFile(filepath);
    }
    free2DArray(list, length);
}

int get_back()
{
    char currentDir[FILENAME_MAX];
    getcwd(currentDir, sizeof(currentDir));
    chdir("..");
    getcwd(currentDir, sizeof(currentDir));
    return 0;
}

// int new_task()
// {
    
//     pritnf("enter your task: ");
//     fgets(, 100, stdin);

//     pritnf("enter your task: ");
//     pritnf("enter your task: ");
// }

int time_calculate(char* f_time)
{
    char *token = strtok(f_time, "-");
    char* year = token;
    token = strtok(NULL, "-");
    char* mounth = token;
    token = strtok(NULL, "-");
    char* day = token;

    // Define two time structures
    struct tm time1 = {0}; // Initialize to all zeros
    struct tm time2 = {0};

    time_t current_time = time(NULL);
    struct tm *time_info = localtime(&current_time);

    // Set the values for the two times
    time1.tm_year = atoi(year) - 1900; // Year - 1900
    time1.tm_mon = atoi(mounth) - 1;            // Month (0-11)
    time1.tm_mday = atoi(day);           // Day

    time2.tm_year = time_info->tm_year;
    time2.tm_mon = time_info->tm_mon;
    time2.tm_mday = time_info->tm_mday;

    // Convert time structures to time_t
    time_t t1 = mktime(&time1);
    time_t t2 = mktime(&time2);

    // Calculate the time difference in seconds
    double difference_seconds = difftime(t2, t1);

    // Convert seconds to days
    double difference_days = difference_seconds / (24 * 3600);

    return difference_days;
}