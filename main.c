#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_ls.h"

int main(int argc, char* argv[]){

    // allocate memmory for all files and directories entered as argv
    t_lists* all = (t_lists*)malloc(sizeof(t_lists));
    all->list = malloc(50*sizeof(char*));
    for(int i=0; i<50; i++){
        all->list[i] = malloc(100*sizeof(char));
    }
    
    // check for -at flags and assign entered argv values to all
    t_args* p_args = readArgs(argc, argv, all);

    // if there is only "./my_ls" or "./myls" + flags, it will check folder "." 
    if(argc == 1 || (argc == 2 && (p_args->a_arg == 1 || p_args->t_arg == 1))){
        strcpy(all->list[0], ".");
        all->n = 1;
    }
    // allocate memmory for files and dirs in order to seperate contents of "all" 
    t_lists* files = (t_lists*)malloc(sizeof(t_lists));
    files->list = malloc(50*sizeof(char*));
    for(int i=0; i<50; i++){
        files->list[i] = malloc(100*sizeof(char));
    }
    
    t_lists* dirs = (t_lists*)malloc(sizeof(t_lists));
    dirs->list = malloc(50*sizeof(char*));
    for(int i=0; i<50; i++){
        dirs->list[i] = malloc(100*sizeof(char));
    }

    // seperate all into files and dirs
    seperateList(files, dirs, all);

    // create an array of dirs that will include files and folders inside them
    t_lists** folders = (t_lists**)malloc(sizeof(t_lists*)*dirs->n);

    for(int i = 0; i < dirs->n; i++){
        folders[i] = readDir(dirs->list[i], p_args); // readDir functions reads the contents of the directory and 
                                                     // returns a pointer to it 
        folders[i]->dirName = dirs->list[i];
        
        if(p_args->t_arg == 0){ 
            // Sort by lexicographical order
            sortByLex(folders[i]);
        } else{
            // Sort by time since last modification
            sortByTime(folders[i]);
        }
    }

    if(p_args->t_arg == 0){ 
        // Sort by lexicographical order
        sortByLex(files);
    } else{
        // Sort by time since last modification
        sortByTime(files);
    }
    if(files->n > 0){
        print_files(files);
        if(dirs->n != 0)
            printf("\n");
    }

    if(dirs->n == 1 && files->n == 0){
        print_files(folders[0]);
    } else{
        print_folder(dirs->n, folders, p_args->t_arg);
    }

    // free up allocated memory
    for(int i = 0; i < dirs->n; i++){
        for(int j = 0; j < 20; j++){
            free(folders[i]->list[j]);
        }
        free(folders[i]->list);
        free(folders[i]);
    }
    free(folders);

    for(int i = 0; i < 50; i++){
        free(all->list[i]);
        free(files->list[i]);
        free(dirs->list[i]);
    }

    free(all->list);
    free(files->list);
    free(dirs->list);
    free(all);
    free(files);
    free(dirs);
    free(p_args); 
    return 0;
}