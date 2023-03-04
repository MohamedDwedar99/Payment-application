#include <stdio.h>
#include "../Card/card.h"
#include "terminal.h"

ST_terminalData_t termdata;
ST_terminalData_t* termData = &termdata;

ST_cardData_t  extern carddata;
ST_cardData_t extern* cardData;


getTransactionDate(ST_terminalData_t* termData)
{
	uint8_t i = 0;
	while (termData->transactionDate[i] != NULL)
	{
		i++;
	}

	if (i != 10)
	{
		return WRONG_DATE;
	}


	if (termData->transactionDate[0] < '0' || termData->transactionDate[0] > '3')
		{
			return WRONG_DATE;
		}
	else if (termData->transactionDate[1] < '0' || termData->transactionDate[1] > '9')
	{
		return WRONG_DATE;
	}
	else if (termData->transactionDate[0] == '3' && termData->transactionDate[1] > '1')
	{
		return WRONG_DATE;
	}
	else if (termData->transactionDate[2] != '/')
	{
		return WRONG_DATE;
	}
	else if (termData->transactionDate[3] < '0' || termData->transactionDate[3] > '1')
	{
		return WRONG_DATE;
	}
	else if (termData->transactionDate[4] < '0' || termData->transactionDate[4] > '9')
	{
		return WRONG_DATE;
	}
	if (termData->transactionDate[3] == '1' && termData->transactionDate[4] > '2')
	{
		return WRONG_DATE;
	}
	if (termData->transactionDate[5] != '/')
	{
		return WRONG_DATE;
	}
	if (termData->transactionDate[6] < '0' || termData->transactionDate[0] > '9')
	{
		return WRONG_DATE;
	}
	if (termData->transactionDate[7] < '0' || termData->transactionDate[7] > '9')
	{
		return WRONG_DATE;
	}
	if (termData->transactionDate[8] < '0' || termData->transactionDate[8] > '9')
	{
		return WRONG_DATE;
	}
	if (termData->transactionDate[9] < '0' || termData->transactionDate[9] > '9')
	{
		return WRONG_DATE;
	}
	else
	{
		return TERMINAL_OK;
	}
}

// Test the Date
void getTransactionDateTest(void)
{
	//assume expire 05/23
	uint8_t E_date[] = "05/23";
	uint8_t testdate[][15] = {"01/01/1999","31/12/1999","aSDdccsdsd","32/12/1999","15/13/1999","01_12?1999"};
	uint8_t testresult[6] = { 0,0,1,1,1,1 };
	uint8_t test[][30] = { "TERMINAL_OK","WRONG_DATE","EXPIRED_CARD", "INVALID_CARD", "INVALID_AMOUNT", "EXCEED_MAX_AMOUNT", "INVALID_MAX_AMOUNT"};
	ST_terminalData_t data;
	ST_terminalData_t* TerminalData = &data;
	printf("Tester Name: Mohamed Dwedar\n");
	printf("Function Name: getTransactionDateTest\n");
	for (uint8_t i = 0; i < 6; i++)
	{
		printf("Test Case %d\n", i + 1);
		uint32_t j = 0;
		while (testdate[i][j] != NULL)
		{
			TerminalData->transactionDate[j] = testdate[i][j];
			j++;
		}
		TerminalData->transactionDate[j] = NULL;
		printf("Input Data: %s\n", testdate[i]);
		j = testresult[i];
		printf("Expected Result: %s\n", test[j]);
		j = getTransactionDate(TerminalData);
		printf("Actual Result: %s\n", test[j]);
	}
}
EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData)
{

	uint32_t E_month = (cardData->cardExpirationDate[0] - 48) * 10 + (cardData->cardExpirationDate[1] -48);
	uint32_t E_year = (cardData->cardExpirationDate[3] - 48) * 10 + (cardData->cardExpirationDate[4] - 48);
	uint32_t N_month = (termData->transactionDate[3] - 48) * 10 + (termData->transactionDate[4] - 48);
	uint32_t N_year = (termData->transactionDate[8] - 48) * 10 + (termData->transactionDate[9] - 48);
	if (N_year > E_year)
	{
		return EXPIRED_CARD;
	}
	else if (N_year < E_year)
	{
		return TERMINAL_OK;
	}
	if (N_year == E_year)
	{
		if (N_month < E_month)
		{
			return TERMINAL_OK;
		}
		else
		{
			return EXPIRED_CARD;
		}
	}
}

