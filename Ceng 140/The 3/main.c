#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the structures used in the code
typedef struct {
    char name[100];
    int size;
} Media;

typedef struct DomainFreqNode {
    char* name;
    int freq;
    struct DomainFreqNode* prev;
    struct DomainFreqNode* next;
} DomainFreqNode;

typedef struct {
    DomainFreqNode* head;
    DomainFreqNode* tail;
} DomainFreqList;

typedef struct CacheNode {
    Media media;
    struct CacheNode* prev;
    struct CacheNode* next;
    DomainFreqList* domainFreqList;
} CacheNode;

typedef struct {
    CacheNode* head;
    CacheNode* tail;
    int currentSize;
    int cacheLimit;
    int mediaCount;
} Cache;

// Function prototype"

Cache* createCache()
{
    int cache_size = 0,video_count=0,i=0,max_cacheSize;
    char scan[1000];
    
    Cache* cache=(Cache*)malloc(sizeof(Cache));
    CacheNode* first_cnode=(CacheNode*)malloc(sizeof(CacheNode));
    CacheNode* curr_cnode;
    CacheNode* prev_cnode;
    
    cache->head = first_cnode;
    cache->currentSize = 0;
    curr_cnode = first_cnode;
    prev_cnode = NULL;
    
    scanf("%d %d",&max_cacheSize,&video_count);
    fgets(scan,1000,stdin);
    
    for(i=0;i<video_count;i++){
        CacheNode* next_cnode=(CacheNode*)malloc(sizeof(CacheNode));
        DomainFreqList* f_node=(DomainFreqList*)malloc(sizeof(DomainFreqList));
        DomainFreqNode* first_ffnode=(DomainFreqNode*)malloc(sizeof(DomainFreqNode));
        DomainFreqNode* last_ffnode=(DomainFreqNode*)malloc(sizeof(DomainFreqNode));
        DomainFreqNode* curr_ffnode=(DomainFreqNode*)malloc(sizeof(DomainFreqNode));
        DomainFreqNode* prev_ffnode=(DomainFreqNode*)malloc(sizeof(DomainFreqNode));
        DomainFreqNode* sort_h=(DomainFreqNode*)malloc(sizeof(DomainFreqNode));
        DomainFreqNode* sort_t=(DomainFreqNode*)malloc(sizeof(DomainFreqNode));
        DomainFreqNode* next_ffnode = NULL;
        char* asd = (char*) malloc(1000*sizeof(char));
        char* dsa = (char*) malloc(1000*sizeof(char));
        char* str;
        
        fgets(scan,1000,stdin);
        
        str = strtok(scan," ");
        strcpy(asd, str);
        curr_cnode->media.name = asd;
        
        str = strtok(NULL," ");
        cache_size += atoi(str);
        curr_cnode->media.size = atoi(str);
        
        str = strtok(NULL," ");
        strcpy(dsa, str);
        first_ffnode->name = dsa;
        
        str = strtok(NULL," ");
        first_ffnode->freq = atoi(str);
        
        
        str = strtok(NULL," ");
        
        prev_ffnode = NULL;
        
        while(str != NULL){
            DomainFreqNode* temp_ffnode=(DomainFreqNode*)malloc(sizeof(DomainFreqNode));
            char* dsad = (char*) malloc(1000*sizeof(char));
            
            strcpy(dsad, str);
            temp_ffnode->name = dsad;
            str = strtok(NULL," ");
            temp_ffnode->freq = atoi(str);
            str = strtok(NULL," ");
            
            curr_ffnode = first_ffnode;
            
            
            while(curr_ffnode != NULL){
                if(curr_ffnode->freq > temp_ffnode->freq){
                    if(curr_ffnode->next == NULL){
                        curr_ffnode->next = temp_ffnode;
                        temp_ffnode->prev = curr_ffnode;
                        prev_ffnode = curr_ffnode;
                        curr_ffnode = temp_ffnode;
                        break;
                    }
                    prev_ffnode = curr_ffnode;
                    curr_ffnode = curr_ffnode->next;
                }
                else if( (curr_ffnode->freq < temp_ffnode->freq) || ( (curr_ffnode->freq == temp_ffnode->freq) && (strcmp(curr_ffnode->name,temp_ffnode->name) > 0) ) ){
                    if (curr_ffnode == first_ffnode){
                        temp_ffnode->next = curr_ffnode;
                        curr_ffnode->prev = temp_ffnode;
                        prev_ffnode = temp_ffnode;
                        first_ffnode = temp_ffnode;
                        break;
                    }
                    else{
                        prev_ffnode->next = temp_ffnode;
                        temp_ffnode->next = curr_ffnode;
                        temp_ffnode->prev = prev_ffnode;
                        curr_ffnode->prev = temp_ffnode;
                        prev_ffnode = temp_ffnode;
                        break;
                    }
                }
                else{
                    prev_ffnode = curr_ffnode;
                    curr_ffnode = curr_ffnode->next;
                }
            }
        
            
            if(str == NULL){
                last_ffnode = curr_ffnode;
            }
        }
        
        sort_h = first_ffnode;
        while(sort_h->prev != NULL){
            sort_h = sort_h->prev;
        }
        f_node->head = sort_h;
        sort_t = last_ffnode;
        while(sort_t->next != NULL){
            sort_t = sort_t->next;
        }
        f_node->tail = sort_t;
        
        
        
        curr_cnode->domainFreqList = (DomainFreqList*) malloc(sizeof(DomainFreqList));
        
        curr_cnode->domainFreqList->head = f_node->head;
        curr_cnode->domainFreqList->tail = f_node->tail;
        
        if(i<video_count-1){
            curr_cnode->next = next_cnode;
            next_cnode->prev = curr_cnode;
            curr_cnode = curr_cnode->next;
        }
        else{
            curr_cnode->next = NULL;
            cache->tail = curr_cnode;
            break;
        }
    }
    cache->mediaCount = video_count;
    cache->cacheLimit = max_cacheSize;
    cache->currentSize = cache_size;
    
    return cache;
}

