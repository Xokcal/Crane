//
// Created by 26432 on 2026/8/10.
//

#include <stdio.h>
#include "String.h"
#include <stdlib.h>
#include <string.h>

/**
 * 创建String结构体
 * */
String* create_string(char* str){
    String *string = (String*)malloc(sizeof(String));
    string->length = strlen(str);
    string->str = malloc((string->length + 1) * sizeof(char));
    for (int i = 0; i < string->length + 1; ++i)
        string->str[i] = str[i];
    return string;
}

/**
 * 结合String与char*类型字符串，在末尾。
 * */
void combine_tail(String *origin , char* nstr){
    int origin_len = strlen(origin->str);
    int nstr_len = strlen(nstr);
    char *combine_r_str = malloc((origin_len + nstr_len + 1) * sizeof(char));
    for (int i = 0; i < origin_len; ++i)
        combine_r_str[i] = origin->str[i];
    for (int i = 0; i < nstr_len; ++i)
        combine_r_str[origin_len + i] = nstr[i];
    combine_r_str[(origin_len + nstr_len)] = '\0';
    free(origin->str);
    origin->str = combine_r_str;
    origin->length = (origin_len + nstr_len);
}


void combine_tail_char(String *origin , char nstr){
    // we   +    l
    origin->length++; //2 -> 3
    char* temp =  (char*)malloc((origin->length + 1) * sizeof(char)); // malloc(4)
    for (int i = 0; i < origin->length - 1; ++i)
        temp[i] = origin->str[i];
    temp[origin->length - 1] = nstr;
    temp[origin->length] = '\0';
    free(origin->str);
    origin->str = (char*)malloc((origin->length + 1) * sizeof(char));
    for (int i = 0; i < origin->length ; ++i)
        origin->str[i] = temp[i];
    origin->str[origin->length] = '\0';
    free(temp);
}

/**
 * 替换字符串String中的一串换成新传入的*/
 void replace_str(String *string , char* old , char* new){
    // |helloworldmarr aaa skxmsoimcas aaa simascoaijecioac aaa|     old->aaa  new -> b
    if(string->length < strlen(old))return;
    long string_len = strlen(string->str);
    char temp_old[strlen(old) + 1];
    int place = 0;
    String *string_r = create_string("");
    for (int i = 0; i < string_len; ++i) {
        for (int j = 0; j < strlen(old); ++j)
            temp_old[j] = string->str[i + j];
        temp_old[strlen(old)]       = '\0';
        String *temp_old_string         = create_string(temp_old);
        if(i        == string_len - strlen(old)){
            if(!compare(temp_old_string , old)){
                for (int j = i; j < string->length; ++j)
                    combine_tail_char(string_r , string->str[j]);
            } else if(compare(temp_old_string , old))
                combine_tail(string_r , new);
            break;
        }
        int is_compare      = compare(temp_old_string , old);
        if(is_compare){
            place++;
            i += strlen(old) - 1;
            string_free(temp_old_string);
            combine_tail(string_r , new);
            continue;
        }
        combine_tail_char(string_r , string->str[i]);
        string_free(temp_old_string);
    }
    copy_string(string_r , string);
    string_free(string_r);
 }

/**
 * 比较String与字符串char*是否不同*/
int compare(String *string , char* compare_c){
    long string_len = strlen(string->str);
    long compare_len = strlen(compare_c);
    if(string_len != compare_len)
        return 0;
    for (int i = 0; i < string_len; ++i) {
        if(string->str[i] != compare_c[i])
            return 0;
    }
    return 1;
}

/**
 * 删除通过指定索引index*/
 void delete_indexof(String *string , int index){
     String *temp_string = create_string("");
    for (int i = 0; i < string->length; ++i) {
        if(i != index)
            combine_tail_char(temp_string , string->str[i]);
        continue;
    }
    free(string->str);
    string->str = malloc((temp_string->length + 1) * sizeof(char));
    for (int i = 0; i <temp_string->length; ++i) {
        string->str[i] = temp_string->str[i];
    }
    string->str[temp_string->length] = '\0';
    string->length =        temp_string->length;
    string_free(temp_string);
 }

/**
 * 通过索引查询String里面str的此索引char字符*/
char indexof(String *string , int index){
    if(string->length < index)return '\0';
    return string->str[index];
}

/**
 * 截取字符串，从指定开始直到最后*/
String *substring_to_end(String *string , int start){
    int is_start_substring = 0;
    String *string_r = create_string("");
    for (int i = 0; i < string->length; ++i) {
        if(i == start)is_start_substring = 1;
        if(is_start_substring)
            combine_tail_char(string_r , string->str[i]);
        else continue;
    }
    return string_r;
}

/**
 * 复制String里面的内容给另一个String*/
 void copy_string(String* copyer , String *be_copyer){
    free(be_copyer->str);
    be_copyer->str = malloc((copyer->length + 1) * sizeof(char));
    for (int i = 0; i < copyer->length; ++i)
        be_copyer->str[i] = copyer->str[i];
    be_copyer->str[copyer->length]      = '\0';
    be_copyer->length       = copyer->length;
 }

/**
 * 截取字符串，通过开头索引和结尾索引*/
String *substring(String *string , int start , int end){
    if(start >  end)return create_string("");
    if(end > string->length)return create_string("");
    char* subr = (char *)malloc((end - start + 1) * sizeof(char));
    for (int i = 0; i < end - start; ++i)
        subr[i] = string->str[start + i];
    subr[end - start + 1] = '\0';
    String *string_r        = create_string(subr);
    free(subr);
    return string_r;
}

