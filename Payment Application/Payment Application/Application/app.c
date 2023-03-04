#include <stdio.h>
#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "../Server/server.h"
#include "app.h"

ST_transaction_t extern transdata[255];
ST_transaction_t extern* transData;
ST_accountsDB_t extern accountsDB[255];

ST_cardData_t  extern carddata;
ST_cardData_t extern* cardData;

ST_accountsDB_t extern* accountRefrence;

ST_terminalData_t extern termdata;
ST_terminalData_t extern* termData;




void appStart(void)
{
	printf("--------------------\n");
	printf("Start The Applicatin\n");
	printf("--------------------\n");
	// Start Card Module
	printf("Card Holder Name: ");
	scanf(" %[^\n]%*c", &cardData->cardHolderName);
	printf("Primary Account Number: ");
	scanf(" %s", &cardData->primaryAccountNumber);
	printf("Expiration Date(MM/YY): ");
	scanf("%s", &cardData->cardExpirationDate);
	uint8_t cardError;
	cardError = getCardHolderName(cardData);
	if (cardError == WRONG_NAME)
	{
		printf("Wrong Name formula\n");
		return WRONG_NAME;
	}
	cardError = getCardExpiryDate(cardData);
	if (cardError == WRONG_EXP_DATE)
	{
		printf("Wrong Date formula\n");
		return WRONG_EXP_DATE;
	}
	cardError = getCardPAN(cardData);
	if (cardError == WRONG_PAN)
	{
		printf("Wrong PAN formula\n");
		return WRONG_PAN;
	}
	printf("------------------------------\n");
	printf("Card Ok\nChecking The Terminal\n");
	printf("------------------------------\n");
	//Sart Terminal Module
	printf("Enter The Transaction Amount: ");
	scanf("%f",&termData->transAmount);
	printf("Enter The Transaction Date(DD/MM/YYYY): ");
	scanf("%s",&termData->transactionDate);
	printf("Enter The Max Transaction: ");
	scanf("%f",&termData->maxTransAmount);
	uint8_t TermError;
	uint8_t TermError_Arr[7][19] = { "TERMINAL_OK","WRONG_DATE","EXPIRED_CARD", "INVALID_CARD", "INVALID_AMOUNT", "EXCEED_MAX_AMOUNT", "INVALID_MAX_AMOUNT" };
	TermError = getTransactionDate(termData);
	if (TermError == WRONG_DATE)
	{
		printf("Worng Date Formula\n");
		return WRONG_DATE;
	}
	TermError = isCardExpired(cardData, termData);
	if (TermError == EXPIRED_CARD)
	{
		printf("Expired Card\n");
		return EXPIRED_CARD;
	}
	TermError = getTransactionAmount(termData);
	if (TermError == INVALID_AMOUNT)
	{
		printf("Invalid Amount\n");
		return INVALID_AMOUNT;
	}
	TermError = isBelowMaxAmount(termData);
	if (TermError == EXCEED_MAX_AMOUNT)
	{
		printf("Exceed Max Amount\n");
		return EXCEED_MAX_AMOUNT;
	}
	TermError = setMaxAmount(termData, termData->maxTransAmount);
	if (TermError == INVALID_MAX_AMOUNT)
	{
		printf("INVALID_MAX_AMOUNT\n");
		return INVALID_MAX_AMOUNT;
	}
	printf("------------------------------\n");
	printf("Terminal Ok\nChecking The Server\n");
	printf("------------------------------\n");
	//APPROVED, DECLINED_INSUFFECIENT_FUND, DECLINED_STOLEN_CARD, FRAUD_CARD, INTERNAL_SERVER_ERROR
	uint8_t ServerError = recieveTransactionData(transData);
	switch (ServerError)
	{
	case DECLINED_INSUFFECIENT_FUND:
		printf("Declined Insuffcient fund\n");
		return DECLINED_INSUFFECIENT_FUND;
		break;
	case FRAUD_CARD:
		printf("Fraud Card\n");
		return FRAUD_CARD;
		break;
	case DECLINED_STOLEN_CARD:
		printf("Stoled Card\n");
		return DECLINED_STOLEN_CARD;
		break;
	case INTERNAL_SERVER_ERROR:
		printf("Internal Server Error\n");
		return INTERNAL_SERVER_ERROR;
		break;
	}
	printf("------------------------------\n");
	printf("Server Ok\nTransaction Complete\n");
	printf("------------------------------\n");
}
