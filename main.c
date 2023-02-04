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

int count , sz_c[1000];
char command[maxn] , Com[1000][1000];

void Error() {
    printf("Invalid command\n");
    exit(0);
}

int to_int(char s[]) {
    int sz = strlen(s) , p = 1 , res = 0;
    for (int i = sz - 1; i >= 0; --i) {
        int dig = s[i] - '0';
        res += dig * p;
        p *= 10;
    }
    return res;
}

int fix_commands() {
    int id = 0 , t = 0 , sz = strlen(command);
    for (int i = 0; i < 1000; ++i) {
        memset(Com[i] , '\0' , sizeof(Com[i]));
        sz_c[i] = 0;
    }
    while(id < sz) {
        if(command[id] == ' ' || command[id] == '\n') {
            id++;
            continue;
        }

        if(command[id] == 34) {
            int j = id + 1;
            while(j < sz && (command[j] != 34 || command[j - 1] == 92)) {
                if(command[j] == 34) sz_c[t]--;
                Com[t][sz_c[t]++] = command[j++];
            }
            id = j + 1;
        }
        else {
            int j = id;
            while(j < sz && command[j] != ' ' && command[j] != '\n') {
                Com[t][sz_c[t]++] = command[j++];
            }
            id = j;
        }
        // printf("%s %d \n" , Com[t] , t);
        t++;
    }
    // printf("\n%d\n" , t);
    return t;
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

// int scan_string(char string[]) {
//     char x , value[maxn] = {}; 
//     scanf("%c" , &x);
//     scanf("%c" , &x);
//     if(x != 34) {
//         scanf("%s" , value);
//         string[0] = x;
//         for (int i = 0; i < strlen(value); ++i) {
//             string[i + 1] = value[i];
//         }
//         // for (int i = 0; i < strlen(value) + 1; ++i) {
//         //     printf("%c" , string[i]);
//         // }
//         // printf("\n");
//         return strlen(value) + 1;
//     }
//     else {
//         int cnt = 0;
//         char bef = x , curr;
//         scanf("%c" , &curr);
//         while(curr != 34 || bef == 92) {
//             string[cnt++] = bef = curr;
//             scanf("%c" , &curr);
//         }
//         return cnt;
//     }
// }

int add(char path[] , char string[] , char line[]) {
    char value[maxn] = {} , dir[maxn] = {};
    int cnt = file_to_string(value , dir , path);
    if(cnt == -1) return 0;

    FILE *fp = fopen(dir , "w");
    int L = line[0] - '0' , P = line[2] - '0' , cntline = 0 , SZ = strlen(string);
    printf("%d %d\n" , L , P);
    printf("%s \n" , line);
    printf("%s \n" , string);
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

int find(int sz , char string[] , char path[] , int *res) {
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
                *(res + SZ) = i;
                *(res + maxn + SZ) = word_cnt;
                SZ++;
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
                *(res + SZ) = i;
                *(res + maxn + SZ) = word_cnt;
                SZ++;
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
                *(res + SZ) = i;
                *(res + maxn + SZ) = word_cnt;
                SZ++;
            }
        }
    }
    return SZ;
}

void change(char path[] , int id , int sz , char string[]) {
    char value[maxn] = {} , dir[maxn] = {};
    int cnt = file_to_string(value , dir , path);
    // printf("%s \n %d %d %d" , string , cnt , id , sz);
    FILE *fp;
    fp = fopen(dir , "w");
    for (int i = 0; i < id; ++i) {
        fprintf(fp , "%c" , value[i]);
    }
    for (int i = 0; i < strlen(string); ++i) {
        fprintf(fp , "%c" , string[i]);
    }
    for (int i = id + sz; i < cnt; ++i) {
        fprintf(fp , "%c" , value[i]);
    }
    fclose(fp);
}

void create_file() {
    if(strcmp(Com[1] , "-file") == 0) {
        make_dir(Com[2]);
    }
    else Error();
}

void insert_string() {
    if(!add(Com[2] , Com[4] , Com[6])) {
        printf("ERROR! file mojood nist!\n");
        return ;
    }
}

void remove_string() {
    int sz = to_int(Com[6]);
    if(!rem(Com[2] , Com[4] , sz , Com[7])) {
        printf("ERROR! file mojood nist!\n");
    }
}

