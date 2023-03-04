#include <stdio.h>
#include "Card/card.h"
#include "Terminal/terminal.h"
#include "Server/server.h"
#include "Application/app.h"

ST_accountsDB_t extern accountsDB;
ST_accountsDB_t extern* account_Refrence;

void main(void)
{
	//while (1)
	
	getCardExpiryDateTest();

	
}


/*
Testing Application
data Base
	{2000.0, RUNNING, "8989374615436851"},
	{3000, BLOCKED,  "3131069584321845"},
	{500, RUNNING, "1039500099388999"},
	{200, RUNNING,    "3059992884020950"},
	{0, RUNNING,      "1193009487294060"}

	Case 1: Sucessful payment
		account Name :Mohamed Hassan Dwedar pan 8989374615436851 Expirey 01/25 || trans ammount 300 max amount 1000 date 20/01/2023
	Case 2: Exceed Max Ammount
		account Name :Mohamed Hassan Dwedar 8989374615436851 Expirey 01/25 || trans ammount 1200 max amount 1000 date 20/01/2023
	Case 3: Insufficent Fund
		account Name :Mohamed Ahmed elshzly 3059992884020950 Expirey 01/25 || trans ammount 700 max amount 1000 date 20/01/2023
	Case 4: Expirey Card
		account Name :Ahmed AlaaEldin hossam 1039500099388999 Expirey 01/20 || trans ammount 200 max amount 1000 date 20/01/2023
	case 5:Blocked Card
		account Name :Fatma Mohamed AlaaEldin pan 3131069584321845 Expirey 01/25 || trans ammount 700 max amount 1500 date 20/01/2023
*/