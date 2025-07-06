#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#pragma warning(disable : 4996)
using namespace std;

const string ClientsFileName = "Clients.txt";

void ShowMainMenueScreen();
void Login();

struct stClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance = 0;
	bool MarkForDelete = false;
};
struct stDate
{
	short Year = 0;
	short Month = 0;
	short Day = 0;
};

enum enMainMenueChoice
{
	enQuickWithdraw = 1, enNormalWithdraw = 2,
	enDeposit = 3, enCheckBalances = 4, enLogout = 5
};


stClient CurrentClient;

stDate GetSystemDate()
{
	stDate Date1;
	time_t t = time(0);
	tm* now = localtime(&t);

	Date1.Year = now->tm_year + 1900;
	Date1.Month = now->tm_mon + 1;
	Date1.Day = now->tm_mday;

	return Date1;
}

enMainMenueChoice ReturnChoiceNumberTran()
{
	short Num = 0;
	do
	{
		cout << "Choice what do you want to do ? [ 1 to 5 ] ? ";
		cin >> Num;

	} while (Num > 5 || Num < 0);

	return	enMainMenueChoice(Num);
}

string ConvertRecordToLine(stClient Client, string Seperator = "#//#")
{

	string stClientRecord = "";
	stClientRecord += Client.AccountNumber + Seperator;
	stClientRecord += Client.PinCode + Seperator;
	stClientRecord += Client.Name + Seperator;
	stClientRecord += Client.Phone + Seperator;
	stClientRecord += to_string(Client.AccountBalance);
	return stClientRecord;
}

vector <stClient> SaveCleintsDataToFile(string FileName, vector <stClient> vClients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);//overwrite

	string DataLine;

	if (MyFile.is_open())
	{
		for (stClient C : vClients)
		{

			if (C.MarkForDelete == false)
			{
				//we only write records that are not marked for delete.  
				DataLine = ConvertRecordToLine(C);
				MyFile << DataLine << endl;
			}

		}

		MyFile.close();
	}

	return vClients;
}

string PrintSpace(int length)
{
	string Space;
	for (short i = 0; i < length; i++)
	{
		Space += "\t";
	}

	return Space;
}

void PrintClientBalances(stClient stClient)
{
	cout << "| " << left << setw(15) << stClient.AccountNumber;
	cout << "| " << left << setw(40) << stClient.Name;
	cout << "| " << left << setw(15) << stClient.AccountBalance;

}

bool DepositBalancesClientByAccountNumber(string AccountNumber, vector <stClient> vClients, stClient& Client, double Amount)
{
	char Answer = 'n';
	cout << "\n\nAre sure you want perform this transaction ? y/n ? ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
	{
		for (stClient& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += Amount;
				CurrentClient.AccountBalance = C.AccountBalance;
				SaveCleintsDataToFile(ClientsFileName, vClients);
				cout << "\n\nDone successfully ,New Balance is " << C.AccountBalance;
				return true;
			}
		}

	}

	return false;
}

void PrintClientCard(stClient Client)
{
	cout << "\nThe following are the client details:\n";
	cout << "-----------------------------------";
	cout << "\nAccout Number: " << Client.AccountNumber;
	cout << "\nPin Code     : " << Client.PinCode;
	cout << "\nName         : " << Client.Name;
	cout << "\nPhone        : " << Client.Phone;
	cout << "\nAccount Balance: " << Client.AccountBalance;
	cout << "\n-----------------------------------\n";
}

string GetAccountNumberAndCheck(vector <stClient> vClient)
{


	string AccountNumber;
	cout << "\nPlease Enter AccountNumber ? ";
	cin >> AccountNumber;



	return AccountNumber;

}

int SumOfTotalBalances(vector <stClient> vClient)
{
	int SumBalances = 0;
	for (stClient C : vClient)
	{
		SumBalances += C.AccountBalance;


	}

	return SumBalances;

}

vector<string> SplitString(string S1, string Delim)
{
	vector<string> vString;
	short pos = 0;
	string sWord; // define a string variable  

	// use find() function to get the position of the delimiters  
	while ((pos = S1.find(Delim)) != std::string::npos)
	{
		sWord = S1.substr(0, pos); // store the word   
		if (sWord != "")
		{
			vString.push_back(sWord);
		}

		/* erase() until positon and move to next word. */
		S1.erase(0, pos + Delim.length());
	}

	if (S1 != "")
	{
		vString.push_back(S1); // it adds last word of the string.
	}

	return vString;

}

stClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
	stClient Client;
	vector<string> vClientData;
	vClientData = SplitString(Line, Seperator);

	Client.AccountNumber = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.Name = vClientData[2];
	Client.Phone = vClientData[3];
	Client.AccountBalance = stod(vClientData[4]);
	//cast string to double
	return Client;
}

vector <stClient> LoadCleintsDataFromFile(string FileName)
{
	vector <stClient> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);//read Mode

	if (MyFile.is_open())
	{
		string Line;
		stClient Client;

		while (getline(MyFile, Line))
		{
			Client = ConvertLinetoRecord(Line);
			vClients.push_back(Client);
		}
		MyFile.close();
	}
	return vClients;
}

void PrintChoiceScreen(string Text)
{
	cout << "===================================\n";

	cout << "      " << Text;

	cout << "===================================\n";
}

void ShowDepositClientScreen()
{
	vector <stClient> vClient = LoadCleintsDataFromFile(ClientsFileName);
	string AccountNumber = CurrentClient.AccountNumber;

	double Amount = 0;
	cout << "\n\nPlease enter deposit amount ?";
	cin >> Amount;

	DepositBalancesClientByAccountNumber(AccountNumber, vClient, CurrentClient, Amount);

}

void GoBackToTheMenue(string Txt)
{

	cout << Txt;
	system("pause >>nul  ");


}

int ReadQuickWithdrawChoice()
{
	int Choic = 0;
	cout << "\nChoose What to Withdraw from [1] to 8 ? \b";
	cin >> Choic;

	switch (Choic)
	{
	case 1:
		return 20;
		break;
	case 2:
		return 50;
		break;
	case 3:
		return 100;
		break;
	case 4:
		return 200;
		break;
	case 5:
		return 400;
		break;
	case 6:
		return 600;
		break;
	case 7:
		return 800;
		break;
	case 8:
		return 1000;
		break;

	case 9:
		return 0;
		break;
	default:


		cout << "\aWrong Choic .";
		ReadQuickWithdrawChoice();
		break;

	}


}

bool CkeckMultiplesOfFive(int Amount)
{

	return (Amount % 5 == 0);

}

double ReadNormalWithdrawChoice()
{
	double  Amount = 0;
	cout << "Enter an amount maltiple of 5's ? ";
	cin >> Amount;

	while (!CkeckMultiplesOfFive(Amount))
	{
		cout << "Enter an amount maltiple of 5's ? ";
		cin >> Amount;
	}
	return Amount;
}

void ShowQuickWithdrawClientScreen()
{
	printf("     [1] %d       [2] %d", 20, 50);
	printf("\n     [3] %d      [4] %d", 100, 200);
	printf("\n     [5] %d      [6] %d", 400, 600);
	printf("\n     [7] %d      [8] %d", 800, 1000);
	cout << "\n     [9] Exit";
	cout << "\n=====================================";
	cout << "\nYour Balance is " << CurrentClient.AccountBalance;
}

void PerfromQuickWithdrawOption(double withdraw)
{
	if (withdraw != 0)
	{


		vector <stClient> vClient = LoadCleintsDataFromFile(ClientsFileName);
		string AccountNumber = CurrentClient.AccountNumber;

		while (withdraw > CurrentClient.AccountBalance)
		{

			cout << "\n\nAmount Exceeds the balance, you can withdraw up to : " << CurrentClient.AccountBalance;
			cout << "\n\aPress anyKey To Continue...";
			system("pause >>nul  ");
			system("cls");
			PrintChoiceScreen("Quick Withdraw Screen\n");
			ShowQuickWithdrawClientScreen();
			withdraw = ReadQuickWithdrawChoice();
		}

		if (withdraw != 0)
		{
			DepositBalancesClientByAccountNumber(AccountNumber, vClient, CurrentClient, withdraw * -1);

		}
	}
}

