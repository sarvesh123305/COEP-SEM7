#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

// Create a substitution alphabet based on the given key
string createSubstitutionAlphabet(const string& key) {
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string keyUpper = key;
    transform(keyUpper.begin(), keyUpper.end(), keyUpper.begin(), ::toupper);

    string keyUnique;
    unordered_map<char, bool> seen;
    for (char c : keyUpper) {
        if (isalpha(c) && !seen[c]) {
            seen[c] = true;
            keyUnique += c;
        }
    }

    string substitutionAlphabet = keyUnique;
    for (char c : alphabet) {
        if (!seen[c]) {
            substitutionAlphabet += c;
        }
    }
    
    return substitutionAlphabet;
}

string substitute(const string& text, const string& substitutionAlphabet, bool encrypt = true) {
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string result;
    unordered_map<char, char> trans;
    
    if (encrypt) {
        for (size_t i = 0; i < alphabet.size(); ++i) {
            trans[alphabet[i]] = substitutionAlphabet[i];
        }
    } else {
        for (size_t i = 0; i < substitutionAlphabet.size(); ++i) {
            trans[substitutionAlphabet[i]] = alphabet[i];
        }
    }

    for (char c : text) {
        if (isalpha(c)) {
            result += trans[toupper(c)];
        } else {
            result += c;
        }
    }

    return result;
}

string substitutionCipher(const string& text, const string& key) {
    string substitutionAlphabet = createSubstitutionAlphabet(key);
    return substitute(text, substitutionAlphabet);
}

string substitutionDecipher(const string& text, const string& key) {
    string substitutionAlphabet = createSubstitutionAlphabet(key);
    return substitute(text, substitutionAlphabet, false);
}

unordered_map<char, int> frequencyAnalysis(const string& text) {
    unordered_map<char, int> frequency;
    for (char c : text) {
        if (isalpha(c)) {
            frequency[toupper(c)]++;
        }
    }
    return frequency;
}

void displayFrequencyAnalysis(const unordered_map<char, int>& frequency) {
    vector<pair<char, int>> freqVector(frequency.begin(), frequency.end());
    sort(freqVector.begin(), freqVector.end(), [](const pair<char, int>& a, const pair<char, int>& b) {
        return b.second < a.second;
    });

    for (const auto& pair : freqVector) {
        cout << pair.first << ": " << pair.second << endl;
    }
}

int main() {
    string key = "KEYWORD";
    string plaintext ;
    
    cout << "Enter the plain text : " ;
    cin >> plaintext;
    
    string ciphertext = substitutionCipher(plaintext, key);
    cout << "Ciphertext: " << ciphertext << endl;

    string decryptedText = substitutionDecipher(ciphertext, key);
    cout << "Decrypted text: " << decryptedText << endl;


    return 0;
}
