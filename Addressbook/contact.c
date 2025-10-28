#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "contact.h"
#include "file.h"
#include "populate.h"

int for_edit = 0; // as a flag for edit function 
//int foundContacts[100]; // stores the found indexes of contacts with same name
//int matched_contact = 0; // count of founded contacts

void listContacts(struct AddressBook *addressBook) // prints all contacts 
{
    int size = addressBook->contactCount;
    printf("\n--------------------------------------------------------------------------\n");

    printf("%-15s %-20s %-20s %-20s\n","Sr.No","Name","Phone","Email");

    printf("-----------------------------------------------------------------------------\n");
    for(int i=0;i<size;i++)
    {
       

        printf("\n%-13d %-18s %-18s %-18s\n",i+1,addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
       
       
       
        /*printf("Student - %d\n",i+1);
        printf("Name - %s\n",addressBook->contacts[i].name);
        printf("Phone - %s\n",addressBook->contacts[i].phone);
        printf("Email - %s\n",addressBook->contacts[i].email);
        */

        
    }
    printf("-----------------------------------------------------------------------------\n");
}

void initialize(struct AddressBook *addressBook) {
    addressBook->contactCount = 0;
    populateAddressBook(addressBook);

    // Load contacts from file during initialization 
    loadContactsFromFile(addressBook);
}

void saveAndExit(struct AddressBook *addressBook) {
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}


void createContact(struct AddressBook *addressBook)
{
    char mob[11];
    char mail[50];

    int s = addressBook->contactCount; // contact count initialization
    //printf("%d\n",s);

    int flag = 0;
    
    printf("\nEnter your name: ");
    scanf(" %[^\n]",addressBook->contacts[s].name);
    getchar();

    do{
        printf("Enter your Number: ");      // phone validation 
        scanf(" %[^\n]",mob);

        if(!phnNo_validation(mob))
        {
            printf("\n");
            printf("\nEnter correct number!!!\n");
            printf("\n");
        }else
        if(!duplicate_phone(addressBook,mob)) // duplicate No checks here
        {
            printf("\n");
            printf("The Number is Already present!!\n");
            printf("\n");
        }
        else{
            flag = 1;
            strcpy(addressBook->contacts[s].phone,mob);
        }


    }while(flag != 1);

    flag = 0;
    do{
        printf("Enter your Email: ");
        scanf(" %[^\n]",mail);

        if(!duplicate_email(addressBook,mail))
        {
            printf("\n");
            printf("Email Already present!!\n");
            printf("\n");
        }else
        if(!email_validation(mail))
        {
            printf("\n");
            printf("Enter correct mail ID!!\n");
            printf("\n");
        }
        else{
            flag = 1;
            strcpy(addressBook->contacts[s].email,mail);
        }
    }while(flag != 1);

    printf("\nContct Added successfully!!\n");

    addressBook->contactCount++; // contactcount incrementing here
}

void searchContact(struct AddressBook *addressbook) 
{
    int searchOpt;
    do{

        printf("1. Search By Name\n2. Search By Phone\n3. Search By Email\n4. Back To Menu\n(*if you're in Edit/Delete, after search go with 4th option)\n");
        
        

        printf("\nEnter option you want to Search with(1-4): ");

        scanf("%d",&searchOpt);
        printf("\n");

        switch(searchOpt)
        {
            case 1:
                search_with_Name(addressbook);
                break;
            case 2:
                search_with_phone(addressbook);
                break;
            case 3:
                search_with_email(addressbook);
                break;
            case 4:
                printf("\nBack To Menu..\n");
                break;    
            default:
                printf("\n");
                printf("Invalid Option !!\n"); 
                printf("\n");          
        }
    }while(searchOpt != 4);

}

void editContact(struct AddressBook *addressBook)
{
    int editopt; // option variable for switch case
    int index; // index or Serial No of specific contact we want to edit 
    searchContact(addressBook);
   
    
    // if(for_edit == 1){ // globallly initailize for_edit as a flag = 0 
    //     printf("\nFirst Search and then come to edit!!\n");
    //     return ;
    // }
        

    do{

        printf("\n----Edit options Menu----\n");
        printf("1. Edit Name\n2. Edit Phone\n3. Edit Email\n4. Back to Menu\n");

        printf("\nSelect Option to Edit(1-4): ");
        scanf("%d",&editopt);
       
        switch(editopt)
        {
            case 1:
                Edit_Name(addressBook);
                /*char new_name[50];
                printf("\nEnter New change: ");
                scanf(" %[^\n]",new_name);
                printf("\nEnsure Serial-No of contact(Sr.No): ");
                scanf("%d",&index);                                    // Index stars with 0 here 
                strcpy(addressBook->contacts[index-1].name,new_name);  // thats why index -1*/

                //printf("\nContact Modified Successfully!\n");
                break;
            case 2:
                edit_phone(addressBook);
                break;
            case 3:
                edit_email(addressBook);
                break;
            case 4:
                printf("\n");    
                printf("Back To Menu...\n");
                break;
            default:
                printf("\nInvalid choice!!\n");     
        }
    }while(editopt != 4);

}

void deleteContact(struct AddressBook *addressBook)
{
    int size = addressBook->contactCount;
    
    searchContact(addressBook); // search fun call here sreach -->>delete
    
    if(for_edit == 1) // global flag
    {
        printf("\nNo contact found to delete!!\n");
        return;
    }
    
    int serial_no;
    printf("\nEnsure the index(Sr/No.) of contact you want to delete: ");
    scanf("%d",&serial_no);
    serial_no--;
   

    //int idx = foundContacts[serial_no - 1]; //new index with same names starts with 1

    if(serial_no < 0 || serial_no > size) // checks the correct index that user entered
    {
        printf("\nInvalid Index!!\nSelct correct Index!!\n");
        return;
    }

    for(int i=serial_no;i<size-1;i++) // shifts the contacts after deleted specific contact
    {
        strcpy(addressBook->contacts[i].name,addressBook->contacts[i+1].name);
        strcpy(addressBook->contacts[i].phone,addressBook->contacts[i+1].phone);
        strcpy(addressBook->contacts[i].email,addressBook->contacts[i+1].email);
    }

    addressBook->contactCount--; // shifts all contact to left
    printf("\nProcessing");
    fflush(stdout);
    for(int i=0;i<3;i++)
    {
        printf(".");
        fflush(stdout);
        sleep(1);
    }
    printf("\n");
    printf("\nContact Deleted!!\n");
    
    


}

//----------------------------------- Phone - Email validation functions --------------------------------------------

int phnNo_validation(char* phone)
{
    int len = strlen(phone); // Phn Nos length

    if(len != 10)
        return 0;
    
    for(int i=0;i<len;i++)
    {
        if(phone[i] < 48 || phone[i] > 57) // checks here that No '0' -> '9'
        {
            return 0;
        }
    }
    return 1;  
}

int duplicate_phone(struct AddressBook* addressbook,char* mob)
{   // for number duplication
    int size = addressbook->contactCount;
    for(int i=0;i<size;i++)
    {
        if(!strcmp(addressbook->contacts[i].phone,mob)) // compares with every number
        {
            return 0;
        }
    }
    return 1;
}

int email_validation(char* mail)
{
    int len = strlen(mail); 

    if(!strstr(mail,".com")) // finds substring 
        return 0;
    else if(!strstr(mail,"@"))
        return 0;
    
    for(int i=0;i<len;i++)
    {
        if(isupper(mail[i]) && !(mail[i] >= 48 && mail[i] <= 57)) // checks is uppercase is there
        {
            return 0;
        }
    }

    char *ch = strchr(mail,'@'); // strchr finds first position of str
    char *ch2 = strchr(mail,'.');

    for(int i=0;i<len;i++)
    {                                                           // mail 0'th index 
        if(ch == mail || *(ch+1) == '\0' || *(ch + 1) == *ch2) // checks Before and After of @ at least onr character is present
        {                                                     // # 3rd condition here checks no directly dot ( . ) appears after @.
            return 0;
        }
        else if(ch2 != mail+len-4) // Ensures mail ends with the .com
        {
            return 0;
        }
        /*else if( mail[i] >= 0 && mail[i] == ' ')
        {
            i--;
        }*/
    }
    return 1;
}
int duplicate_email(struct AddressBook* addressbook,char* mail)
{
    int size = addressbook->contactCount;
    for(int i=0;i<size;i++)
    {
        if(!strcmp(addressbook->contacts[i].email,mail))
        {
            return 0;
        }
    }
    return 1;
}

// --------------------------------search oprations--------------------------------------

void search_with_Name(struct AddressBook* addressbook)
{
    int size = addressbook->contactCount;

    char Ename[20]; // extra name variable 

    printf("Enter Name to search: ");
    scanf(" %[^\n]",Ename);

    int flag = 0;
    //int matched_contacts = 0; // as a searched contact count for serial number with same names
    int i;
    printf("\nSearching");
    fflush(stdout);
    for(int i=0;i<3;i++)
    {
        printf(".");
        fflush(stdout);
        sleep(1);
    }
    printf("\n");

    printf("\n------------------------------------------------------------------------------\n");

        printf("%-15s %-20s %-20s %-20s\n","Sr.No","Name","Phone","Email");

        printf("-----------------------------------------------------------------------------\n");
    for(i=0;i<size;i++)
    {
        if(strstr(addressbook->contacts[i].name,Ename))
        {
            flag = 1;
            
            //foundContacts[matched_contacts] = i;
           // matched_contacts++;
            printf("\n%-13d %-18s %-18s %-18s\n",i+1,addressbook->contacts[i].name,addressbook->contacts[i].phone,addressbook->contacts[i].email);
            //break;
        }
    }
    if(flag==0){
        printf("\n");
        printf("Search result is not found!!\nPlease try Again!!!"); 
        for_edit = 1;
        printf("\n");
    }
    printf("-----------------------------------------------------------------------------\n");

}
void search_with_phone(struct AddressBook* addressbook)    
{
    int size = addressbook->contactCount;

    char phn[20]; // extra input for PhnNo to compare

    printf("\n");
    printf("Enter Number to search: ");
    scanf(" %[^\n]",phn);

    int flag = 0;
    int i;
    for(i=0;i<size;i++)
    {
        if(!strcmp(addressbook->contacts[i].phone,phn))
        {
            flag = 1;
            break;
        }
    }
    if(flag == 1)
    {
        printf("\nSearching");
        fflush(stdout);
        for(int i=0;i<3;i++)
        {
            printf(".");
            fflush(stdout);
            sleep(1);
        }
        printf("\n");

        printf("\n--------------------------------------------------------------------------\n");

        printf("%-15s %-20s %-20s %-20s\n","Sr.No","Name","Phone","Email");

        printf("-----------------------------------------------------------------------------\n");
        
       

        printf("\n%-13d %-18s %-18s %-18s\n",i+1,addressbook->contacts[i].name,addressbook->contacts[i].phone,addressbook->contacts[i].email);       
    }
    else{
        printf("\n");
        printf("Search result is not found!!\nPlease try Again!!!");
        for_edit = 1;
        printf("\n");
    }
    printf("-----------------------------------------------------------------------------\n");

}   
void search_with_email(struct AddressBook* addressbook)
{
    int size = addressbook->contactCount;
    printf("%d\n",size);
    char mail[50];


    printf("\n");
    printf("Enter Email to search: ");
    scanf(" %[^\n]",mail);

    int flag = 0;
    int i;

    for(i=0;i<size;i++)
    {
        if(!strcmp(addressbook->contacts[i].email,mail))
        {
            flag = 1;
            break;
        }
    }
    if(flag == 1)
    {
        printf("\nSearching");
        fflush(stdout);
        for(int i=0;i<3;i++)
        {
            printf(".");
            fflush(stdout);
            sleep(1);
        }
        printf("\n");
        printf("\n--------------------------------------------------------------------------\n");

        printf("%-15s %-20s %-20s %-20s\n","Sr.No","Name","Phone","Email");

        printf("-----------------------------------------------------------------------------\n");
        
       

        printf("\n%-13d %-18s %-18s %-18s\n",i+1,addressbook->contacts[i].name,addressbook->contacts[i].phone,addressbook->contacts[i].email);       
    }
    else{
        printf("\n");
        printf("Search result is not found!!\nPlease try Again!!!\n");
        for_edit = 1;
        printf("\n");
    }
    printf("-----------------------------------------------------------------------------\n");

}

//-----------------------------------------edit contact functions--------------------------------------------

void Edit_Name(struct AddressBook* addressBook)
{
    if(for_edit == 1)
    {
        printf("\nSearch result not found!!\nTry Again with search!!!");
        //editContact(addressBook);
        return;
    }
    else
    {
       // for(int i=0;i<matched_contact;i++)

        
        char new_name[50];
        printf("\nEnter New change: ");
        scanf(" %[^\n]",new_name);
        int serial_no;
        printf("\nEnsure Serial-No of contact(Sr.No): ");
        scanf("%d",&serial_no);

       // int idx = foundContacts[serial_no-1]; // serial numbers starts from 1  
       
        
        strcpy(addressBook->contacts[serial_no-1].name,new_name);  // Index stars with 0 here thats why index -1
     
        printf("\nContact Modified Successfully !!\n");
    }
}
void edit_phone(struct AddressBook* addressBook)
{
    if(for_edit == 1) // global flag 
    {
        printf("\nSearch result not found!!\nTry Again with search!!!");
        //editContact(addressBook);
        return;
    }
    int index;
    char mob1[20];
    int flag = 0;
     do
    {
        printf("\nEnter your Number: ");      // phone validation 
        scanf(" %[^\n]",mob1);

        if(!phnNo_validation(mob1))
        {
                       
            printf("\nEnter correct number!!!\n");
                        
        }else
            if(!duplicate_phone(addressBook,mob1))
            {
                 printf("\nThe Number is Already present!!\n");
            }
            else
            {
                printf("\nEnsure Serial-No of contact(Sr.No): ");
                scanf("%d",&index); // Index stars with 0 here 

                flag = 1;
                strcpy(addressBook->contacts[index-1].phone,mob1); // thats why assign index -1 .
                printf("\nContact Modified Successfully !!\n");
            }
    }while(flag != 1); 
}
void edit_email(struct AddressBook* addressBook)
{
    if(for_edit == 1) // as a flag 
    {
        printf("\nSearch result not found!!\nTry Again with search!!!");
        //editContact(addressBook);
        return;
    }
    int index;
    char mail[50];
    int flag1 = 0;
    do{
        printf("\nEnter your Email: ");
        scanf(" %[^\n]",mail);

        if(!duplicate_email(addressBook,mail))
        {
            printf("\n");
            printf("Email Already present!!\n");
            printf("\n");
        }else
        if(!email_validation(mail))
        {
            printf("\n");
            printf("Enter correct mail ID!!\n");
            printf("\n");
        }
        else{
            printf("\nEnsure Serial-No of Contact(Sr.No): ");
            scanf("%d",&index); // Index stars with 0 here 

            flag1 = 1;
            strcpy(addressBook->contacts[index-1].email,mail);
            printf("\nContact Modified Successfully !!\n");
        }
    }while(flag1 != 1);
}