void ShowWithdrawClientScreen(double withdraw = 0)
{
	if (withdraw != 0)
	{


		vector <stClient> vClient = LoadCleintsDataFromFile(ClientsFileName);
		string AccountNumber = CurrentClient.AccountNumber;

		while (withdraw > CurrentClient.AccountBalance)
		{
			cout << "\n\nAmount Exceeds the balance, you can withdraw up to : " << CurrentClient.AccountBalance;
			cout << "\n\aPress anyKey To Continue...";
			system("pause >>nul  ");
			system("cls");
			PrintChoiceScreen("Withdraw Screen\n");
			ShowWithdrawClientScreen();
			withdraw = ReadNormalWithdrawChoice();
		}

		DepositBalancesClientByAccountNumber(AccountNumber, vClient, CurrentClient, withdraw * -1);
	}
}

void ShowCurrentBalance()
{
	cout << "Your Balance is " << CurrentClient.AccountBalance;
}

bool FindClientByAccountNumberAndPinCode(string AccounNumber, string Pincod, stClient& CurrentClient)
{
	vector <stClient> Clients = LoadCleintsDataFromFile(ClientsFileName);

	for (stClient Data : Clients)
	{
		if (AccounNumber == Data.AccountNumber && Pincod == Data.PinCode)
		{
			CurrentClient = Data;
			return true;
		}
	}
	return false;
}

bool LoadClientInfo(string AccounNumber, string Pincod)
{

	if (FindClientByAccountNumberAndPinCode(AccounNumber, Pincod, CurrentClient))
		return true;
	else
		return false;

}

void PerfromShowMainMenue(enMainMenueChoice Choices)
{
	switch (Choices)
	{

	case enMainMenueChoice::enQuickWithdraw:

		system("cls");
		PrintChoiceScreen("Quick Withdraw Screen\n");
		ShowQuickWithdrawClientScreen();
		PerfromQuickWithdrawOption(ReadQuickWithdrawChoice());
		GoBackToTheMenue("\n\nPress any key to go back to the MainMenue menue...");
		ShowMainMenueScreen();
		break;

	case enMainMenueChoice::enNormalWithdraw:

		system("cls");
		PrintChoiceScreen("Withdraw Screen\n");
		ShowWithdrawClientScreen(ReadNormalWithdrawChoice());
		GoBackToTheMenue("\n\nPress any key to go back to the MainMenue menue...");
		ShowMainMenueScreen();
		break;

	case enMainMenueChoice::enDeposit:

		system("cls");
		PrintChoiceScreen("Deposit Screen\n");
		ShowDepositClientScreen();
		GoBackToTheMenue("\n\nPress any key to go back to the MainMenue menue...");
		ShowMainMenueScreen();
		break;

	case enMainMenueChoice::enCheckBalances:

		system("cls");
		PrintChoiceScreen("Check Balance Screen\n");
		ShowCurrentBalance();
		GoBackToTheMenue("\n\nPress any key to go back to the MainMenue menue...");
		ShowMainMenueScreen();

		break;
	case enMainMenueChoice::enLogout:

		Login();

		break;
	}
}

void ShowMainMenueScreen()
{
	stDate Date = GetSystemDate();

	system("cls");
	cout << "Date :" << Date.Day << "/" << Date.Month << "/" << Date.Year;
	cout << " - Client Name :" << CurrentClient.Name << endl;
	cout << "===============================================\n";

	cout << PrintSpace(1) << " ATM Main Menue Screen ";

	cout << "\n===============================================\n";

	cout << PrintSpace(1) << "   [1] Quick Withdraw.\n";
	cout << PrintSpace(1) << "   [2] Normal Withdraw.\n";
	cout << PrintSpace(1) << "   [3] Deposit.\n";
	cout << PrintSpace(1) << "   [4] Check Balance.\n";
	cout << PrintSpace(1) << "   [5] Logout.\n";
	cout << "===============================================\n";
	PerfromShowMainMenue(ReturnChoiceNumberTran());
}

void Login()
{
	bool LoginFiled = false;

	string AccounNumber, PinCode;

	do
	{
		system("cls");

		cout << "\n-----------------------------\n";
		cout << "\tLogin Screen";
		cout << "\n-----------------------------\n";

		if (LoginFiled)
		{
			cout << "Invlaid AccounNumber/PinCode!\n";
		}

		cout << "Enter AccounNumber ? ";
		cin >> AccounNumber;

		cout << "Enter PinCode ? ";
		cin >> PinCode;


		// if true convert to false to Getout of do while
		LoginFiled = !LoadClientInfo(AccounNumber, PinCode);

	} while (LoginFiled);

	ShowMainMenueScreen();
}

int main()
{

	Login();

	system("pause>0");
	return 0;
}


