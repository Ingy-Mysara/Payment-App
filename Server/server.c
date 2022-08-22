#include "server.h"
#include "../LinearSearch.h"

EN_transState_t transState;
EN_serverError_t serverError;


ST_accountsDB_t accountsDB[10] = {
	//balance status acc number
	{1000, RUNNING, "3782822463310005"},
	{200, BLOCKED,"3714496353984231"},
	{100000, RUNNING, "3787344936371000"},
	{1400, RUNNING, "56105910810183250"},
	{928364, RUNNING, "305693090259304"},
	{1110, RUNNING, "385200000233237"},
	{1092348, RUNNING, "60111111131111117"},
	{120, RUNNING, "60110009901339424"},
	{1120938, RUNNING, "35301113333300000"},
	{200000, RUNNING, "35660020203360505"}
};



EN_transState_t receiveTransactionData(ST_transaction_t* transData)
{
	//ST_accountsDB_t ptrToTargetAcc= accountsDB[0] ;
	ST_accountsDB_t targetAcc;
	ST_accountsDB_t* ptrToTargetAcc = &targetAcc;
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
		if (transState == SAVING_FAILED) {
			transState = INTERNAL_SERVER_ERROR;
			printf("INTERNAL_SERVER_ERROR \n");
			return transState;
		}
		else {
			transState = APPROVED;
			printf("APPROVED\n");
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
	for (int i = 0; i < 20; i++) {
		targetPAN[i] = cardData.primaryAccountNumber[i];
	}
	int accountindex = linearSearch(accountsDB, targetPAN, 10);

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
		printf("BLOCKED_ACCOUNT\n");
		return transState;
	}
	else {
		transState = SERVER_OK;
		printf("SERVER_OK\n");
		return transState;
	}
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence)
{
	//check if transaction amount is available or not
	if (termData->transAmount > accountRefrence->balance) {
		printf("LOW_BALANCE\n");
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
		printf("SAVING_FAILED\n");
		return SAVING_FAILED;
	}
	printf("SERVER_OK\n");
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
		printf("SERVER_OK\n");
		return SERVER_OK;
	}
	printf("TRANSACTION_NOT_FOUND\n");
	return TRANSACTION_NOT_FOUND;
}
