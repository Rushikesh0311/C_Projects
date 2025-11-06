

#include "comman.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define XOR_KEY 'K'   // XOR key used everywhere

/* ------------------ small helpers ------------------ */

/* remove trailing newline from fgets */
void strip_newline(char *s)
{
    if (!s) return;
    size_t n = strlen(s);
    while (n > 0 && (s[n-1] == '\n' || s[n-1] == '\r')) s[--n] = '\0';
}

/* Encrypt or Decrypt password using XOR (same key does both) */
void xor_encrypt_decrypt(char text[], char key)
{
    // loop through each character in text
    for (int i = 0; text[i] != '\0'; i++)
        text[i] = text[i] ^ key;   // XOR each character with the key
}

/* Convert normal text into HEX form (for saving safely in file) */
void text_to_hex(char text[], char hex_out[])
{
    char hex_digits[] = "0123456789ABCDEF";  // all hex characters
    int len = strlen(text);                  // find text length
    int j = 0;                               // output index

    // loop through each character in text
    for (int i = 0; i < len; i++)
    {
        unsigned char val = (unsigned char)text[i]; // get ASCII value
        hex_out[j++] = hex_digits[val >> 4];        // take high 4 bits
        hex_out[j++] = hex_digits[val & 0x0F];      // take low 4 bits
    }

    hex_out[j] = '\0'; // end the hex string
}

/* Convert one HEX character (like 'A') into number (10) */
int hex_char_value(char c)
{
    if (c >= '0' && c <= '9') return c - '0';        // for 0–9
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;   // for A–F
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;   // for a–f
    return -1; // if not valid hex char
}

/* Convert HEX text back to normal text (reverse of text_to_hex) */
// int hex_to_text(char hex[], char text_out[])
// {
//     int len = strlen(hex);        // get hex string length
//     if (len % 2 != 0) return -1;  // must be even (2 hex = 1 char)

//     int j = 0;                    // output index
//     // process 2 hex chars at a time
//     for (int i = 0; i < len; i += 2)
//     {
//         int high = hex_char_value(hex[i]);       // convert 1st hex char
//         int low  = hex_char_value(hex[i + 1]);   // convert 2nd hex char
//         if (high < 0 || low < 0) return -1;      // invalid hex check

//         text_out[j++] = (high << 4) | low;       // join both nibbles
//     }
//     text_out[j] = '\0'; // end the normal text
//     return j;            // return total bytes written
// }


/* Convert HEX back to normal text (reverse of text_to_hex)
   hex: input hex string (NUL terminated)
   text_out: output buffer (will receive bytes + NUL)
   out_max: size of text_out buffer (including NUL)
   returns: number of bytes written (not counting NUL), or -1 on error
*/
int hex_to_text(const char hex[], char text_out[], size_t out_max)
{
    if (!hex || !text_out) return -1;

    int len = (int)strlen(hex);          // hex string length
    if (len % 2 != 0) return -1;         // must be even length

    int bytes = len / 2;                 // how many bytes will be produced
    if (bytes + 1 > (int)out_max)        // +1 for terminating NUL
        return -1;                       // output buffer too small

    for (int i = 0; i < bytes; ++i)
    {
        char a = hex[2 * i];
        char b = hex[2 * i + 1];

        int high = hex_char_value(a);    // reuse your helper
        int low  = hex_char_value(b);
        if (high < 0 || low < 0) return -1; // invalid hex chars

        text_out[i] = (char)((high << 4) | low); // combine into one byte
    }

    text_out[bytes] = '\0';              // null-terminate
    return bytes;                        // return number of bytes written
}




/* ------------------ init & meta ------------------ */

/* initialize manager in memory */
void initialize(struct PasswordManager *m)
{
    if (!m) return;
    m->credentialCount = 0;
    loadCredentialsFromFile(m);
}

