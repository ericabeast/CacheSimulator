#include <iostream>

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