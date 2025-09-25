#ifndef CONTACT_H
#define CONTACT_H
#include<stdio.h>

struct Contact
{
    char name[50];
    char phone[20];
    char email[50];
};

struct AddressBook
{
    struct Contact contacts[100];
    int contactCount;
};

void createContact(struct AddressBook *addressBook); 
void searchContact(struct AddressBook *addressBook); 
void editContact(struct AddressBook *addressBook);
void deleteContact(struct AddressBook *addressBook);
void listContacts(struct AddressBook *addressBook); 
void initialize(struct AddressBook *addressBook);
void saveContactsToFile(struct AddressBook *AddressBook);

// phone validations
int phnNo_validation(char* phone);
int duplicate_phone(struct AddressBook* addressbook,char* mob);

// email validations
int email_validation(char* mail);
int duplicate_email(struct AddressBook* addressbook,char* mail);

// search opration functions
void search_with_Name(struct AddressBook*  addressbook);
void search_with_phone(struct AddressBook* addressbook);
void search_with_email(struct AddressBook*  addressbook);

// Edit contact functions
void Edit_Name(struct AddressBook* addressBook);
void edit_phone(struct AddressBook* addressBook);
void edit_email(struct AddressBook* addressBook);
#endif
