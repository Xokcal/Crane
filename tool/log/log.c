//
// Created by 26432 on 2026/8/26.
//
#include <stdio.h>
#include <time.h>
#include "../String/String.h"
#include <stdlib.h>

static char* curr_time_pattern(){
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char *buf = (char*)malloc(80 * sizeof(char));
    strftime(buf , 80 , "%Y-%m-%d %H:%M:%S" , t);
    return buf;
}

void LOG_INFO(char *log , char *topic){
    char* curr_time = curr_time_pattern();
    printf("[%s] [INFO]---[TOOT_LOG_LOG_C_STATEMENT]-[LOG_INFO]-[%s] %s\n" , curr_time , topic ,log);
    free(curr_time);
}

void LOG_ERROR(char *log , char *topic){
    char* curr_time = curr_time_pattern();
    printf("[%s] [ERROR]---[TOOT_LOG_LOG_C_STATEMENT]-[LOG_ERROR]-[%s] %s\n" , curr_time , topic ,log);
    free(curr_time);
}

void LOG_INFO_CHAR(char log , char *topic){
    char* curr_time = curr_time_pattern();
    printf("[%s] [INFO]---[TOOT_LOG_LOG_C_STATEMENT]-[LOG_INFO_CHAR]-[%s] %c\n" , curr_time , topic ,log);
    free(curr_time);
}

void LOG_ERROR_CHAR(char log , char *topic){
    char* curr_time = curr_time_pattern();
    printf("[%s] [ERROR]---[TOOT_LOG_LOG_C_STATEMENT]-[LOG_ERROR_CHAR]-[%s] %c\n" , curr_time , topic ,log);
    free(curr_time);
}
void LOG_INFO_INT(int log , char *topic){
    char* curr_time = curr_time_pattern();
    printf("[%s] [INFO]---[TOOT_LOG_LOG_C_STATEMENT]-[LOG_INFO_CHAR]-[%s] %d\n" , curr_time , topic ,log);
    free(curr_time);
}

void LOG_ERROR_INT(int log , char *topic){
    char* curr_time = curr_time_pattern();
    printf("[%s] [ERROR]---[TOOT_LOG_LOG_C_STATEMENT]-[LOG_ERROR_CHAR]-[%d] %c\n" , curr_time , topic ,log);
    free(curr_time);
}