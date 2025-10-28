#include "contact.h"
#include<stdio.h>
#include<stdio_ext.h>
#include <unistd.h>
/*
Name : Rushikesh Rajendra Gore
Date : 10-10-2025
Batch Id: 25021E - 22
Student_ID: 25022_008
language : C programming

Project Discription:

  Project Name: AddressBook Project
  =================================
  A simple and interactive C program to manage your personal or professional contacts.
 
  Project Overview:
    - This application allows users to create/store, search, edit, and delete contact information.
    - Each contact consists of a name, phone number, and email address.
    - All contacts are stored in memory and can be saved to a file for future use.
 
  Main Features:
    1. Create Contact: Add a new contact with validation for unique phone and email.
    2. Search Contact: Find contacts by name, phone, or email with flexible matching.
    3. Edit Contact: Update the name, phone, or email of an existing contact.
    4. Delete Contact: Remove a contact from the address book.
    5. List All Contacts: Display all saved contacts in a formatted table.
    6. Save Contacts & Exit: Write all contacts to a file for accessing it later and Quit the program safely.
 
  File Structure:
    - main.c: Contains the main menu and program loop.
    - contact.c/h: Core logic for contact management --> Create, Edit, Delete, Validate.
    - file.c/h: Handles saving and loading contacts from a file.
 
  Extensibility:
    - The code is modular and can be extended to support more fields.
    - Additional features like import/export, group contacts, or favourite contacts group can be added.
  
*/
int main() 
{
    int choice;

    struct AddressBook addressBook;
    
	initialize(&addressBook); // Initialize the address book

    do 
    {
	printf("\n--- Address Book Menu ---\n");
	printf("1. Create contact\n");
	printf("2. Search contact\n");
	printf("3. Edit contact\n");
	printf("4. Delete contact\n");
	printf("5. List all contacts\n");
	printf("6. Save and Exit\n");
	printf("-------------------------\n");

	printf("\nEnter your choice (1-6): ");
	scanf("%d", &choice);
	__fpurge(stdin); // removes buffer
	switch (choice) 
	{
	    case 1:
			createContact(&addressBook);
			break;
	    case 2:
			searchContact(&addressBook);
			break;
	    case 3:
			editContact(&addressBook);
			break;
	    case 4:
			deleteContact(&addressBook);
			break;
	    case 5: 
			listContacts(&addressBook);
			break;
	    case 6:
			printf("\nSaving");
    		fflush(stdout);
    		for(int i=0;i<3;i++)
    		{
        		printf(".");
        		fflush(stdout);
        		sleep(1);
    		}
    		printf("\n");
    		printf("\nContacts Saved ->Exited!!\n");
			saveContactsToFile(&addressBook);
			break;
	    default:
			printf("Invalid choice. Please try again.\n");
		}
    } while (choice != 6);

    return 0;
}
