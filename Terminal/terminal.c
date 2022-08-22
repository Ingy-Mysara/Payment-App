#include "terminal.h"
#include"..\Card\card.h"
#include <stdio.h>
#include <stdlib.h>

En_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
    printf("Enter Date: ");
    for (int i = 0; i < 10; i++)
    {
        scanf("%d", &termData->transactionDate[i]);
    }
    if (termData->transactionDate[2] != '/' || termData->transactionDate[5] != '/'
        || termData->transactionDate == NULL || strlen(termData->transactionDate) < 10)
        return WRONG_DATE;
    else return TERMINAL_OK;
}
En_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
    if (cardData.cardExpirationDate[4] < termData.transactionDate[9])
    {
        return EXPIRED_CARD;
    }
    else if (cardData.cardExpirationDate[4] == termData.transactionDate[9])
    {
        if (cardData.cardExpirationDate[5] > termData.transactionDate[10])
            return TERMINAL_OK;
        else
        {
            if (cardData.cardExpirationDate[1] < termData.transactionDate[4])
                return EXPIRED_CARD;
            else if (cardData.cardExpirationDate[1] > termData.transactionDate[4])
                return TERMINAL_OK;
            else {
                if (cardData.cardExpirationDate[2] > termData.transactionDate[5])
                    return TERMINAL_OK;
                else
                    return EXPIRED_CARD;
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
    printf("enter amount \n");
    scanf("%f",&termData->transAmount);
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