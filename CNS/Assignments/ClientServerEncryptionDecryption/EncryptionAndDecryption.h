#ifndef IMPROVED_TRANSFORM_H
#define IMPROVED_TRANSFORM_H

#include <string>
#include <vector>

using namespace std; 

void display();

class ImprovedTransform {
private:
    vector<unsigned char> key;

    void generateKey();

public:
    ImprovedTransform();

    string encrypt(string& input);

    string decrypt(string& input);
};

#endif // IMPROVED_TRANSFORM_H
