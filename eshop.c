#include "eshop.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
// αρχικοποιηση καταλογου προιοντων με τιμες και περιγραφες
void initialize_log(Product *log) {
    for (int i = 0; i < CATALOG_SIZE; i++) {
        snprintf(log[i].description, 50, "product-%d", i);
        log[i].price = (rand() % 1000) / 100.0; // τυχαια τιμη προιοντος
        log[i].itemc = 2;// αρχικο αποθεμα
        log[i].requests = 0;
        log[i].sold = 0;
        log[i].failed_user_count = 0;
    }
}
// Συναρτηση για τους πελατες , στελνει αιτηματα και λαμβανει απαντησεις
void customer(int customer_id, int order_pipe[], int response_pipe[]) {
    close(order_pipe[0]); // κλεισιμο αχρηστων ακρων σωληνων για αποφυγη σφαλματος
    close(response_pipe[1]);

    char buffer[100];
    for (int i = 0; i < 10; i++) { //10 τυχαιες παραγγελιες ανα πελατη
        int product_id = rand() % CATALOG_SIZE;
        snprintf(buffer, sizeof(buffer), "%d", product_id);
        write(order_pipe[1], buffer, strlen(buffer) + 1);// στελνετε το ID προιοντος

        read(response_pipe[0], buffer, sizeof(buffer)); // λαμβανεται η απαντηση
        printf("Customer %d: %s\n", customer_id, buffer);
        sleep(1);
    }

    close(order_pipe[1]);
    close(response_pipe[0]);
    exit(0);
}
// Συναρτηση eshop για διαχειρηση παραγγελιων

void eshop(Product *log, int order_pipe[], int response_pipe[], int customer_id) {
 
    close(order_pipe[1]);
    close(response_pipe[0]);

    char buffer[100];
    while (read(order_pipe[0], buffer, sizeof(buffer)) > 0) { // αναμονη παραγγελιας
        int product_id = atoi(buffer);
        log[product_id].requests++;

        sleep(1);  // χρονος επεξεργασιας παραγγελιας
        if (log[product_id].itemc > 0) {
            log[product_id].itemc--;//μειωση αποθεματος
            log[product_id].sold++;//αυξηση πωλησεων
            snprintf(buffer, sizeof(buffer), "success: product %d purchased", product_id);
        } else {
            snprintf(buffer, sizeof(buffer), "failed: product %d unavailable", product_id);
        }
        write(response_pipe[1], buffer, strlen(buffer) + 1);
    }
    close(order_pipe[0]);
    close(response_pipe[1]);
}
//Αναφορα του καταστηματος σχετικα με παραγγελιες , πωλησεις,αποτυχιων και τζιρου
void report(Product *log) {
    printf("report\n");
    int total_orders = 0, successful_orders = 0, failed_orders = 0;
    float total_revenue = 0.0;

    for (int i = 0; i < CATALOG_SIZE; i++) {
        printf("Product: %s\n", log[i].description);
        printf("Total Requests: %d\n", log[i].requests);
        printf("Sold: %d\n", log[i].sold);
        printf("Unfulfilled Orders: %d\n", log[i].failed_user_count);

        total_orders += log[i].requests;
        successful_orders += log[i].sold;
        failed_orders += log[i].failed_user_count;
        total_revenue += log[i].sold *log[i].price;
    }

    printf("\nTotal Orders: %d\n", total_orders);
    printf("Successful Orders: %d\n", successful_orders);
    printf("Failed Orders: %d\n", failed_orders);
    printf("Total Revenue: %.2f\n", total_revenue);
}
