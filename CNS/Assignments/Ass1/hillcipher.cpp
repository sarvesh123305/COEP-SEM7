#include <iostream>
#include <vector>
using namespace std;

// Function to calculate the modular inverse
int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++)
        if ((a * x) % m == 1)
            return x;
    return -1;
}

// Function to calculate the determinant of a matrix
int determinant(vector<vector<int>>& mat, int n) {
    int det = 0;
    if (n == 2) {
        return (mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0]);
    } else {
        for (int x = 0; x < n; x++) {
            vector<vector<int>> submat(n - 1, vector<int>(n - 1));
            for (int i = 1; i < n; i++) {
                int sub_j = 0;
                for (int j = 0; j < n; j++) {
                    if (j == x) continue;
                    submat[i - 1][sub_j] = mat[i][j];
                    sub_j++;
                }
            }
            det += (x % 2 == 0 ? 1 : -1) * mat[0][x] * determinant(submat, n - 1);
        }
    }
    return det;
}

// Function to find the adjoint of a matrix
void adjoint(vector<vector<int>>& mat, vector<vector<int>>& adj, int n) {
    if (n == 2) {
        adj[0][0] = mat[1][1];
        adj[0][1] = -mat[0][1];
        adj[1][0] = -mat[1][0];
        adj[1][1] = mat[0][0];
        return;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            vector<vector<int>> submat(n - 1, vector<int>(n - 1));
            int sub_i = 0;
            for (int x = 0; x < n; x++) {
                if (x == i) continue;
                int sub_j = 0;
                for (int y = 0; y < n; y++) {
                    if (y == j) continue;
                    submat[sub_i][sub_j] = mat[x][y];
                    sub_j++;
                }
                sub_i++;
            }
            adj[j][i] = ((i + j) % 2 == 0 ? 1 : -1) * determinant(submat, n - 1);
        }
    }
}

// Function to find the inverse of a matrix
bool inverseMatrix(vector<vector<int>>& mat, vector<vector<int>>& inv, int n, int mod) {
    int det = determinant(mat, n);
    int det_inv = modInverse(det % mod, mod);
    if (det_inv == -1) {
        cout << "Matrix is not invertible!" << endl;
        return false;
    }

    vector<vector<int>> adj(n, vector<int>(n));
    adjoint(mat, adj, n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            inv[i][j] = (adj[i][j] * det_inv) % mod;
            if (inv[i][j] < 0)
                inv[i][j] += mod;
        }
    }
    return true;
}

// Function to multiply matrix and vector mod 26
vector<int> multiplyMatrix(vector<vector<int>>& mat, vector<int>& vec, int n, int mod) {
    vector<int> res(n, 0);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            res[i] += mat[i][j] * vec[j];
        }
        res[i] = res[i] % mod;
    }
    return res;
}

// Function to encrypt using Hill Cipher
string encrypt(string plaintext, vector<vector<int>>& keyMatrix, int n) {
    int mod = 26;
    while (plaintext.size() % n != 0) {
        plaintext += 'x'; // Padding with 'x'
    }

    string ciphertext = "";
    for (int i = 0; i < plaintext.size(); i += n) {
        vector<int> vec(n);
        for (int j = 0; j < n; j++) {
            vec[j] = plaintext[i + j] - 'a';
        }
        vector<int> res = multiplyMatrix(keyMatrix, vec, n, mod);
        for (int j = 0; j < n; j++) {
            ciphertext += (res[j] + 'a');
        }
    }
    return ciphertext;
}

// Function to decrypt using Hill Cipher
string decrypt(string ciphertext, vector<vector<int>>& keyMatrix, int n) {
    int mod = 26;
    vector<vector<int>> invMatrix(n, vector<int>(n));
    if (!inverseMatrix(keyMatrix, invMatrix, n, mod)) {
        return "";
    }

    string plaintext = "";
    for (int i = 0; i < ciphertext.size(); i += n) {
        vector<int> vec(n);
        for (int j = 0; j < n; j++) {
            vec[j] = ciphertext[i + j] - 'a';
        }
        vector<int> res = multiplyMatrix(invMatrix, vec, n, mod);
        for (int j = 0; j < n; j++) {
            plaintext += (res[j] + 'a');
        }
    }
    return plaintext;
}

int main() {
    int n;
    cout << "Enter the size of the key matrix (n x n): ";
    cin >> n;

    vector<vector<int>> keyMatrix(n, vector<int>(n));
    cout << "Enter the key matrix (row-wise):\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> keyMatrix[i][j];
        }
    }

    string plaintext;
    cout << "Enter the plaintext (lowercase, no spaces): ";
    cin >> plaintext;

    string ciphertext = encrypt(plaintext, keyMatrix, n);
    cout << "Ciphertext: " << ciphertext << endl;

    string decryptedText = decrypt(ciphertext, keyMatrix, n);
    cout << "Decrypted Text: " << decryptedText << endl;

    return 0;
}

