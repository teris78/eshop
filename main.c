#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "eshop.h"

int main() {
    Product log[CATALOG_SIZE];
    initialize_log(log);
 
    for (int i = 0; i < 5; i++) {
        int order_pipe[2], response_pipe[2];
        pipe(order_pipe); //pipe για αποστολη παραγγελιων
        pipe(response_pipe);//pipe για ληψη απαντησεων

        if (fork() == 0) { // Δημιουργια νεας διεργασιας πελατη / παιδι
            customer(i, order_pipe, response_pipe);//Λειτουργια πελατη
        } else {
            eshop(log, order_pipe, response_pipe, i);//Λειτουργια eshop
        } }
//Αναμονη για την ολοκληρωση ολων των διεργασιων
    for (int j = 0; j < 5; j++) {
        wait(NULL);
    }
//εκτυπωση αναφορας 
    report(log);
    return 0;
}
