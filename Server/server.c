#include "server.h"
#include "../LinearSearch.h"


EN_transState_t transState;
EN_serverError_t serverError;




EN_transState_t receiveTransactionData(ST_transaction_t* transData)
{
	ST_accountsDB_t ptrToTargetAcc;


	transState = isValidAccount(transData->cardHolderData, ptrToTargetAcc); // ptrToTargetAcc updated
	if (transState == SERVER_OK) {


		transState = isAmountAvailable(&transData->terminalData, &ptrToTargetAcc);
		if (transState != SERVER_OK) {// check if amount available
			// amount not available
			transState = DECLINED_INSUFFICIENT_FUND;
			return transState;
		}


		transState = isBlockedAccount(&ptrToTargetAcc);
		if (transState == BLOCKED_ACCOUNT) {// check account state
			transState = DECLINED_STOLEN_CARD;
			return transState;
		}


		transState = saveTransaction(transData);
		if (transState = SAVING_FAILED) {
			transState = INTERNAL_SERVER_ERROR;
			return transState;
		}
		else {
			transState = APPROVED;
			return transState;
		}

	}
	else { // account dosent Exist
		transState = FRAUD_CARD;
		return transState;
	}


}

EN_serverError_t isValidAccount(ST_cardData_t cardData, ST_accountsDB_t accountRefrence)
{
	int arraySize = (sizeof(accountsDB) / sizeof(accountsDB[0]));
	uint8_t targetPAN = cardData.primaryAccountNumber;
	int accountindex = linearSearch(accountsDB, arraySize, targetPAN);
	if (accountindex == -1) {
		serverError = ACCOUNT_NOT_FOUND;
	}
	else {
		serverError = SERVER_OK;
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
