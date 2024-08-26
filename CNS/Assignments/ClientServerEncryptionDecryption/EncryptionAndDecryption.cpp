#include "EncryptionAndDecryption.h"
#include <iostream>
#include <cstdlib>  // for srand, rand

void display() {
    cout << "HI sarvwesh" << endl;
}

void ImprovedTransform::generateKey() {
    int length = 10;
    const unsigned char fixedKey[] = {0x1F, 0x2A, 0x3B, 0x4C, 0x5D, 0x6E, 0x7F, 0x8A, 0x9B, 0xAC};
    key.resize(length);
    for (int i = 0; i < length; ++i) {
        key[i] = fixedKey[i % sizeof(fixedKey)];
    }
}

ImprovedTransform::ImprovedTransform() {
    generateKey();
}

string ImprovedTransform::encrypt(string& input) {
    string output = input;
    for (size_t i = 0; i < input.length(); ++i) {
        output[i] = input[i] ^ key[i % key.size()];
        output[i] = (output[i] + key[(i + 1) % key.size()]) % 256;
    }
    return output;
}

string ImprovedTransform::decrypt(string& input) {
    string output = input;
    for (size_t i = 0; i < input.length(); ++i) {
        output[i] = (input[i] - key[(i + 1) % key.size()] + 256) % 256;
        output[i] = output[i] ^ key[i % key.size()];
    }
    return output;
}

// int main() {
//     int seed = 5, keyLength = 10;
//     string message;

//     cout << "Enter the message: ";
//     getline(cin, message);

//     ImprovedTransform transformer(seed, keyLength);

//     string encrypted = transformer.encrypt(message);
//     string decrypted = transformer.decrypt(encrypted);

//     cout << "Original: " << message << endl;
//     cout << "Encrypted: ";
//     for (unsigned char c : encrypted) {
//         cout << hex << (int)c << " ";
//     }
//     cout << endl;
//     cout << "Decrypted: " << decrypted << endl;

//     return 0;
// }
