#include "Card/card.h"
#include "Server/server.h"

int linearSearch(ST_accountsDB_t accountsDB[], int size, uint8_t target) {
    for (int i = 0; i < size; i++) {
        // check whether the two strings are equal or not
        if (strcmp((char*)accountsDB[i].primaryAccountNumber, (char*)target) == 0)
            // if found return index of the customer
            return i;
    }
    return -1;
}