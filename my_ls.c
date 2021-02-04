#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "my_ls.h"

// readArgs() - a function that reads the arguments enetered by user and
// assigns file names and directory names to "all"(array of files and dirs)
// Return value: pointer to t_args, which represents enabled flags

t_args* readArgs(int argc, char** argv, t_lists* all){
    t_args* p_args = (t_args*)malloc(sizeof(t_args));
    p_args->a_arg = 0;
    p_args->t_arg = 0;
    all->n = 0;

    for(int i=1; i<argc; i++){
        // if enetered by user argument starts with '-' and flag is typed
        if(argv[i][0] == '-' && strlen(argv[i])>1){
            for(int j = 1; j<(int)strlen(argv[i]); j++){
                if(argv[i][j] == 'a')
                    p_args->a_arg = 1;  // -a flag enabled
                
                if(argv[i][j] == 't')
                    p_args->t_arg = 1;  // -t flag enabled
            }
        } else{
            // if it is a filename or a directory name 
            strcpy(all->list[all->n], argv[i]);
            all->n++;
        }
    }

    return p_args;
}

// seperateList() - a function that seperetes files and directories into two different lists
void seperateList(t_lists* files, t_lists* dirs, t_lists* all){
    struct stat sb;
    int countFiles = 0, countDirs = 0;
    for(int i=0; i<all->n; i++){
        lstat(all->list[i], &sb);
        switch (sb.st_mode & S_IFMT) {
            case S_IFDIR:   // Directory
                //printf("directory\n");    
                // copies to dirs list
                strcpy(dirs->list[countDirs], all->list[i]);
                countDirs++;
                break;
            case S_IFREG:   // Regular File
                //printf("regular file\n");
                // copies to files list
                strcpy(files->list[countFiles], all->list[i]);
                countFiles++;
                break;
            default:    // Unknown File
               //printf("unknown?\n");
               break;
           }
    }
    dirs->n = countDirs;
    files->n = countFiles;
}

// readDir() - a function that reads directory contents and returns a pointer to it
t_lists* readDir(char* dirName, t_args* p_args){
    t_lists* dirContent = malloc(sizeof(t_lists));
    dirContent->list = malloc(sizeof(char*)*20);
    for(int i = 0; i < 20; i++){
        dirContent->list[i] = malloc(sizeof(char)*50);
    }
    dirContent->n = 0;

    DIR *folder;
    struct dirent *entry;
    
    folder = opendir(dirName);
    /* if(folder == NULL){
        puts("Unable to open the folder!\n");
        exit(1);
    } */
    while((entry = readdir(folder))){
        if(p_args->a_arg == 0 && entry->d_name[0] == '.'){
            continue;
        }
        strcpy(dirContent->list[dirContent->n], entry->d_name);
        dirContent->n++;
    }
    closedir(folder);
    return dirContent;
}

void swap(char* str1, char* str2){
    char* temp = malloc(sizeof(char)*50);
    strcpy(temp, str1);
    strcpy(str1, str2);
    strcpy(str2, temp);
    free(temp);
}

void sortByLex(t_lists* list){
    int i, j;
    for (i = 0; i < list->n - 1; i++) {
        for (j = 0; j < list->n - 1 - i; j++) {
            if (strcmp(list->list[j], list->list[j+1]) > 0){
                swap(list->list[j], list->list[j+1]);
            }
        }
    }
}

void sortFoldersByLex(int n, t_lists* folders[]){
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if(strcmp(folders[j]->dirName, folders[j+1]->dirName) > 0){
                t_lists* temp = folders[j];
                folders[j] = folders[j+1];
                folders[j+1] = temp;
            }
        }
    }
}

void sortByTime(t_lists* list){
    struct stat sb1;
    struct stat sb2;
    
    for(int i = 0; i < list->n; i++){
        for(int j = 0; j < list->n - 1 - i; j++){
            lstat(list->list[j], &sb1);
            lstat(list->list[j+1], &sb2);
            if(sb1.st_mtim.tv_sec < sb2.st_mtim.tv_sec){
                swap(list->list[j], list->list[j+1]);
            } else if(sb1.st_mtim.tv_sec == sb2.st_mtim.tv_sec){
                if(strcmp(list->list[j], list->list[j+1]) > 0){
                    swap(list->list[j], list->list[j+1]);
                }
            }
        }
    }
}

void sortFoldersByTime(int n, t_lists* folders[]){
    struct stat sb1;
    struct stat sb2;
    t_lists* temp;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n - 1 - i; j++){
            lstat(folders[j]->dirName, &sb1);
            lstat(folders[j+1]->dirName, &sb2);
            if(sb1.st_mtim.tv_sec < sb2.st_mtim.tv_sec){
                temp = folders[j];
                folders[j] = folders[j+1];
                folders[j+1] = temp;
            } else if(sb1.st_mtim.tv_sec == sb2.st_mtim.tv_sec){
                if(strcmp(folders[j]->dirName, folders[j+1]->dirName) > 0){
                    temp = folders[j];
                    folders[j] = folders[j+1];
                    folders[j+1] = temp;
                }
            }
        }
    }
}

void print_files(t_lists* files){
    for(int i = 0; i < files->n; i++){
        printf("%s  ", files->list[i]);
    }
    printf("\n");
}

void print_folder(int n, t_lists* folders[], int t_arg){

    if(t_arg == 0){
        sortFoldersByLex(n, folders);
    } else{
        sortFoldersByTime(n, folders);
    }

    for(int i = 0; i < n; i++){
        printf("%s:\n", folders[i]->dirName);
        if(folders[i]->n != 0)
            print_files(folders[i]);
        if(i != n-1)
            printf("\n");
    }
}
