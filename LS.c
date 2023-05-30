#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdbool.h>

#define MAX_PATH_LENGTH 1024
#define DATE_LENGTH 100

bool printLongInfo; //[-ls -l]
bool useRecursiveListing; //[-ls -R]

bool isNameSplit(char* pathName) {
    int i=0;
    while(pathName[i] != '\0') {
        if(pathName[i] == ' ') 
            return true;
        i++;
    }
    return false;
}

void printPermissions(struct stat buf) {
    printf( (S_ISDIR(buf.st_mode)) ? "d" : "-");
    printf( (buf.st_mode & S_IRUSR) ? "r" : "-");
    printf( (buf.st_mode & S_IWUSR) ? "w" : "-");
    printf( (buf.st_mode & S_IXUSR) ? "x" : "-");
    printf( (buf.st_mode & S_IRGRP) ? "r" : "-");
    printf( (buf.st_mode & S_IWGRP) ? "w" : "-");
    printf( (buf.st_mode & S_IXOTH) ? "x" : "-");
    printf( (buf.st_mode & S_IROTH) ? "r" : "-");
    printf( (buf.st_mode & S_IWOTH) ? "w" : "-");
    printf( (buf.st_mode & S_IXGRP) ? "x" : "-");
    printf("\t");
}

//dirent - struktura, która zawiera numer i węzeł pliku oraz jego nazwe
void printNumberOfListsAndDirectories(struct dirent* entry, char* pathName) {
    DIR* dir; // DIR - struktura reprezentująca strumień katologowy
    int linksAndDirectoriesCount=0;
    char path[MAX_PATH_LENGTH];
    //entry->d_name + pathName = path(pathName/entry->d_name)
    snprintf(path, sizeof(path), "%s/%s", pathName, entry->d_name);
    if(entry->d_type == DT_DIR) { //d_type -> rodzaj pliku np. zwykły plik, katalog, link itp.
        dir=opendir(path);
        //nadpisujemy entry
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_DIR || entry->d_type == DT_LNK) { //DT_DIR-katalog DT_LNK-link
                linksAndDirectoriesCount++;
            }
        }
        closedir(dir);
        printf("%2d\t",linksAndDirectoriesCount);
        return;
    }
    printf("%2d\t", 1); //Każdy plik, który nie jest katalogiem ma wartość "1"
}
void printUser(struct stat buf) {
    struct passwd *pw = getpwuid(buf.st_uid); //passwd - struktura posiadająca informacje o użytkowniku
                                              //zwraca user pliku                          
    printf("%s\t", pw->pw_name); //wypisuje imie user'a
}
void printGroup(struct stat buf) {
    struct group *gr = getgrgid(buf.st_gid);
    printf("%s\t", gr->gr_name);
}
void printSize(struct stat buf) {
    printf("%10ld\t", buf.st_size); //10 robi żeby formatowało się do prawej
}
void printDate(struct stat buf) {
    char t[DATE_LENGTH] = "";
    strftime(t, DATE_LENGTH, "%Y-%m-%d %H-%M", localtime(&buf.st_mtime)); //time to sting w danym formacie
    printf("%s\t", t);
}
void printEntryName(struct dirent* entry) {
    int i=0;
    char fileName[255];
    fileName[254] = '\0';
    strncpy(fileName, entry->d_name, 254);
    if(isNameSplit(fileName)) printf("\'%s\'\t", entry->d_name);
    else printf("%s\t", entry->d_name);
}

void printEntryInfo(struct dirent* entry, char* pathName) {
    struct stat buf;
    char path[MAX_PATH_LENGTH];

    //entry->d_name + pathName = path(pathName/entry->d_name)
    snprintf(path, sizeof(path), "%s/%s", pathName, entry->d_name);
    stat(path, &buf);

    //Print long info
    if(printLongInfo == true) {
        printPermissions(buf);
        printNumberOfListsAndDirectories(entry, pathName);
        printUser(buf);
        printGroup(buf);
        printSize(buf);
        printDate(buf);
        printEntryName(entry);
        printf("\n");
    } else {
        //Print short info
        printEntryName(entry);
    }
}

void listdir(char* pathName, int indent) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(pathName);

    //wysypuje się na przykład gdy poda się ścieżkę która nie istnieje
    if (dir == NULL) {
        exit(-1);
    }

    if(isNameSplit(pathName)) printf("\'%s\':\n", pathName);
    else printf("%s:\n", pathName);    

    while ((entry = readdir(dir))) {
        if (entry->d_type == DT_DIR || entry->d_type == DT_REG) //DT_REG - zwykły plik
            if (entry->d_name[0] != '.') {
                printEntryInfo(entry, pathName);
            }
    }
    printf("\n");
   
    closedir(dir);

    if(!printLongInfo) printf("\n");
    if (useRecursiveListing == true) {
        dir = opendir(pathName);
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_DIR && entry->d_name[0] != '.') {
                char path[MAX_PATH_LENGTH];
                snprintf(path, sizeof(path), "%s/%s", pathName, entry->d_name);

                listdir(path, indent + 2 );
            }
        }
        closedir(dir);
    }
}

int main(int argc, char *argv[]) {
    int i=1;
    char path[MAX_PATH_LENGTH] = ".";

    while(argv[i] != NULL) {
        if(argv[i][0] == '-') {
            int j=1;
            while (argv[i][j] != '\0') {
                if(argv[i][j] == 'l') printLongInfo = true;
                if(argv[i][j] == 'R') useRecursiveListing = true;   
                j++;
            }
        } else {
            int j=0;
            while (argv[i][j] != '\0') {
                path[j] = argv[i][j];
                j++;
            }
        }
        i++;
    }
    listdir(path, 0);
    return 0;
}
