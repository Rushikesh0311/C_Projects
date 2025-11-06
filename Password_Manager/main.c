#include "comman.h"
#include <stdio.h>
#include <string.h>

int main() {
    // char stored_pass[50];
    // char entered_pass[50];

    // // Open meta file or create if not exists
    // FILE *fp = fopen(META_FILE, "r");
    // if (fp == NULL) {
    //     printf("No master file found. Please set master password.\n");
    //     create_metaFile(META_FILE);
    //     fp = fopen(META_FILE, "r");
    //     if (fp == NULL) {
    //         perror("Failed to open meta file after creation");
    //         return 1;
    //     }
    // }

    // // Read stored master password
    // if (!fgets(stored_pass, sizeof(stored_pass), fp)) {
    //     fclose(fp);
    //     printf("Meta file is empty!\n");
    //     return 1;
    // }
    // fclose(fp);

    // // Remove newline from stored password
    // strip_newline(stored_pass);

    // // Master password verification
    // int flag = 0;
    // for (int attempt = 0; attempt < 3; ++attempt) 
    // {
    //     printf("Enter your master password: ");
    //     if (!fgets(entered_pass, sizeof(entered_pass), stdin))
    //         return 0;
    //     strip_newline(entered_pass);

    //     if (!strcmp(stored_pass, entered_pass)) 
    //     {
    //         printf("\nMaster password verified!\n");
    //         flag = 1;
    //         break;
    //     } else 
    //     {
    //         printf("Incorrect password. %d attempts left.\n", 2 - attempt);
    //     }
    // }

    // if (flag == 0) {
    //     printf("Failed to verify master password. Exiting.\n");
    //     return 0;
    // }

    int verifying_master_pass = verify_master_pass(META_FILE,3); // verifies the master pass from meta.dat

    if(verifying_master_pass == 0)
        printf("\nFailed to verify master password. Exiting.\n");
    else if(verifying_master_pass == -1) 
        printf("\nError in META File\n");
    else
    {       

        // Initialize manager
        struct PasswordManager manager;
        initialize(&manager);

        // Menu loop
        int opt;
        do{
            printf("\n-----Password Manager Menu-----\n");
        
            printf("1. Add Credentials\n");
            printf("2. List Credentials\n");
            printf("3. Search Credential\n");
            printf("4. Delete Credential\n");
            printf("5. Save Credentials & Exit\n");
            printf("6. Reset master PassWord\n");
            printf("---------------------------------\n");
            printf("Enter your choice (1-4): ");
            scanf("%d", &opt);
            getchar(); // consume leftover newline

            switch (opt) {
                case 1:
                    add_credentials(&manager);
                    break;
                case 2:
                    list_credentials(&manager);
                    break;
                case 3:
                    search_credentials(&manager); 
                    break;
                case 5:
                    saveCredentialsToFile(&manager);
                    printf("\nCredentials saved to file.\n");
                    break;
                case 4:
                    delete_credential(&manager);
                    printf("\nCredential Deleted!\n");
                    break;  
                case 6:
                    if (reset_master_pass(META_FILE, 3) == 1) 
                    {
                        printf("Master password reset OK.\n");
                    } 
                    else 
                    {
                        printf("Master password NOT changed or cancelled.\n");
                    }
                    break;;      
                default:
                    printf("Invalid option.\n");
            }
        } while (opt != 6);
    }

    return 0;

}







/*

🔹 Step 1: Original Password
Example: "1234"

🔹 Step 2: XOR each character with the key:
| Character | ASCII | Binary       | XOR with 75 | Result (Decimal) | Result (Char) |
|------------|--------|--------------|--------------|------------------|----------------|
| '1'        | 49     | 00110001     | 01001011     | 122              | 'z'            |
| '2'        | 50     | 00110010     | 01001011     | 121              | 'y'            |
| '3'        | 51     | 00110011     | 01001011     | 120              | 'x'            |
| '4'        | 52     | 00110100     | 01001011     | 127              | (DEL char)     |

So after XOR:
Encrypted (binary form): z y x (DEL)

The last character is unreadable — that’s why we need hex encoding next.

🔹 Step 3: Convert XOR result to Hex
Each byte → two hex digits

| Char | ASCII (Decimal) | Hex |
|------|------------------|------|
| z    | 122              | 7A   |
| y    | 121              | 79   |
| x    | 120              | 78   |
| DEL  | 127              | 7F   |

So after encoding:
Final Encrypted (hex) = 7A79787F

✅ This is what gets saved to your credential.csv file.

🔹 Step 4: Decryption (reverse process)
When you read from the file:
You have "7A79787F".
Convert each two hex chars → one byte → get [122,121,120,127].
XOR each again with 'K' (75):
122 ^ 75 = 49 → '1'
121 ^ 75 = 50 → '2'
120 ^ 75 = 51 → '3'
127 ^ 75 = 52 → '4'

✅ You get back "1234" exactly!

🔹 In short:
| Stage               | Example      |
|---------------------|--------------|
| Original Password   | 1234         |
| After XOR           | z y x (DEL)  |
| After Hex Encode    | 7A79787F     |
| Stored in File      | 7A79787F     |
| After Decode + XOR  | 1234 ✅       |
*/
