#include <iostream>
#include <fstream>
#include "file_operations.h"

using namespace std;

void viewMetadata(const string &filename) {
    ifstream file(filename, ios::binary | ios::ate);
    if (!file) {
        cout << "Error accessing file!\n";
        return;
    }

    cout << "File Name: " << filename << "\n";
    cout << "File Size: " << file.tellg() << " bytes\n";
    file.close();
}