void isCardExpiredTest(void)
{
	ST_cardData_t card;
	ST_cardData_t* cardData = &card;
	uint8_t now_date[] = "05/23";
	uint32_t k = 0;
	while (now_date[k] != NULL)
	{
		cardData->cardExpirationDate[k] = now_date[k];
		k++;
	}
	cardData->cardExpirationDate[k] = NULL;
	uint8_t testdate[][15] = {"01/01/2024","01/06/2023","01/05/2023","01/04/2023","01/05/2022"};
	uint8_t testresult[5] = { 2,2,2,0,0 };
	uint8_t test[][30] = { "TERMINAL_OK","WRONG_DATE","EXPIRED_CARD", "INVALID_CARD", "INVALID_AMOUNT", "EXCEED_MAX_AMOUNT", "INVALID_MAX_AMOUNT"};
	ST_terminalData_t data;
	ST_terminalData_t* TerminalData = &data;
	printf("Tester Name: Mohamed Dwedar\n");
	printf("Function Name: isCardExpiredTest\n");
	for (uint8_t i = 0; i < 5; i++)
	{
		printf("Test Case %d\n", i + 1);
		uint32_t j = 0;
		while (testdate[i][j] != NULL)
		{
			TerminalData->transactionDate[j] = testdate[i][j];
			j++;
		}
		TerminalData->transactionDate[j] = NULL;
		printf("Input Data: %s\n", testdate[i]);
		j = testresult[i];
		printf("Expected Result: %s\n", test[j]);
		j = isCardExpired(cardData,TerminalData);
		printf("Actual Result: %s\n", test[j]);
	}
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	if (termData->transAmount <= 0)
	{
		return INVALID_AMOUNT;
	}
	else
	{
		return TERMINAL_OK;
	}
}

void getTransactionAmountTest(void)
{
	ST_terminalData_t Terminal;
	ST_terminalData_t* TerminalData = &Terminal;
	float amm[] = { 0,1.1,2000.5,-1000 };
	int result[] = { 4,0,0,4 };
	uint8_t test[][30] = { "TERMINAL_OK","WRONG_DATE","EXPIRED_CARD", "INVALID_CARD", "INVALID_AMOUNT", "EXCEED_MAX_AMOUNT", "INVALID_MAX_AMOUNT" };
	printf("Tester Name: Mohamed Dwedar\n");
	printf("Function Name: getTransactionAmountTest\n");
	for (uint8_t i = 0; i < 4; i++)
	{
		TerminalData->transAmount = amm[i];
		printf("Test Case %d\n", i + 1);
		uint32_t j = 0;
		printf("Input Data: %f\n", amm[i]);
		j = result[i];
		printf("Expected Result: %s\n", test[j]);
		j = getTransactionAmount(TerminalData);
		printf("Actual Result: %s\n", test[j]);
	}
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	if (termData->transAmount <= termData->maxTransAmount)
	{
		return TERMINAL_OK;
	}
	else
	{
		return EXCEED_MAX_AMOUNT;
	}
}

void isBelowMaxAmountTest(void)
{
	ST_terminalData_t Terminal;
	ST_terminalData_t* TerminalData = &Terminal;
	//assume max ammount = 10000
	TerminalData->maxTransAmount = 10000;
	float amm[] = { 100,10000,15000 };
	int result[] = { 0,0,5};
	uint8_t test[][30] = { "TERMINAL_OK","WRONG_DATE","EXPIRED_CARD", "INVALID_CARD", "INVALID_AMOUNT", "EXCEED_MAX_AMOUNT", "INVALID_MAX_AMOUNT" };
	printf("Tester Name: Mohamed Dwedar\n");
	printf("Function Name: isBelowMaxAmountTest\n");
	for (uint8_t i = 0; i < 3; i++)
	{
		TerminalData->transAmount = amm[i];
		printf("Test Case %d\n", i + 1);
		uint32_t j = 0;
		printf("Input Data: %f\n", amm[i]);
		j = result[i];
		printf("Expected Result: %s\n", test[j]);
		j = isBelowMaxAmount(TerminalData);
		printf("Actual Result: %s\n", test[j]);
	}
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData, float maxAmount)
{
	if (maxAmount <= 0)
	{
		return INVALID_MAX_AMOUNT;
	}
	else
	{
		termData->maxTransAmount = maxAmount;
		return TERMINAL_OK;
	}
}

void setMaxAmountTest(void)
{
	ST_terminalData_t Terminal;
	ST_terminalData_t* TerminalData = &Terminal;
	//assume max ammount = 10000
	float amm[] = { -100,0,15000 };
	int result[] = { 6,6,0 };
	uint8_t test[][30] = { "TERMINAL_OK","WRONG_DATE","EXPIRED_CARD", "INVALID_CARD", "INVALID_AMOUNT", "EXCEED_MAX_AMOUNT", "INVALID_MAX_AMOUNT" };
	printf("Tester Name: Mohamed Dwedar\n");
	printf("Function Name: setMaxAmountTest\n");
	for (uint8_t i = 0; i < 3; i++)
	{
		printf("Test Case %d\n", i + 1);
		uint32_t j = 0;
		printf("Input Data: %f\n", amm[i]);
		j = result[i];
		printf("Expected Result: %s\n", test[j]);
		j = setMaxAmount(TerminalData, amm[i]);
		printf("Actual Result: %s\n", test[j]);
	}
}