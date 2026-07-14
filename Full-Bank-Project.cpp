#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <fstream>
#include <cstdlib>
using namespace std;
const string ClientsFileName("ClientsData.txt");

enum enMainMenuChoises { ShowClientList = 1, AddNewClient = 2, DeleteClient = 3,
	UpdateClientInformations = 4, FindClient = 5, Transactions = 6 , Exit = 7};

enum enTransactionMenuChoises {Deposite = 1, Withdraw = 2, TotalBalances = 3, MainMenu = 4};
struct sClient
{
	string AccountNumber;
	string PinCode;
	string FullName;
	string PhoneNumber;
	double AccountBalance;
	bool MarkForDelete = false;
};

void StartBank();
void StartTransactions();

vector <string> Split(string ClientLineRecord, string Delimiter)
{
	vector <string> vData;
	string Data;
	short Position;

	while ((Position = ClientLineRecord.find(Delimiter)) != string::npos)
	{
		Data = ClientLineRecord.substr(0, Position);
		if (Data != "")
			vData.push_back(Data);

		ClientLineRecord.erase(0, Position + Delimiter.length());
	}
	if (ClientLineRecord != "")
		vData.push_back(ClientLineRecord);

	return (vData);
}

sClient ConvertClientLineRecordToData(string ClientLineRecord)
{
	vector <string> vClientData = Split(ClientLineRecord, "#//#");
	sClient ClientData;

	ClientData.AccountNumber = vClientData[0];
	ClientData.PinCode = vClientData[1];
	ClientData.FullName = vClientData[2];
	ClientData.PhoneNumber = vClientData[3];
	ClientData.AccountBalance = stod(vClientData[4]);

	return (ClientData);
}

vector <sClient> ConvertAllClientsLineRecordsToData(string FileName)
{
	vector <sClient> vClientsData;
	sClient ClientData;
	string ClientLineRecord;

	fstream MyFile(FileName, ios::in);
	if (MyFile.is_open())
	{
		while (getline(MyFile, ClientLineRecord))
		{
			ClientData = ConvertClientLineRecordToData(ClientLineRecord);
			vClientsData.push_back(ClientData);
		}
		MyFile.close();
	}

	return (vClientsData);
}

void PrintClientDataInShowClientList(sClient ClientData)
{
	cout << "| " << setw(15) << left << ClientData.AccountNumber;
	cout << "| " << setw(10) << left << ClientData.PinCode;
	cout << "| " << setw(40) << left << ClientData.FullName;
	cout << "| " << setw(12) << left << ClientData.PhoneNumber;
	cout << "| " << setw(12) << left << ClientData.AccountBalance;
}

sClient AddNewClientData(string AccountNumber)
{
	sClient ClientData;

	ClientData.AccountNumber = AccountNumber;

	cout << "\nEntre Pin Code ? ";
	getline(cin >> ws, ClientData.PinCode);

	cout << "Entre Full Name ? ";
	getline(cin, ClientData.FullName);

	cout << "Entre Phone Number ? ";
	getline(cin, ClientData.PhoneNumber);

	cout << "Entre Account Balance ? ";
	cin >> ClientData.AccountBalance;

	return (ClientData);
}

string ConvertClientDataToRecordLine(sClient ClientData)
{
	string ClientRecordLine;
	string Delimiter = "#//#";

	ClientRecordLine += ClientData.AccountNumber + Delimiter;
	ClientRecordLine += ClientData.PinCode + Delimiter;
	ClientRecordLine += ClientData.FullName + Delimiter;
	ClientRecordLine += ClientData.PhoneNumber + Delimiter;
	ClientRecordLine += to_string(ClientData.AccountBalance);

	return (ClientRecordLine);
}

void SaveClientLineRecordToFile(string FileName, string ClientLineRecord)
{
	fstream MyFile(FileName, ios::app);

	if (MyFile.is_open())
	{
		MyFile << ClientLineRecord << endl;
	}
	MyFile.close();
}

vector <sClient> SaveClientLineRecordToFileAfterUpdate(string FileName, vector <sClient> &vClientsData)
{
	string ClientLineRecord;
	fstream MyFile(FileName, ios::out);

	if (MyFile.is_open())
	{
		for (sClient& Client : vClientsData)
		{
			if (Client.MarkForDelete != true)
			{
				ClientLineRecord = ConvertClientDataToRecordLine(Client);
				MyFile << ClientLineRecord << endl;
			}
		}
		MyFile.close();
	}

	return (vClientsData);
	
}

