#include <iostream>
#include "authentication.h"
#include "encryption.h"
#include "file_operations.h"

using namespace std;

int main() {
    Role userRole = authenticate();

    if (userRole == INVALID) {
        cout << "Authentication Failed!\n";
        return 1;
    }

    int choice;
    string filename;

    do {
        cout << "\nSecure File Management System\n";
        cout << "1. Encrypt File\n";
        cout << "2. Decrypt File\n";
        cout << "3. View File Metadata\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                if (userRole == ADMIN) {
                    cout << "Enter filename to encrypt: ";
                    cin >> filename;
                    encryptFile(filename);
                } else {
                    cout << "Access Denied: Admins only.\n";
                }
                break;
            case 2:
                if (userRole == ADMIN) {
                    cout << "Enter filename to decrypt: ";
                    cin >> filename;
                    decryptFile(filename);
                } else {
                    cout << "Access Denied: Admins only.\n";
                }
                break;
            case 3:
                cout << "Enter filename to view metadata: ";
                cin >> filename;
                viewMetadata(filename);
                break;
            case 4:
                cout << "Exiting the system.\n";
                break;
            default:
                cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 4);

    return 0;
}
