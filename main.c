#define _CRT_SECURE_NO_WARNINGS
#include "Card/card.h"
#include "Server/server.h"
#include "Terminal/terminal.h"
#include "application.h"
#include <stdio.h>
#include<string.h>







void appStart() {
    ST_cardData_t cardData;
    ST_cardData_t* ptrCardData= &cardData;
    getCardHolderName(ptrCardData);
    getCardExpiryDate(ptrCardData);
    getCardPAN(ptrCardData);

    ST_terminalData_t termData;
    ST_terminalData_t* ptrTermData = &termData;

    setMaxAmount(ptrTermData);
    getTransactionDate(ptrTermData);
    if (isCardExpired(ptrCardData, ptrTermData) == EXPIRED_CARD) {
        printf("EXPIRED_CARD \n");
        exit();
    }
    En_terminalError_t result = TERMINAL_OK;
    result = getTransactionAmount(ptrTermData);
    /*while (result != TERMINAL_OK) {
        result = getTransactionAmount(ptrTermData);

    }*/
    if (isBelowMaxAmount(ptrTermData) == EXCEED_MAX_AMOUNT) {
        printf("amount exceds the limits \n ");
        exit();
    }
    if (receiveTransactionData(ptrTermData) != SERVER_OK) {
        printf("invalid account \n ");
        exit();
    }

   

      
    
}



int main() {
    
    appStart();
          
}