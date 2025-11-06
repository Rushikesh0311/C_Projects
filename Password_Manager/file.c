#include <stdio.h>
#include "comman.h"

// // Save credentials to file
// void saveCredentialsToFile(struct PasswordManager *manager)
// {
//     FILE *fp = fopen("credential.csv", "w");
//     if(!fp) return;

//     int size = manager->credentialCount;

//     fprintf(fp, "%d\n", manager->credentialCount);
//     for(int i=0;i<size;i++)
//     {
//         fprintf(fp,"%s,%s,%s\n",
//             manager->credentials[i].site,
//             manager->credentials[i].username,
//             manager->credentials[i].password);
//     }

//     fclose(fp);
// }

// // Load credentials from file
// void loadCredentialsFromFile(struct PasswordManager *manager)
// {
//     FILE *fp = fopen("credential.csv", "r");
//     if(!fp) return; // No file yet

//     int count;
//     if(fscanf(fp, "%d\n", &count) != 1) {
//         fclose(fp);
//         return;
//     }

//     manager->credentialCount = count;

//     for(int i=0;i<count;i++)
//         fscanf(fp, "%[^,],%[^,],%[^\n]\n",
//             manager->credentials[i].site,
//             manager->credentials[i].username,
//             manager->credentials[i].password);

//     fclose(fp);
// }

/* ------------------ file I/O ------------------ */

/* save credentials to file (count then CSV lines) */
void saveCredentialsToFile(struct PasswordManager *m)
{
    if (!m) 
        return;
        
    FILE *f = fopen("credential.csv", "w");
    if (!f) 
    { 
        perror("open credential.csv"); 
        return; 
    }

    fprintf(f, "%d\n", m->credentialCount);
    for (int i = 0; i < m->credentialCount; ++i) {
        fprintf(f, "%s,%s,%s\n",
                m->credentials[i].site,
                m->credentials[i].username,
                m->credentials[i].password); // password stored as hex string
    }
    fclose(f);
}

/* load credentials from file (reads hex into password[] field) */
void loadCredentialsFromFile(struct PasswordManager *m)
{
    if (!m) return;
    FILE *f = fopen("credential.csv", "r");
    if (!f) 
    { 
        m->credentialCount = 0; 
        return; 
    }

    int cnt = 0;
    if (fscanf(f, "%d\n", &cnt) != 1) 
    { 
        m->credentialCount = 0; 
        fclose(f); 
        return; 
    }
    if (cnt < 0) cnt = 0;
    if (cnt > 100) cnt = 100; // safety cap
    m->credentialCount = cnt;

    for (int i = 0; i < cnt; ++i) 
    {
        if (fscanf(f, "%[^,],%[^,],%[^\n]\n",
                   m->credentials[i].site,
                   m->credentials[i].username,
                   m->credentials[i].password) != 3) 
        {
            m->credentialCount = i;
            break;
        }
    }
    fclose(f);
}