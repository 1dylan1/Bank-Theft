#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"transaction.h"

Transaction *createTransaction(const char* uuid, const char *transaction_type, double amount, const char *account_number, double before_bal, double after_bal, const char *merchant_account) {
      if(uuid == NULL || transaction_type == NULL || account_number == NULL || merchant_account == NULL) {
        printf("Error(createTransaction): One of the values you entered point to NULL.\n");
        return NULL;
    }
    
    Transaction *new_transaction = (Transaction *) malloc(sizeof(Transaction) * 1);
    
    new_transaction->uuid = (char *) malloc(sizeof(char) * (strlen(uuid) + 1));
    strcpy(new_transaction->uuid, uuid);
    new_transaction->transaction_type = (char *) malloc(sizeof(char) * (strlen(transaction_type) + 1));
    strcpy(new_transaction->transaction_type, transaction_type);
    new_transaction->account_number = (char *) malloc(sizeof(char) * (strlen(account_number) + 1));
    strcpy(new_transaction->account_number, account_number);
    new_transaction->merchant_account = (char *) malloc(sizeof(char) * (strlen(merchant_account) + 1));
    strcpy(new_transaction->merchant_account, merchant_account);

    new_transaction->amount = amount;
    new_transaction->before_bal = before_bal;
    new_transaction->after_bal = after_bal;

    return new_transaction;

}

void initTransaction(Transaction *transaction, const char* uuid, const char *transaction_type, double amount, const char *account_number, double before_bal, double after_bal, const char *merchant_account) {
    
    transaction->uuid = deepStringCopy(uuid);
    transaction->transaction_type = deepStringCopy(transaction_type);
    transaction->amount = amount;
    transaction->account_number = deepStringCopy(account_number);
    transaction->before_bal = before_bal;
    transaction->after_bal = after_bal;
    transaction->merchant_account = deepStringCopy(merchant_account);
}
char *transactionToString(const Transaction *transaction) {
      if(transaction == NULL) {
        printf("Error: transaction was NULL\n");
        return NULL;
    }
    char temp[1000];
    sprintf(temp, "%s %s %f %s %f %f %s",transaction->uuid, transaction->transaction_type, transaction->amount, transaction->account_number, transaction->before_bal, transaction->after_bal, transaction->merchant_account);
    char *result = deepStringCopy(temp);
    return result;
}
void printTransaction(const Transaction *transaction) {
    char *s = transactionToString(transaction);
    printf("%s\n", s);
    free(s);
    return;
}
char *deepStringCopy(const char *s) {
    char *copy = (char *) malloc( (strlen(s) + 1) * sizeof(char));
    strcpy(copy, s);
    return copy;
}

Transaction * loadFile(const char *fileName, int *size) {
    
    FILE *f = fopen(fileName, "r");
    if(f == NULL) {
        return NULL; 
    }
    char tempBuffer[1000]; //temp buffer PER line
    fgets(tempBuffer, 1000, f);//reading first line, THIS IS OUR NUMBER OF RECORDS IN THE FILE
    int n = atoi(tempBuffer);
    //set up our array of records
    Transaction * transaction = (Transaction *) malloc(sizeof(Transaction) * n);
    for(int i = 0; i<n; i++) {
        fgets(tempBuffer, 1000, f);
        tempBuffer[strlen(tempBuffer)-1] = '\0';
        //tokenize & process
        char *Struuid = strtok(tempBuffer, ",");
        char *Strtransaction_type = strtok(NULL, ",");
        char *Stramount = strtok(NULL, ",");
        char *Straccount_number = strtok(NULL, ",");
        char *Strbefore_bal = strtok(NULL, ",");
        char *Strafter_bal = strtok(NULL, ",");
        char *Strmerchant_account = strtok(NULL, ",");
        
        double amount = atof(Stramount);
        double before_bal = atof(Strbefore_bal);
        double after_bal = atof(Strafter_bal);
        
        //initialize the [i]th transaction with init function
        initTransaction(&transaction[i], Struuid, Strtransaction_type, amount, Straccount_number, before_bal, after_bal, Strmerchant_account);
    }
    *size = n;
    return transaction;
}

int CountTypeString(Transaction *transaction, char *desired_string) {
    char *str = transaction->transaction_type;
    if(strcmp(str, desired_string) == 0) {
        return 1; //they are the same so print true
    } else {
        return 0; //print false if not
    }
}


int cmpByCustAcc(const void* a, const void* b) {
    const Transaction *x = (const Transaction *)a;
    const Transaction *y = (const Transaction *)b;
    int result = strcmp(x->account_number, y->account_number);
    return result;
}

int cmpByuuid(const void *a, const void *b) {
    const Transaction *x = (const Transaction *)a;
    const Transaction *y = (const Transaction *)b;
    int result = strcmp(x->uuid, y->uuid);
}
