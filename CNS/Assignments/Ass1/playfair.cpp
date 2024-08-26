#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class PlayfairCipher {
private:
    string key;
    vector<vector<char>> playfairMatrix;
    map<char, pair<int, int>> charPositionMap;
    string plaintext;
    string ciphertext;

public:
    PlayfairCipher(const string& ct)
        : ciphertext(ct) {}

    void setKey(const string& k) {
        key = preprocessKey(k);
        generatePlayfairMatrix();
    }

    void decryptWithKey() {
        plaintext.clear();
        for (size_t i = 0; i < ciphertext.length(); i += 2) {
            char first = ciphertext[i];
            char second = ciphertext[i + 1];
            pair<int, int> pos1 = charPositionMap[first];
            pair<int, int> pos2 = charPositionMap[second];

            if (pos1.first == pos2.first) {
                // Same row
                plaintext += playfairMatrix[pos1.first][(pos1.second + 4) % 5];
                plaintext += playfairMatrix[pos2.first][(pos2.second + 4) % 5];
            } else if (pos1.second == pos2.second) {
                // Same column
                plaintext += playfairMatrix[(pos1.first + 4) % 5][pos1.second];
                plaintext += playfairMatrix[(pos2.first + 4) % 5][pos2.second];
            } else {
                // Rectangle
                plaintext += playfairMatrix[pos1.first][pos2.second];
                plaintext += playfairMatrix[pos2.first][pos1.second];
            }
        }
    }

    void displayPlayfairMatrix() const {
        cout << "Playfair Matrix:" << endl;
        for (const auto& row : playfairMatrix) {
            for (char c : row) {
                cout << c << " ";
            }
            cout << endl;
        }
    }

    void displayPlaintext() const {
        cout << "Decrypted Plaintext: " << plaintext << endl;
    }

private:
    string preprocessKey(const string& k) {
        string processedKey;
        map<char, bool> seen;

        for (char c : k) {
            c = toupper(c);
            if (c == 'J') {
                c = 'I';
            }
            if (!seen[c] && isalpha(c)) {
                processedKey += c;
                seen[c] = true;
            }
        }

        return processedKey;
    }

    void generatePlayfairMatrix() {
        playfairMatrix = vector<vector<char>>(5, vector<char>(5));
        charPositionMap.clear();
        string alphabet = "ABCDEFGHIKLMNOPQRSTUVWXYZ";
        string fullKey = key + alphabet;

        map<char, bool> placed;
        int row = 0, col = 0;

        for (char c : fullKey) {
            if (!placed[c]) {
                playfairMatrix[row][col] = c;
                charPositionMap[c] = {row, col};
                placed[c] = true;

                if (++col == 5) {
                    col = 0;
                    if (++row == 5) {
                        break;
                    }
                }
            }
        }
    }
};

int main() {
    string ciphertext = "GATLMZCLQQYAYX";

    PlayfairCipher cipher(ciphertext);

    string key = "PLAYFAIREXAMPLE";
    cipher.setKey(key);

    cipher.displayPlayfairMatrix();

    cipher.decryptWithKey();

    cipher.displayPlaintext();

    return 0;
}
