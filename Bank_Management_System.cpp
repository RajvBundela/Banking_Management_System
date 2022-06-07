#include <iostream>
#include <fstream>
#include <ctype.h>
#include <iomanip>
#include <cstdlib>
#include <stdio.h>
#include <string.h>

using namespace std;

class Date{
    public:

    int day;
	int month;
	int year;

	Date operator+ (int m){
		Date res;
        int x = m/12+year;
        int y = m%12+ month;
		if (y>12)
		{
			y = y-12;
			x++;
		}
        if ((y==4 or y==6 or y==9 or y==11) and day==31)
        {
            day = 1;
            y++;
        }
        else if (y==2 )
        {
            if (x%4==0 and day>29)
            {
                day = day-29;
                y++;
            }
            else if (day>28)
            {
                day = day-28;
                y++;
            }
            
        }
        res.day = day;
        res.month = y;
        res.year = x;
        
		return res;
	}
};

class Account
{
	int AccNo;
	char Name[50];
	int dep;
	char accType;

public:
	void createAcc();
	void displayAcc();
	void update();
	void credit(int);
	void debit(int);
	void query();
	int getAccNo();
	int getCredit();
	char getType();
};

class recurrDep : public Account
{
	Date d;
	int per;

	public:

	void getrecurr(){
		Account::createAcc();
		cout<<"Enter date of FD in dd mm yyyy format : ";
		cin>>d.day>>d.month>>d.year;
		cout<<"Enter period of maturity in months : ";
		cin>>per;
	}

	void showDet(){
		Account::displayAcc();
		cout<<"\nDate of FD : "<<d.day<<" : "<<d.month<<" : "<<d.year<<endl;
		cout<<"Period of maturity : "<<per<<endl; 
		maturity();
	}

	void maturity(){
		Date res = d+per;
		
		cout<<"Date of maturity : "<<res.day<<" : "<<res.month<<" : "<<res.year<<endl;
	}

};

void Account::createAcc()
{
	cout << endl
		 << "Enter your Account Number : ";
	cin >> AccNo;

	cout << endl
		 << "Enter the Account holder's name : ";
	fflush(stdin);
	cin >> Name;
	fflush(stdin);

	cout << endl
		 << "Enter your Account accType, C for Current Account/ S for Savings Account/R for Fixed Deposit : ";
	cin >> accType;

	cout << endl
		 << "Enter initial amount (for Saving and Fixed Deposit >=500Rs. and for Current >=1000 ) : ";
	cin >> dep;

	cout << endl
		 << endl
		 << "                       \t\t\t\t\t      Account Created Successfully! \n\n     \t\t\t\t\t                   ";
}

void Account::displayAcc()
{
	cout << endl
		 << "Account No. is"
		 << "\t: " << AccNo;
	cout << endl
		 << "Account Holder's name is"
		 << ": " << Name;
	cout << endl
		 << "\nType of Account"
		 << "\t: " << accType << endl;
	cout << "Balance amount is"
		 << "\t: " << dep;
}

void Account::update()
{
	cout << endl
		 << "The Account No. is"
		 << "\t: " << AccNo;
	cout << endl
		 << endl
		 << "Enter account holder's name"
		 << "\t: ";
	fflush(stdin);
	cin >> Name;
	fflush(stdin);
	cout << endl
		 << "Enter your Account accType, C for Current Account/ S for Savings Account :"
		 << "\t: ";
	cin >> accType;
	accType = toupper(accType);
	cout << endl
		 << "Enter the amount to be credited"
		 << "\t: ";
	cin >> dep;
}

void Account::credit(int x)
{
	dep += x;
}

void Account::debit(int x)
{
	if (dep >= x)
		dep -= x;
	else
	{
		cout << "Not enought balance!!" << endl;
		return;
	}
}

void Account::query()
{
	cout << AccNo << "\t\t" << Name;
	int length = strlen(Name);
	for (int count = 0; count < 32 - length; count++)
		cout << ' ';

	cout << accType;

	for (int count = 0; count < 20; count++)
		cout << ' ';

	cout << dep << endl;
}

int Account::getAccNo()
{
	return AccNo;
}

int Account::getCredit()
{
	return dep;
}

char Account::getType()
{
	return accType;
}

void writeAcc()
{
	Account Acc;
	ofstream outFile;
	outFile.open("Account.dat", ios::binary | ios::app);
	Acc.createAcc();
	outFile.write((char *)&Acc, sizeof(Account));
	outFile.close();
}

