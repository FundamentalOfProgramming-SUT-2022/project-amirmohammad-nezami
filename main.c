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
 
int file_to_string(char value[] , char dir[] , char path[]) {
    int SZ = strlen(path) , k = 0; 
    for (int i = 1; i < SZ; ++i) {
        dir[k++] = path[i];
    }
    FILE *fp;
    fp = fopen(dir , "r");
    if(fp == NULL) return -1;

    int cnt = 0;
    char x = fgetc(fp);
    while(x != EOF) {
        value[cnt++] = x;
        x = fgetc(fp);
    }
    return cnt;
}

int scan_string(char string[]) {
    char x , value[maxn] = {}; 
    scanf("%c" , &x);
    if(x != 34) {
        scanf("%s" , value);
        string[0] = x;
        for (int i = 0; i < strlen(value); ++i) {
            string[i + 1] = value[i];
        }
        return strlen(value) + 1;
    }
    else {
        int cnt = 0;
        char bef = x , curr;
        scanf("%c" , &curr);
        while(curr != 34 || bef == 92) {
            string[cnt++] = bef = curr;
            scanf("%c" , &curr);
        }
        return cnt;
    }
}

int add(char path[] , char string[] , char line[]) {
    char value[maxn] = {} , dir[maxn] = {};
    int cnt = file_to_string(value , dir , path);
    if(cnt == -1) return 0;

    FILE *fp = fopen(dir , "w");
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
    char value[maxn] = {} , dir[maxn] = {};
    int cnt = file_to_string(value , dir , path);
    if(cnt == -1) return 0;

    if(!sz) return 1;

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
    FILE *fp = fopen(dir , "w");
    for (int i = 0; i < cnt; ++i) {
        if(l_bad <= i && i < r_bad) continue;
        else fprintf(fp , "%c" , value[i]);
    }
    fclose(fp);
    return 1;
}

int wild_card(int *sz , char string[]) {
    if(string[0] == '*') return 0;
    if(string[*sz - 1] == '*' && string[*sz - 2] != 92) return 1;
    int SZ = *sz;
    for (int i = 0; i < *sz; ++i) {
        if(string[i] == '*') {
            for (int j = i; j < *sz; ++j) {
                string[j - 1] = string[j];
            }
            SZ--;
        }
    }
    *sz = SZ;
    return 2;
}

int find(int sz , char string[] , char path[] , int res[][]) {
    char value[maxn] = {} , dir[maxn] = {};
    int cnt = file_to_string(value , dir , path) , n = -1;
    if(cnt == -1) return -1; //no file

    int type = wild_card(&sz , string) , SZ = 0 , word_cnt = 0;
    if(type == 0) {
        for (int i = 0; i < cnt; ++i) {
            int is = 1;
            if((!i && value[i] != ' ') || (value[i] != ' ' && value[i - 1] == ' ')) {
                word_cnt++;
            }
            for (int j = 1; j < sz; ++j) {
                is &= (i + j <= cnt && string[j] == value[i + j - 1]);
            }
            if(is && i && value[i - 1] != ' ' && value[i - 1] != '\n') {
                res[0][SZ] = i;
                res[1][SZ++] = word_cnt;
            }
        }
    }
    else if(type == 1) {
        for (int i = 0; i < cnt; ++i) {
            int is = 1;
            if((!i && value[i] != ' ') || (value[i] != ' ' && value[i - 1] == ' ')) {
                word_cnt++;
            }
            for (int j = 0; j < sz - 1; ++j) {
                is &= (i + j < cnt && string[j] == value[i + j]);
            }
            if(is && i + sz - 1 < cnt && value[i + sz - 1] != ' ' && value[i + sz - 1] != '\n') {
                res[0][SZ] = i;
                res[1][SZ++] = word_cnt;
            }
        }
    }
    else {
        for (int i = 0; i < cnt; ++i) {
            int is = 1;
            if((!i && value[i] != ' ') || (value[i] != ' ' && value[i - 1] == ' ')) {
                word_cnt++;
            }
            for (int j = 0; j < sz; ++j) {
                is &= (i + j < cnt && string[j] == value[i + j]);
            }
            if(is) {
                res[0][SZ] = i;
                res[1][SZ++] = word_cnt;
            }
        }
    }
    return SZ;
}

