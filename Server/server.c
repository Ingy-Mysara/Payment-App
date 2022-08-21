#include "server.h"

EN_transState_t receiveTransactionData(ST_transaction_t* transData)
{
	return;
}

EN_serverError_t isValidAccount(ST_cardData_t cardData, ST_accountsDB_t accountRefrence)
{
	return;
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence)
{
	return;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData)
{
	printf("Amount to be deducted=%f\n", termData->transAmount);
	
	return;
}

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	static int seqNum = 0;
	transactionsDB[seqNum].cardHolderData = transData->cardHolderData;
	transactionsDB[seqNum].terminalData = transData->terminalData;
	transactionsDB[seqNum].transactionSequenceNumber = seqNum;
	transactionsDB[seqNum].transState = transData->transState;
	if (getTransaction(seqNum, transData) != APPROVED) {
		return SAVING_FAILED;
	}
	return SERVER_OK;
}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData)
{
	return;
}
