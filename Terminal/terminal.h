#pragma once
#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdio.h>

typedef char uint8_t;
typedef unsigned int uint32_t;

typedef struct ST_terminalData_t
{
    float transAmount;
    float maxTransAmount;
    uint8_t transactionDate[11];
}ST_terminalData_t;

typedef enum En_terminalError_t
{
    TERMINAL_OK, WRONG_DATE, EXPIRED_CARD, INVALID_CARD, INVALID_DATE, EXCEED_MAX_AMOUNT, INVALID_MAX_AMOUNT
}En_terminalError_t;

En_terminalError_t getTransactionDate(ST_terminalData_t* termData);
En_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData);
En_terminalError_t isValidCardPAN(ST_cardData_t* cardData);
En_terminalError_t getTransactionAmount(ST_terminalData_t* termData);
En_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData);
En_terminalError_t setMaxAmount(ST_terminalData_t* termData);

#endif // TERMINAL_H
