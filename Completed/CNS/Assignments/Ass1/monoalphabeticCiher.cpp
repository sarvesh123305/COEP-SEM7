#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;


class MonoalphabeticCipher {
private:
    string ciphertext;
    string plaintext;
    map<char, char> decryptionKey;


    vector<pair<char, double>> englishLetterFrequency = {
        {'E', 12.02}, {'T', 9.10}, {'A', 8.12}, {'O', 7.68}, {'I', 7.31},
        {'N', 6.95}, {'S', 6.28}, {'R', 6.02}, {'H', 5.92}, {'D', 4.32},
        {'L', 3.98}, {'U', 2.88}, {'C', 2.71}, {'M', 2.61}, {'F', 2.30},
        {'Y', 2.11}, {'W', 2.09}, {'G', 2.03}, {'P', 1.82}, {'B', 1.49},
        {'V', 1.11}, {'K', 0.69}, {'X', 0.17}, {'Q', 0.11}, {'J', 0.10},
        {'Z', 0.07}
    };

public:

    MonoalphabeticCipher(const string& ct)
        : ciphertext(ct) {}


    void performFrequencyAnalysis() {
        map<char, int> frequencyCounter;


        for (char c : ciphertext) {
            if (isalpha(c)) {
                frequencyCounter[toupper(c)]++;
            }
        }


        vector<pair<char, int>> sortedCiphertextFrequency(frequencyCounter.begin(), frequencyCounter.end());


        sort(sortedCiphertextFrequency.begin(), sortedCiphertextFrequency.end(),
            [](const pair<char, int>& a, const pair<char, int>& b) {
                return a.second > b.second;
            });


        for (size_t i = 0; i < sortedCiphertextFrequency.size(); ++i) {
            decryptionKey[sortedCiphertextFrequency[i].first] = englishLetterFrequency[i].first;
        }
    }


    void decryptCiphertext() {
        plaintext = ciphertext;

        for (size_t i = 0; i < ciphertext.length(); ++i) {
            char c = toupper(ciphertext[i]);
            if (isalpha(c) && decryptionKey.find(c) != decryptionKey.end()) {
                plaintext[i] = decryptionKey[c];
            } else {
                plaintext[i] = ciphertext[i];  
             }
        }
    }
	void displayDecryptionKey() const {
        cout << "Recovered Decryption Key:" << endl;
        for (const auto& pair : decryptionKey) {
            cout << pair.first << " -> " << pair.second << endl;
        }
    }


    void displayPlaintext() const {
        cout << "Decrypted Plaintext: " << plaintext << endl;
    }
};

int main() {

    string ciphertext = "WTAAD LDGAS BA";


    MonoalphabeticCipher cipher(ciphertext);


    cipher.performFrequencyAnalysis();


    cipher.decryptCiphertext();


    cipher.displayDecryptionKey();
    cipher.displayPlaintext();

    return 0;
}

