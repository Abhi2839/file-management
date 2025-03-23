#include <iostream>
#include <fstream>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <cstring>

const unsigned char ENCRYPTION_KEY[32] = "1234567890abcdef1234567890abcdef";

void handleErrors() {
    std::cerr << "An error occurred during OpenSSL operation." << std::endl;
    exit(1);
}

void encryptFile(const std::string &filename) {
    unsigned char iv[16];
    if (!RAND_bytes(iv, sizeof(iv))) {
        handleErrors();
    }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        handleErrors();
    }

    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, ENCRYPTION_KEY, iv) != 1) {
        handleErrors();
    }

    std::ifstream infile(filename, std::ios::binary);
    std::ofstream outfile(filename + ".enc", std::ios::binary);

    outfile.write(reinterpret_cast<const char *>(iv), sizeof(iv));

    unsigned char buffer[1024];
    unsigned char cipher[1024 + EVP_CIPHER_block_size(EVP_aes_256_cbc())];
    int len;

    while (infile.read(reinterpret_cast<char *>(buffer), sizeof(buffer))) {
        if (EVP_EncryptUpdate(ctx, cipher, &len, buffer, infile.gcount()) != 1) {
            handleErrors();
        }
        outfile.write(reinterpret_cast<char *>(cipher), len);
    }

    if (EVP_EncryptFinal_ex(ctx, cipher, &len) != 1) {
        handleErrors();
    }
    outfile.write(reinterpret_cast<char *>(cipher), len);

    EVP_CIPHER_CTX_free(ctx);
    infile.close();
    outfile.close();
    std::cout << "Encryption complete: " << filename << ".enc" << std::endl;
}

void decryptFile(const std::string &filename) {
    unsigned char iv[16];

    std::ifstream infile(filename, std::ios::binary);
    std::ofstream outfile("decrypted_" + filename, std::ios::binary);

    infile.read(reinterpret_cast<char *>(iv), sizeof(iv));

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        handleErrors();
    }

    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, ENCRYPTION_KEY, iv) != 1) {
        handleErrors();
    }

    unsigned char buffer[1024];
    unsigned char plain[1024 + EVP_CIPHER_block_size(EVP_aes_256_cbc())];
    int len;

    while (infile.read(reinterpret_cast<char *>(buffer), sizeof(buffer))) {
        if (EVP_DecryptUpdate(ctx, plain, &len, buffer, infile.gcount()) != 1) {
            handleErrors();
        }
        outfile.write(reinterpret_cast<char *>(plain), len);
    }

    if (EVP_DecryptFinal_ex(ctx, plain, &len) != 1) {
        handleErrors();
    }
    outfile.write(reinterpret_cast<char *>(plain), len);

    EVP_CIPHER_CTX_free(ctx);
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
    std::cin >> filename;
    decryptFile(filename);

    return 0;
}
