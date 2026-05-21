#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

// User Account Class (Demonstrating Object-Oriented Principles)
class BankAccount {
private:
    int accountNumber;
    string accountHolderName;
    double balance;

public:
    // Constructor to initialize default values
    BankAccount() {
        accountNumber = 0;
        accountHolderName = "";
        balance = 0.0;
    }

    // Function to create a new account layout
    void createAccount() {
        cout << "\n=====================================";
        cout << "\n    ENTER NEW ACCOUNT DETAILS       ";
        cout << "\n=====================================";
        cout << "\nEnter Account Number: ";
        cin >> accountNumber;
        cin.ignore(); // Clears input buffer before taking string input
        cout << "Enter Account Holder Name: ";
        getline(cin, accountHolderName);
        cout << "Enter Initial Deposit Amount (Min ?500): ";
        cin >> balance;
        while (balance < 500) {
            cout << "Minimum initial deposit must be ?500. Try again: ";
            cin >> balance;
        }
        cout << "\nAccount Created Successfully!\n";
    }

    // Function to display account overview
    void displayAccount() const {
        cout << "\n-------------------------------------";
        cout << "\nAccount Number : " << accountNumber;
        cout << "\nAccount Holder : " << accountHolderName;
        cout << "\nCurrent Balance: ?" << fixed << setprecision(2) << balance;
        cout << "\n-------------------------------------\n";
    }

    // Core Banking Operation: Deposit
    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "\n?" << amount << " Deposited Successfully!";
        } else {
            cout << "\nInvalid Amount! Transaction Aborted.";
        }
    }

    // Core Banking Operation: Withdrawal
    bool withdraw(double amount) {
        if (amount <= 0) {
            cout << "\nInvalid Amount! Transaction Aborted.";
            return false;
        }
        if (amount > balance) {
            cout << "\nInsufficient Balance! Transaction Failed.";
            return false;
        }
        balance -= amount;
        cout << "\n?" << amount << " Withdrawn Successfully!";
        return true;
    }

    // Encapsulation Getters and Setters
    int getAccountNumber() const { return accountNumber; }
    string getAccountHolderName() const { return accountHolderName; }
    double getBalance() const { return balance; }
    
    void setBalance(double newBalance) { balance = newBalance; }
    void setAccountNumber(int num) { accountNumber = num; }
    void setAccountHolderName(string name) { accountHolderName = name; }
};

// File Management & Operations Helper Functions
void openNewAccount();
void displayAllRecords();
void checkBalanceInquiry(int targetAccNum);
void handleTransaction(int targetAccNum, int transactionType); // 1 for Deposit, 2 for Withdraw

int main() {
    int choice;
    int accNum;

    do {
        cout << "\n=====================================";
        cout << "\n     BANK MANAGEMENT APPLICATION     ";
        cout << "\n=====================================";
        cout << "\n1. OPEN NEW ACCOUNT";
        cout << "\n2. DEPOSIT MONEY";
        cout << "\n3. WITHDRAW MONEY";
        cout << "\n4. BALANCE INQUIRY";
        cout << "\n5. DISPLAY ALL ACCOUNTS LIST";
        cout << "\n6. EXIT";
        cout << "\n=====================================";
        cout << "\nSelect Your Option (1-6): ";
        cin >> choice;

        switch (choice) {
            case 1:
                openNewAccount();
                break;
            case 2:
                cout << "\nEnter Account Number: ";
                cin >> accNum;
                handleTransaction(accNum, 1);
                break;
            case 3:
                cout << "\nEnter Account Number: ";
                cin >> accNum;
                handleTransaction(accNum, 2);
                break;
            case 4:
                cout << "\nEnter Account Number: ";
                cin >> accNum;
                checkBalanceInquiry(accNum);
                break;
            case 5:
                displayAllRecords();
                break;
            case 6:
                cout << "\nThank you for using our Banking System!\n";
                break;
            default:
                cout << "\nInvalid Choice! Please select between 1-6.";
        }
    } while (choice != 6);

    return 0;
}

