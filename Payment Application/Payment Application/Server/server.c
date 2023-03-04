#include <stdio.h>
#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "server.h"
#include <string.h>

ST_cardData_t  extern carddata;
ST_cardData_t extern* cardData;
ST_terminalData_t extern termdata;
ST_terminalData_t extern* termData;

ST_accountsDB_t accountsDB[255] = {
	{2000.0, RUNNING, "8989374615436851"},
	{3000, BLOCKED,  "3131069584321845"},
	{500, RUNNING, "1039500099388999"},
	{200, RUNNING,    "3059992884020950"},
	{0, RUNNING,      "1193009487294060"}
};

ST_accountsDB_t * account_Refrence = &accountsDB[0];
ST_transaction_t transdata[255];
ST_transaction_t* transData = &transdata[0];

EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	uint8_t server_Error = isValidAccount(cardData, account_Refrence);
	if (server_Error == ACCOUNT_NOT_FOUND)
	{
		return FRAUD_CARD;
	}
	server_Error = isBlockedAccount(account_Refrence);
	if (server_Error == BLOCKED_ACCOUNT)
	{
		return DECLINED_STOLEN_CARD;
	}
	server_Error = isAmountAvailable(termData, account_Refrence);
	if (server_Error == LOW_BALANCE)
	{
		return DECLINED_INSUFFECIENT_FUND;
	}
	server_Error == saveTransaction(transData);
	if (server_Error == SAVING_FAILED)
	{
		return INTERNAL_SERVER_ERROR;
	}
	account_Refrence->balance = account_Refrence->balance - transData->terminalData.transAmount;
	return APPROVED;
}

void recieveTransactionDataTest(void)
{
	printf("Tester Name: Mohamed Dwedar\n");
	printf("Function Name: recieveTransactionDataTest\n");
	ST_cardData_t Cardarr[4] = {
		{"Mohamed Dwedar","1193009487294010","01/25"},// Not Found
		{"Ahmed Mohamed","3131069584321845","01/25"},//Blocked
		{"Fawzy Fattoh","1039500099388999","01/25"},//No money
		{"Mahmoud El Drieny","8989374615436851","01/25"}//ok
	};
	uint8_t test[5][30] = { "APPROVED", "DECLINED_INSUFFECIENT_FUND", "DECLINED_STOLEN_CARD","FRAUD_CARD","INTERNAL_SERVER_ERROR" };
	uint8_t result[4] = { 3,2,1,0 };
	for (uint8_t i = 0; i < 4; i++)
	{
		termData->transAmount = 550;
		printf("Test Case %d\n", i + 1);
		cardData = &Cardarr[i];
		printf("Input Data: %s %s %s\n", Cardarr[i].cardHolderName, Cardarr[i].primaryAccountNumber, Cardarr[i].cardExpirationDate);
		uint32_t j = result[i];
		printf("Expected Result: %s\n", test[j]);
		recieveTransactionData(transData);
		printf("Actual Result: %s\n", test[j]);
	}
}


EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t * accountRefrence)
{
	for (uint32_t i = 0; i < 255; i++)
	{
		uint8_t j = strcmp(cardData->primaryAccountNumber,accountsDB[i].primaryAccountNumber);
		if (j == 0)
		{
			account_Refrence = &accountsDB[i];
			return SERVER_OK;
		}
	}
	accountRefrence = NULL;
	return ACCOUNT_NOT_FOUND;
}
void isValidAccountTest(void)
{
	ST_accountsDB_t *reference = NULL;
	ST_cardData_t test[6] = {
	{"0","8989374615436851","0"},
	{"0","3131069584321845","0"},
	{"0","1039500099388999","0"},
	{"0","3059992884020950","0"},
	{"0","1193009487294060","0"},
	{"0","1193209487294060","0"}
	};
	printf("Tester Name: Mohamed Dwedar\n");
	printf("Function Name: isValidAccountTest\n");
	uint8_t cases[6][30] = {"SERVER_OK","SAVING_FAILED","TRANSACTION_NOT_FOUND"," ACCOUNT_NOT_FOUND", "LOW_BALANCE","BLOCKED_ACCOUNT" };
	uint8_t result[6] = { 0,0,0,0,0,3 };
	for (uint32_t i = 0; i < 6; i++)
	{
		printf("Test Case: %d\n", i+1);
		ST_cardData_t* validTest = &test[i];
		printf("Input Data: %s\n", validTest->primaryAccountNumber);
		uint8_t j = result[i];
		printf("expected result: %s\n", cases[j]);
		j = isValidAccount(validTest, reference);
		printf("Actual result: %s\n", cases[j]);
	}
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t * accountRefrence)
{
	if (accountRefrence->state == BLOCKED)
	{
		return BLOCKED_ACCOUNT;
	}
		return SERVER_OK;
}
void isBlockedAccountTest(void)
{
	printf("Tester Name: Mohamed Dwedar\n");
	printf("Function Name: isBlockedAccountTest\n");
	uint8_t cases[6][30] = { "SERVER_OK","SAVING_FAILED","TRANSACTION_NOT_FOUND"," ACCOUNT_NOT_FOUND", "LOW_BALANCE","BLOCKED_ACCOUNT" };
	uint8_t result[5] = { 0,BLOCKED_ACCOUNT,0,0,0 };
	for (uint8_t i = 0; i < 5; i++)
	{
		ST_accountsDB_t* reference = &accountsDB[i];
		printf("Test Case: %d\n", i + 1);
		printf("Input Data: %d\n", accountsDB[i].state);
		uint8_t j = result[i];
		printf("expected result: %s\n", cases[j]);
		j = isBlockedAccount(reference);
		printf("Actual result: %s\n", cases[j]);
	}

}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t * accountRefrence)
{
	if (termData->transAmount <= accountRefrence->balance)
	{
		return SERVER_OK;
	}
	else
	{
		return LOW_BALANCE;
	}
}

void isAmountAvailableTest(void)
{
	ST_terminalData_t amount = { 500,50000,"26/01/1999" };
	ST_terminalData_t* Amount = &amount;
	printf("Tester Name: Mohamed Dwedar\n");
	printf("Function Name: isAmountAvailableTest\n");
	uint8_t cases[6][30] = { "SERVER_OK","SAVING_FAILED","TRANSACTION_NOT_FOUND"," ACCOUNT_NOT_FOUND", "LOW_BALANCE","BLOCKED_ACCOUNT" };
	uint8_t result[5] = { 0,0,0,LOW_BALANCE,LOW_BALANCE };
	for (uint8_t i = 0; i < 5; i++)
	{
		ST_accountsDB_t* reference = &accountsDB[i];
		printf("Test Case: %d\n", i + 1);
		printf("Input Data: %f\n", accountsDB[i].balance);
		uint8_t j = result[i];
		printf("expected result: %s\n", cases[j]);
		j = isAmountAvailable(Amount,reference);
		printf("Actual result: %s\n", cases[j]);
	}
}

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	uint32_t i = 0;
	while (transdata[i].transactionSequenceNumber != 0)
	{
		i++;
	}
	if (i > 255)
	{
		return SAVING_FAILED;
	}
	transData = &transdata[i];
	transData->transactionSequenceNumber = i+1;
	ST_cardData_t* card_Data = &transData->cardHolderData;
	memcpy(card_Data, cardData, sizeof carddata); 
	ST_terminalData_t* term_Data = &transData->terminalData;
	memcpy(term_Data, termData, sizeof termdata);
	transData->transState = APPROVED;
	return SERVER_OK;
}
void saveTransactionTest(void)
{
	printf("Tester Name: Mohamed Dwedar\n");
	printf("Function Name: saveTransactionTest\n");
	ST_cardData_t card_data[3] = { {"MMMMMM","1111111","24/01"},{"YYYYYY","2222222","24/01"},{"MMMMMM","1111111","24/01"} };
	ST_terminalData_t term_data[3] = { {500,2000,"22/01/2020"},{100,2000,"23/01/2023"},{500,2000,"22/01/2020"} };
	for (uint8_t i = 0; i < 3; i++)
	{
		ST_cardData_t* card_Data = &card_data[i];
		ST_terminalData_t* term_Data = &term_data[i];
		printf("Case: %d\n", i);
		if (i < 2)
		{
			printf("Input Data:\n	1-Card %s %s %s\n	2-Terminal %d %d %s\n", card_data[i].cardHolderName, card_data[i].primaryAccountNumber, card_data[i].cardExpirationDate, term_data[i].transAmount, term_data[i].maxTransAmount, term_data[i].transactionDate);
			printf("Expected Result : % d\n", SERVER_OK);
		}
		if (i == 2)
		{
			printf("Input Data:\n	Sequence Num > 255\n");
			printf("Expected Result : % d\n", SAVING_FAILED);
		}
		memcpy(cardData, card_Data, sizeof carddata);
		memcpy(termData, term_Data, sizeof termdata);
		uint32_t j;
		uint32_t w;
		if (i < 2)
		{
			j = saveTransaction(transData);
		}
		if (i == 2)
		{
			for (w = 0; w <= 300; w++)
			{
				j = saveTransaction(transData);
			}
		}
		printf("Actual Result : % d\n", j);
		
	}
}

