#include "server.h"
#include "../LinearSearch.h"


EN_transState_t transState;
EN_transState_t receiveTransactionData(ST_transaction_t* transData)
{
	// should validate the data 
	
	
	uint8_t targetAccountNumber = transData->cardHolderData.primaryAccountNumber;
	int arraySize = (sizeof(accountsDB) / sizeof(accountsDB[0]));
	
	
	
	int accountIndex = linearSearch(accountsDB, arraySize, targetAccountNumber);
	if (accountIndex == -1) {
		transState = FRAUD_CARD;
		printf("card not found \n");
		return transState;
	}
	// at this point i have account endex
	int neededAmount = transData->terminalData.transAmount;

	ST_accountsDB_t targetAccount = accountsDB[accountIndex];
	float availableAmount = targetAccount.balance;

	if (availableAmount < neededAmount) {
		printf("avaliable amount is less than needed amount \n");
		transState = DECLINED_INSUFFICIENT_FUND;
		return transState;
	}
	
	if (targetAccount.state == BLOCKED) {//// check Blocked or 1
		transState = DECLINED_STOLEN_CARD;
		printf("911 is on their way \n");
		return transState;
	}

	//missing if condidion depending on save transaction function

	transState = APPROVED;
	return transState;
}

EN_serverError_t isValidAccount(ST_cardData_t cardData, ST_accountsDB_t accountRefrence)
{
	return;
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence)
{
	return;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence)
{
	//check if transaction amount is available or not
	if (termData->transAmount > accountRefrence->balance) {
		return LOW_BALANCE;
	}
	return SERVER_OK;
}

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	//SERVER_OK, SAVING_FAILED, TRANSACTION_NOT_FOUND, ACCOUNT_NOT_FOUND, LOW_BALANCE, BLOCKED_ACCOUNT
	static int seqNum = -1;
	seqNum++;
	transactionsDB[seqNum].cardHolderData = transData->cardHolderData;
	transactionsDB[seqNum].terminalData = transData->terminalData;
	transactionsDB[seqNum].transactionSequenceNumber = seqNum;
	transactionsDB[seqNum].transState = receiveTransactionData(transData);
	if (getTransaction(seqNum, transData) != SERVER_OK) {
		return SAVING_FAILED;
	}
	return SERVER_OK;
}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData)
{
	int seqNum = transactionSequenceNumber;
	if (transactionsDB[seqNum].cardHolderData.primaryAccountNumber != 0) {
		transData->cardHolderData = transactionsDB[seqNum].cardHolderData;
		transData->terminalData = transactionsDB[seqNum].terminalData;
		transData->transState = transactionsDB[seqNum].transState;
		transData->transactionSequenceNumber = transactionsDB[seqNum].transactionSequenceNumber;
		return SERVER_OK;
	}
	return TRANSACTION_NOT_FOUND;
}