// Write account data securely to a text file
void openNewAccount() {
    BankAccount acc;
    acc.createAccount();

    ofstream outFile("bank_records.txt", ios::app);
    if (!outFile) {
        cout << "\nDatabase Error: File could not be opened!";
        return;
    }

    // Storing data sequentially separated by spaces/newlines
    outFile << acc.getAccountNumber() << endl;
    outFile << acc.getAccountHolderName() << endl;
    outFile << acc.getBalance() << endl;
    outFile.close();
}

// Inquire balance of a specific customer account number
void checkBalanceInquiry(int targetAccNum) {
    ifstream inFile("bank_records.txt");
    if (!inFile) {
        cout << "\nNo records found! Open an account first.";
        return;
    }

    int accNum;
    string name;
    double bal;
    bool found = false;

    while (inFile >> accNum) {
        inFile.ignore(); // Clean leftover newline character
        getline(inFile, name);
        inFile >> bal;

        if (accNum == targetAccNum) {
            BankAccount temp;
            temp.setAccountNumber(accNum);
            temp.setAccountHolderName(name);
            temp.setBalance(bal);
            temp.displayAccount();
            found = true;
            break;
        }
    }
    inFile.close();

    if (!found) {
        cout << "\nError: Account Number " << targetAccNum << " does not exist!";
    }
}

// Securely handles modification (Deposits/Withdrawals) utilizing temporary file swaps
void handleTransaction(int targetAccNum, int transactionType) {
    ifstream inFile("bank_records.txt");
    ofstream tempFile("temp_records.txt");

    if (!inFile) {
        cout << "\nNo database records exist yet!";
        return;
    }

    int accNum;
    string name;
    double bal;
    bool found = false;
    double transactionAmount;

    while (inFile >> accNum) {
        inFile.ignore();
        getline(inFile, name);
        inFile >> bal;

        BankAccount temp;
        temp.setAccountNumber(accNum);
        temp.setAccountHolderName(name);
        temp.setBalance(bal);

        if (accNum == targetAccNum) {
            found = true;
            if (transactionType == 1) { // Deposit
                cout << "\nEnter Amount to Deposit: ?";
                cin >> transactionAmount;
                temp.deposit(transactionAmount);
            } else if (transactionType == 2) { // Withdraw
                cout << "\nEnter Amount to Withdraw: ?";
                cin >> transactionAmount;
                temp.withdraw(transactionAmount);
            }
            cout << "\nUpdated Balance: ?" << fixed << setprecision(2) << temp.getBalance() << endl;
        }

        // Re-write records back to the temporary storage file
        tempFile << temp.getAccountNumber() << endl;
        tempFile << temp.getAccountHolderName() << endl;
        tempFile << temp.getBalance() << endl;
    }

    inFile.close();
    tempFile.close();

    // Replace the database with the modified tracking data
    remove("bank_records.txt");
    rename("temp_records.txt", "bank_records.txt");

    if (!found) {
        cout << "\nError: Account Number " << targetAccNum << " was not found!";
    }
}

// Display summary list of all banking customers
void displayAllRecords() {
    ifstream inFile("bank_records.txt");
    if (!inFile) {
        cout << "\nDatabase file missing or empty.";
        return;
    }

    int accNum;
    string name;
    double bal;

    cout << "\n========================================================";
    cout << "\n                      ACCOUNT LIST                      ";
    cout << "\n========================================================";
    cout << "\n" << left << setw(15) << "Acc No." << setw(25) << "Holder Name" << "Balance";
    cout << "\n--------------------------------------------------------";

    while (inFile >> accNum) {
        inFile.ignore();
        getline(inFile, name);
        inFile >> bal;

        cout << "\n" << left << setw(15) << accNum 
             << setw(25) << name 
             << "?" << fixed << setprecision(2) << bal;
    }
    cout << "\n========================================================\n";
    inFile.close();
}
