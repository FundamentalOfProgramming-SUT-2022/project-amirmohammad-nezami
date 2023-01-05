#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define maxn 10000

struct stat st = {0};

char command[maxn];

void make_dir(char path[]) {
    int sz = strlen(path) , k = 0;
    char dir[maxn] = {};
    for (int i = 1; i < sz; ++i) {
        int j = i; 
        while(j < sz && path[j] != '/') j++;
        while(i < j) dir[k++] = path[i++];
        if(i == sz) {
            FILE *fp; 
            fp = fopen(dir , "r");
            if(fp != NULL) {
                printf("ERORR! file mojood hast!\n");
                return;
            }
            else {
                fp = fopen(dir , "w");
                fclose(fp);
            }
            return ;
        } 
        mkdir(dir , 0700);
        dir[k++] = '/';
    }
}

int edit_exist(char path[] , char string[] , char line[]) {
    int sz = strlen(path) , k = 0 , j = 0; 
    char dir[maxn] = {} , dir2[maxn] = {};
    for (int i = 1; i < sz; ++i) {
        dir[k++] = path[i];
        if(path[i] == '/') j = i;
    }
    FILE *fp , *fc;
    fp = fopen(dir , "r");
    if(fp == NULL) return 0;

    for (int i = 0; i <= j; ++i) dir2[i] = path[i];
    strcat(dir2 , "target.txt");
    fp = fopen(dir , "w");
    fc = fopen(dir2 , "w");
    int L = line[0] - '0' , P = line[2] - '0' , cntline = 0;
    char x = fgetc(fp);
    while(x != NULL) {
        if(x == '\n') cntline++;
        if(cntline == L - 1) {
            int is = (cntline == 0);
            for (int i = 0; i <= P - is; ++i) {
                fprintf(fc , "%c" , x);
                x = fgetc(fp);
            }
            int SZ = strlen(string);
            for (int i = 0; i < SZ; ++i) {
                fprintf(fc , "%c" , string[i]);
            }
            while(x != '\n') {
                fprintf(fc , "%c" , x);
                x = fgetc(fp);
            }
        }
        else {
            fprintf(fc , "%c" , x);
        }
    }
    fclose(fp);
    fclose(fc);
    

}

void create_file() {
    char option[maxn], path[maxn];
    scanf("%s" , option);
    scanf("%s" , path);

    make_dir(path);
}

void insert_string() {
    char op1[maxn] , path[maxn] , op2[maxn] , string[maxn] , op3[maxn] , line[maxn];
    scanf("%s%s%s%s%s%s" , op1 , path , op2 , string , op2 , line);
    if(!edit_exist(path , string , line)) {
        printf("ERROR! file mojood nist!\n");
        return ;
    }
    
}

int main() {
    mkdir("root" , 0700);
    scanf("%s" , command);
    while(strcmp(command , "exit") != 0) {
        if(strcmp(command , "createfile") == 0) {
            create_file();
        }
        else if(strcmp(command , "insertstr") == 0) {
            insert_string();
        }
    }
}