string ReadClientAccountNumber()
{
	string AccountNumber;
	cout << "\nPlease Entre Client Account Number ? ";
	cin >> AccountNumber;

	return (AccountNumber);
}

bool CheckClient(string AccountNumber, vector <sClient> vClientsData)
{
	for (sClient& ClientData : vClientsData)
	{
		if (ClientData.AccountNumber == AccountNumber)
		{
			return (true);
		}
	}
	return (false);
}

void AddNewClientToFile(string FileName, string AccountNumber, vector <sClient> &vClientsData)
{
	sClient ClientData = AddNewClientData(AccountNumber);
	string ClientLineRecord = ConvertClientDataToRecordLine(ClientData);
	SaveClientLineRecordToFile(FileName, ClientLineRecord);
	//Refresh vector of clients
	vClientsData = ConvertAllClientsLineRecordsToData(FileName);
}

void AddNewClientScreenTemplate()
{
	cout << "-------------------------------------------\n";
	cout << "	Add New Clients Screen	";
	cout << "\n-------------------------------------------\n";
	cout << "Adding New Client:\n";
}

void AddMoreClients(char Answer, string FileName, vector <sClient>& vClientsData)
{

	while (toupper(Answer) == 'Y')
	{
		system("cls");
		AddNewClientScreenTemplate();
		string AccountNumber = ReadClientAccountNumber();

		while (CheckClient(AccountNumber, vClientsData) == true)
		{
			cout << "\nClient with Account Number [" << AccountNumber << "] already exists, Entre another Account Number ? ";
			cin >> AccountNumber;
			if (AccountNumber == "exit")
				exit(1);
		}
		AddNewClientToFile(FileName, AccountNumber, vClientsData);

		cout << "Client added succsseflly\ndo you want to add more clients ? Yes = y | No = n ";
		cin >> Answer;
	}
	cout << endl;
}

void DeleteClientScreenTemplate()
{
	cout << "-------------------------------------------\n";
	cout << "	Delete Clients Screen	";
	cout << "\n-------------------------------------------\n";
	cout << endl;
}

void PrintClientData(sClient ClientData)
{
	cout << "------------------------------------------\n";
	cout << "\nAccount Number   : " << setw(15) << left << ClientData.AccountNumber;
	cout << "\nPin Code         : " << setw(10) << left << ClientData.PinCode;
	cout << "\nFull Name        : " << setw(40) << left << ClientData.FullName;
	cout << "\nPhone Number     : " << setw(12) << left << ClientData.PhoneNumber;
	cout << "\nAccount Balanace : " << setw(12) << left << ClientData.AccountBalance;
	cout << "\n------------------------------------------\n";
	cout << endl;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClientsData)
{
	for (sClient& ClientData : vClientsData)
	{
		if (ClientData.AccountNumber == AccountNumber)
		{
			ClientData.MarkForDelete = true;
			return (true);
		}
	}
	return (false);
}

void DeleteClientFromFile(string FileName, string AccountNumber, vector <sClient>& vClientsData)
{
	char Answer;
	for (sClient& ClientData : vClientsData)
	{
		if (ClientData.AccountNumber == AccountNumber)
		{
			PrintClientData(ClientData);

			cout << "\nAre you sure you want to delete this client ? Yes = y | No = n ";
			cin >> Answer;
			if (Answer == 'Y' || Answer == 'y')
			{
				MarkClientForDeleteByAccountNumber(AccountNumber, vClientsData);
				SaveClientLineRecordToFileAfterUpdate(FileName, vClientsData);
				//Refresh vector of clients
				vClientsData = ConvertAllClientsLineRecordsToData(FileName);
			}
			break;
		}
	}
}

void DeleteMoreClientsFromFile(string FileName, string AccountNumber, vector <sClient> vClientsData, char Answer)
{
	while(Answer == 'Y' || Answer == 'y')
	{
		system("cls");
		DeleteClientScreenTemplate();
		string AccountNumber = ReadClientAccountNumber();
		if (AccountNumber == "exit")
			exit(1);

		while (CheckClient(AccountNumber, vClientsData) == false)
		{
			cout << "Client with Account Number [" << AccountNumber << "] Not Found!, Entre another Account Number ? ";
			cin >> AccountNumber;
			if (AccountNumber == "exit")
				exit(1);
		}
		DeleteClientFromFile(FileName, AccountNumber, vClientsData);

		cout << "Client deleted succsseflly, Do you want to delete more clients ? Yes = y | No = n ";
		cin >> Answer;
	}

	cout << endl;
}

