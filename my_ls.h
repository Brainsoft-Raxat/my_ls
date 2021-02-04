#ifndef MY_LS
#define MY_LS

typedef struct T_args{   
    int a_arg;  // flag a
    int t_arg;  // flag t
} t_args;

typedef struct T_lists{
    int n;          // number of elements in the list
    char* dirName;  // current directory name
    char** list;    // list of files and directories inside the current directory
} t_lists;

t_lists* readDir(char* dirName, t_args* p_args);

t_args* readArgs(int argc, char** argv, t_lists* all);

void seperateList(t_lists* files, t_lists* dirs, t_lists* all);

void swap(char* str1, char* str2);

void sortByLex(t_lists* list);

void sortFoldersByLex(int n, t_lists* folders[]);

void sortByTime(t_lists* list);

void print_files(t_lists* files);

void print_folder(int n, t_lists* folder[], int t_arg);

#endif