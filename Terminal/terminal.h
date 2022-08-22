#pragma once
#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdio.h>

#pragma warning(disable : 4996)
typedef char uint8_t;
typedef unsigned int uint32_t;

typedef struct ST_terminalData_t
{
    float transAmount; //Transfer amount
    float maxTransAmount; //Maximum transfer amount
    uint8_t transactionDate[10]; //10 characters for the date
}ST_terminalData_t;

typedef enum En_terminalError_t
{
    TERMINAL_OK, WRONG_DATE, EXPIRED_CARD, INVALID_CARD, INVALID_AMOUNT, EXCEED_MAX_AMOUNT, INVALID_MAX_AMOUNT
}En_terminalError_t;

En_terminalError_t getTransactionDate(ST_terminalData_t* termData);
En_terminalError_t isCardExpired( cardData, termData);
//ST_cardData_t* cardData;
En_terminalError_t isValidCardPAN(cardData);
En_terminalError_t getTransactionAmount(ST_terminalData_t* termData);
En_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData);
En_terminalError_t setMaxAmount(ST_terminalData_t* termData);

#endif // TERMINAL_H