#include <iostream>
#include <fstream>
#include <cstring>
#include <openssl/aes.h>

using namespace std;

const unsigned char AES_KEY_VAL[32] = "1234567890abcdef1234567890abcdef";
const unsigned char AES_IV[AES_BLOCK_SIZE] = "abcdef1234567890";

void encryptFile(const string &filename) {
    AES_KEY enc_key;
    AES_set_encrypt_key(AES_KEY_VAL, 256, &enc_key);

    ifstream input(filename, ios::binary);
    ofstream output("encrypted_" + filename, ios::binary);

    if (!input || !output) {
        cerr << "Error opening file." << endl;
        return;
    }

    unsigned char buffer[16];
    unsigned char cipher[16];
    while (input.read((char *)buffer, sizeof(buffer)) || input.gcount() > 0) {
        AES_cfb128_encrypt(buffer, cipher, input.gcount(), &enc_key, (unsigned char *)AES_IV, nullptr, AES_ENCRYPT);
        output.write((char *)cipher, input.gcount());
    }

    input.close();
    output.close();
    cout << "Encryption complete. Output saved to 'encrypted_" << filename << "'." << endl;
}

void decryptFile(const string &filename) {
    AES_KEY dec_key;
    AES_set_decrypt_key(AES_KEY_VAL, 256, &dec_key);

    ifstream input(filename, ios::binary);
    ofstream output("decrypted_" + filename, ios::binary);

    if (!input || !output) {
        cerr << "Error opening file." << endl;
        return;
    }

    unsigned char buffer[16];
    unsigned char plain[16];
    while (input.read((char *)buffer, sizeof(buffer)) || input.gcount() > 0) {
        AES_cfb128_encrypt(buffer, plain, input.gcount(), &dec_key, (unsigned char *)AES_IV, nullptr, AES_DECRYPT);
        output.write((char *)plain, input.gcount());
    }

    input.close();
    output.close();
    cout << "Decryption complete. Output saved to 'decrypted_" << filename << "'." << endl;
}

int main() {
    string filename;
    int choice;

    cout << "Enter the filename: ";
    cin >> filename;

    cout << "1. Encrypt File\n2. Decrypt File\nChoose an option: ";
    cin >> choice;

    if (choice == 1) {
        encryptFile(filename);
    } else if (choice == 2) {
        decryptFile(filename);
    } else {
        cout << "Invalid option!" << endl;
    }

    return 0;
}
