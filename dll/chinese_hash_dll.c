#define DLL_EXPORT __declspec(dllexport)

#include <string.h>//比较键
#include <stdlib.h>//分配内存
#include <stdio.h>//用于报错
#include "cJSON.h"//json文件存取

/*哈希表大小*/
#define SIZE 4673 

//哈希表结构
typedef struct _HASH
{
    int value;
    char key[4];
} Hash;

//节点为链表结构
typedef struct _HASH_CHAIN
{
    Hash Pair;
    struct _HASH_CHAIN * next;
} Chain;

Chain hash[SIZE];

DLL_EXPORT void __stdcall create_hash(void)
{
    for (int i = 0;i < SIZE;i++)
    {
        hash[i].next = NULL;
        hash[i].Pair.key[0] = '\0';
        hash[i].Pair.key[3] = '\0';
        hash[i].Pair.value = -1;
    }
}

int hash_func(char * ch)
{
    int n = (int)*(ch + 1) * (int)*(ch + 2);//计算哈希值

    return n % SIZE;
}

void insert(char * key, int value)
{
    int n = hash_func(key);
    key[3] = '\0';//去除换行符

    if (hash[n].Pair.value == -1)
    {
        strncpy(hash[n].Pair.key, key, 4);
        hash[n].Pair.value = value;
    }
    else if (hash[n].Pair.value != value)
    {
        Chain * new_node = (Chain *)malloc(sizeof(Chain));
        strncpy(new_node->Pair.key, key, 4);
        new_node->Pair.value = value;
        new_node->Pair.key[3] = '\0';
        new_node->next = NULL;

        Chain * current = &hash[n];
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new_node;
    }
}

DLL_EXPORT int __stdcall find_value(char * key)
{
    int n = hash_func(key);
    if (hash[n].Pair.value == -1)
    {
        return -1;
    }

    if (strcmp(key, hash[n].Pair.key) == 0)
    {
        printf("%d", hash[n].Pair.value);
        return hash[n].Pair.value;
    }
    else
    {
        Chain * now = hash[n].next;
        while (strcmp(key, now->Pair.key) != 0)
        {
            now = now->next;
        }
        printf("%d", now->Pair.value);
        return now->Pair.value;
    }
}

void free_hash_next()
{
    for (int i = 0; i < SIZE; i++)
    {
        Chain * current = hash[i].next;
        while (current != NULL)
        {
            Chain * temp = current;
            current = current->next;
            free(temp);
        }
        hash[i].next = NULL;
    }
}

void save_hash_to_json(const char * filename)
{
    cJSON * json_array = cJSON_CreateArray();
    for (int i = 0;i < SIZE;i++)
    {
        Chain * current = hash + i;
        while (current != NULL)
        {
            cJSON * json_object = cJSON_CreateObject();
            cJSON_AddStringToObject(json_object, "key", current->Pair.key);
            cJSON_AddNumberToObject(json_object, "value", current->Pair.value);
            cJSON_AddItemToArray(json_array, json_object);
            current = current->next;
        }
    }

    char * json_string = cJSON_Print(json_array);
    FILE * file = fopen(filename, "w");
    if (file) 
    {
        fprintf(file, "%s", json_string);
        fclose(file);
    }
    else
    {
        printf("error");
    }
    
    cJSON_Delete(json_array);
    free(json_string);
}

DLL_EXPORT void __stdcall read_hash_from_json(const char *filename) 
{
    FILE *file = fopen(filename, "r");
    if (!file) 
    {
        fprintf(stderr, "Could not open file %s for reading\n", filename);
        return;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *json_string = (char *)malloc((length + 1) * sizeof(char));
    if (json_string) 
    {
        fread(json_string, 1, length, file);
        json_string[length] = '\0';
    }
    fclose(file);

    cJSON *json_array = cJSON_Parse(json_string);
    if (!json_array) 
    {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        free(json_string);
        return;
    }

    int array_size = cJSON_GetArraySize(json_array);
    for (int i = 0; i < array_size; i++) 
    {
        cJSON *json_object = cJSON_GetArrayItem(json_array, i);
        if (json_object) 
        {
            const cJSON *key = cJSON_GetObjectItemCaseSensitive(json_object, "key");
            const cJSON *value = cJSON_GetObjectItemCaseSensitive(json_object, "value");

            if (value->valueint != -1) 
            {
                int index = hash_func(key->valuestring); 
                Chain *current = &hash[index];

                if (current->Pair.value != -1) 
                { // 如果当前位置已被占用，找到链表末尾
                    while (current->next != NULL) 
                    {
                        current = current->next;
                    }
                    current->next = (Chain *)malloc(sizeof(Chain));
                    current = current->next;
                }

                strncpy(current->Pair.key, key->valuestring, 4);
                current->Pair.value = value->valueint;
                current->next = NULL;
            }
        }
    }

    cJSON_Delete(json_array);
    free(json_string);
}

char ** read_chi(char ** chi, FILE * fp)
{
    int i = 0;
    while (fgets(chi[i], 5, fp) != NULL)
    {
        i++;
        if (i == 3500)
            break;
    }

    fclose(fp);

    return chi;
}

DLL_EXPORT int * __stdcall FFF(char * ch)
{
    if (*ch == '\0')
    {
        fprintf(stderr, "ERRPR");
        exit(EXIT_FAILURE);
    }

    int n = strlen(ch) / 3;

    int * word = (int *)malloc(n * sizeof(int));
    char * ah = (char*)calloc(4, sizeof(char));
    for (int i = 0;i < n;i++)
    {
        memcpy(ah, ch + i * 3, 3);
        word[i] = find_value(ah);
    }
    free(ah);

    return word;
}

DLL_EXPORT void __stdcall font_free(int * n)
{
    free(n);
}

DLL_EXPORT int __stdcall strlen_use(char * ch)
{
    return strlen(ch) / 3;
}