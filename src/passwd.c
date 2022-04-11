/*
 * @Descripttion: 
 * @version: 0.1.0
 * @Author: PandaYoung
 * @Date: 2022-04-10 01:51:03
 */
#include <string.h>
#include <conio.h>
#include "../inc/list.h"

void HiddenInput(char *passwd)
{
    char ch = 0;
    int len = 0;
    while(ch = getch())
    {
        if(len >= 20 || ch == 13) { // 密码输入过长或enter结束输入
            break;
        }

        if(ch != '\b') { // 回车删除字符
            if(!((ch<='Z'&&ch>='A')||(ch<='z'&&ch>='a')||(ch<='9'&&ch>='0'))) {
                continue; // 密码仅由大小写字母和数字组成
            }
            passwd[len] = ch;
            printf("*");
            len++;
        } else {
            printf("\b \b");
            len--;
        }
    }
    passwd[len] = '\0';
    printf("\n");
}

int verify_passwd()
{
    printf("Please input password.\n");
    char passwd[20];
    int try_num = 0;
    while (try_num != TRY_TIMES) {
        try_num++;
        HiddenInput(passwd);
        if (strcmp(passwd, "hello") == 0) {
            return EOK;
        }
        printf("Mismatch! Please reinput password. %d/%d\n", try_num, TRY_TIMES);
    }
    return EPASSWD;
}