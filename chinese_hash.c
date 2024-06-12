#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"// save/load .json

/*hashmap size*/
#define SIZE 4673 

typedef struct _HASH
{
    int value;
    char key[4];
} Hash;

typedef struct _HASH_CHAIN
{
    Hash Pair;
    struct _HASH_CHAIN * next;
} Chain;

//初始化哈希表
void create_hash(Chain *);

//哈希函数
int hash_func(char *);

//插入元素
void insert(char *, int, Chain *);

//查找元素, 返回值
int find_value(char *, Chain *);

//释放内存
void free_hash_next(Chain *);


//保存哈希表至.json文件
void save_hash_to_json(Chain * , const char *);

//从.json文件读取哈希表
void read_hash_from_json(Chain *, const char *);

//从.txt读取汉字
char ** read_chi(char **, FILE *);

int main(void)
{
    Chain hash[SIZE];//静态哈希表
    create_hash(hash);//初始化
    
    read_hash_from_json(hash, "chinese_hash.json");//从chinese_hash.json中读取

    FILE * fp;//创建文件指针
    if ((fp = fopen("1.txt", "r")) == NULL)//打开1.txt文件
    {
        fprintf(stderr, "1.txt error");
        exit(EXIT_FAILURE);
    }

    char ** chi = (char **)malloc(3500 * sizeof(char *));//为汉字数组分配内存
    for (int i = 0;i < 3500;i++)
    {
        chi[i] = (char *)calloc(4, sizeof(char));
    }
    read_chi(chi, fp);//从1.txt中获取key

    for (int i = 0;i < 3500;i++)
    {
        //insert(chi[i], i, hash);//向哈希表插入元素
        chi[i][3] = '\0';
        printf("%d ", find_value(chi[i], hash));
    }

    free_hash_next(hash);//释放链表节点内存

    
    free(chi);//释放汉字数组内存

    return 0;
}

void create_hash(Chain * hash)
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

void insert(char * key, int value, Chain * hash)
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

int find_value(char * key, Chain * hash)
{
    int n = hash_func(key);
    if (hash[n].Pair.value == -1)
    {
        return -1;
    }

    if (strcmp(key, hash[n].Pair.key) == 0)
    {
        return hash[n].Pair.value;
    }
    else
    {
        Chain * now = hash[n].next;
        while (strcmp(key, now->Pair.key) != 0)
        {
            now = now->next;
        }
        return now->Pair.value;
    }
}

void free_hash_next(Chain * hash)
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

void save_hash_to_json(Chain * hash, const char * filename)
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

void read_hash_from_json(Chain * hash, const char *filename) 
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
