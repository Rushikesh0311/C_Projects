#include <stdio.h>
#include "file.h"

void saveContactsToFile(struct AddressBook *addressBook) 
{
    FILE* fp = fopen("AddressBook.csv","w");

    int size = addressBook->contactCount;

    fprintf(fp,"%d\n",size); // contact count store in file

    for(int i=0;i<size;i++)
    {
        fprintf(fp,"%s,%s,%s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,
            addressBook->contacts[i].email); // saves the contacts in the with 6th opt 
    }

    fclose(fp);
}

void loadContactsFromFile(struct AddressBook *addressBook) 
{
    FILE* fp = fopen("AddressBook.csv","r+");
    
     
    fscanf(fp, "%d\n", &addressBook->contactCount); // contact count stored in file
    int size = addressBook->contactCount; 

    for(int i = 0; i < size; i++)
    {
        (fscanf(fp,"%[^,],%[^,],%[^\n]\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,
        addressBook->contacts[i].email));
        
    }

    fclose(fp);
}
