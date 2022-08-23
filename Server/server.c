#include "server.h"
#include "../LinearSearch.h"

EN_transState_t transState;
EN_serverError_t serverError;


ST_accountsDB_t accountsDB[10] = {
	//balance status acc number
	{1000, RUNNING, "37828224633100057"},
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
	
	ST_accountsDB_t targetAcc;
	transState = isValidAccount(transData->cardHolderData, &targetAcc); // ptrToTargetAcc updated
	



	if (transState == SERVER_OK) {

		transState = isAmountAvailable(&transData->terminalData, &targetAcc);
		if (transState != SERVER_OK) {// check if amount available
			// amount not available
			transState = DECLINED_INSUFFICIENT_FUND;
			printf("DECLINED_INSUFFICIENT_FUND \n");
			return transState;
		}

		transState = isBlockedAccount(&targetAcc);
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
			printf("your current balance = %f \n", targetAcc.balance);
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

		accountRefrence->balance = accountsDB[accountindex].balance;
		strcpy(accountRefrence->primaryAccountNumber ,accountsDB[accountindex].primaryAccountNumber);
		accountRefrence->state = accountsDB[accountindex].state;
		
		


		serverError = SERVER_OK;
		
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
		
		return transState;
	}
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence)
{
	printf("acctual amount % f \n", accountRefrence->balance);
	//check if transaction amount is available or not
	if (termData->transAmount > (float)(accountRefrence->balance)) {
		
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

	if (getTransaction(seqNum, transData) != SERVER_OK) {
		printf("SAVING_FAILED\n");
		transactionsDB[seqNum].transState = INTERNAL_SERVER_ERROR;
		return SAVING_FAILED;
	}
	
	transactionsDB[seqNum].transState = APPROVED;
	int accountindex = linearSearch(accountsDB, transData->cardHolderData.primaryAccountNumber, 10);
	accountsDB[accountindex].balance = accountsDB[accountindex].balance - transData->terminalData.transAmount;
	


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
	printf("TRANSACTION_NOT_FOUND\n");
	return TRANSACTION_NOT_FOUND;
}
