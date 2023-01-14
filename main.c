#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
// #include "windows.h"
// #include "string.h"
// #include <direct.h>

#define maxn 10000

struct stat st = {0};

char command[maxn];

void Error() {
    printf("Invalid command\n");
    exit(0);
}

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

int add(char path[] , char string[] , char line[]) {
    // check exitense
    int sz = strlen(path) , k = 0; 
    char dir[maxn] = {};
    for (int i = 1; i < sz; ++i) {
        dir[k++] = path[i];
    }
    FILE *fp;
    fp = fopen(dir , "r");
    if(fp == NULL) return 0;

    // add
    int cnt = 0;
    char value[maxn] = {};

    char x = fgetc(fp);
    while(x != EOF) {
        value[cnt++] = x;
        x = fgetc(fp);
    }

    fp = fopen(dir , "w");
    int L = line[0] - '0' , P = line[2] - '0' , cntline = 0 , SZ = strlen(string);
    if(cnt == 0) {
        for (int j = 0; j < SZ; ++j) {
            fprintf(fp , "%c" , string[j]);
        }
    }
    else {
        int edited = 0;
        for (int i = 0; i < cnt; ++i) {
            if(value[i] == '\n') cntline++;
            if(cntline == L - 1) {

                if(cntline != 0) fprintf(fp , "%c" , value[i++]);
                for (int j = 0; j < P; ++j) {
                    if(i >= cnt || value[i] == '\n') {
                        fprintf(fp , "%c" , ' ');
                    }
                    else fprintf(fp , "%c" , value[i++]);
                }
                for (int j = 0; j < SZ; ++j) {
                    fprintf(fp , "%c" , string[j]);
                }
                while(i < cnt && value[i] != '\n') {
                    fprintf(fp , "%c" , value[i++]);
                }
                i--;
                edited = 1;
            }
            else if(i < cnt) fprintf(fp , "%c" , value[i]);
        }
        if(!edited) {

            while(cntline < L - 1) {
                cntline++;
                fprintf(fp , "%c" , '\n');
            }
            for (int i = 0; i < P; ++i) {
                fprintf(fp , "%c" , ' ');
            }
            for (int i = 0; i < SZ; ++i) {
                fprintf(fp , "%c" , string[i]);
            }
        }
    }
    fclose(fp);
    return 1;
}

int rem(char path[] , char line[] , int sz , char type[]) {
    // printf("HELLLLL\n");
    int SZ = strlen(path) , k = 0; 
    char dir[maxn] = {};
    for (int i = 1; i < SZ; ++i) {
        dir[k++] = path[i];
    }
    FILE *fp;
    fp = fopen(dir , "r");
    if(fp == NULL) return 0;

    if(!sz) return 1;

    int cnt = 0;
    char value[maxn] = {};

    char x = fgetc(fp);
    while(x != EOF) {
        value[cnt++] = x;
        x = fgetc(fp);
    }

    fp = fopen(dir , "w");
    int L = line[0] - '0' , P = line[2] - '0' , cntline = 0;
    int l_bad , r_bad;
    // printf("%d %d\n" , L , P);
    for (int i = 0; i < cnt; ++i) {
        if(value[i] == '\n') cntline++;
        if(cntline == L - 1) {
            int id = i + P + (cntline != 0);
            if(type[1] == 'f') {
                l_bad = id , r_bad = id + sz;
            }
            else {
                r_bad = id + 1 , l_bad = id - sz + 1;
            }
            break;
        }
    }
    // printf("%d %d    %d\n" , l_bad , r_bad , cnt);
    // l_bad = max(l_bad , 0);
    // r_bad = min(r_bad , SZ);

    for (int i = 0; i < cnt; ++i) {
        if(l_bad <= i && i < r_bad) continue;
        else fprintf(fp , "%c" , value[i]);
    }
    fclose(fp);
    return 1;
}

void create_file() {
    char option[maxn], path[maxn];
    scanf("%s" , option);
    if(strcmp(option , "-file") == 0) {
        scanf("%s" , path);
        make_dir(path);
    }
    else Error();
}

void insert_string() {
    char op1[maxn] , path[maxn] , op2[maxn] , string[maxn] , op3[maxn] , line[maxn];
    scanf("%s%s%s%s%s%s" , op1 , path , op2 , string , op3 , line);
    if(!add(path , string , line)) {
        printf("ERROR! file mojood nist!\n");
        return ;
    }
}

void remove_string() {
    int sz;
    char op1[maxn] , path[maxn] , op2[maxn] , line[maxn] , op3[maxn] , op4[maxn];
    scanf("%s%s%s%s%s%d%s" , op1 , path , op2 , line , op3 , &sz , op4);
    if(!rem(path , line , sz , op4)) {
        printf("ERROR! file mojood nist!\n");
    }
}

int get_string(char res[] , char dir[] , int L , int P , int sz , char type[]) {
    FILE *fp;
    fp = fopen(dir , "r");
    if(!sz) return ;

    int cnt = 0;
    char value[maxn] = {};

    char x = fgetc(fp);
    while(x != EOF) {
        value[cnt++] = x;
        x = fgetc(fp);
    }

    int cntline = 0 , l , r;
    for (int i = 0; i < cnt; ++i) {
        if(value[i] == '\n') cntline++;
        if(cntline == L - 1) {
            int id = i + P + (cntline != 0);
            if(type[1] == 'f') {
                l = id , r = id + sz;
            }
            else {
                r = id + 1 , l = id - sz + 1;
            }
            break;
        }
    }

    int counter = 0;
    for (int i = l; i < r && i < cnt; ++i) {
        res[i - l] = value[i]; counter++;
    }
    return counter;
}

void copy() {
    int L , P , sz;
    char op1[maxn] , path[maxn] , op2[maxn] , pos[maxn] , op3[maxn] , op4[maxn] , res[maxn];
    scanf("%s%s%s%d%s%d%s%d%s" , op1 , path , op2 , &L , pos , &P , op3 , &sz , op4);
    int SZ = strlen(path) , k = 0; 
    char dir[maxn] = {};
    for (int i = 1; i < SZ; ++i) {
        dir[k++] = path[i];
    }
    FILE *fp;
    fp = fopen(dir , "r");
    if(fp == NULL) {
        printf("ERROR! file mojood nist!\n");
        return;
    } 
    int cnt = get_string(res , dir , L , P , sz , op4) + 1;
    // HGLOBAL hMem =  GlobalAlloc(GMEM_MOVEABLE, cnt);
    // memcpy(GlobalLock(hMem), res, cnt);
    // GlobalUnlock(hMem);
    // OpenClipboard(0);
    // EmptyClipboard();
    // SetClipboardData(CF_TEXT, hMem);
    // CloseClipboard();
}

int cat() {
    char path[maxn];
    scanf("%s" , path);
    int sz = strlen(path) , k = 0, cnt = 0; 
    char dir[maxn] = {} , value[maxn] = {};
    for (int i = 1; i < sz; ++i) {
        dir[k++] = path[i];
    }
    FILE *fp;
    fp = fopen(dir , "r");
    if(fp == NULL) return 0;

    char x = fgetc(fp);
    while(x != EOF) {
        value[cnt++] = x;
        x = fgetc(fp);
    }

    for(int i = 0; i < cnt; ++i) {
        printf("%c" , value[i]);
    }
    printf("\n");
    return 1;
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
        else if(strcmp(command , "cat") == 0) {
            if(!cat()) printf("File mojood nist!\n");
        }
        else if(strcmp(command , "removestr") == 0) {
            remove_string();
        }
        scanf("%s" , command);
    }
    
}
