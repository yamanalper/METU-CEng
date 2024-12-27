#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

//=======================//
// Implemented Functions //
//=======================//
template<int MAX_SIZE>
int HashTable<MAX_SIZE>::PRIMES[3] = {102523, 100907, 104659};

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::PrintLine(int tableIndex) const
{
    const HashData& data = table[tableIndex];

    // Using printf here it is easier to format
    if(data.sentinel == SENTINEL_MARK)
    {
        printf("[%03d]         : SENTINEL\n", tableIndex);
    }
    else if(data.sentinel == EMPTY_MARK)
    {
        printf("[%03d]         : EMPTY\n", tableIndex);
    }
    else
    {
        printf("[%03d] - [%03d] : ", tableIndex, data.lruCounter);
        printf("(%-5s) ", data.isCostWeighted ? "True" : "False");
        size_t sz = data.intArray.size();
        for(size_t i = 0; i < sz; i++)
        {
            if(i % 2 == 0)
                printf("[%03d]", data.intArray[i]);
            else
                printf("/%03d/", data.intArray[i]);

            if(i != sz - 1)
                printf("-->");
        }
        printf("\n");
    }
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::PrintTable() const
{
    printf("____________________\n");
    printf("Elements %d\n", elementCount);
    printf("[IDX] - [LRU] | DATA\n");
    printf("____________________\n");
    for(int i = 0; i < MAX_SIZE; i++)
    {
        PrintLine(i);
    }
}

//=======================//
//          TODO         //
//=======================//
template<int MAX_SIZE>
int HashTable<MAX_SIZE>::Hash(int startInt, int endInt, bool isCostWeighted)
{
    int h = 0;
    h += PRIMES[0] * startInt;
    h += PRIMES[1] * endInt;
    h += PRIMES[2] * isCostWeighted;
    return h % MAX_SIZE;
}

template<int MAX_SIZE>
HashTable<MAX_SIZE>::HashTable()
{
    elementCount = 0;
    for (int i = 0; i < MAX_SIZE; i++)
    {
        table[i].intArray.clear();
        table[i].sentinel = EMPTY_MARK;
        table[i].isCostWeighted = false;
        table[i].startInt = 0;
        table[i].endInt = 0;
        table[i].lruCounter = 0;
    }
}

template<int MAX_SIZE>
int HashTable<MAX_SIZE>::Insert(const std::vector<int>& intArray, bool isCostWeighted)
{
    if(intArray.empty()) throw InvalidTableArgException();
    for(int i = 0; i < MAX_SIZE ; i++){
        if(intArray.front() == table[i].startInt && intArray.back() == table[i].endInt && isCostWeighted == table[i].isCostWeighted){
            table[i].lruCounter++;
            return table[i].lruCounter;
        }
    }
    if(elementCount > MAX_SIZE/2) throw TableCapFullException(elementCount);
    int hash_place = Hash(intArray.front(), intArray.back(), isCostWeighted);
    int x = 1;
    int valid_place = hash_place;
    while(table[valid_place].sentinel != EMPTY_MARK){
        valid_place = (hash_place + x*x ) % MAX_SIZE;
        x++;
    }
    table[valid_place].intArray = intArray;
    table[valid_place].sentinel = OCCUPIED_MARK;
    table[valid_place].isCostWeighted = isCostWeighted;
    table[valid_place].startInt = intArray.front();
    table[valid_place].endInt = intArray.back();
    table[valid_place].lruCounter = 1;
    elementCount++;
    return 0;
    
    
}

template<int MAX_SIZE>
bool HashTable<MAX_SIZE>::Find(std::vector<int>& intArray,
                               int startInt, int endInt, bool isCostWeighted,
                               bool incLRU)
{
    for(int i = 0; i < MAX_SIZE ; i++){
        if(startInt == table[i].startInt && endInt == table[i].endInt && isCostWeighted == table[i].isCostWeighted){
            if(incLRU == true){
                table[i].lruCounter++;
            }
            intArray = table[i].intArray;
            return true;
        }
    }
    return false;
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::InvalidateTable()
{
    elementCount = 0;
    for (int i = 0; i < MAX_SIZE; i++){
        table[i].intArray.clear();
        table[i].sentinel = EMPTY_MARK;
        table[i].isCostWeighted = false;
        table[i].startInt = 0;
        table[i].endInt = 0;
        table[i].lruCounter = 0;
    }
    
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::GetMostInserted(std::vector<int>& intArray) const
{
    int most = 0;
    for(int i = 0; i < MAX_SIZE ; i++){
        if(table[i].lruCounter > most){
            most = table[i].lruCounter;
            intArray = table[i].intArray;
        } 
    }
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::Remove(std::vector<int>& intArray,
                                 int startInt, int endInt, bool isCostWeighted)
{
    for(int i = 0; i < MAX_SIZE ; i++){
        if(startInt == table[i].startInt && endInt == table[i].endInt && isCostWeighted == table[i].isCostWeighted){
            intArray = table[i].intArray;
            table[i].intArray.clear();
            table[i].sentinel = SENTINEL_MARK;
            table[i].isCostWeighted = false;
            table[i].startInt = 0;
            table[i].endInt = 0;
            table[i].lruCounter = 0;
            elementCount--;
        }
    }
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::RemoveLRU(int lruElementCount)
{
    int least;
    for(int i = 0; i<MAX_SIZE ; i++){
        if(table[i].lruCounter != 0){
            least = table[i].lruCounter;
            break;
        }
    }
    for(int i = 0; i < MAX_SIZE ; i++){
        if(table[i].lruCounter < least && table[i].lruCounter != 0) least = table[i].lruCounter;
    }
    for( int i = 0; i < MAX_SIZE ; i++){
        if(lruElementCount > 0){
            if(table[i].lruCounter == least){
                table[i].sentinel = SENTINEL_MARK;
                table[i].isCostWeighted = false;
                table[i].startInt = 0;
                table[i].endInt = 0;
                table[i].lruCounter = 0;
                elementCount--;
                lruElementCount--;
            }
        }
    }
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::PrintSortedLRUEntries() const
{
    MaxPairHeap<int,int> que;
    for(int i = 0; i<MAX_SIZE ; i++){
        if(table[i].lruCounter != 0) que.push({table[i].lruCounter , i});
    }
    while(!que.empty()){
        Pair<int,int> i = que.top();
        PrintLine(i.value);
        que.pop();
    }
    
}

template<int MAX_SIZE>
int HashTable<MAX_SIZE>::lruElementCount()
{
    int least;
    for(int i = 0; i<MAX_SIZE ; i++){
        if(table[i].lruCounter != 0){
            least = table[i].lruCounter;
            break;
        }
    }
    for(int i = 0; i < MAX_SIZE ; i++){
        if(table[i].lruCounter < least && table[i].lruCounter != 0) least = table[i].lruCounter;
    }
    int count = 0;
    for(int i = 0; i < MAX_SIZE ; i++){
        if(table[i].lruCounter == least) count++;
    }
    return count;
}








#endif // HASH_TABLE_HPP