void UpdateClientScreenTemplate()
{
	cout << "-------------------------------------------\n";
	cout << "	Update Client Info Screen	";
	cout << "\n-------------------------------------------\n";
	cout << endl;
}

sClient UpdateClientDataInfo(string AccountNumber, sClient& ClientData)
{
	sClient Client;
	Client = AddNewClientData(AccountNumber);

	return (Client);
}

void UpdateClientInfo(string FileName, string AccountNumber, vector <sClient>& vClientsData)
{
	string ClientLineRecord;
	char Answer;
	for (sClient& Client : vClientsData)
	{
		if (Client.AccountNumber == AccountNumber)
		{
			PrintClientData(Client);

			cout << "Are you sure you want to update this client informations ? Yes = y | No = n ";
			cin >> Answer;
			if (Answer == 'Y' || Answer == 'y')
			{
				MarkClientForDeleteByAccountNumber(AccountNumber, vClientsData);
				Client = UpdateClientDataInfo(AccountNumber, Client);
				ClientLineRecord = ConvertClientDataToRecordLine(Client);
				SaveClientLineRecordToFileAfterUpdate(FileName, vClientsData);
				//Refresh vector of clients
				vClientsData = ConvertAllClientsLineRecordsToData(FileName);
			}
			break;
		}
	}
}

void UpdateMoreClientInfo(string FileName, string AccountNumber, vector <sClient> vClientsData, char Answer)
{
	while (toupper(Answer) == 'Y')
	{
		system("cls");
		UpdateClientScreenTemplate();

		if (AccountNumber == "exit")
			exit(1);

		while (CheckClient(AccountNumber, vClientsData) == false)
		{
			cout << "Client with Account Number [" << AccountNumber << "] Not Found !, Entre another Account Number ? ";
			cin >> AccountNumber;
			if (AccountNumber == "exit")
				exit(1);
		}
		UpdateClientInfo(FileName, AccountNumber, vClientsData);
		cout << "\nClient updated succsseflly, Do you want to update more client ? Yes = y | No = n ";
		cin >> Answer;
	}

	cout << endl;
}

void FindClientScreenTemplate()
{
	cout << "-------------------------------------------\n";
	cout << "	Find Client Screen	";
	cout << "\n-------------------------------------------\n";
	cout << endl;
}

void FindClientByAccountNumber(string AccountNumber, vector <sClient> vClientsData)
{
	for (sClient& Client : vClientsData)
	{
		if (Client.AccountNumber == AccountNumber)
		{
			PrintClientData(Client);
			break;
		}
	}
}

void FindMoreClientByAccountNumber(vector <sClient> vClientsData, char Answer)
{
	while (Answer == 'Y' || Answer == 'y')
	{
		system("cls");
		FindClientScreenTemplate();
		string AccountNumber = ReadClientAccountNumber();
		if (AccountNumber == "exit")
			exit(1);

		while (CheckClient(AccountNumber, vClientsData) == false)
		{
			cout << "Client with Account Number [" << AccountNumber << "] Not Found !, Entre another Account Number ? ";
			cin >> AccountNumber;
			if (AccountNumber == "exit")
				exit(1);
		}

		FindClientByAccountNumber(AccountNumber, vClientsData);
		cout << "Do you want to find more clients ? Yes = y | No = n ";
		cin >> Answer;
	}

	cout << endl;
}





void ShowClientListScreen(vector <sClient> &vClientsData)
{
	cout << "\n\t\t\t\t\t Client List (" << vClientsData.size() << ") Client(s).\t\t\t\t\t\n";
	cout << "--------------------------------------------------";
	cout << "--------------------------------------------------\n\n";
	cout << "| " << setw(15) << left << "Account Number ";
	cout << "| " << setw(10) << left << "Pin Code ";
	cout << "| " << setw(40) << left << "Full Name ";
	cout << "| " << setw(12) << left << "Phone ";
	cout << "| " << setw(12) << left << "Balanace";
	cout << "\n\n--------------------------------------------------";
	cout << "--------------------------------------------------\n\n";

	for (sClient& Client : vClientsData)
	{
		PrintClientDataInShowClientList(Client);
		cout << endl;
	}
	cout << "\n--------------------------------------------------";
	cout << "--------------------------------------------------\n\n";
}


