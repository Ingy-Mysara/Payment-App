#include "terminal.h"
#include"..\Card\card.h"
#include <stdio.h>
#include <stdlib.h>

En_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	printf("Enter Date: \n");
	for (int i = 0; i < 10; i++)
	{
		scanf("%c", &termData->transactionDate[i]);
	}
	if (termData->transactionDate[2] != '/' || termData->transactionDate[5] != '/'
		|| termData->transactionDate == NULL || strlen(termData->transactionDate) < 10)
		return WRONG_DATE;
	else return TERMINAL_OK;
}
//En_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
//{
//	// inCard    mm/yy
//	// inTrans   dd/mm/yyyy
//	//           0123456789
//	if (cardData.cardExpirationDate[3] < termData.transactionDate[8])
//	{
//		return EXPIRED_CARD;
//	}
//	else if (cardData.cardExpirationDate[3] == termData.transactionDate[8])
//	{
//		if (cardData.cardExpirationDate[4] > termData.transactionDate[9])
//			return TERMINAL_OK;
//		else
//		{
//			if (cardData.cardExpirationDate[1] < termData.transactionDate[4])
//				return EXPIRED_CARD;
//			else if (cardData.cardExpirationDate[1] > termData.transactionDate[4])
//				return TERMINAL_OK;
//			else {
//				if (cardData.cardExpirationDate[2] > termData.transactionDate[5])
//					return TERMINAL_OK;
//				else
//					return EXPIRED_CARD;
//			}
//		}
//	}
//}

En_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
	// inCard    mm/yy
	// inTrans   dd/mm/yyyy
	//           0123456789
	

	if ((int)(cardData.cardExpirationDate[3]) < (int)(termData.transactionDate[8]))
	{
		printf("expired \n");
		return EXPIRED_CARD;
	}
	if ((int)(cardData.cardExpirationDate[3]) == (int)(termData.transactionDate[8])) {
		if ((int)cardData.cardExpirationDate[4] < (int)(termData.transactionDate[9])) {
			printf("expired \n");
			return EXPIRED_CARD;
		}
		else if ((int)cardData.cardExpirationDate[4] > (int)termData.transactionDate[9]) {
			return TERMINAL_OK;
		}
		else {// same year now check for month 
			int CardmonthInInt = (int)(cardData.cardExpirationDate[0]) + (int)(cardData.cardExpirationDate[1]);
			int tranmonthInInt = (int)(termData.transactionDate[3]) + (int)(termData.transactionDate[4]);
			if (CardmonthInInt <= tranmonthInInt) {
				printf("expired \n");
				return EXPIRED_CARD;
			}
			else {
				return TERMINAL_OK;
			}

		}
	}
	
}


En_terminalError_t isValidCardPAN(ST_cardData_t* cardData)
{
	if (getCardPAN(cardData) == WRONG_PAN)
		return INVALID_CARD;
	else
		return TERMINAL_OK;
}
En_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	float amount=0;
	printf("Enter Amount:\n");
	scanf("%f", &amount);
	termData->transAmount = amount;
	printf("Deducted Amount will be: %f EGP\n", termData->transAmount);
	if (termData->transAmount <= 0)
		return INVALID_AMOUNT;
	else
		return TERMINAL_OK;
}
En_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	if (termData->transAmount > termData->maxTransAmount)
		return EXCEED_MAX_AMOUNT;
	else
		return TERMINAL_OK;
}
En_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
	termData->maxTransAmount = 5000.0;
	if (termData->maxTransAmount <= 0)
		return INVALID_MAX_AMOUNT;
	else
		return TERMINAL_OK;
}