/**
 * 删除String所有的字符串为空"\0"*/
void delete_all(String *string){
    free(string->str);
    string->str = malloc(sizeof(char));
    for (int i = 0; i < 1; ++i)
        string->str[i] = '\0';
    string->length = 0;
}

/**
 * 通过字符串，查找这个字符串，
 * 在原字符串里面的所有的开头位置
 * 注意：返回的int* 不需要了必须free掉*/
int *index_strof(String *string , char *target){
    int target_len = strlen(target);
    int *index_place = (int*)malloc((string->length + 1) * sizeof(int));
    int count = 0;
    for (int i = 0; i < string->length; ++i) {
        String *temp_window_str = create_string("");
        for (int j = 0; j < target_len; ++j)
            combine_tail_char(temp_window_str,string->str[i + j]);
        if(compare(temp_window_str , target))
            index_place[count++] = i;
    }
    return index_place;
}

/**
 * 去除字符串String的两端空格*/
void string_trim(String *string){
    int is_in_hand = 1;
    int is_in_tail;
    String *temp_string = create_string("");
    for (int i = 0; i < string->length; ++i) {
        is_in_tail = 1;
        for (int j = i; j < string->length; ++j) {
            if(string->str[j] != ' ') {
                is_in_tail = 0;
                break;
            }
        }
        if(is_in_tail)break;
        if(string->str[i] != ' ')is_in_hand = 0;
        if(!is_in_hand)
            combine_tail_char(temp_string , string->str[i]);
    }
    copy_string(temp_string , string);
    string_free(temp_string);
}

/**
 * 释放String内存*/
void string_free(String *string){
    free(string->str);
    free(string);
}

/**
 * 通过指定字符拆分字符串*/

// "论点;论据;论文;查看"  --- split ---> ";" --> "论点"  "论据"  ...
 Splitor *split(String *string , char *split_c){
    if(string->length < strlen(split_c)){
        Splitor *splitor = (Splitor*)malloc(sizeof(Splitor));
        splitor->count = 0;
        splitor->splits = NULL;
        return splitor;
    }
    int *index_place = (int*)malloc((string->length + 1) * sizeof(int));
    int split_len = strlen(split_c);
    int count = 0;
    for (int i = 0; i < string->length; ++i) {
        String *window = create_string("");
        for (int j = 0; j < split_len; ++j)
            combine_tail_char(window , string->str[i + j]);
        if(compare(window , split_c))
            index_place[count++] = i;
        string_free(window);
    }
    String **split_arr_r = (String **)malloc((count + 1) * sizeof(String));
    int split_arr_count = 0;
    int is_enter_next;
    int is_end = 0;
    String *temp = create_string("");
    for (int i = 0; i < string->length; ++i) {
        is_enter_next = 0;
        for (int j = i; j < string->length; ++j) {
            for (int k = 0; k < count; ++k) {
                if(i == index_place[k]) {
                    String *num = create_string("");
                    copy_string(temp , num);
                    split_arr_r[split_arr_count++] = num;
                    is_enter_next = 1;
                    delete_all(temp);
                    if(i == index_place[count - 1]){
                        String *end_num = create_string("");
                        for (int j = i + split_len; j < string->length; ++j)
                            combine_tail_char(end_num , string->str[j]);
                        split_arr_r[split_arr_count++] = end_num;
                        is_end = 1;
                        break;
                    }
                    i = i + split_len - 1;
                    break;
                }
            }
            if(is_enter_next)break;
        }
        if(is_end)break;
        if(is_enter_next)continue;
        if(i == string->length - 1){
            split_arr_r[split_arr_count++] = temp;
            break;
        }
        combine_tail_char(temp , string->str[i]);
    }
    Splitor *splitor = (Splitor*)malloc(sizeof(Splitor));
    splitor->splits = split_arr_r;
    splitor->count = split_arr_count;
    return splitor;
 }

 /**
  * 释放Splitor处理器*/
 void splitor_free(Splitor *splitor){
     for (int i = 0; i < splitor->count; ++i)
         free(splitor->splits[i]);
     free(splitor->splits);
     free(splitor);
 }

 /**
  * 将String**的每个元素拼接成一个字符串，
  * 并且用指定字符串连接*/
 String *join(String **strings , int join_count , char* join_str){
     String *str_r = create_string("");
     for (int i = 0; i < join_count; ++i) {
         if(i == join_count - 1){
             combine_tail(str_r , strings[i]->str);
             break;
         }
         combine_tail(str_r , strings[i]->str);
         combine_tail(str_r , join_str);
     }
     return str_r;
 }

/**
* 删除String中的指定char* 字符串*/
void delete_str_strof(String* string , char* target){
    String *str_r = create_string("");
    int target_len = strlen(target);
    String *window = create_string("");
    for (int i = 0; i < string->length; ++i) {
        if(i > string->length - target_len){
            for (int j = i; j < string->length; ++j) {
                combine_tail_char(str_r , string->str[j]);
            }
        }
        for (int j = 0; j < target_len; ++j) {
            combine_tail_char(window , string->str[i + j]);
        }
        if(compare(window , target)){
            i += target_len;
            delete_all(window);
            continue;
        }
        combine_tail_char(str_r , string->str[i]);
        delete_all(window);
    }
    string_free(window);
    copy_string(str_r , string);
}

/**
 * 打印字符串*/
void string_println(String *string){
    printf("%s\n" , string->str);
}