void find_string() {
    int n = 0 , all = 0 , by_w = 0 , c = 0;
    for (int i = 0; i < count; ++i) {
        if(strcmp(Com[i] , "-at") == 0) n = to_int(Com[i + 1]);
        if(strcmp(Com[i] , "-all") == 0) all = 1;
        if(strcmp(Com[i] , "-by_word") == 0) by_w = 1;
        if(strcmp(Com[i] , "-count") == 0) c = 1;
    }

    if(n != 0 && all != 0) Error(); return;

    int res[2][maxn] = {};
    int SZ = find(sz_c[2] , Com[2] , Com[4] , (int *)res);
    if(SZ == -1) printf("File mojood nist!\n");
    else if(SZ == 0) printf("String mojood nist\n");
    else {
        if(c != 0) {
            printf("-count resault: %d\n" , SZ);
        }
        if(n != 0) {
            if(n > SZ) printf("-1\n");
            else {
                printf("%d\n" , res[by_w][n - 1]);
            }
        }
        else {
            if(all != 0) {
                for (int i = 0; i < SZ; ++i) printf("%d " , res[by_w][i]);
            }
            else {
                printf("%d\n" , res[by_w][0]);
            }
        }
    }
}

void replace() {
    int n = 0 , all = 0;
    for (int i = 0; i < count; ++i) {
        if(strcmp(Com[i] , "-at") == 0) n = to_int(Com[i + 1]);
    }

    int res[2][maxn] = {};
    int SZ = find(sz_c[2] , Com[2] , Com[6] , (int *)res);
    if(SZ == -1) {printf("File mojood nist!\n"); return; }
    else if(SZ == 0) {printf("String mojood nist\n"); return; }

    int t = (n == 0 ? (all == 0 ? 1 : all) : n);
    // printf("%d \n" , t);
    for (int i = t - 1; i >= 0; --i) {
        // printf("%d h\n" , res[0][i]);
        change(Com[6] , res[0][i] , sz_c[2] , Com[4]);
    }
    printf("Success\n");
}

void grep() {
    int op_c = 0 , op_l = 0 , st_id , file_id;
    if(strcmp(Com[1] , "c") == 0) op_c = 1;
    if(strcmp(Com[1] , "l") == 0) op_l = 1;
    for (int i = 0; i < count; ++i) if(strcmp(Com[i] , "-str") == 0) st_id = i + 1;
    for (int i = 0; i < count; ++i) if(strcmp(Com[i] , "-file") == 0) file_id = i + 1;

    
    for (int i = file_id; i < count; ++i) {
        int res[2][maxn] = {} , cnt_line = 0;
        int SZ = find(sz_c[st_id] , Com[st_id] , Com[i] , (int *)res);
        char value[maxn] = {} , dir[maxn] = {};
        int cnt = file_to_string(value , dir , Com[i]);
        if(SZ <= 0) continue;
        if(op_l == 1) printf("%s\n" , Com[i]);
        else {
            int bef = -1;
            if(!op_c) printf("%s :\n" , Com[i]);
            for (int j = 0; j < SZ; ++j) {
                int start = res[0][j];
                int l = start , r = start;
                while(l > 0 && value[l] != '\n') l--;
                while(r < cnt && value[r] != '\n') r++;
                if(r == bef) continue;
                bef = r;
                cnt_line++;
                if(op_c == 1) continue;
                if(value[l] == '\n') l++;
                printf("    ");
                for (int t = l; t < r; ++t) {
                    printf("%c" , value[t]);
                } 
                printf("\n");
            }
        }
    }
}

