#include <bits/stdc++.h>
using namespace std;

class AffineCipher {
private:
    int a, b;
    string plainText;
    string encryptedText;
    string decryptedText;
    const int mod = 26;

    int modularInverse(int a, int m) {
        a = a % m;
        for (int x = 1; x < m; x++) {
            if ((a * x) % m == 1)
                return x;
        }
        throw runtime_error("Modular inverse does not exist.");
    }

    char encryptChar(char ch) {
        int asciiValue = islower(ch) ? ch - 'a' : ch - 'A';
        int encryptedValue = (a * asciiValue + b) % mod;
        return islower(ch) ? (encryptedValue + 'a') : (encryptedValue + 'A');
    }

    char decryptChar(char ch) {
        int asciiValue = islower(ch) ? ch - 'a' : ch - 'A';
        int a_inv = modularInverse(a, mod);
        int decryptedValue = (a_inv * (asciiValue - b + mod)) % mod;
        return islower(ch) ? (decryptedValue + 'a') : (decryptedValue + 'A');
    }

public:
    AffineCipher(int a, int b) : a(a), b(b) {}

    void setText( string& inputText) {
        plainText = inputText;
        encryptedText.resize(plainText.size());
        decryptedText.resize(plainText.size());
    }

    string encrypt() {
        for (int i = 0 ; i < plainText.size(); i++) {
            char ch =  plainText[i];
            if (isalpha(ch))
                encryptedText[i] = encryptChar(ch);
            else 
                encryptedText[i] = ch; 
        }
        return encryptedText;
    }

    string decrypt() {
        for (int i = 0 ; i < encryptedText.size(); i++) {
            char ch =  encryptedText[i];
            if (isalpha(ch)) 
                decryptedText[i] = decryptChar(ch);
            else 
                decryptedText[i] = ch;
        }
        return decryptedText;
    }
};

int main() {
    AffineCipher aff(17, 20);

    string inputText = "TWENTY";
    cout << "Enter the text to encrypt: ";
    getline(cin, inputText);
    aff.setText(inputText);

    cout << "Encrypted Text : "<< aff.encrypt() << endl; 
    cout << "Decrypted Text : " << aff.decrypt() << endl; 

    return 0;
}
