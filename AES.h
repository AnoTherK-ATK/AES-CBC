#ifndef __AES_H__
#define __AES_H__
#include <bits/stdc++.h>
#include "Constants.h"
#include "keyExpansion.h"
using namespace std;
class AES{
public:
    vector<vector<unsigned char>> round_keys;
    vector<unsigned char> key;
    
    AES(vector<unsigned char> &key){
        this->key = key;
        round_keys = keyExpansion(key);
    }
    ~AES(){}
    void setKey(unsigned char* key){}

    vector<vector<unsigned char>> subBytes(vector<vector<unsigned char>> &state){
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                state[i][j] = SBox[state[i][j]];
            }
        }
        return state;
    }
    vector<vector<unsigned char>> invSubBytes(vector<vector<unsigned char>> &state){
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                state[i][j] = invSBox[state[i][j]];
            }
        }
        return state;
    }

    vector<vector<unsigned char>> shiftRows(vector<vector<unsigned char>> &state){
        vector<vector<unsigned char>> temp(4, vector<unsigned char>(4));
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                temp[i][j] = state[i][(j+i)%4];
            }
        }
        return temp;
    }
    vector<vector<unsigned char>> invShiftRows(vector<vector<unsigned char>> &state){
        vector<vector<unsigned char>> temp(4, vector<unsigned char>(4));
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                temp[i][(j+i)%4] = state[i][j];
            }
        }
        return temp;
    }

    unsigned char mul(unsigned char a, unsigned char b){
        unsigned char p = 0;
        unsigned char hi_bit_set;
        for (int i = 0; i < 8; i++) {
            if ((b & 1) == 1) 
                p ^= a;
            hi_bit_set = (unsigned char) (a & 0x80);
            a <<= 1;
            if (hi_bit_set) 
                a ^= 0x1b;		
            b >>= 1;
        }
        return p % 256;
    }
    vector<vector<unsigned char>> mixColumns(vector<vector<unsigned char>> &state){
        for(int i = 0; i < 4; ++i){
            vector<unsigned char> temp(4);
            for(int j = 0; j < 4; ++j){
                temp[j] = state[j][i];
            }
            state[0][i] = mul(0x02, temp[0]) ^ mul(0x03, temp[1]) ^ temp[2] ^ temp[3];
            state[1][i] = temp[0] ^ mul(0x02, temp[1]) ^ mul(0x03, temp[2]) ^ temp[3];
            state[2][i] = temp[0] ^ temp[1] ^ mul(0x02, temp[2]) ^ mul(0x03, temp[3]);
            state[3][i] = mul(0x03, temp[0]) ^ temp[1] ^ temp[2] ^ mul(0x02, temp[3]);
        }
        return state;
    }
    vector<vector<unsigned char>> invMixColumns(vector<vector<unsigned char>> &state){
        for(int i = 0; i < 4; ++i){
            vector<unsigned char> temp(4);
            for(int j = 0; j < 4; ++j){
                temp[j] = state[j][i];
            }
            state[0][i] = mul(0x0e, temp[0]) ^ mul(0x0b, temp[1]) ^ mul(0x0d, temp[2]) ^ mul(0x09, temp[3]);
            state[1][i] = mul(0x09, temp[0]) ^ mul(0x0e, temp[1]) ^ mul(0x0b, temp[2]) ^ mul(0x0d, temp[3]);
            state[2][i] = mul(0x0d, temp[0]) ^ mul(0x09, temp[1]) ^ mul(0x0e, temp[2]) ^ mul(0x0b, temp[3]);
            state[3][i] = mul(0x0b, temp[0]) ^ mul(0x0d, temp[1]) ^ mul(0x09, temp[2]) ^ mul(0x0e, temp[3]);
        }
        return state;
    }

    vector<vector<unsigned char>> addRoundKey(vector<vector<unsigned char>> &state, int round){
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                state[i][j] ^= round_keys[round*4+i][j];
            }
        }
        return state;
    }

    vector<unsigned char> encrypt(vector<unsigned char> plaintext){
        vector<vector<unsigned char>> state(4, vector<unsigned char>(4));
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                state[j][i] = plaintext[i * 4 + j];
            }
        }
        state = addRoundKey(state, 0);
        for(int round = 1; round < 10; round++){
            state = subBytes(state);
            state = shiftRows(state);
            state = mixColumns(state);
            state = addRoundKey(state, round);
        }
        state = subBytes(state);
        state = shiftRows(state);
        state = addRoundKey(state, 10);
        vector<unsigned char> ciphertext(16);
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                ciphertext[i * 4 + j] = state[j][i];
            }
        }
        return ciphertext;
    }

    vector<unsigned char> decrypt(vector<unsigned char> ciphertext){
        vector<vector<unsigned char>> state(4, vector<unsigned char>(4));
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                state[j][i] = ciphertext[i * 4 + j];
            }
        }
        state = addRoundKey(state, 10);
        for(int round = 9; round > 0; round--){
            state = invShiftRows(state);
            state = invSubBytes(state);
            state = addRoundKey(state, round);
            state = invMixColumns(state);
        }
        state = invShiftRows(state);
        state = invSubBytes(state);
        state = addRoundKey(state, 0);
        vector<unsigned char> plaintext(16);
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                plaintext[i * 4 + j] = state[j][i];
            }
        }
        return plaintext;
    }
};
#endif