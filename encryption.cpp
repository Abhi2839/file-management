#include <iostream>
#include <fstream>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <cstring>

const unsigned char AES_KEY[32] = "1234567890abcdef1234567890abcdef";

void handleErrors() {
    std::cerr << "Error occurred during OpenSSL operation." << std::endl;
    exit(EXIT_FAILURE);
}

void encryptFile(const std::string &filename) {
    unsigned char iv[16];
    if (!RAND_bytes(iv, sizeof(iv))) {
        handleErrors();
    }

    std::ifstream infile(filename, std::ios::binary);
    std::ofstream outfile(filename + ".enc", std::ios::binary);

    if (!infile.is_open() || !outfile.is_open()) {
        std::cerr << "File open error." << std::endl;
        return;
    }

    outfile.write(reinterpret_cast<const char *>(iv), sizeof(iv));

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();

    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, AES_KEY, iv) != 1) {
        handleErrors();
    }

    unsigned char buffer[1024];
    unsigned char ciphertext[1024 + EVP_CIPHER_block_size(EVP_aes_256_cbc())];
    int len;

    while (infile.read(reinterpret_cast<char *>(buffer), sizeof(buffer)) || infile.gcount() > 0) {
        if (EVP_EncryptUpdate(ctx, ciphertext, &len, buffer, infile.gcount()) != 1) {
            handleErrors();
        }
        outfile.write(reinterpret_cast<char *>(ciphertext), len);
    }

    if (EVP_EncryptFinal_ex(ctx, ciphertext, &len) != 1) {
        handleErrors();
    }
    outfile.write(reinterpret_cast<char *>(ciphertext), len);

    EVP_CIPHER_CTX_free(ctx);
    infile.close();
    outfile.close();
    std::cout << "Encryption complete: " << filename << ".enc" << std::endl;
}

void decryptFile(const std::string &filename) {
    unsigned char iv[16];

    std::ifstream infile(filename, std::ios::binary);
    std::ofstream outfile("decrypted_" + filename, std::ios::binary);

    if (!infile.is_open() || !outfile.is_open()) {
        std::cerr << "File open error." << std::endl;
        return;
    }

    infile.read(reinterpret_cast<char *>(iv), sizeof(iv));

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();

    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, AES_KEY, iv) != 1) {
        handleErrors();
    }

    unsigned char buffer[1024];
    unsigned char plaintext[1024 + EVP_CIPHER_block_size(EVP_aes_256_cbc())];
    int len;

    while (infile.read(reinterpret_cast<char *>(buffer), sizeof(buffer)) || infile.gcount() > 0) {
        if (EVP_DecryptUpdate(ctx, plaintext, &len, buffer, infile.gcount()) != 1) {
            handleErrors();
        }
        outfile.write(reinterpret_cast<char *>(plaintext), len);
    }

    if (EVP_DecryptFinal_ex(ctx, plaintext, &len) != 1) {
        handleErrors();
    }
    outfile.write(reinterpret_cast<char *>(plaintext), len);

    EVP_CIPHER_CTX_free(ctx);
    infile.close();
    outfile.close();
    std::cout << "Decryption complete: decrypted_" << filename << std::endl;
}

int main() {
    std::string filename;
    std::cout << "Enter the filename to encrypt: ";
    std::cin >> filename;
    encryptFile(filename);

    std::cout << "Enter the filename to decrypt: ";
    std::cin >> filename;
    decryptFile(filename);

    return 0;
}