/* create meta file and save master password (plain for prototype) */
void create_metaFile(const char *meta_file)
{
    char a[64], b[64];
    printf("\nSet master Password: ");
    if (!fgets(a, sizeof(a), stdin)) 
        return;
    strip_newline(a);
    printf("\nConfirm your password: ");
    if (!fgets(b, sizeof(b), stdin)) return;
    strip_newline(b);

    if (strcmp(a, b) == 0) {
        FILE *f = fopen(META_FILE, "w");
        if (!f) 
        { 
            perror("create meta"); 
            return; 
        }
        fprintf(f, "%s\n", a);
        fclose(f);
        printf("\nMaster password saved.\n");
    } else {
        printf("\nPasswords do not match.\n");
    }
}

/* verify master password; returns 1 ok, 0 fail, -1 error */
int verify_master_pass(const char *meta_file, int max_attempts)
{
    char stored[128];
    char ent[128];

    FILE *f = fopen(META_FILE, "r");
    if (!f) {
        printf("No master file. Create now.\n");
        create_metaFile(META_FILE);
        f = fopen(META_FILE, "r");
        if (!f) 
        { perror("open meta"); 
            return -1; 
        }
    }

    if (!fgets(stored, sizeof(stored), f)) 
    { 
        fclose(f); 
        return -1; 
    }
    fclose(f);
    strip_newline(stored);

    for (int i = 0; i < max_attempts; ++i) 
    {
        printf("\nEnter your master password: ");
        if (!fgets(ent, sizeof(ent), stdin)) return 0;
        strip_newline(ent);
        if (strcmp(stored, ent) == 0) 
        { 
            printf("\nMaster verified.\n");
            return 1; 
        }
        printf("\nWrong. %d attempts left.\n", max_attempts - 1 - i);
    }
    return 0;
}

/* ------------------ CRUD operations ------------------ */

/* add credential:
   - read plain password
   - encrypt with XOR
   - convert encrypted bytes to hex
   - store hex in password[]
*/
void add_credentials(struct PasswordManager *m)
{
    if (!m) return;
    int s = m->credentialCount;
    if (s >= 100) { printf("Full\n"); return; }

    char site[64], user[64], plain[128];
    printf("Enter the Site name: ");
    if (!fgets(site, sizeof(site), stdin)) 
        return; 
    strip_newline(site);
    printf("Enter the Username: ");
    if (!fgets(user, sizeof(user), stdin)) 
        return; 
    strip_newline(user);
    printf("Enter the Password: ");
    if (!fgets(plain, sizeof(plain), stdin)) 
        return; 
    strip_newline(plain);

    // encrypt plain in-place
    xor_encrypt_decrypt(plain, XOR_KEY);

    // convert encrypted -> hex
    // hex buffer size: 2 * strlen(plain) + 1; use temp with safe size
    char hexbuf[512];
    text_to_hex(plain, hexbuf);

    // copy into struct (use size of fields)
    strncpy(m->credentials[s].site, site, sizeof(m->credentials[s].site)-1);
    m->credentials[s].site[sizeof(m->credentials[s].site)-1] = '\0';

    strncpy(m->credentials[s].username, user, sizeof(m->credentials[s].username)-1);
    m->credentials[s].username[sizeof(m->credentials[s].username)-1] = '\0';

    strncpy(m->credentials[s].password, hexbuf, sizeof(m->credentials[s].password)-1);
    m->credentials[s].password[sizeof(m->credentials[s].password)-1] = '\0';

    m->credentialCount++;
    saveCredentialsToFile(m);
    printf("Added\n");
}

/* list credentials:
   - for each record: hex -> binary, decrypt, print
*/
void list_credentials(struct PasswordManager *m)
{
    if (!m) return;
    if (m->credentialCount == 0) { printf("No credentials.\n"); return; }

    printf("\n---------------------------------------------------------------\n");
    printf("%-4s | %-12s | %-15s | %-15s\n", "No", "Site", "Username", "Password");
    printf("---------------------------------------------------------------\n");

    char bin[512]; // temp for decoded bytes
    for (int i = 0; i < m->credentialCount; ++i) {
        if (hex_to_text(m->credentials[i].password, bin, sizeof(bin)) < 0) {
            strncpy(bin, "[err]", sizeof(bin)-1);
            bin[sizeof(bin)-1] = '\0';
        } else {
            xor_encrypt_decrypt(bin, XOR_KEY); // decrypt
        }
        printf("%-4d | %-12s | %-15s | %-15s\n",
               i+1,
               m->credentials[i].site,
               m->credentials[i].username,
               bin);
    }
    printf("---------------------------------------------------------------\n");
}

