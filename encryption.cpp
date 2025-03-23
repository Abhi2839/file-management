#include <iostream>
#include <fstream>
#include <cstring>
#include <openssl/aes.h>
#include <openssl/rand.h>

const unsigned char AES_KEY_VAL[32] = "1234567890abcdef1234567890abcdef";
const unsigned char AES_IV[AES_BLOCK_SIZE] = "abcdef1234567890";

void encryptFile(const std::string &filename) {
    AES_KEY enc_key;
    AES_set_encrypt_key(AES_KEY_VAL, 256, &enc_key);

    std::ifstream infile(filename, std::ios::binary);
    std::ofstream outfile(filename + ".enc", std::ios::binary);

    unsigned char buffer[AES_BLOCK_SIZE];
    unsigned char cipher[AES_BLOCK_SIZE];

    while (infile.read(reinterpret_cast<char*>(buffer), AES_BLOCK_SIZE)) {
        AES_encrypt(buffer, cipher, &enc_key);
        outfile.write(reinterpret_cast<char*>(cipher), AES_BLOCK_SIZE);
    }

    infile.close();
    outfile.close();
}

void decryptFile(const std::string &filename) {
    AES_KEY dec_key;
    AES_set_decrypt_key(AES_KEY_VAL, 256, &dec_key);

    std::ifstream infile(filename, std::ios::binary);
    std::ofstream outfile("decrypted_" + filename, std::ios::binary);

    unsigned char buffer[AES_BLOCK_SIZE];
    unsigned char plain[AES_BLOCK_SIZE];

    while (infile.read(reinterpret_cast<char*>(buffer), AES_BLOCK_SIZE)) {
        AES_decrypt(buffer, plain, &dec_key);
        outfile.write(reinterpret_cast<char*>(plain), AES_BLOCK_SIZE);
    }

    infile.close();
    outfile.close();
}

int main() {
    std::string filename;
    std::cout << "Enter the file name to encrypt: ";
    std::cin >> filename;

    encryptFile(filename);
    std::cout << "Encryption completed!\n";

    std::cout << "Enter the encrypted file name to decrypt: ";
    std::cin >> filename;

    decryptFile(filename);
    std::cout << "Decryption completed!\n";

    return 0;
}
