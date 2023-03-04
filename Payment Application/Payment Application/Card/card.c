#include <stdio.h>
#include "card.h"

ST_cardData_t  carddata;
ST_cardData_t  *  cardData = &carddata;

EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	uint32_t i = 0;
	while (cardData->cardHolderName[i] != NULL)
	{
		if (cardData->cardHolderName[i] < 'A' && cardData->cardHolderName[i] != 32)
		{
			return WRONG_NAME;
		}
		else if (cardData->cardHolderName[i] > 'z' && cardData->cardHolderName[i] != 32)
		{
			return WRONG_NAME;
		}
		i++;
	}
	if (i < 19 || i > 24)
	{
		return WRONG_NAME;
	}
	return CARD_OK;
}
// Get Holder Name Test
void getCardHolderNameTest(void)
{
	uint8_t testarr[][30] = { "01234567890123456789","AAAAAAAAAA AAAAAAAAAA","Mohamed","MohamedAhmedMahmoudhh","Mohamed Hassan Dwedar" ,"Mohamed Hassan DwedarAly", "Mohamed Hassan Dwedar Alya"};
	uint8_t testresult[7] = { 1,0,1,0,0,0,1 };
	uint8_t test[][30] = { "CARD_OK", "WRONG_NAME", "WRONG_EXP_DATE","WRONG_PAN"};
	ST_cardData_t data;
	ST_cardData_t* cardData = &data;
	printf("Tester Name: Mohamed Dwedar\n");
	printf("Function Name: getCardHolderName\n");
	for (uint8_t i = 0; i < 7; i++)
	{
		printf("Test Case %d\n", i+1);
		uint32_t j = 0;
		while (testarr[i][j] != NULL)
		{
			cardData->cardHolderName[j] = testarr[i][j];
			j++;
		}
		cardData->cardHolderName[j] = NULL;
		printf("Input Data: %s\n", testarr[i]);
		j = testresult[i];
		printf("Expected Result: %s\n", test[j]);
		j = getCardHolderName(cardData);
		printf("Actual Result: %s\n", test[j]);
	}
}


EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
	
	//check year
	uint32_t E_month = (cardData->cardExpirationDate[0] - 48) * 10 + (cardData->cardExpirationDate[1] - 48);
	uint32_t E_year = (cardData->cardExpirationDate[3] - 48) * 10 + (cardData->cardExpirationDate[0] - 48);
	if (E_month > 12 || E_month < 0 || E_year > 99)
	{
		return WRONG_EXP_DATE;
	}
	if (cardData->cardExpirationDate[2] != '/')
	{
		return WRONG_EXP_DATE;
	}
	return CARD_OK;
}
void getCardExpiryDateTest(void)
{
	uint8_t date[][7] = { "05/24","05/23","06/23","05/22","07076","ww/aa","05-24"};
	uint8_t testdate[] = { 0,0,0,0,2,2,2};
	uint8_t test[][30] = { "CARD_OK", "CARD_OK", "WRONG_NAME", "WRONG_EXP_DATE","WRONG_PAN" };
	ST_cardData_t data;
	ST_cardData_t* cardData = &data;
	printf("Tester Name: Mohamed Dwedar\n");
	printf("Function Name: getCardExpiryDate\n");
	for (uint8_t i = 0; i < 7; i++)
	{
		printf("Test Case %d\n", i + 1);
		uint32_t j = 0;
		while (date[i][j] != NULL)
		{
			cardData->cardExpirationDate[j] = date[i][j];
			j++;
		}
		cardData->cardExpirationDate[j] = NULL;
		printf("Input Data: %s\n", date[i]);
		j = testdate[i];
		printf("Expected Result: %s\n", test[j]);
		j = getCardExpiryDate(cardData);
		printf("Actual Result: %s\n", test[j]);
	}
}
EN_cardError_t getCardPAN(ST_cardData_t* cardData) 
{

	uint32_t i = 0;
	while (cardData->primaryAccountNumber[i] != NULL)
	{
		if (cardData->primaryAccountNumber[i] < '0' || cardData->primaryAccountNumber[i] > '9')
		{
			return WRONG_PAN;
		}
		i++;
	}
	if (i < 16 || i > 19)
	{
		return WRONG_PAN;
	}
	return CARD_OK;
}
void getCardPANTest(void)
{
	uint8_t pan[][30] = { "012345678901234","01234567890123456","0123456789012345678","0123456789 01234567","01234567890A123456" };
	uint8_t testpan[] = { 3,0,0,3,3 };
	uint8_t test[][30] = { "CARD_OK", "WRONG_NAME", "WRONG_EXP_DATE","WRONG_PAN" };
	ST_cardData_t data;
	ST_cardData_t* cardData = &data;
	printf("Tester Name: Mohamed Dwedar\n");
	printf("Function Name: getCardPANTest\n");
	for (uint8_t i = 0; i < 5; i++)
	{
		printf("Test Case %d\n", i + 1);
		uint32_t j = 0;
		while (pan[i][j] != NULL)
		{
			cardData->primaryAccountNumber[j] = pan[i][j];
			j++;
		}
		cardData->primaryAccountNumber[j] = NULL;
		printf("Input Data: %s\n", pan[i]);
		j = testpan[i];
		printf("Expected Result: %s\n", test[j]);
		j = getCardPAN(cardData);
		printf("Actual Result: %s\n", test[j]);
	}
}