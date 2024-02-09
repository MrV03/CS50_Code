#include <sodium.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 1024
#define KEY_LEN crypto_secretbox_KEYBYTES
#define SALT_LEN crypto_pwhash_SALTBYTES
#define NONCE_LEN crypto_secretbox_NONCEBYTES

void handleErrors(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
    exit(EXIT_FAILURE);
}

void deriveKeyAndSalt(const char *password, unsigned char *key, unsigned char *salt) {
    if (sodium_init() < 0) {
        handleErrors("Error initializing libsodium");
    }

    // Generate the salt from the password
    if (crypto_pwhash(key, KEY_LEN, password, strlen(password),
                       salt, crypto_pwhash_OPSLIMIT_INTERACTIVE, crypto_pwhash_MEMLIMIT_INTERACTIVE, crypto_pwhash_ALG_ARGON2ID13) != 0) {
        handleErrors("Key derivation failed");
    }
}


void encryptFile(const char *inputFileName, const char *outputFileName, const char *password) {
    FILE *inputFile = fopen(inputFileName, "rb");
    if (!inputFile) {
        handleErrors("Error opening input file");
    }

    FILE *outputFile = fopen(outputFileName, "wb");
    if (!outputFile) {
        fclose(inputFile);
        handleErrors("Error opening output file");
    }

    // Generate the salt and derive the key from the password
    unsigned char key[KEY_LEN], salt[SALT_LEN];
    deriveKeyAndSalt(password, key, salt);

    // Write the salt to the output file
    fwrite(salt, 1, SALT_LEN, outputFile);

    size_t bytesRead, totalBytes = 0;
    unsigned char inputBuffer[MAX_BUFFER_SIZE], cipherBuffer[MAX_BUFFER_SIZE + crypto_secretbox_MACBYTES];

    while ((bytesRead = fread(inputBuffer, 1, sizeof(inputBuffer), inputFile)) > 0) {
        // Generate a new nonce for each block
        unsigned char nonce[NONCE_LEN];
        randombytes_buf(nonce, NONCE_LEN);

        // Encrypt the data using secretbox (authenticated encryption)
        if (crypto_secretbox_easy(cipherBuffer, inputBuffer, bytesRead, nonce, key) != 0) {
            fclose(inputFile);
            fclose(outputFile);
            handleErrors("Encryption error");
        }

        // Write nonce and encrypted data to the output file
        fwrite(nonce, 1, NONCE_LEN, outputFile);
        fwrite(cipherBuffer, 1, bytesRead + crypto_secretbox_MACBYTES, outputFile);

        totalBytes += bytesRead;
        printf("Progress: %zu bytes encrypted\r", totalBytes);
        fflush(stdout);
    }

    fclose(inputFile);
    fclose(outputFile);

    printf("\nFile encrypted successfully.\n");
}

void decryptFile(const char *inputFileName, const char *outputFileName, const char *password) {
    FILE *inputFile = fopen(inputFileName, "rb");
    if (!inputFile) {
        handleErrors("Error opening input file");
    }

    FILE *outputFile = fopen(outputFileName, "wb");
    if (!outputFile) {
        fclose(inputFile);
        handleErrors("Error opening output file");
    }

    // Read the salt from the input file
    unsigned char salt[SALT_LEN];
    if (fread(salt, 1, SALT_LEN, inputFile) != SALT_LEN) {
        fclose(inputFile);
        fclose(outputFile);
        handleErrors("Error reading salt");
    }

    // Derive the key from the password and the read salt
    unsigned char key[KEY_LEN];
    deriveKeyAndSalt(password, key, salt);

    size_t bytesRead, totalBytes = 0;
    unsigned char nonce[NONCE_LEN], cipherBuffer[MAX_BUFFER_SIZE + crypto_secretbox_MACBYTES], plainBuffer[MAX_BUFFER_SIZE];

    while ((bytesRead = fread(nonce, 1, NONCE_LEN, inputFile)) == NONCE_LEN &&
           (bytesRead = fread(cipherBuffer, 1, MAX_BUFFER_SIZE + crypto_secretbox_MACBYTES, inputFile)) > 0) {
        // Decrypt the data using secretbox (authenticated decryption)
        if (crypto_secretbox_open_easy(plainBuffer, cipherBuffer, bytesRead, nonce, key) != 0) {
            fclose(inputFile);
            fclose(outputFile);
            handleErrors("Decryption error");
        }

        fwrite(plainBuffer, 1, bytesRead - crypto_secretbox_MACBYTES, outputFile);

        totalBytes += bytesRead - crypto_secretbox_MACBYTES;
        printf("Progress: %zu bytes decrypted\r", totalBytes);
        fflush(stdout);
    }

    fclose(inputFile);
    fclose(outputFile);

    printf("\nFile decrypted successfully.\n");
}

int main() {
    char inputFileName[256], outputFileName[256], password[256];

    if (sodium_init() < 0) {
        handleErrors("Invalid choice");
    }

    printf("Enter input file name: ");
    scanf("%255s", inputFileName);

    printf("Enter output file name: ");
    scanf("%255s", outputFileName);

    printf("Enter encryption/decryption password: ");
    scanf("%255s", password);

    int choice;
    printf("Choose operation:\n");
    printf("1. Encrypt\n");
    printf("2. Decrypt\n");
    scanf("%d", &choice);

    if (choice == 1) {
        encryptFile(inputFileName, outputFileName, password);
    } else if (choice == 2) {
        decryptFile(inputFileName, outputFileName, password);
    } else {
        printf("Invalid choice.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
