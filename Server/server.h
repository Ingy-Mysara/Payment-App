#pragma once
#ifndef SERVER_H
#define SERVER_H
#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include<stdio.h>
#include<string.h>

typedef enum EN_transState_t
{
	APPROVED, DECLINED_INSUFFICIENT_FUND, DECLINED_STOLEN_CARD, FRAUD_CARD, INTERNAL_SERVER_ERROR
}EN_transState_t;

typedef struct ST_transaction_t
{
	ST_cardData_t cardHolderData;
	ST_terminalData_t terminalData;
	EN_transState_t transState;
	uint32_t transactionSequenceNumber;
}ST_transaction_t;

typedef enum EN_serverError_t
{
	SERVER_OK, SAVING_FAILED, TRANSACTION_NOT_FOUND, ACCOUNT_NOT_FOUND, LOW_BALANCE, BLOCKED_ACCOUNT
}EN_serverError_t;

typedef enum EN_accountState_t
{
	RUNNING, BLOCKED
}EN_accountState_t;

typedef struct ST_accountsDB_t
{
	float balance;
	EN_accountState_t state;
	uint8_t primaryAccountNumber[20];
}ST_accountsDB_t;

ST_transaction_t transactionsDB[255] = { 0 };
ST_accountsDB_t accountsDB[10] = {
	//balance status acc number
	{1000, RUNNING, 378282246310005},
	{200, BLOCKED, 371449635398431},
	{100000, RUNNING, 5610591081018250},
	{1400, RUNNING, 6011111111111117},
	{928364, RUNNING, 6011000990139424},
	{1110, RUNNING, 3530111333300000},
	{1092348, RUNNING, 3566002020360505},
	{120, RUNNING, 5555555555554444},
	{1120938, RUNNING, 5105105105105100},
	{200000, RUNNING, 4012888888881881}
};

EN_transState_t receiveTransactionData(ST_transaction_t* transData);
EN_serverError_t isValidAccount(ST_cardData_t cardData, ST_accountsDB_t accountRefrence);
EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence);
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence);
EN_serverError_t saveTransaction(ST_transaction_t* transData);
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData);

#endif