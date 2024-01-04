#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <direct.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <windows.h>
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
int view(int *s);
void free2DArray(char **arr, int rows);
int Delete();
int get_back();

int main()
{
    system("color 70");
    char x;
    printf("are you already have an account?\nif yes press 1, and if no\npress 2 and make an account :) ");
    while (1)
    {
        x = getch();
        if (x == 50)
        {
            sign_up();
            break;
        }
        if (x == 49)
        {
            log_in();
            break;
        }
        printf("\nyou entered wrong key, please try again ~>");
    }

    while (1)
    {
        printf("1. view Boards\n2. Create New Boards\n3. Delete Board");
        int c = getch();
        int s;
        system("cls");
        switch (c)
        {
        case '1':
            view(&s);
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
                printf("1. View List\n2. Create New list\n3. Delete List\n4. Go Back");
                int x = getch();
                switch (x)
                {
                case '1':
                    view(&s);
                    system("cls");
                    if (s == 1)
                    {
                        system("cls");
                        printf("THERE IS NO LIST!");
                        Sleep(1000);
                        system("cls");
                        s = 0;
                    }
                    break;
                case '2':
                    system("cls");
                    printf("What's the name of the new List?");
                    making_boards_and_lists();
                    break;
                case '3':
                    Delete();
                    break;
                case '4':
                    z = 1;
                    system("cls");
                    get_back();
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
        case '2':
            printf("whats the name of the new Board?");
            making_boards_and_lists();
            break;
        case '3':
            Delete();
        default:
            break;
        }
    }
    system("cls");
    return 0;
}

int sign_up()
{
    char username[100];
    char passkey[100];
    char filepath[200];
    printf("\nplease enter your username:\n");
    scanf("%s", username);
    if (mkdir(username) == 0)
    {
        printf("hello <%s> now make sure you have a strong password :)\n", username);
    }
    else
    {
        printf("Failed to create directory.\n");
        perror("Error");
    }
    if (chdir(username) != 0)
    {
        perror("Error changing directory");
        return 1;
    }

    printf("Please enter your password (Atleast 6 character):\n");
    scanf("%s", passkey);

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
        scanf("%s", username);

        if (chdir(username) != 0)
        {
            printf("there is no such an account");
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
        scanf("%s", passkey);
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
        perror("Error");
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

int view(int *s)
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
        *s = 1;
        return 0;
    }

    while (fgets(line, sizeof(line), name) != NULL)
    {
        char *s = strtok(line, ",");
        while (s != NULL)
        {
            i++;
            printf("\n%d. %s", i, s);
            s = strtok(NULL, ",");
            length++;
        }
    }
    if (length == 0)
    {
        fclose(name);
        if (DeleteFile(filepath))
        {
            printf("File deleted successfully.\n");
        }
        return 0;
    }
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
    int n;
    scanf("%d", &n);

    if (chdir(list[n - 1]) != 0)
    {
        printf("there is no such an account");
    }
    else
    {
        printf("<%s> is now open\n", list[n - 1]);
    }
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

    printf("Which list Do you want to Delete?\n");
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
            printf("\n%d. %s", i, s);
            s = strtok(NULL, ",");
            length++;
        }
    }
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
    scanf("%d", &n);
    if (remove_directory(list[n - 1]) == 0)
    {
        printf("%s is sucsessfully Deleted!", list[n - 1]);
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
    free2DArray(list, length);
}

int get_back(){
    char currentDir[FILENAME_MAX];
    getcwd(currentDir, sizeof(currentDir));
    chdir("..");
    getcwd(currentDir, sizeof(currentDir));
    return 0;
}