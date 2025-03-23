#include <iostream>
#include <fstream>
#include <openssl/aes.h>
#include "encryption.h"

using namespace std;

const unsigned char AES_KEY[32] = "1234567890abcdef1234567890abcdef";
const unsigned char AES_IV[AES_BLOCK_SIZE] = "abcdef1234567890";

void encryptFile(const string &filename) {
    ifstream inFile(filename, ios::binary);
    ofstream outFile(filename + ".enc", ios::binary);

    if (!inFile || !outFile) {
        cout << "File error!\n";
        return;
    }

    AES_KEY enc_key;
    AES_set_encrypt_key(AES_KEY, 256, &enc_key);

    unsigned char buffer[16];
    unsigned char cipher[16];

    while (inFile.read((char *)buffer, sizeof(buffer))) {
        AES_cfb128_encrypt(buffer, cipher, sizeof(buffer), &enc_key, (unsigned char *)AES_IV, nullptr, AES_ENCRYPT);
        outFile.write((char *)cipher, sizeof(cipher));
    }

    inFile.close();
    outFile.close();
    cout << "File encrypted successfully!\n";
}

void decryptFile(const string &filename) {
    ifstream inFile(filename, ios::binary);
    ofstream outFile(filename.substr(0, filename.size() - 4), ios::binary);

    if (!inFile || !outFile) {
        cout << "File error!\n";
        return;
    }

    AES_KEY dec_key;
    AES_set_decrypt_key(AES_KEY, 256, &dec_key);

    unsigned char buffer[16];
    unsigned char plain[16];

    while (inFile.read((char *)buffer, sizeof(buffer))) {
        AES_cfb128_encrypt(buffer, plain, sizeof(buffer), &dec_key, (unsigned char *)AES_IV, nullptr, AES_DECRYPT);
        outFile.write((char *)plain, sizeof(plain));
    }

    inFile.close();
    outFile.close();
    cout << "File decrypted successfully!\n";
}