void AddNewClientScreen(string FileName, vector <sClient> &vClientsData)
{
	AddNewClientScreenTemplate();
	string AccountNumber = ReadClientAccountNumber();
	if (AccountNumber == "exit")
		exit(1);

	while (CheckClient(AccountNumber, vClientsData) == true)
	{
		cout << "Client with Account Number [" << AccountNumber << "] already exists, Entre another Account Number ? ";
		cin >> AccountNumber;
		if (AccountNumber == "exit")
			exit(1);
	}
	AddNewClientToFile(FileName, AccountNumber, vClientsData);

	char Answer;

	cout << "\nClient added succsseflly\ndo you want to add more clients ? Yes = y | No = n ";
	cin >> Answer;
	AddMoreClients(Answer, FileName, vClientsData);

	cout << endl;
}


void DeleteClientScreen(string FileName, vector <sClient> &vClientsData)
{
	DeleteClientScreenTemplate();
	string AccountNumber = ReadClientAccountNumber();
	char Answer;

	if (AccountNumber == "exit")
		exit(1);
	while (CheckClient(AccountNumber, vClientsData) == false)
	{
		cout << "Client with Account Number [" << AccountNumber << "] Not Found!, Entre another Account Number ? ";
		cin >> AccountNumber;
		if (AccountNumber == "exit")
			exit(1);
	}

	DeleteClientFromFile(FileName, AccountNumber, vClientsData);
	cout << "Client deleted succsseflly, Do you want to delete more clients ? Yes = Y | No = n ";
	cin >> Answer;
	DeleteMoreClientsFromFile(FileName, AccountNumber, vClientsData, Answer);
}


void UpdateClientInfoScreen(string FileName, vector <sClient> &vClientsData)
{
	UpdateClientScreenTemplate();

	string AccountNumber = ReadClientAccountNumber();
	char Answer;

	if (AccountNumber == "exit")
		exit(1);

	while (CheckClient(AccountNumber, vClientsData) == false)
	{
		cout << "Client with Account Number [" << AccountNumber << "] Not Found !, Entre another Account Number ? ";
		cin >> AccountNumber;

		if (AccountNumber == "exit")
			exit(1);
	}
	UpdateClientInfo(FileName, AccountNumber, vClientsData);
	cout << "\nClient updated succsseflly, Do you want to update more client ? Yes = y | No = n ";
	cin >> Answer;
	UpdateMoreClientInfo(FileName, AccountNumber, vClientsData, Answer);
}


void FindClientScreen(vector <sClient> &vClientsData)
{
	char Answer;

	FindClientScreenTemplate();
	string AccountNumber = ReadClientAccountNumber();

	if (AccountNumber == "exit")
		exit(1);

	while (CheckClient(AccountNumber, vClientsData) == false)
	{
		cout << "Client with Account Number [" << AccountNumber << "] Not Found !, Entre another Account Number ? ";
		cin >> AccountNumber;

		if (AccountNumber == "exit")
			exit(1);
	}
	FindClientByAccountNumber(AccountNumber, vClientsData);
	cout << "Do you want to find more clients ? Yes = y | No = n ";
	cin >> Answer;
	FindMoreClientByAccountNumber(vClientsData, Answer);
}


//Transactions Screen

void DepositScreenTemplate()
{
	cout << "-------------------------------------------\n";
	cout << "	Deposit Screen	";
	cout << "\n-------------------------------------------\n";
	cout << endl;
}

void PrintClientDetails(string AccountNumber, vector <sClient> vClientsData)
{
	for (sClient& Client : vClientsData)
	{
		if (Client.AccountNumber == AccountNumber)
		{
			cout << "The following are client details :\n";
			PrintClientData(Client);
			break;
		}
	}
}

void DepositTransaction(long int DepositAmount, string AccountNumber, vector <sClient> &vClientsData)
{
	for (sClient& ClientData : vClientsData)
	{
		if (ClientData.AccountNumber == AccountNumber)
		{
			ClientData.AccountBalance = ClientData.AccountBalance + DepositAmount;
			cout << endl;
			cout << "New Balance is : " << ClientData.AccountBalance;
			break;
		}
	}
}

void DepositToClientAccountBalance(string FileName, string AccountNumber, vector <sClient>& vClientsData)
{
	long int DepositAmount;
	char Answer;

	cout << "\nPlease entre deposit amount ? ";
	cin >> DepositAmount;
	while (DepositAmount < 0)
	{
		cout << "\nPlease entre a positive deposit amount !\n";
		cin >> DepositAmount;
	}

	cout << "Are you sure you want to perform this transaction ? Yes = y | No = n ? ";
	cin >> Answer;
	if (Answer == 'Y' || Answer == 'y')
	{
		DepositTransaction(DepositAmount, AccountNumber, vClientsData);
	}
	SaveClientLineRecordToFileAfterUpdate(FileName, vClientsData);
	//Refresh vector of clients
	vClientsData = ConvertAllClientsLineRecordsToData(FileName);
}