void printCache(Cache* cache)
{
    CacheNode* curr;
    DomainFreqNode* curr_ff;
    
    curr = cache->head;
    curr_ff = curr->domainFreqList->head;
    
    if(curr == NULL){
        printf("The Cache is Empty");
    }
    else{
        printf("-------------- Cache Information --------------\n");
        printf("Cache Limit: %d KB\n",cache->cacheLimit);
        printf("Current Size: %d KB\n",cache->currentSize);
        printf("Media Count: %d\n",cache->mediaCount);
        printf("Cache Media Info:\n");
        while(curr != NULL){
            printf("    Name: %s    Size: %d KB\n",curr->media.name,curr->media.size);
            printf("    ------- Media Domain Frequencies -------\n");
            while(curr_ff != NULL){
                printf("        Name: %s    Freq: %d\n",curr_ff->name,curr_ff->freq);
                curr_ff = curr_ff->next;
                if(curr_ff == NULL){
                    printf("    ----------------------------------------\n");
                }
            }
            curr = curr->next;
            if(curr == NULL){
                break;
            }
            else{
                curr_ff = curr->domainFreqList->head;
            }
        }
        printf("----------------------------------------------\n");
    }
    
}



CacheNode* addMediaRequest(Cache* cache, Media media, char* domain)
{
    CacheNode* curr = (CacheNode*)malloc(sizeof(CacheNode));
    CacheNode* prev;
    CacheNode* head = (CacheNode*)malloc(sizeof(CacheNode));
    CacheNode* tail;
    DomainFreqNode* curr_ff;
    
    curr = cache->head;
    head = cache->head;
    while(curr != NULL){
        if (strcmp(curr->media.name , media.name) == 0){
            curr_ff = curr->domainFreqList->head;
            while(curr_ff != NULL){
                if( strcmp(curr_ff->name , domain) == 0 ){
                    curr_ff->freq += 1;
                    break;
                }
                else if(curr_ff->next == NULL){
                    DomainFreqNode* temp=(DomainFreqNode*)malloc(sizeof(DomainFreqNode));
                    temp->name = domain;
                    temp->freq = 1;
                    temp->prev = curr_ff;
                    curr_ff->next = temp;
                    break;
                }
                else{
                    curr_ff = curr_ff->next;
                }
            }
            if(curr == cache->head){
                cache->head = curr->next;
                curr->next->prev = NULL;
                break;
            }
            else{
                prev = curr->prev;
                prev->next = curr->next;
                if(curr->next != NULL){
                    curr->next->prev = prev;
                }
                else{
                    cache->tail = prev;
                }
                curr->next = head;
                curr->prev = NULL;
                head->prev = curr;
                cache->head = curr;
                break;
            }
            
            
        }
        else if (curr->next == NULL){
            CacheNode* new_node = (CacheNode*)malloc(sizeof(CacheNode));
            DomainFreqList* new_list = (DomainFreqList*)malloc(sizeof(DomainFreqList));
            DomainFreqNode* new_ffnode = (DomainFreqNode*)malloc(sizeof(DomainFreqNode));
            
            strcpy(new_node->media.name , media.name);
            new_node->media.size = media.size;
            new_node->next = head;
            new_node->prev = NULL;
            
            strcpy (new_ffnode->name , domain);
            new_ffnode->freq = 1;
            new_ffnode->prev = NULL;
            new_ffnode->next = NULL;
            new_list->head = new_ffnode;
            new_list->tail = new_ffnode;
            new_node->domainFreqList = new_list;
            
            head->prev = new_node;
            cache->head = new_node;
            
            cache->currentSize += new_node->media.size;
            
            while(cache->currentSize > cache->cacheLimit){
                cache->tail->prev->next = NULL;
                cache->currentSize -= cache->tail->media.size;
                cache->mediaCount -= 1;
                cache->tail = cache->tail->prev;
            }
            
            
            
            break;
        }
        else{
            curr = curr->next;
        }
        
    }
    
    return cache->head;
}


CacheNode* findMedia(Cache* cache, char* name)
{
    int i=0;
    CacheNode* next=(CacheNode*)malloc(sizeof(CacheNode));
    next = cache->head;
    
    for (i=0;i<cache->mediaCount;i++){
        if(strcmp(next->media.name,name) == 0){
            return next;
        }
        else{
            next = next->next;
        }
    }
    return NULL;
}


void deleteMedia(Cache* cache, char* name)
{
    CacheNode* curr;
    CacheNode* prev;
    
    curr = cache->head;
    prev = NULL;
    while(curr != NULL){
        if(strcmp( curr->media.name , name) == 0 ){
            cache->currentSize -= curr->media.size;
            cache->mediaCount -= 1;
            if(curr == cache->head){
                cache->head = cache->head->next;
                cache->head->prev = NULL;
                curr = NULL;
            }
            else if(curr->next == NULL){
                prev->next = NULL;
                curr = NULL;
            }
            else{
                prev->next = curr->next;
                curr->next->prev = prev;
                curr = NULL;
            }
        }
        else{
            prev = curr;
            curr = curr->next;
        }
    }
}



int main(int argc, char** argv)
{
    Cache* cache;
    Media media1;

    /* Create the cache */
    cache = createCache();

    /* Add new media */
    media1.name = "newVideo";
    media1.size = 300;

    addMediaRequest(cache, media1, "tr");

    deleteMedia(cache, "video2");

    printCache(cache);
    
    return 0;
}
