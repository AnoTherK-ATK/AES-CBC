#ifndef __CBC_H__
#define __CBC_H__
#include <bits/stdc++.h>
#include "AES.h"
using namespace std;
class CBC{
public:
    AES *aes;
    vector<unsigned char> iv;
    CBC(vector<unsigned char> &key, vector<unsigned char> &iv){
        aes = new AES(key);
        this->iv = iv;
    }
    vector<unsigned char> pkcs7Padding(vector<unsigned char> &data){
        int padding = 16 - (data.size() % 16);
        for(int i = 0; i < padding; i++){
            data.push_back((unsigned char)padding);
        }
        return data;
    }
    vector<unsigned char> pkcs7Unpadding(vector<unsigned char> &data){
        int padding = data[data.size()-1];
        if(padding > 16) return data;
        for(int i = 0; i < padding; i++){
            data.pop_back();
        }
        return data;
    }
    vector<unsigned char> encrypt(vector<unsigned char> &data){
        data = pkcs7Padding(data);
        vector<unsigned char> encrypted_data;
        vector<unsigned char> temp(16);
        for(int i = 0; i < data.size(); i+=16){
            for(int j = 0; j < 16; j++){
                temp[j] = data[i+j] ^ iv[j];
            }
            temp = aes->encrypt(temp);
            for(int j = 0; j < 16; j++){
                iv[j] = temp[j];
                encrypted_data.push_back(temp[j]);
            }
        }
        return encrypted_data;
    }
    vector<unsigned char> decrypt(vector<unsigned char> &data){
        vector<unsigned char> decrypted_data;
        vector<unsigned char> temp(16);
        for(int i = 0; i < data.size(); i+=16){
            for(int j = 0; j < 16; j++){
                temp[j] = data[i+j];
            }
            temp = aes->decrypt(temp);
            for(int j = 0; j < 16; j++){
                decrypted_data.push_back(temp[j] ^ iv[j]);
                iv[j] = data[i+j];
            }
        }
        decrypted_data = pkcs7Unpadding(decrypted_data);
        return decrypted_data;
    }
};
#endif