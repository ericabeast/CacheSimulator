#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <bits/stdc++.h>
#include <bitset>
#include <cstdint>
#include <vector>
#include <cstdlib>
#include <iomanip>
using namespace std;
void getInput(int &nk, int &assoc, int &blocksize, char &repl, bool &mode)
{
    cout << "Cache Capacity, the capacity of the cache in kilobytes (an int): ";
    cin >> nk;
    cout << "Associativity, the associativity of the cache (an int): ";
    cin >> assoc;
    cout << "Block Size, the size of a single cache block in bytes (an int): ";
    cin >> blocksize;
    cout << "Replacement Policythe, 'l' means LRU, 'r' means random: ";
    cin >> repl;
    // cin >> nk >> assoc >> blocksize >> repl;
    if (repl == 'l')
        mode = true;
    else
        mode = false;
}
void getLength(const int &blockSize, int &offset_length, const int &sets, int &index_length, int &tag_length)
{
    offset_length = log2(blockSize);
    index_length = log2(sets);
    tag_length = 64 - offset_length - index_length;
    // cout << "Offset length: " << offset_length << endl;
}
unsigned long long int getAddress(const string input)
{
    char c = input[0];
    string s = input.substr(2, input.size() - 2);
    // cout << s << endl;
    char *cc;
    cc = &s[0];
    // cout << cc << endl;
    unsigned long long int address;
    address = strtoull(cc, NULL, 16);
    return address;
}
void updateLRU(vector<vector<int>> &v, const unsigned long long int set, int target)
{
    int val = v[set][target];
    for (int i = 0; i < v[set].size(); i++)
    {
        if (v[set][i] < val)
            v[set][i]++;
    }
    v[set][target] = 0;

    // for (int i = 0; i < v[set].size(); i++)
    // {
    //     cout << v[set][i] << " ";
    // }
    // cout << endl;
}
void checkCache(int &countReadMiss, const bool &mode, const int &assoc, const unsigned long long int &index, const unsigned long long int &tag, vector<vector<unsigned long long int>> &table, vector<vector<bool>> &isValid, vector<vector<int>> &LRU)
{
    bool isFound = false;
    bool isFull = false;
    for (int i = 0; i < assoc; i++)
    {
        if (isValid[index][i] == false)
        {
            isValid[index][i] = true;
            table[index][i] = tag;
            updateLRU(LRU, index, i);
            break;
        }
        if (isValid[index][i] == true && table[index][i] == tag)
        {
            isFound = true;
            updateLRU(LRU, index, i);
            break;
        }
        if (i == assoc - 1)
        {
            isFull = true;
        }
    }
    if (!isFound)
    {
        countReadMiss++;
    }
    if (isFull && mode)
    {
        // do LRU
        int least;
        for (int i = 0; i < assoc; i++)
        {
            if (LRU[index][i] == assoc - 1)
            {
                least = i;
                break;
            }
        }
        table[index][least] = tag;
        updateLRU(LRU, index, least);
    }
    if (isFull && !mode)
    {
        // do random
        // cout << "random!!!!!!!!!!!" << endl;
        int randomNumber = rand();
        randomNumber %= assoc;
        table[index][randomNumber] = tag;
    }
}
int main(int argc, char *argv[])
{
    // nk: the capacity of the cache in kilobytes (an int)
    // assoc: the associativity of the cache (an int)
    // blocksize: the size of a single cache block in bytes (an int)
    // repl: the replacement policy (a char); 'l' means LRU, 'r' means random.
    int nk, assoc, blocksize;
    char repl;
    bool mode; // true means LRU, false means random
    bool read;
    getInput(nk, assoc, blocksize, repl, mode);
    int cacheCapacity = nk * 1024;
    int blocks = cacheCapacity / blocksize;
    int sets = blocks / assoc;
    int offset_length, tag_length, index_length;
    getLength(blocksize, offset_length, sets, index_length, tag_length);
    // cout << "offset length: " << offset_length << " tag length: " << tag_length << " index length: " << index_length << endl;
    // cout << "nk: " << nk << " assoc: " << assoc << " blocksize: " << blocksize << " repl: " << repl << " mode: " << mode << endl;
    // cout << "cacheCapacity: " << cacheCapacity << " blocks: " << blocks << " sets: " << sets << endl;

    ///////////// count /////////////
    int countInstr = 0, countReadMiss = 0, countWriteMiss = 0, countInstrRead = 0, countInstrWrite = 0;
    /////////////////////////////////
    ///////////// build cache table /////////////
    vector<vector<unsigned long long int>> table(sets, vector<unsigned long long int>(assoc, 0));
    vector<vector<bool>> isValid(sets, vector<bool>(assoc, false));
    vector<vector<int>> LRU(sets, vector<int>(assoc, 0));
    for (int i = 0; i < sets; i++)
    {
        for (int j = 0; j < assoc; j++)
        {
            LRU[i][j] = j;
        }
    }
    /////////////////////////////////////////////
    string input;
    ifstream ifs;
    ifs.open(argv[1]);
    while (getline(ifs, input))
    {
        countInstr++;
        if (input[0] == 'r')
        {
            read = true;
            countInstrRead++;
        }
        else
        {
            read = false;
            countInstrWrite++;
        }
        unsigned long long int address = getAddress(input);
        address = address >> offset_length;
        unsigned long long int mask = pow(2, index_length) - 1;
        unsigned long long int index = mask & address;
        unsigned long long int tag = address >> index_length;
        // cout << "index: " << index << " tag: " << tag;
        if (read)
        {
            checkCache(countReadMiss, mode, assoc, index, tag, table, isValid, LRU);
        }
        else
        {
            checkCache(countWriteMiss, mode, assoc, index, tag, table, isValid, LRU);
        }
        // cout << "mask: " << mask << endl;
        // cout << "index: " << index << endl;
        // cout << "tag: " << tag << endl;
        // cout << address << endl;
    }
    cout << endl;
    cout << endl;
    cout << "total misses: " << countReadMiss + countWriteMiss << endl;
    cout << "percent of misses: " << fixed << setprecision(6) << (float(countReadMiss + countWriteMiss) / countInstr) * 100 << "%" << endl;
    cout << "read misses: " << countReadMiss << endl;
    cout << "percent of read misses: " << (float(countReadMiss) / (countInstrRead)*100) << "%" << endl;
    cout << "write misses: " << countWriteMiss << endl;
    cout << "percent of write misses: " << (float(countWriteMiss) / (countInstrWrite)*100) << "%" << endl;
    return 1;
}
