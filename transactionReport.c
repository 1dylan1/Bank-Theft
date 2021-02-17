#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>

#include"transaction.h"

int main(int argc, char **argv) {
    
    int deposit = 0;
    double deposit_num = 0;
    
    int withdrawal = 0;
    double withdrawal_num = 0;
    
    int payment = 0;
    double payment_num = 0;
    
    int debit = 0;
    double debit_num = 0;
    
    int transfer = 0;
    double transfer_num = 0;
    
    int size;
    int size2;
    Transaction *transaction = loadFile(argv[1], &size);
    Transaction *transaction2 = loadFile(argv[2], &size2);
    for(int i = 0; i<size; i++) {
      // printTransaction(&transaction[i]);
        
        if(CountTypeString(&transaction[i], "DEPOSIT") == 1) {
            deposit++;
            deposit_num += transaction[i].amount;
        } else if(CountTypeString(&transaction[i], "WITHDRAWAL") == 1) {
            withdrawal++;
            withdrawal_num += transaction[i].amount;
        } else if(CountTypeString(&transaction[i], "PAYMENT") == 1) {
            payment++;
            payment_num += transaction[i].amount;
        } else if(CountTypeString(&transaction[i], "DEBIT") == 1) {
            debit++;
            debit_num += transaction[i].amount;
        } else if(CountTypeString(&transaction[i], "TRANSFER") == 1) {
            transfer++;
            transfer_num += transaction[i].amount;
        }
    }
/*
Basic Report(Hack 14.0);
*/
    int total = deposit + withdrawal + payment + debit + transfer;
    double total_amount = deposit_num + withdrawal_num + payment_num + debit_num + transfer_num;
    printf("======================================\n");
    printf("Totals Report\n");
    printf("======================================\n");
    printf("Type          Count              Total\n");
    printf("======================================\n");
    printf("Payment:      %5d $ %16.2lf\n", payment, payment_num);
    printf("Transfer:     %5d $ %16.2lf\n", transfer, transfer_num);
    printf("Withdraw:     %5d $ %16.2lf\n", withdrawal, withdrawal_num);
    printf("Debit:        %5d $ %16.2lf\n", debit, debit_num);
    printf("Deposit:      %5d $ %16.2lf\n", deposit, deposit_num);
    printf("======================================\n");
    printf("Total:        %5d $ %16.2lf \n\n", total, total_amount);
/*
#1 - Finding invalid transactions
*/
int invalid_balances = 0;
double offby = 0;
printf("======================================\n");
printf("Balance Validation Report\n");
printf("======================================\n");
for(int q = 0; q < size; q++) {
    if(CountTypeString(&transaction[q],"PAYMENT") == 1 || CountTypeString(&transaction[q],"WITHDRAWAL") == 1 || CountTypeString(&transaction[q], "TRANSFER") == 1 || CountTypeString(&transaction[q],"DEBIT") == 1) {
        //double p_total = transaction[q].before_bal - transaction[q].amount; 
        if(fabs(transaction[q].before_bal - transaction[q].amount - transaction[q].after_bal >= 0.001)) {
            offby = transaction[q].before_bal - transaction[q].amount - transaction[q].after_bal;
            printf("Invalid Transaction, off by $%.2lf\n", offby);
            printf("    %s: %s -> %s\n", transaction[q].uuid, transaction[q].account_number, transaction[q].merchant_account);
            printf("     (%s) $ %.2lf ($%.2lf -> $%.2lf)\n", transaction[q].transaction_type,transaction[q].amount, transaction[q].before_bal, transaction[q].after_bal);
            invalid_balances++;
        }

    } else {
         if(fabs(transaction[q].before_bal + transaction[q].amount - transaction[q].after_bal >= 0.001)) {
            offby = transaction[q].before_bal + transaction[q].amount - transaction[q].after_bal;
            printf("Invalid Transaction, off by $%.2lf\n", offby);
            printf("    %s: %s -> %s\n", transaction[q].uuid, transaction[q].account_number, transaction[q].merchant_account);
            printf("     (%s) $ %.2lf ($%.2lf -> $%.2lf)\n", transaction[q].transaction_type,transaction[q].amount, transaction[q].before_bal, transaction[q].after_bal);
            invalid_balances++;
    }
}

printf("Invalid balances found: %d\n\n", invalid_balances);

/*
#2 - Finding balanced over or near $10,000
*/
int approaches_limit = 0;
int exceeds_limit = 0;
for(int k = 0; k < size; k++) {
    if( strcmp(transaction[k].transaction_type, "DEPOSIT") == 0) {
        if(transaction[k].amount >= 9900.00 && transaction[k].amount < 9999.99) {
            approaches_limit++;
        } else if(transaction[k].amount > 10000.00) {
            exceeds_limit++;
        }
    }
}
printf("=========================================\n");
printf("Deposit Limit Report\n");
printf("=========================================\n");
printf("Deposits exceedings 10k:   %d\n", exceeds_limit);
printf("Desposits approaching 10k: %d\n\n", approaches_limit);

/*
#3 - Finding repeated transactions
*/
printf("=========================================\n");
printf("Repeated Transaction Report\n");
printf("=========================================\n");

int repeated_transactions = 0;
int check = 0;
//Sort list
int e = 1; //keep e ahead of w by 1
qsort(transaction, size, sizeof(Transaction), cmpByCustAcc);
for(int w = 0; w<size-1; w++) {
    if(strcmp(transaction[w].account_number, transaction[e].account_number) == 0 && strcmp(transaction[w].merchant_account, transaction[e].merchant_account) == 0 && transaction[w].amount == transaction[e].amount) {
        printf("    %s: %s -> %s\n",transaction[w].uuid, transaction[w].account_number, transaction[w].merchant_account);
        printf("    (%s) $ %.2lf ($%.2lf -> $%.2lf)\n", transaction[w].transaction_type, transaction[w].amount,transaction[w].before_bal,transaction[w].after_bal);
        repeated_transactions++;
        check = 1;
    } else if(check == 1) {
        repeated_transactions++;
        printf("    %s: %s -> %s\n",transaction[w].uuid, transaction[w].account_number, transaction[w].merchant_account);
        printf("    (%s) $ %.2lf ($%.2lf -> $%.2lf)\n", transaction[w].transaction_type, transaction[w].amount,transaction[w].before_bal,transaction[w].after_bal);
        check = 0;
    }
    e++; //reset e to be 1 above w for the next iteration of the loop
}

printf("Number of Repeated Transactions Flagged: %d\n",repeated_transactions);

/*
#4 - Benford's Law
*/
printf("=========================================\n");
printf("Benford Analysis\n");
printf("=========================================\n");
char tempy[2];
int freq_1 = 0, freq_2 = 0, freq_3 = 0, freq_4 = 0, freq_5 = 0, freq_6 = 0, freq_7 = 0, freq_8 = 0, freq_9 = 0;
int val;
for(int t = 0; t <size; t++) {
    snprintf(tempy, 2, "%d",(int)transaction[t].amount);
    val = atoi(tempy);
    if(val == 1) {
        freq_1++;
    } else if(val == 2) {
        freq_2++;
    } else if(val == 3) {
        freq_3++;
    } else if(val == 4) {
        freq_4++;
    } else if(val == 5) {
        freq_5++;
    } else if(val == 6) {
        freq_6++;
    } else if(val == 7) {
        freq_7++;
    } else if(val == 8) {
        freq_8++;
    } else if(val == 9) {
        freq_9++;
    }
}

double f1 = freq_1/(double) size;
double f2 = freq_2/(double) size;
double f3 = freq_3/(double) size;
double f4 = freq_4/(double) size;
double f5 = freq_5/(double) size;
double f6 = freq_6/(double) size;
double f7 = freq_7/(double) size;
double f8 = freq_8/(double) size;
double f9 = freq_9/(double) size;

double d1 = f1- 0.301;
double d2 = f2 - 0.176;
double d3 = f3 - 0.125;
double d4 = f4 - 0.097;
double d5 = f5 - 0.079;
double d6 = f6 - 0.067;
double d7 = f7 - 0.058;
double d8 = f8 - 0.051;
double d9 = f9 - 0.046;

printf("Number  Count  Frequency  Expected  Difference\n");
printf("1 %11d %11.3lf       0.301 %11.3lf\n",freq_1, f1, d1);
printf("2 %11d %11.3lf       0.176 %11.3lf\n",freq_2, f2, d2);
printf("3 %11d %11.3lf       0.125 %11.3lf\n",freq_3, f3, d3);
printf("4 %11d %11.3lf       0.097 %11.3lf\n",freq_4, f4, d4);
printf("5 %11d %11.3lf       0.079 %11.3lf\n",freq_5, f5, d5);
printf("6 %11d %11.3lf       0.067 %11.3lf\n",freq_6, f6, d6);
printf("7 %11d %11.3lf       0.058 %11.3lf\n",freq_7, f7, d7);
printf("8 %11d %11.3lf       0.051 %11.3lf\n",freq_8, f8, d8);
printf("9 %11d %11.3lf       0.046 %11.3lf\n",freq_9, f9, d9);

/*
#5 Finding Fraud and Detecting Anomalies
*/
printf("=========================================\n");
printf("Fraud Dection Report\n");
printf("=========================================\n");

qsort(transaction, size, sizeof(Transaction), cmpByuuid);
qsort(transaction2, size2, sizeof(Transaction), cmpByuuid);

Transaction *key = createTransaction("","",0,"",0,0,"");
Transaction *search;
int missing_records = 0;
int inconsistent_records = 0;
for(int d = 0; d < size; d++) {
    initTransaction(key, transaction[d].uuid, transaction[d].transaction_type, transaction[d].amount, transaction[d].account_number, transaction[d].before_bal, transaction[d].after_bal, transaction[d].merchant_account);
    search = bsearch(key, transaction2, size2, sizeof(Transaction), cmpByuuid);
    if(search != NULL) {
        if(strcmp(transaction[d].account_number, search->account_number) != 0 || strcmp(transaction[d].transaction_type, search->transaction_type) != 0 || strcmp(transaction[d].merchant_account, search->merchant_account) != 0 || transaction[d].amount != search->amount || transaction[d].before_bal != search->before_bal || transaction[d].after_bal != search->after_bal) {
            printf("Inconsistent Data:\n");
            printf("    A: %s: %s -> %s\n       (%s) $ %.2lf ($%.2lf -> $%.2lf)\n",transaction[d].uuid, transaction[d].account_number, transaction[d].merchant_account, transaction[d].transaction_type, transaction[d].amount, transaction[d].before_bal, transaction[d].after_bal);
            printf("    B: %s: %s -> %s\n       (%s) $ %.2lf ($%.2lf -> $%.2lf)\n",search->uuid, search->account_number, search->merchant_account, search->transaction_type, search->amount, search->before_bal, search->after_bal);
            inconsistent_records++;
        }
    } else {
        printf("Missing transaction: %s is in database A but not B\n", transaction[d].uuid);
        missing_records++;
    }
    search = NULL;
}
for(int d = 0; d < size2; d++) {
    initTransaction(key, transaction2[d].uuid, transaction2[d].transaction_type, transaction2[d].amount, transaction2[d].account_number, transaction2[d].before_bal, transaction2[d].after_bal, transaction2[d].merchant_account);
    search = bsearch(key, transaction, size, sizeof(Transaction), cmpByuuid);
    if(search == NULL) {
        printf("Missing transaction: %s is in database B but not A\n", transaction2[d].uuid);
        missing_records++;
    }
    search = NULL;
}
printf("Total missing records: %d\n", missing_records);
printf("Total inconsistent records: %d\n", inconsistent_records);
    return 0;
}



