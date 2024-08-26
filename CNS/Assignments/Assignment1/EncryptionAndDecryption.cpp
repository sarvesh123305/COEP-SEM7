#include <bits/stdc++.h>

using namespace std;

class ImprovedTransform {
private:
    vector<unsigned char> key;

    void generateKey(int seed, int length) {
        srand(seed);
        key.resize(length);
        for (int i = 0; i < length; ++i) {
            key[i] = rand() % 256;
        }
    }

public:
    ImprovedTransform(int seed, int length) {
        generateKey(seed, length);
    }

    string encrypt(const string& input) {
        string output = input;
        for (size_t i = 0; i < input.length(); ++i) {
            output[i] = input[i] ^ key[i % key.size()];
            output[i] = (output[i] + key[(i + 1) % key.size()]) % 256;
        }
        return output;
    }

    string decrypt(const string& input) {
        string output = input;
        for (size_t i = 0; i < input.length(); ++i) {
            output[i] = (input[i] - key[(i + 1) % key.size()] + 256) % 256;
            output[i] = output[i] ^ key[i % key.size()];
        }
        return output;
    }
};

int main() {
    int seed, keyLength;
    string message;

    cout << "Enter a seed for key generation: ";
    cin >> seed;
    cout << "Enter key length: ";
    cin >> keyLength;
    cin.ignore();  
    cout << "Enter the message: ";
    getline(cin, message);

    ImprovedTransform transformer(seed, keyLength);

    string encrypted = transformer.encrypt(message);
    string decrypted = transformer.decrypt(encrypted);

    cout << "Original: " << message << endl;
    cout << "Encrypted: ";
    for (unsigned char c : encrypted) {
        cout << hex << (int)c << " ";
    }
    cout << endl;
    cout << "Decrypted: " << decrypted << endl;

    return 0;
}