void displayAccDetails(int n)
{
	Account Acc;
	int key = 0;
	ifstream inFile;
	inFile.open("Account.dat", ios::binary);
	if (!inFile)
	{
		cout << "Unable to open the file..." << endl;
		return;
	}
	cout << endl
		 << "Details of balance are : " << endl;
	while (inFile.read((char *)&Acc, sizeof(Account)))
	{
		if (Acc.getAccNo() == n)
		{
			Acc.displayAcc();
			key = 1;
		}
	}
	inFile.close();
	if (key == 0)
		cout << endl
			 << "Invalid Account Number!";
}

void updateAcc(int n)
{
	int key = 0;
	Account Acc;
	fstream File;
	File.open("Account.dat", ios::binary | ios::in | ios::out);
	if (!File)
	{
		cout << "Unable to open the file..." << endl;
		return;
	}
	while (File.read((char *)&Acc, sizeof(Account)) && key == 0)
	{
		if (Acc.getAccNo() == n)
		{
			Acc.displayAcc();
			cout << endl
				 << "Enter new Account Details" << endl;
			Acc.update();
			int location = (-1) * sizeof(Account);
			File.seekp(location, ios::cur);
			File.write((char *)&Acc, sizeof(Account));
			system("cls");
			cout << "\n\n                   \t\t\t\t\t      Record Updated Successfully\n\n     \t\t\t\t\t                  ";
			key = 1;
		}
	}
	File.close();
	if (key == 0)
		cout << endl
			 << "Record was not found!";
}

void deleteAcc(int n)
{
	Account Acc;
	ifstream inFile;
	ofstream outFile;
	inFile.open("Account.dat", ios::binary);
	if (!inFile)
	{
		cout << "Unable to open the file..." << endl;
		return;
	}
	outFile.open("Temp.dat", ios::binary);
	inFile.seekg(0, ios::beg);
	while (inFile.read((char *)&Acc, sizeof(Account)))
	{
		if (Acc.getAccNo() != n)
			outFile.write((char *)&Acc, sizeof(Account));
	}
	inFile.close();
	outFile.close();
	remove("Account.dat");
	rename("Temp.dat", "Account.dat");
	cout << "\n\n                   \t\t\t\t\t      Account Deleted Successfully\n\n     \t\t\t\t\t                      ";
}

void showAll()
{
	Account Acc;
	ifstream inFile;
	inFile.open("Account.dat", ios::binary);
	if (!inFile)
	{
		cout << "Unable to open the file..." << endl;
		return;
	}
	cout << "\n\n\t\tList of Active Account Holders : " << endl
		 << endl;

	cout << "Acc No.\t\tNAME\t\t\t\tType\t\t     Balance" << endl
		 << endl;

	while (inFile.read((char *)&Acc, sizeof(Account)))
		Acc.query();

	inFile.close();
}

void debt(int n, int amount)
{
	int key = 0;
	Account Acc;
	fstream File;
	File.open("Account.dat", ios::binary | ios::in | ios::out);
	if (!File)
	{
		cout << "\n\n                   \t\t\t\t\t      Unable to open the file..." << endl;
		return;
	}
	while (File.read((char *)&Acc, sizeof(Account)) && key == 0)
	{
		if (Acc.getAccNo() == n)
		{
			Acc.displayAcc();
			 
				int balance = Acc.getCredit() - amount;
				if ((balance < 500 && Acc.getType() == 'S') || (balance < 1000 && Acc.getType() == 'C'))
					cout << "Balance not sufficient";
				else
					Acc.debit(amount);
		 
			int location = (-1) * sizeof(Acc);
			File.seekp(location, ios::cur);
			File.write((char *)&Acc, sizeof(Account));
			cout << "\n\n                   \t\t\t\t\t      Record Updated Successfully\n\n     \t\t\t\t\t                   ";
			key = 1;
		}
	}
	File.close();
	if (key == 0)
		cout << "\n\n Record Was Not Found! ";
}

void cred(int recieverAcc, int amount)
{

	int key = 0;
	Account Acc;
	fstream File;

	File.open("Account.dat", ios::binary | ios::in | ios::out);
	if (!File)
	{
		cout << "\n\n                   \t\t\t\t\t      Unable to open the file..." << endl;
		return;
	}
	while (File.read((char *)&Acc, sizeof(Account)) && key == 0){
		if (Acc.getAccNo() == recieverAcc )
		{
			Acc.displayAcc();
			Acc.credit(amount);
			int location = (-1) * sizeof(Acc);
			File.seekp(location, ios::cur);
			File.write((char *)&Acc, sizeof(Account));
			cout << "\n\n                   \t\t\t\t\t      Records Updated Successfully\n\n     \t\t\t\t\t                     ";
			key = 1;
		}
	}
	File.close();
	if (key == 0)
		cout << "\n\n Record Was Not Found! ";
}

