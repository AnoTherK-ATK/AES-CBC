#ifndef __KEYEXPANSION_H__
#define __KEYEXPANSION_H__
#include <bits/stdc++.h>
#include "Constants.h"
using namespace std;
vector<unsigned char> subWord(vector<unsigned char> &word){
    vector<unsigned char> temp(4);
    for(int i = 0; i < 4; i++){
        temp[i] = SBox[word[i]];
    }
    return temp;
}
vector<unsigned char> rotWord(vector<unsigned char> &word){
    vector<unsigned char> temp(4);
    temp[0] = word[1];
    temp[1] = word[2];
    temp[2] = word[3];
    temp[3] = word[0];
    return temp;
}
vector<vector<unsigned char>> keyExpansion(vector<unsigned char> &key){
    vector<vector<unsigned char>> round_keys;
    for(int i = 0; i < 4; i++){
        vector<unsigned char> temp;
        for(int j = 0; j < 4; j++){
            temp.push_back(key[i*4+j]);
        }
        round_keys.push_back(temp);
    }

    for(int i = 4; i < 44; i++){
        vector<unsigned char> temp = round_keys[i-1];
        if(i % 4 == 0){
            temp = rotWord(temp);
            temp = subWord(temp);
        }
        for (int j = 0; j < 4; j++) {
            temp[j] ^= RCON[(i - 4) / 4][j];
        }
        std::vector<uint8_t> new_round_key(4);
        for (int j = 0; j < 4; j++) {
            new_round_key[j] = round_keys[i - 4][j] ^ temp[j];
        }
        round_keys.push_back(new_round_key);
    }
    return round_keys;
}
#endif