#include "application.h"

void appStart() {
	ST_cardData_t cardData;
	ST_cardData_t* ptrCardData = &cardData;
	getCardHolderName(ptrCardData);
	getCardExpiryDate(ptrCardData);
	getCardPAN(ptrCardData);

	ST_terminalData_t termData;
	ST_terminalData_t* ptrTermData = &termData;

	setMaxAmount(ptrTermData);
	getTransactionDate(ptrTermData);
	if (isCardExpired(*ptrCardData, *ptrTermData) == EXPIRED_CARD) {
		printf("EXPIRED_CARD \n");
		exit();
	}
	En_terminalError_t result = TERMINAL_OK;
	result = getTransactionAmount(ptrTermData);
	/*while (result != TERMINAL_OK) {
		result = getTransactionAmount(ptrTermData);

	}*/
	if (isBelowMaxAmount(ptrTermData) == EXCEED_MAX_AMOUNT) {
		printf("amount exceeds the limits \n ");
		exit(0);
	}
	if (receiveTransactionData(ptrTermData) != SERVER_OK) {
		printf("invalid account \n ");
		exit(0);
	}
	else {
		printf("*****Successful Payment*****\n");
		exit(1);
	}

}


int main() {

	appStart();

}