#include <bits/stdc++.h>
#include "CBC.h"
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#endif
/* Convert string <--> utf8*/
#include <locale>
using namespace std;

string skey, siv, cipher, plain;
vector<unsigned char> key, iv, plainBinary, cipherBinary;

string binaryToString(vector<unsigned char> &data){
    string temp;
    for(int i = 0; i < data.size(); i++){
        temp += data[i];
    }
    return temp;
}

vector<unsigned char> stringToBinary(string &data){
    vector<unsigned char> temp;
    for(int i = 0; i < data.size(); i++){
        temp.push_back(data[i]);
    }
    return temp;
}

vector<unsigned char> hexToBinary(string &data){
    vector<unsigned char> temp;
    for(int i = 0; i < data.size(); i+=2){
        unsigned char byte = (unsigned char)strtol(data.substr(i, 2).c_str(), NULL, 16);
        temp.push_back(byte);
    }
    return temp;
}

string binaryToHex(vector<unsigned char> &data){
    string temp;
    for(int i = 0; i < data.size(); i++){
        char buf[3];
        sprintf(buf, "%02x", data[i]);
        temp += buf;
    }
    return temp;
}


signed main(){
    #ifdef __linux__
    std::locale::global(std::locale("C.UTF-8"));
    #endif
  
    #ifdef _WIN32
    // Set console code page to UTF-8 on Windows
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    #endif

    cout << "Enter the key: ";
    cin >> skey;
    cin.ignore();
    key = hexToBinary(skey);
    cout << "Enter the iv: ";
    cin >> siv;
    cin.ignore();
    iv = hexToBinary(siv);
    cout << "choose for encryption(1) or decryption(2): ";
    int mode;
    cin >> mode;
    cin.ignore();
    CBC cbc(key, iv);
    switch(mode){
        case 1:
            cout << "Enter the plain text: ";
            getline(cin, plain);
            plainBinary = stringToBinary(plain);
            cipherBinary = cbc.encrypt(plainBinary);
            cout << "Cipher text: " << binaryToHex(cipherBinary) << endl;
            break;
        case 2:
            cout << "Enter the cipher text: ";
            getline(cin, cipher);
            cipherBinary = hexToBinary(cipher);
            plainBinary = cbc.decrypt(cipherBinary);
            cout << "Plain text: " << binaryToString(plainBinary) << endl;
            break;
        default:
            cout << "Invalid choice" << endl;
    }

}