/* search by site substring; prints decrypted password */
void search_credentials(struct PasswordManager *m)
{
    if (!m) return;
    if (m->credentialCount == 0) { printf("No credentials.\n"); return; }

    char q[64];
    printf("Enter Site name to search: ");
    if (!fgets(q, sizeof(q), stdin)) return; strip_newline(q);

    printf("\n---------------------------------------------------------------\n");
    printf("%-4s | %-12s | %-15s | %-15s\n", "No", "Site", "Username", "Password");
    printf("---------------------------------------------------------------\n");

    char bin[512];
    int found = 0;
    for (int i = 0; i < m->credentialCount; ++i) {
        if (strstr(m->credentials[i].site, q)) {
            if (hex_to_text(m->credentials[i].password, bin, sizeof(bin)) < 0) 
                strncpy(bin, "[err]", sizeof(bin)-1);
            else 
                xor_encrypt_decrypt(bin, XOR_KEY);
                
            printf("%-4d | %-12s | %-15s | %-15s\n",
                   i+1,
                   m->credentials[i].site,
                   m->credentials[i].username,
                   bin);
            found = 1;
        }
    }
    if (!found) printf("No match\n");
    printf("---------------------------------------------------------------\n");
}

/* delete credential by index (safe, returns to menu) */
void delete_credential(struct PasswordManager *m)
{
    if (!m) return;
    if (m->credentialCount == 0) { printf("No credentials.\n"); return; }

    list_credentials(m);
    printf("Enter index to delete: ");
    char line[32];
    if (!fgets(line, sizeof(line), stdin)) return;
    int idx = 0;
    if (sscanf(line, "%d", &idx) != 1) { printf("Invalid\n"); return; }
    if (idx < 1 || idx > m->credentialCount) { printf("Out of range\n"); return; }

    int p = idx - 1;
    for (int i = p; i < m->credentialCount - 1; ++i) m->credentials[i] = m->credentials[i + 1];

    // clear last 
    m->credentials[m->credentialCount - 1].site[0] = '\0';
    m->credentials[m->credentialCount - 1].username[0] = '\0';
    m->credentials[m->credentialCount - 1].password[0] = '\0';

    m->credentialCount--;
    saveCredentialsToFile(m);
    printf("Deleted\n");
}

int reset_master_pass(const char *meta_file, int max_attempts)
{
    printf("\nVerify your current Master Password First!!\n");
    int result = verify_master_pass(meta_file, max_attempts);

    if (result != 1)
    {
        printf("\nInvalid Verification\n");
        return 0;  // failed verification
    }
    else
    {
        int opt;

        do
        {
            printf("\n-----Reset Pass Menu-----\n");
            printf("1. Set your own Password\n");
            printf("2. Set strong Password\n");
            printf("3. Exit\n");
            printf("\nSelect Option: ");
            scanf("%d", &opt);
            getchar(); // to clear leftover '\n' from scanf

            switch (opt)
            {
                case 1:
                {
                    char a[64], b[64];
                    printf("\nSet master Password: ");
                    if (!fgets(a, sizeof(a), stdin))
                        return 0; // return 0 instead of bare return

                    strip_newline(a);

                    printf("\nConfirm your password: ");
                    if (!fgets(b, sizeof(b), stdin))
                        return 0; // return 0 instead of bare return

                    strip_newline(b);

                    if (strcmp(a, b) == 0)
                    {
                        FILE *f = fopen(meta_file, "w");
                        if (!f)
                        {
                            perror("create meta");
                            return 0;
                        }
                        fprintf(f, "%s\n", a);
                        fclose(f);
                        printf("\nMaster password saved.\n");
                    }
                    else
                    {
                        printf("\nPasswords do not match.\n");
                    }
                    break;
                }

                case 2:
                {
                    printf("Work in progress\n");
                    break;
                }

                case 3:
                    printf("Exited\n");
                    return 1; // exit loop and return

                default:
                    printf("Invalid Option, try again.\n");
                    break;
            }

        } while (opt != 3);

        return 1; // password reset process done
    }
}

