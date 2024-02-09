# File Encryption / Decryption System
## Video demonstration here: https://youtu.be/WY1NBQnuks0
### Description:
This system is a command-line tool that provides a secure way to encrypt and decrypt files using the Libsodium library. It utilizes password-based key derivation with a salt to enhance security.

# How to run:
## Prerequisites:
Libsodium Library
Ensure that the libsodium library is installed on the system. If not, it can be downloaded from the Libsodium official website:
(https://libsodium.gitbook.io/doc/)

For example, on Ubuntu/Debian systems:
```
sudo apt-get install libsodium-dev
```

**Compiling the Program:**
Use the following command to compile the program with the libsodium library:
```
gcc -o project project.c -lsodium -lm
```
**Execution:**
Once compiled, run the executable file by using:
```
./project
```
Then follow the prompts. This assumes that an input file already exists.

Enter the input file name, output file name, and a password (remember this password as it will be used for both processes) when prompted.
Choose the operation whether encryption or decryption by entering '1' or '2' respectively.
**Monitor progress:**
During the encryption/decryption process, the program will display progress messages indicating the number of bytes processed.
Completion message:
Once the process is complete, the program will display a "Success" message.
The completed file can be viewed in the directory.

# Usage:
Once compiled and running the executable file:

Enter input file name: Specify the path to the input file for encryption or decryption.
Enter output file name: Provide the path for the output file where the result will be saved.
Enter Encryption/Decryption password: Enter the password for key derivation.
Choose Operation: Select "1" for Encryption or "2" for decryption.
Examples usage:
***Encryption:***

```
./project
Enter input file name: input.txt
Enter output file name: encrypted.txt
Enter Encryption/Decryption password: Password1234
Choose Operation:
1. Encrypt
2. Decrypt
1
```

***Decryption:***

```
./project
Enter input file name: encrypted.txt
Enter output file name: decrypted.txt
Enter Encryption/Decryption password: Password1234
Choose Operation:
1. Encrypt
2. Decrypt
2
```

It should be noted that the security of the program is dependent on the strength of the password, as it is used to create the key for encryption/decryption. A stronger password results in a stronger key.

# In-Depth Explanation:
**Password-based Key Derivation:**
The key derivation process involves generating a cryptographic key from a user-provided password. In this system, the deriveKeyAndSalt function is responsible for this process. It utilizes the Libsodium crypto_pwhash function with the Argon2id algorithm, which is a secure key derivation function designed to resist GPU and side-channel attacks.

**Encryption Process:**
The encryptFile function takes an input file, generates a unique nonce for each block, encrypts the block using the secret key derived from the user's password and the provided salt, and writes the encrypted data along with the nonce to the output file. This process is repeated for each block until the entire file is encrypted.

**Decryption Process:**
On the other hand, the decryptFile function reads the salt from the input file, derives the secret key from the user's password and the salt, and uses this key to decrypt the file. The unique nonces associated with each block during encryption are used to ensure secure decryption.

# Security Measures:
**Password Strength:**
The security of the program heavily relies on the strength of the user-provided password. Users are encouraged to choose strong passwords containing a mix of uppercase and lowercase letters, numbers, and special characters.

**Cryptographic Algorithms:**
Libsodium employs state-of-the-art cryptographic algorithms such as Argon2id for key derivation and XSalsa20-Poly1305 for encryption. These algorithms are widely recognized for their security properties.

**Random Nonces:**
A unique nonce is generated for each block during encryption, ensuring that even if the same data is encrypted multiple times, the ciphertext will be different. This prevents certain types of attacks, such as replay attacks.

**Constant-Time Operations:**
The code is designed to execute in constant time, mitigating timing-based side-channel attacks.

# Use Cases:
**Secure File Transfer:**
Encrypt sensitive files before transferring them over untrusted networks.

**Data Backup:**
Securely store sensitive backups by encrypting them with a strong password.

**Confidential Documents:**
Protect confidential documents by encrypting them with a secure key derived from a strong password.

# Conclusion:
This file encryption/decryption system provides a robust and secure solution for protecting sensitive data. Users should follow best practices for choosing strong passwords to ensure the effectiveness of the encryption process. The implementation leverages modern cryptographic techniques and algorithms to deliver a reliable tool for safeguarding information.

Feel free to explore the source code for a deeper understanding of the cryptographic processes involved. Experimenting with the tool in different scenarios can provide insights into its versatility and usefulness in various security-conscious applications.

Note:
The provided source code and documentation are intended for educational purposes, and users should exercise caution and adhere to legal and ethical considerations when using such tools in real-world scenarios.