void transfer(int senderAcc,int recieverAcc, int amount){

	debt(senderAcc, amount);
	cred(recieverAcc,amount );

}

void writeRecurrAcc(){
	recurrDep Acc;
	ofstream outFile;
	outFile.open("rdAccount.dat", ios::binary | ios::app);
	Acc.getrecurr();
	outFile.write((char *)&Acc, sizeof(recurrDep));
	outFile.close();

}


void displayRecurrAccDetails(int n){
	recurrDep Acc;
	int key = 0;
	ifstream inFile;
	inFile.open("rdAccount.dat", ios::binary);
	if (!inFile)
	{
		cout << "Unable to open the file..." << endl;
		return;
	}
	cout << endl
		 << "Details of balance are : " << endl;
	while (inFile.read((char *)&Acc, sizeof(recurrDep)))
	{
		if (Acc.getAccNo() == n)
		{
			Acc.showDet();
			key = 1;
		}
	}
	inFile.close();
	if (key == 0)
		cout << endl
			 << "Invalid Account Number!";
}



int main()
{
	int c;
	int count = 0;
	int acn;
	int racn;
	int amt;
	do
	{
		if (count == 0)
		{
			system("clear");
			count++;
		}

		cout << "\n\n\t\t\t\t  ******Bank Management System******";
		cout << "\n\t\t\t\t======================================";
		cout << "\n\n\t\t\t\t  ******ABV-IIITM Bank******";
		cout << "\n\t\t\t\t==========================================";
		cout << "\n\n\t\t\t\t\t**MAIN MENU**";
		cout << "\n\n\t\t\t\t\t1. NEW ACCOUNT";
		cout << "\n\n\t\t\t\t\t2. CREDIT AMOUNT";
		cout << "\n\n\t\t\t\t\t3. DEBIT AMOUNT";
		cout << "\n\n\t\t\t\t\t4. ACCOUNT DETAILS";
		cout << "\n\n\t\t\t\t\t5. ALL ACCOUNT HOLDER'S LIST";
		cout << "\n\n\t\t\t\t\t6. CLOSE AN ACCOUNT";
		cout << "\n\n\t\t\t\t\t7. UPDATE AN ACCOUNT";
		cout << "\n\n\t\t\t\t\t8. TRANSFER FUND TO ANOTHER ACCOUNT";
		cout << "\n\n\t\t\t\t\t9. NEW FIXED DEPOSIT";
		cout << "\n\n\t\t\t\t\t10. DETAILS OF FIXED DEPOSIT";
		cout << "\n\n\t\t\t\t\t11. EXIT";
		cout << "\n\n\t\t\t\t==>>Enter Your Choice: ";
		cin >> c;

		switch (c)
		{
		case 1:
			writeAcc();
			break;
		case 2:
			cout << "\n\n\t\t\t\tEnter The Account No. : ";
			cin >> acn;
			cout << "\n\n\t\t\t\tEnter The Amount To Transfer: ";
			cin >>amt ;
			cred(acn, amt);
			break;
		case 3:
			cout << "\n\n\t\t\t\tEnter The Account No. : ";
			cin >> acn;
			cout << "\n\n\t\t\t\tEnter The Amount To Withdraw: ";
			cin >>amt ;
			debt(acn, amt);
			break;
		case 4:
			cout << "\n\n\t\t\t\tEnter The Account No. : ";
			cin >> acn;
			displayAccDetails(acn);
			break;
		case 5:
			showAll();
			break;
		case 6:
			cout << "\n\n\t\t\t\tEnter The Account No. : ";
			cin >> acn;
			deleteAcc(acn);
			break;
		case 7:
			cout << "\n\n\t\t\t\tEnter The Account No. : ";
			cin >> acn;
			updateAcc(acn);
			break;
		case 8:
		     
			cout << "\n\n\t\t\t\tEnter The Account No. Of The Sender: ";
			cin >>acn ;
			cout << "\n\n\t\t\t\tEnter The Account No. Of The Reciever: ";
			cin >>racn ;
			cout << "\n\n\t\t\t\tEnter The Amount To Transfer: ";
			cin >>amt ;
			transfer(acn,racn,amt);
			break;
		case 9:
			writeRecurrAcc();
			break;
		case 10:
			cout << "\n\n\t\t\t\tEnter The RD Account No. : ";
			cin >> acn;
			displayRecurrAccDetails(acn);
			break;
		case 11:
			cout << "\n\n       \t\t\t\t\t      Thank You For Using IIITM Banking System" << endl;
			exit(0);

		default:
			cout << "Invalid Choice";
		}
	} while (c != 11);
	return 0;
}
