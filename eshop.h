#ifndef ESHOP_H
#define ESHOP_H

#define CATALOG_SIZE 20
//Δομη προιοντος
typedef struct {
    char description[50];
    float price;
    int itemc;
    int requests;
    int sold;
    char failed_users[100][50];
    int failed_user_count;
} Product;
// Δηλωση συναρτησεων που χρησιμοποιουνται στο προγραμμα
void initialize_log(Product *log); //Αρχικποιηση καταλογου
void eshop(Product *log, int order_pipe[], int response_pipe[], int customer_id);//Διαχειρηση eshop
void customer(int customer_id, int order_pipe[], int response_pipe[]); //Συναρτηση πελατη
void report(Product *log);//Αναφορα αποτελεσματων

#endif