void WithdrawScreenTemplate()
{
	cout << "-------------------------------------------\n";
	cout << "	Withdraw Screen	";
	cout << "\n-------------------------------------------\n";
	cout << endl;
}

void WithdrawTransaction(long int WithdrawAmount, string AccountNumber, vector <sClient>& vClientsData)
{
	double NewBalance = 0;
	for (sClient& ClientData : vClientsData)
	{
		if (ClientData.AccountNumber == AccountNumber)
		{
			while (WithdrawAmount > ClientData.AccountBalance)
			{
				cout << "Amount Exceeds the balance, you can withdraw up to " << ClientData.AccountBalance;
				cout << "\n\nPlease entre another amount ? ";
				cin >> WithdrawAmount;
			}
			DepositTransaction(WithdrawAmount * -1, AccountNumber, vClientsData);
			break;
		}
	}
}

void WithdrawFromClientBalance(string FileName, string AccountNumber, vector <sClient>& vClientsData)
{
	long int WithdrawAmount;
	char Answer;

	cout << "\nPlease entre withdraw amount ? ";
	cin >> WithdrawAmount;

	cout << "Are you sure you want to perform this transaction ? Yes = y | No = n ? ";
	cin >> Answer;
	if (Answer == 'Y' || Answer == 'y')
		WithdrawTransaction(WithdrawAmount, AccountNumber, vClientsData);

	SaveClientLineRecordToFileAfterUpdate(FileName, vClientsData);
	//Refresh vector of clients
	vClientsData = ConvertAllClientsLineRecordsToData(FileName);
}

void PrintClientDataInBalancesList(sClient ClientData)
{
	cout << "| " << setw(15) << left << ClientData.AccountNumber;
	cout << "| " << setw(40) << left << ClientData.FullName;
	cout << "| " << setw(12) << left << ClientData.AccountBalance;
}


void DepositScreen(string FileName, vector <sClient> &vClientsData)
{
	DepositScreenTemplate();

	string AccountNumber = ReadClientAccountNumber();
	sClient ClientData;

	if (AccountNumber == "exit")
		exit(1);

	while (CheckClient(AccountNumber, vClientsData) == false)
	{
		cout << "Client with Account Number [" << AccountNumber << "] Not Found !";
		AccountNumber = ReadClientAccountNumber();
		if (AccountNumber == "exit")
			exit(1);
	}

	PrintClientDetails(AccountNumber, vClientsData);
	DepositToClientAccountBalance(FileName, AccountNumber, vClientsData);
}


void WithdrawScreen(string FileName, vector <sClient>& vClientsData)
{
	WithdrawScreenTemplate();

	string AccountNumber = ReadClientAccountNumber();

	if (AccountNumber == "exit")
		exit(1);

	while (CheckClient(AccountNumber, vClientsData) == false)
	{
		cout << "Client with Account Number [" << AccountNumber << "] Not Found !";
		AccountNumber = ReadClientAccountNumber();
		if (AccountNumber == "exit")
			exit(1);
	}

	PrintClientDetails(AccountNumber, vClientsData);
	WithdrawFromClientBalance(FileName, AccountNumber, vClientsData);

}


void TotalBalancesScreen(vector <sClient>& vClientsData)
{
	double TotalBalances = 0;

	cout << "\n\t\t\t\t\t Blanaces List (" << vClientsData.size() << ") Client(s).\t\t\t\t\t\n";
	cout << "--------------------------------------------------";
	cout << "--------------------------------------------------\n\n";
	cout << "| " << setw(15) << left << "Account Number ";
	cout << "| " << setw(40) << left << "Full Name ";
	cout << "| " << setw(12) << left << "Balanace";
	cout << "\n\n--------------------------------------------------";
	cout << "--------------------------------------------------\n\n";

	for (sClient& ClientData : vClientsData)
	{
		PrintClientDataInBalancesList(ClientData);
		TotalBalances += ClientData.AccountBalance;
		cout << endl;
	}
	cout << "\n--------------------------------------------------";
	cout << "--------------------------------------------------\n\n";
	cout << "			Total Balances : " << TotalBalances;
}


