#ifndef COMMAM_H
#define COMMAM_H

#include <stdio.h>
#include <string.h>

#define META_FILE "meta.dat"


struct Credential {
    char site[50];
    char username[50];
    char password[128];  // encrypted later
    //char timestamp[32];
};

struct PasswordManager {
    struct Credential credentials[100];  // max 100 credentials -->array of credentials + count
    int credentialCount;
};

//void XOR_cipher(char* text,char key); // encrypt--> Decrypt


void initialize(struct PasswordManager *manager);
void add_credentials(struct PasswordManager *manager);
void list_credentials(struct PasswordManager *manager);
void search_credentials(struct PasswordManager *manager);
void delete_credential(struct PasswordManager* manager);

void create_metaFile(const char *meta_file); // for storing master pass
int verify_master_pass(const char* meta_file,int max_attempts);
void strip_newline(char *s);

void xor_encrypt_decrypt(char text[], char key);
int hex_char_value(char c);
void text_to_hex(char text[], char hex_out[]);
int hex_to_text(const char hex[], char text_out[], size_t out_max);
int reset_master_pass(const char *meta_file, int max_attempts);


void saveCredentialsToFile(struct PasswordManager *manager);
void loadCredentialsFromFile(struct PasswordManager *manager);

//void strip_newline(char *s);


//void create_metaFile(const char* meta_file);




#endif