void cmp() {

    char value1[maxn] = {} , dir1[maxn] = {};
    int cnt1 = file_to_string(value1 , dir1 , Com[1]);
    char value2[maxn] = {} , dir2[maxn] = {};
    int cnt2 = file_to_string(value2 , dir2 , Com[2]);
    int line1 = 0 , line2 = 0;

    for (int i = 0; i < cnt1; ++i) line1 += (value1[i] == '\n');
    for (int i = 0; i < cnt2; ++i) line2 += (value2[i] == '\n');
    int mn = (line1 < line2 ? line1 : line2);
    int curr1 = 0 , curr2 = 0;
    // printf("%d %d\n" , cnt1 , cnt2);
    for (int i = 0; i < mn; ++i) {
        int r1 = curr1 , r2 = curr2 , is = 0;
        while(r1 < cnt1 && value1[r1] != '\n') r1++;
        while(r2 < cnt2 && value2[r2] != '\n') r2++;
        if(r1 - curr1 != r2 - curr2) {
            printf("================== #%d =================\n" , i + 1);
            for (int j = curr1; j <= r1; ++j) if(value1[j] != '\n') printf("%c" , value1[j]);
            printf("\n");
            for (int j = curr2; j <= r2; ++j) if(value2[j] != '\n') printf("%c" , value2[j]);
            printf("\n");
        }
        else {
            int is = 0;
            for (int j = 0; j <= r1 - curr1; ++j) {
                if(value1[curr1 + j] != value2[curr2 + j]) is = 1;
            }
            if(is) {
                printf("================== #%d =================\n" , i + 1);
                for (int j = curr1; j <= r1; ++j) if(value1[j] != '\n') printf("%c" , value1[j]);
                printf("\n");
                for (int j = curr2; j <= r2; ++j) if(value2[j] != '\n') printf("%c" , value2[j]);
                printf("\n");
            }
        }
        curr1 = r1 + 1 , curr2 = r2 + 1;
    }
    if(line1 > line2) {
        printf("<<<<<<<<<<<<<<<<< #%d - #%d <<<<<<<<<<<<<<<<<\n" , mn + 1 , line1);
        for (int i = curr1; i < cnt1; ++i) {
            printf("%c" , value1[i]);
        }
        printf("\n");
    }
    else if(line2 > line1) {
        printf(">>>>>>>>>>>>>>>>>> #%d - #%d >>>>>>>>>>>>>>>>\n" , mn + 1 , line2);
        for (int i = curr2; i < cnt2; ++i) {
            printf("%c" , value2[i]);
        }
        printf("\n");
    }
}

// int get_string(char res[] , char path[] , int L , int P , int sz , char type[]) {
//     char value[maxn] = {} , dir[maxn] = {};
//     int cnt = file_to_string(value , dir , path);
//     if(cnt == -1) return -1;
//     if(!sz) return 0;
//     int cntline = 0 , l , r;
//     for (int i = 0; i < cnt; ++i) {
//         if(value[i] == '\n') cntline++;
//         if(cntline == L - 1) {
//             int id = i + P + (cntline != 0);
//             if(type[1] == 'f') {
//                 l = id , r = id + sz;
//             }
//             else {
//                 r = id + 1 , l = id - sz + 1;
//             }
//             break;
//         }
//     }

//     int counter = 0;
//     for (int i = l; i < r && i < cnt; ++i) {
//         res[i - l] = value[i]; counter++;
//     }
//     return counter;
// }

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
    char dir[maxn] = {} , value[maxn] = {};
    // printf("joy \n %s" , Com[2]);
    int cnt = file_to_string(value , dir , Com[2]);

    if(cnt == -1) return 0;
    
    for(int i = 0; i < cnt; ++i) {
        printf("%c" , value[i]);
    }
    printf("\n");
    return 1;
}

int main() {
    mkdir("root" , 0700);

    fgets(command , maxn , stdin);
    count = fix_commands();

    while(strcmp(Com[0] , "exit") != 0) {
        // printf("joy\n");
        // printf("%s \n" , Com[0]);
        if(strcmp(Com[0] , "createfile") == 0) {
            create_file();
        }
        else if(strcmp(Com[0] , "insertstr") == 0) {
            // printf("joy\n");
            insert_string();
        }
        else if(strcmp(Com[0] , "cat") == 0) {
            // printf("zhoy\n");
            if(!cat()) printf("File mojood nist!\n");
        }
        else if(strcmp(Com[0] , "removestr") == 0) {
            remove_string();
        }
        else if(strcmp(Com[0] , "find") == 0) {
            find_string();
        }
        else if(strcmp(Com[0] , "replace") == 0) {
            replace();
        }
        else if(strcmp(Com[0] , "grep") == 0) {
            grep();
        }
        else if(strcmp(Com[0] , "compare") == 0) {
            cmp();
        }
        fgets(command , maxn , stdin);
        count = fix_commands();
    }
    
}
