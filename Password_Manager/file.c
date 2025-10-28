#include <stdio.h>
#include "comman.h"

// Save credentials to file
void saveCredentialsToFile(struct PasswordManager *manager)
{
    FILE *fp = fopen("credential.csv", "w");
    if(!fp) return;

    int size = manager->credentialCount;

    fprintf(fp, "%d\n", manager->credentialCount);
    for(int i=0;i<size;i++)
    {
        fprintf(fp,"%s,%s,%s\n",
            manager->credentials[i].site,
            manager->credentials[i].username,
            manager->credentials[i].password);
    }

    fclose(fp);
}

// Load credentials from file
void loadCredentialsFromFile(struct PasswordManager *manager)
{
    FILE *fp = fopen("credential.csv", "r");
    if(!fp) return; // No file yet

    int count;
    if(fscanf(fp, "%d\n", &count) != 1) {
        fclose(fp);
        return;
    }

    manager->credentialCount = count;

    for(int i=0;i<count;i++)
        fscanf(fp, "%[^,],%[^,],%[^\n]\n",
            manager->credentials[i].site,
            manager->credentials[i].username,
            manager->credentials[i].password);

    fclose(fp);
}