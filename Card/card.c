#pragma warning(disable : 4996)
#include<stdio.h>
#include<string.h>
#include"card.h"
typedef char uint8_t;

EN_cardError_t getCardHolderName(ST_cardData_t* cardData) {
	EN_cardError_t Error;
	printf("enter userName of max 24 char and min 20 char \n");
	char tempName[25];
	gets(tempName);
	if ((strlen(tempName) < 25 && strlen(tempName) > 19) && tempName != '\0') {
		for (int index = 0; index < strlen(tempName); index++) {
			if (tempName[index] == '0' ||
				tempName[index] == '1' ||
				tempName[index] == '2' ||
				tempName[index] == '3' ||
				tempName[index] == '4' ||
				tempName[index] == '5' ||
				tempName[index] == '6' ||
				tempName[index] == '7' ||
				tempName[index] == '8' ||
				tempName[index] == '9')
			{
				printf("text including number \n");
				Error = WRONG_NAME;
				return Error;
			}
		}
		Error = CARD_OK;

	}
	else {
		printf("WRONG_NAME\n");
		Error = WRONG_NAME;
		return Error;
	}

	strcpy(cardData->cardHolderName, tempName);
	printf("cardData %s \n", cardData->cardHolderName);
	return Error;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) {
	EN_cardError_t Error;

	printf("enter expiry month \n");
	char monthInstring[10];

	gets(monthInstring);
	printf("enter expiry year (of 2 digits) ex yy \n");
	char yearInstring[3];
	gets(yearInstring);

	int monthInInteger;
	int yearInInteger;
	sscanf(monthInstring, "%d", &monthInInteger);
	sscanf(yearInstring, "%d", &yearInInteger);

	if (monthInInteger > 0 && monthInInteger <= 12) {
		Error = CARD_OK;
	}
	else {
		Error = WRONG_EXP_DATE;
		return Error;
	}

	if (yearInInteger >= 22 && yearInInteger <= 30) {
		Error = CARD_OK;
	}
	else {
		Error = WRONG_EXP_DATE;
		return Error;
	}

	if (yearInInteger == 22 && monthInInteger <= 8) {
		Error = WRONG_EXP_DATE;
		return Error;
	}

	char tempExpiryDate[6] = "";

	char tempSlach[] = "/";
	/*puts(monthInstring);
	puts(yearInstring);*/
	strcat(tempExpiryDate, monthInstring);
	strcat(tempExpiryDate, tempSlach);
	strcat(tempExpiryDate, yearInstring);
	puts(tempExpiryDate);
	for (int index = 0; index < strlen(cardData->cardExpirationDate); index++) {
		cardData->cardExpirationDate[index] = tempExpiryDate[index];
	}

	return Error;
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData) {
	EN_cardError_t Error;
	printf("enter your PAN max 19 number and min 16 number \n");
	char tempPAN[50];
	gets(tempPAN);
	if (strlen(tempPAN) >= 16 && strlen(tempPAN) <= 19 && tempPAN != NULL) {
		Error = CARD_OK;
		printf("CARD_OK\n");
	}
	else {
		Error = WRONG_PAN;
		printf("WRONG_PAN\n");
		return Error;
	}
	for (int index = 0; index < strlen(tempPAN); index++) {
		if ((tempPAN[index] >= 'a' && tempPAN[index] <= 'z') ||
			(tempPAN[index] >= 'A' && tempPAN[index] <= 'Z')) {
			printf("WRONG_PAN\n");
			Error = WRONG_PAN;
			return Error;
		}
	}

	for (int index = 0; index < strlen(cardData->primaryAccountNumber); index++) {
		cardData->primaryAccountNumber[index] = tempPAN[index];
	}
	return Error;

}

