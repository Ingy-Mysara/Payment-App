#include "server.h"
#include "../LinearSearch.h"


EN_transState_t transState;
EN_serverError_t serverError;


ST_accountsDB_t accountsDB[10] = {
	//balance status acc number
	{(float)1000, RUNNING, "12345678912345672"},
	{(float)200, BLOCKED,"12345678912345671"},
	{(float)100000, RUNNING, "12345678912345671"},
	{(float)1400, RUNNING, "12345678912345671"},
	{(float)928364, RUNNING, "12345678912345671"},
	{(float)1110, RUNNING, "12345678912345671"},
	{(float)1092348, RUNNING, "12345678912345671"},
	{(float)120, RUNNING, "12345678912345671"},
	{(float)1120938, RUNNING, "12345678912345671"},
	{(float)200000, RUNNING, "12345678912345671"}
};


ST_accountsDB_t* ptrToTargetAcc;
EN_transState_t receiveTransactionData(ST_transaction_t* transData)
{
	//ST_accountsDB_t ptrToTargetAcc= accountsDB[0] ;


	transState = isValidAccount(transData->cardHolderData, ptrToTargetAcc); // ptrToTargetAcc updated
	if (transState == SERVER_OK) {


		transState = isAmountAvailable(&transData->terminalData, ptrToTargetAcc);
		if (transState != SERVER_OK) {// check if amount available
			// amount not available
			transState = DECLINED_INSUFFICIENT_FUND;
			printf("DECLINED_INSUFFICIENT_FUND \n");
			return transState;
		}


		transState = isBlockedAccount(ptrToTargetAcc);
		if (transState == BLOCKED_ACCOUNT) {// check account state
			transState = DECLINED_STOLEN_CARD;
			printf("DECLINED_STOLEN_CARD \n");
			return transState;
		}


		transState = saveTransaction(transData);
		if (transState = SAVING_FAILED) {
			transState = INTERNAL_SERVER_ERROR;
			printf("INTERNAL_SERVER_ERROR \n");
			return transState;
		}
		else {
			transState = APPROVED;
			return transState;
		}

	}
	else { // account dosent Exist
		transState = FRAUD_CARD;
		printf("FRAUD_CARD \n");
		return transState;
	}


}

EN_serverError_t isValidAccount(ST_cardData_t cardData, ST_accountsDB_t* accountRefrence)
{
	//int arraySize = (sizeof(accountsDB) / sizeof(accountsDB[0]));
	uint8_t targetPAN[20];
	for (int i = 0; i < 10; i++) {
		targetPAN[i] = cardData.primaryAccountNumber;
	}
	int accountindex = linearSearch(accountsDB, 10, targetPAN);

	if (accountindex == -1) {
		serverError = ACCOUNT_NOT_FOUND;
		printf("ACCOUNT_NOT_FOUND \n");
	}
	else {
		accountRefrence = &accountsDB[accountindex];
		serverError = SERVER_OK;
		printf("SERVER_OK \n");
	}

	return serverError;
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence)
{
	if (accountRefrence->state == BLOCKED) {
		transState = BLOCKED_ACCOUNT;
		return transState;
	}
	else {
		transState = SERVER_OK;
		return transState;
	}
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
