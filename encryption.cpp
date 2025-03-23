#include <iostream>
#include <fstream>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <cstring>

const unsigned char ENCRYPTION_KEY[32] = "1234567890abcdef1234567890abcdef";
unsigned char AES_IV[AES_BLOCK_SIZE] = {0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x31, 0x32,
                                        0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30};

void encryptFile(const std::string &filename) {
    AES_KEY enc_key;
    AES_set_encrypt_key(ENCRYPTION_KEY, 256, &enc_key);

    std::ifstream infile(filename, std::ios::binary);
    std::ofstream outfile(filename + ".enc", std::ios::binary);

    unsigned char buffer[AES_BLOCK_SIZE];
    unsigned char cipher[AES_BLOCK_SIZE];

    while (infile.read(reinterpret_cast<char *>(buffer), AES_BLOCK_SIZE)) {
        AES_encrypt(buffer, cipher, &enc_key);
        outfile.write(reinterpret_cast<char *>(cipher), AES_BLOCK_SIZE);
    }

    infile.close();
    outfile.close();
    std::cout << "Encryption complete: " << filename << ".enc" << std::endl;
}

void decryptFile(const std::string &filename) {
    AES_KEY dec_key;
    AES_set_decrypt_key(ENCRYPTION_KEY, 256, &dec_key);

    std::ifstream infile(filename, std::ios::binary);
    std::ofstream outfile("decrypted_" + filename, std::ios::binary);

    unsigned char buffer[AES_BLOCK_SIZE];
    unsigned char plain[AES_BLOCK_SIZE];

    while (infile.read(reinterpret_cast<char *>(buffer), AES_BLOCK_SIZE)) {
        AES_decrypt(buffer, plain, &dec_key);
        outfile.write(reinterpret_cast<char *>(plain), AES_BLOCK_SIZE);
    }

    infile.close();
    outfile.close();
    std::cout << "Decryption complete: decrypted_" << filename << std::endl;
}

int main() {
    std::string filename;
    std::cout << "Enter filename to encrypt: ";
    std::cin >> filename;
    encryptFile(filename);

    std::cout << "Enter filename to decrypt: ";
    std::cin >> filename + ".enc";
    decryptFile(filename + ".enc");

    return 0;
}
