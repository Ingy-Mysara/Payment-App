#include "application.h"

void appStart() {
	
	ST_cardData_t cardData;
	ST_cardData_t* ptrCardData = &cardData;

	if (getCardHolderName(ptrCardData) != CARD_OK) {
		exit();
	}
	if (getCardExpiryDate(ptrCardData) != CARD_OK) {
		printf("wrong date enterd \n");
		exit();
	
	}
	if (getCardPAN(ptrCardData)  != CARD_OK) {
		exit();
	}

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
		printf("EXCEED_MAX_AMOUNT\n ");
		exit(0);
	}
	ST_transaction_t transData;
	//ST_transaction_t* ptrTransData = &transData;
	transData.cardHolderData = cardData;
	transData.terminalData = termData;
	
	
									   
	if (receiveTransactionData(&transData) != SERVER_OK) {
		printf("INVALID PROCESS \n ");
		exit(0);
	}

	
	//saveTransaction(ptrTransData);
	//printf("*****Successful Payment*****\n");
	////exit(1);
	//
	//for (int i = 0; i < 255; i++) {
	//	printf("%s     ", transactionsDB->cardHolderData.primaryAccountNumber);
	//	printf("%s     ", transactionsDB->cardHolderData.cardHolderName);
	//}
	
	
}


int main() {

	appStart();

}