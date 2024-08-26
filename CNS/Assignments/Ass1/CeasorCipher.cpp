#include <bits/stdc++.h>
using namespace std;

class CeasorCipher{
    int shifts;
    string text;
    public:
    CeasorCipher(int shifts,string &text) : shifts(shifts) , text(text){};

    string encrypt(){
        string result = "";

        for (int i = 0; i < text.length(); i++) {
            if (isupper(text[i]))
                result += char(int(text[i] + shifts - 65) % 26 + 65);
            else
                result += char(int(text[i] + shifts - 97) % 26 + 97);
        }
        return result;
    }
    string decrypt(string encryptedKey = "",int shiftsGiven = 0) {
        string result = "";
        if(shiftsGiven == 0)
            shiftsGiven = shifts;

        for (int i = 0; i < encryptedKey.length(); i++) {
            if (isupper(encryptedKey[i])) {
                result += char(int(encryptedKey[i] - shiftsGiven - 65 + 26) % 26 + 65);
            } else {
                result += char(int(encryptedKey[i] - shiftsGiven - 97 + 26) % 26 + 97);
            }
        }
        return result;
    }

    
    void cryptoAnalysis() {
        string encryptedKey = encrypt();
        for (int shift = 0; shift < 26; ++shift) {
            string decryptedKey = decrypt(encryptedKey,shift);
            cout << "Shift " << shift << ": " << decryptedKey << endl;
        }
    }

};

int main()
{
    string text ;
    int shifts ;
    cout << "Enter a string : " ;
    cin >> text;
    cout << "Enter Number of shifts : " ;
    cin >> shifts;

    CeasorCipher *ceasorCipher = new CeasorCipher(shifts,text); 
    string encryptedKey =  ceasorCipher -> encrypt() ;
    cout << encryptedKey << endl;

    ceasorCipher -> cryptoAnalysis();

    return 0;
}