void change(char path[] , int id , int sz , char string[]) {
    char value[maxn] = {} , dir[maxn] = {};
    int cnt = file_to_string(value , dir , path);

    FILE *fp = fopen(dir , "w");
    for (int i = 0; i < id; ++i) {
        fprintf(fp , "%c" , value[i]);
    }
    for (int i = 0; i < strlen(string); ++i) {
        fprintf(fp , "%c" , string[i]);
    }
    for (int i = id + sz; i < cnt; ++i) {
        fprintf(fp , "%c" , value[i]);
    }
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

void find_string() {
    char op1[maxn] , string[maxn] , op2[maxn] , path[maxn] , op3[maxn];
    scanf("%s" , op1);
    int sz = scan_string(string) , n;
    scanf("%s%s%s" , op2 , path, op3);
    if(strcmp(op3 , "-at") == 0) scanf("%d" , &n);

    int res[2][maxn] = {};
    int SZ = find(sz , string , path , res);
    if(SZ == -1) printf("File mojood nist!\n");
    else if(SZ == 0) printf("String mojood nist\n");
    else {
        if(strcmp(op3 , "-at") == 0) {
            if(n > SZ) printf("-1\n");
            else printf("%d\n" , res[0][n - 1]);
        }
        else if(strcmp(op3 , "-count") == 0) {
            printf("%d\n" , SZ);
        }
        else if(strcmp(op3 , "-byword") == 0) {
            printf("%d\n" , res[1][n - 1]);
        }
        else if(strcmp(op3 , "-all") == 0) {
            for (int i = 0; i < SZ; ++i) printf("%d " , res[0][i]);
            printf("\n");
        }
        else {
            printf("%d\n" , res[0][0]);
        }
    }
}

void replace() {
    char op1[maxn] , st1[maxn] , op2[maxn] , st2[maxn] , op3[maxn] , path[maxn] , op4[maxn];
    scanf("%s" , op1);
    int sz = scan_string(st1) , n = 1;
    scanf("%s%s%s%s%s" , op2 , st2 , op3 , path , op4);
    if(strcmp(op4 , "-at") == 0) scanf("%d" , &n);

    int res[2][maxn] = {};
    int SZ = find(sz , st1 , path , res);
    if(SZ == -1) printf("File mojood nist!\n");
    else if(SZ == 0) printf("String mojood nist\n");

    int t = (strcmp(op4 , "-all") == 0 ? SZ : n);
    for (int i = t - 1; i >= 0; --i) {
        change(res[0][i] , sz , st2);
    }
}

int get_string(char res[] , char path[] , int L , int P , int sz , char type[]) {
    char value[maxn] = {} , dir[maxn] = {};
    int cnt = file_to_string(value , dir , path);
    if(cnt == -1) return -1;
    if(!sz) return 0;

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

// void copy() {
//     int L , P , sz;
//     char op1[maxn] , path[maxn] , op2[maxn] , pos[maxn] , op3[maxn] , op4[maxn] , res[maxn];
//     scanf("%s%s%s%d%s%d%s%d%s" , op1 , path , op2 , &L , pos , &P , op3 , &sz , op4);
//     int SZ = strlen(path) , k = 0; 
//     char dir[maxn] = {};
//     for (int i = 1; i < SZ; ++i) {
//         dir[k++] = path[i];
//     }
//     FILE *fp;
//     fp = fopen(dir , "r");
//     if(fp == NULL) {
//         printf("ERROR! file mojood nist!\n");
//         return;
//     } 
//     int cnt = get_string(res , dir , L , P , sz , op4) + 1;
//     // const char* output = "Test";
//     // const size_t len = strlen(output) + 1;
//     // HGLOBAL hMem =  GlobalAlloc(GMEM_MOVEABLE, len);
//     // memcpy(GlobalLock(hMem), output, len);
//     // GlobalUnlock(hMem);
//     // OpenClipboard(0);
//     // EmptyClipboard();
//     // SetClipboardData(CF_TEXT, hMem);
//     // CloseClipboard();
// }

int cat() {
    char path[maxn];
    scanf("%s" , path);
    char dir[maxn] = {} , value[maxn] = {};
    int cnt = file_to_string(value , dir , path);

    if(cnt == -1) return 0;
    
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
        else if(strcmp(command , "find") == 0) {
            find_string();
        }
        scanf("%s" , command);
    }
    
}
