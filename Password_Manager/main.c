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
                default:
                    printf("Invalid option.\n");
            }
        } while (opt != 4);
    }

    return 0;

}
