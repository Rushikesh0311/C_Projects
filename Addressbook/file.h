#ifndef FILE_H
#define FILE_H

#include "contact.h"

void saveContactsToFile(struct AddressBook *addressBook);
void loadContactsFromFile(struct AddressBook *addressBook);

#endif