void listSavedTransactions(void)
{
	uint8_t i = 0;
	while (transdata[i].transactionSequenceNumber != 0)
	{
		printf("#########################\n");
		printf("Transaction Sequence Number: %d\n", transdata[i].transactionSequenceNumber);
		printf("Transaction Date: %s\n",transdata[i].terminalData.transactionDate);
		printf("Transaction Amount: %f\n",transdata[i].terminalData.transAmount);
		printf("Transaction State: %d\n", transdata[i].transState);
		printf("Terminal Max Amount: %f\n", transdata[i].terminalData.maxTransAmount);
		printf("Cardholder Name: %s\n", transdata[i].cardHolderData.cardHolderName);
		printf("PAN: %s\n", transdata[i].cardHolderData.primaryAccountNumber);
		printf("Card Expiration Date: %s\n", transdata[i].cardHolderData.cardExpirationDate);
		printf("#########################\n");
		i++;
	}
}
void listSavedTransactionsTest(void) 
{
	printf("Tester Name: Mohamed Dwedar\n");
	printf("Function Name: listSavedTransactionsTest\n");
	ST_cardData_t card_data[2] = { {"MMMMMM","1111111","24/01"},{"YYYYYY","2222222","24/01"} };
	ST_terminalData_t term_data[2] = { {500,2000,"22/01/2020"},{100,2000,"23/01/2023"} };
	for (uint8_t i = 0; i < 2; i++)
	{
		ST_cardData_t* card_Data = &card_data[i];
		ST_terminalData_t* term_Data = &term_data[i];
		printf("Case: %d\n", i+1);
		printf("Input Data:\n	1-Card %s %s %s\n	2-Terminal %d %d %s\n", card_data[i].cardHolderName, card_data[i].primaryAccountNumber, card_data[i].cardExpirationDate, term_data[i].transAmount, term_data[i].maxTransAmount, term_data[i].transactionDate);
		printf("Expected Result :\n");
		memcpy(cardData, card_Data, sizeof carddata);
		memcpy(termData, term_Data, sizeof termdata);
		uint8_t j = saveTransaction(transData);
		printf("#########################\n");
		printf("Transaction Sequence Number: %d\n", i+1);
		printf("Transaction Date: %s\n", term_Data->transactionDate);
		printf("Transaction Amount: %f\n", term_Data->transAmount);
		printf("Transaction State: %d\n", 0);
		printf("Terminal Max Amount: %f\n", term_Data->maxTransAmount);
		printf("Cardholder Name: %s", card_Data->cardHolderName);
		printf("PAN: %s\n", card_Data->primaryAccountNumber);
		printf("Card Expiration Date: %s\n", card_Data->cardExpirationDate);
		printf("#########################\n");
		printf("Actual Result : % d\n", j);
		listSavedTransactions();
	}

}