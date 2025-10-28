#include "comman.h"
#include <stdio.h>
#include <string.h>



// XOR cipher for credentials
void XOR_cipher(char* text,char key) // encryption decryption 
{
    for(int i=0;text[i] != '\0';i++)
        text[i] ^= key;
}

// Strip newline from strings
void strip_newline(char *s) {
    if (!s) return;
    size_t n = strlen(s);
    while (n > 0 && (s[n-1] == '\n' || s[n-1] == '\r')) {
        s[n-1] = '\0';
        n--;
    }
}

// Initialize password manager
void initialize(struct PasswordManager *manager) {
    manager->credentialCount = 0;
    loadCredentialsFromFile(manager);
}

// Create meta file (set master password)
void create_metaFile(const char* meta_file)
{
    char master[50], confirm[50];
    printf("Set master Password: ");
    scanf(" %[^\n]", master);
    printf("Confirm your password: ");
    scanf(" %[^\n]", confirm);

    if(!strcmp(master,confirm)) {
        FILE *meta = fopen(META_FILE,"w");
        fprintf(meta,"%s",master);
        fclose(meta);
        printf("Master password saved successfully!\n");
    } else {
        printf("Passwords did not match. Try again.\n");
    }
}

int verify_master_pass(const char* meta_file,int max_attempts)
{
    char stored_pass[50];
    char entered_pass[50];

    // Open meta file or create if not exists
    FILE *fp = fopen(META_FILE, "r");
    if (fp == NULL) {
        printf("No master file found. Please set master password.\n");
        create_metaFile(META_FILE);
        fp = fopen(META_FILE, "r");
        if (fp == NULL) {
            perror("Failed to open meta file after creation");
            return 1;
        }

         //try to reopen 
        fp = fopen(meta_file, "r");
        if (fp == NULL) {
            perror("Failed to open meta file after creation");
            return -1;
        }
    }

    // Read stored master password
    if (!fgets(stored_pass, sizeof(stored_pass), fp)) {
        fclose(fp);
        printf("Meta file is empty!\n");
        return 1;
    }
    fclose(fp);

     // Remove trailing newline 
    strip_newline(stored_pass);

    // Prompt user up to max_attempts
    for (int attempt = 0; attempt < max_attempts; ++attempt) 
    {
        printf("Enter your master password: ");
        if(!fgets(entered_pass, sizeof(entered_pass), stdin)) 
        {
            //input error (EOF)
            return 0;
        }
        strip_newline(entered_pass);

        if(!strcmp(stored_pass, entered_pass)) 
        {
            // success
            printf("\nMaster password verified!\n");
            return 1;
        } 
        else{
            int left = max_attempts - 1 - attempt;
            printf("Incorrect password. %d attempts left.\n", left);
        }
    }
    return 0;
}

// Add a credential
void add_credentials(struct PasswordManager *manager)
{
    int s = manager->credentialCount;
    

    printf("Enter the Site name: ");
    scanf(" %[^\n]", manager->credentials[s].site);

    printf("Enter the Username: ");
    scanf(" %[^\n]", manager->credentials[s].username);

    printf("Enter the Password: ");
    scanf(" %[^\n]", manager->credentials[s].password);

    XOR_cipher(manager->credentials[s].password, 'K'); // Encrypt

    manager->credentialCount++;
    printf("\nCredential added successfully!\n");
}

// List credentials
void list_credentials(struct PasswordManager *manager)
{

    char temp[128];
    int size = manager->credentialCount;
    printf("\n---------------------------------------------------------------\n");
    printf("%-4s | %-12s | %-15s | %-15s\n", "No", "Site", "Username", "Password");
    printf("---------------------------------------------------------------\n");
   
    
    for(int i=0; i<size; i++)
    {
        strcpy(temp, manager->credentials[i].password);
        XOR_cipher(temp, 'K'); // decrypt

        printf("%-4d | %-12s | %-15s | %-15s\n",
               i+1,
               manager->credentials[i].site,
               manager->credentials[i].username,
               temp);
    }

    printf("---------------------------------------------------------------\n");
}

void search_credentials(struct PasswordManager *manager)
{
    int size = manager->credentialCount;

    char site[50];
    printf("Enter Site name to search: ");
    scanf("%[^\n]",site);

    printf("\n---------------------------------------------------------------\n");
    printf("%-4s | %-12s | %-15s | %-15s\n", "Sr.No", "Site", "Username", "Password");
    printf("---------------------------------------------------------------\n");
   
    for(int i=0;i<size;i++)
    {
        if(strstr(manager->credentials[i].site,site))
        {
            printf("%-4d | %-12s | %-15s | %-15s\n",
               i+1,
               manager->credentials[i].site,
               manager->credentials[i].username,
               manager->credentials[i].password);
        }

    }
    printf("---------------------------------------------------------------\n");
}

void delete_credential(struct PasswordManager* manager)
{
    int size = manager->credentialCount;
    search_credentials(manager); 
    int index;
    printf("Enter the index of credential you want to delete: ");
    scanf("%d",&index);
    index--;

    for(int i=index;i<size-1;i++)
    {
        strcpy(manager->credentials[i].site,manager->credentials[i+1].site);
        strcpy(manager->credentials[i].username,manager->credentials[i+1].username);
        strcpy(manager->credentials[i].password,manager->credentials[i+1].password);
    }

    manager->credentialCount--;
    
}