void MainMenuScreen()
{
	cout << "================================================\n";
	cout << "		Main Menu Screen				 \n";
	cout << "================================================\n";

	cout << "	[1] Show Client List.\n";
	cout << "	[2] Add New Client.\n";
	cout << "	[3] Delete Client.\n";
	cout << "	[4] Update Client Info.\n";
	cout << "	[5] Find Client.\n";
	cout << "	[6] Transatcions.\n";
	cout << "	[7] Exit.\n";
	cout << "================================================\n";
	cout << endl;
}

void TransactionsMenuScreen()
{
	cout << "================================================\n";
	cout << "		Transactions Menu Screen				 \n";
	cout << "================================================\n";

	cout << "	[1] Deposit.\n";
	cout << "	[2] Withdraw.\n";
	cout << "	[3] Total Balances.\n";
	cout << "	[4] Main Menu.\n";
	cout << "================================================\n";
	cout << endl;
}

enTransactionMenuChoises ReadUserChooseFromTrasactiosMenu()
{
	short TransactionMenuNumber;
	cout << "Choose what do you want to do ? [1 to 6] ";
	cin >> TransactionMenuNumber;

	return (enTransactionMenuChoises)TransactionMenuNumber;
}

enMainMenuChoises ReadUserChooseFromMainMenu()
{
	short MainMenuNumber;
	cout << "Choose what do you want to do ? [1 to 6] ";
	cin >> MainMenuNumber;

	return (enMainMenuChoises)MainMenuNumber;
}


void PresseAnyKeyToBack()
{
	cout << "\n\nPress any key to go back to Main Menu...";
	system("pause>0");
	system("cls");
}

void PresseAnyKeyToBackToTransactionMenu()
{
	cout << "\n\nPress any key to go back to Transaction Menu...";
	system("pause>0");
	system("cls");
}

void ShowEndScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tProgram Ends :-)";
	cout << "\n-----------------------------------\n";
}

void StartTransactions()
{
	vector <sClient> vClientsData = ConvertAllClientsLineRecordsToData(ClientsFileName);

	TransactionsMenuScreen();
	enTransactionMenuChoises UserTransactiosMenuChoise = ReadUserChooseFromTrasactiosMenu();

	switch (UserTransactiosMenuChoise)
	{
	case(enTransactionMenuChoises::Deposite):
		system("cls");
		DepositScreen(ClientsFileName, vClientsData);
		PresseAnyKeyToBackToTransactionMenu();
		StartTransactions();
		break;
	case(enTransactionMenuChoises::Withdraw):
		system("cls");
		WithdrawScreen(ClientsFileName, vClientsData);
		PresseAnyKeyToBackToTransactionMenu();
		StartTransactions();
		break;
	case(enTransactionMenuChoises::TotalBalances):
		system("cls");
		TotalBalancesScreen(vClientsData);
		PresseAnyKeyToBackToTransactionMenu();
		StartTransactions();
		break;
	case(enTransactionMenuChoises::MainMenu):
		system("cls");
		StartBank();
		break;
	}	
}

void StartBank()
{
	vector <sClient> vClients = ConvertAllClientsLineRecordsToData(ClientsFileName);

	MainMenuScreen();
	enMainMenuChoises UserMainMenuChoise = ReadUserChooseFromMainMenu();;

	switch (UserMainMenuChoise)
	{
	case(enMainMenuChoises::ShowClientList):
		system("cls");
		ShowClientListScreen(vClients);
		PresseAnyKeyToBack();
		StartBank();
		break;
	case(enMainMenuChoises::AddNewClient):
		system("cls");
		AddNewClientScreen(ClientsFileName, vClients);
		PresseAnyKeyToBack();
		StartBank();
		break;
	case(enMainMenuChoises::DeleteClient):
		system("cls");
		DeleteClientScreen(ClientsFileName, vClients);
		PresseAnyKeyToBack();
		StartBank();
		break;
	case(enMainMenuChoises::UpdateClientInformations):
		system("cls");
		UpdateClientInfoScreen(ClientsFileName, vClients);
		PresseAnyKeyToBack();
		StartBank();
		break;
	case(enMainMenuChoises::FindClient):
		system("cls");
		FindClientScreen(vClients);
		PresseAnyKeyToBack();
		StartBank();
		break;
	case(enMainMenuChoises::Transactions):
		system("cls");
		StartTransactions();
		break;
	case(enMainMenuChoises::Exit):
		system("cls");
		ShowEndScreen();
		break;
	}
}



int main()
{
	StartBank();

	return 0;
}