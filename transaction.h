/**
 * Author: Dylan Diehl
 * Date: 11/20/20
 * Purpose: Reads in a file with banking information and gives a report on the number of payment types, their amounts transferred, and a total of both of the former values
 * as well as multiple detection methods for inconsistent records and fraudulent activity.
 * 
 */
typedef struct {
    char *uuid;
    char *transaction_type;
    double amount;
    char *account_number;
    double before_bal;
    double after_bal;
    char *merchant_account;
}Transaction;

/*
Creates a transaction with the arguments of the struct "Transaction" stated above.
*/
Transaction *createTransaction(const char* uuid, const char *transaction_type, double amount, const char *account_number, double before_bal, double after_bal, const char *merchant_account);

/*
Initializes an existing allocated transaction with the given values.
*/
void initTransaction(Transaction *transaction, const char* uuid, const char *transaction_type, double amount, const char *account_number, double before_bal, double after_bal, const char *merchant_account);

/*
Converts a transaction struct to a string.
*/
char *transactionToString(const Transaction *transaction);

/*
Prints out the transaction via printf.
*/
void printTransaction(const Transaction *transaction);

/*
Creates a deep copy of a string and returns it.
*/
char *deepStringCopy(const char *s);

/*
loads the file with specified size
*/
Transaction * loadFile(const char *fileName, int *size);

/*
Compares the transaction type to the desired_string type(Ex: if you wanted to find all deposits, desired_string would be "DEPOSIT"), returns true if it is a match.
*/
int CountTypeString(Transaction *transaction, char *desired_string);

/*
Compares two customer account strings, returns 0 if they are the same.
*/
int cmpByCustAcc(const void* a, const void* b);

/*
Compares two uuid strings, returns 0 if they are the same.
*/
int cmpByuuid(const void *a, const void *b);