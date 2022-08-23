#include "Card/card.h"
#include "Server/server.h"
#include <stdio.h>
#include <string.h>

int linearSearch(ST_accountsDB_t accountsDB[], char target[], int size) {
	for (int i = 0; i < size; i++) {
		// check whether the two strings are equal or not
		if (strcmp(accountsDB[i].primaryAccountNumber, target) == 0) {
			
			return i;
		}
			
	}
	return -1;
}