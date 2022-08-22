#include "terminal.h"
#include"card.h"
#include <stdio.h>
#include <stdlib.h>

En_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
    printf("Enter Date: ");
    for(int i = 0; i < 10; i++)
    {
        scanf("%d",&termData->transactionDate[i]);
    }
    if(termData->transactionDate[2] != '/' || termData->transactionDate[5] != '/'
    || termData->transactionDate == NULL || strlen(termData->transactionDate) < 10)
    return WRONG_DATE;
    else TERMINAL_OK;
}
En_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
    if(cardData.cardExpirationDate[4] < termData.transactionDate[9])
    {
        EXPIRED_CARD;
    }
    else if(cardData.cardExpirationDate[4] == termData.transactionDate[9])
    {
        if(cardData.cardExpirationDate[5] > termData.transactionDate[10])
        TERMINAL_OK;
        else
        {
            if(cardData.cardExpirationDate[1] < termData.transactionDate[4])
            EXPIRED_CARD;
            else if (cardData.cardExpirationDate[1] > termData.transactionDate[4])
            TERMINAL_OK;
            else{
                if (cardData.cardExpirationDate[2] > termData.transactionDate[5])
                TERMINAL_OK;
                else
                EXPIRED_CARD;
            }
        }
    }
}
En_terminalError_t isValidCardPAN(ST_cardData_t* cardData)
{
    if(getCardPAN(cardData) == WRONG_PAN)
    INVALID_CARD;
    else
    TERMINAL_OK;
}
En_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
    if(termData->transAmount <= 0)
    INVALID_AMOUNT;
    else
    TERMINAL_OK;
}
En_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
    if(termData->transAmount > termData->maxTransAmount)
    EXCEED_MAX_AMOUNT;
    else
    TERMINAL_OK;
}
En_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
    if(termData->maxTransAmount <= 0)
    INVALID_MAX_AMOUNT;
    else
    TERMINAL_OK;
}
