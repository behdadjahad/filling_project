#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100


typedef struct Book
{
	int id;
	char title[MAX_DATA];
	int pages;
	int price;
	int set;

} Book;

typedef struct Seller
{
	int id;
	char name[MAX_DATA];
	int set;
	struct Book books[MAX_ROWS];

} Seller;

typedef struct Customer
{
	int id;
	char name[MAX_DATA];
	int set;
	struct Book books[MAX_ROWS];

} Customer;

typedef struct Book_db
{
	struct Book rows[MAX_ROWS];
} Book_db;

typedef struct Seller_db
{
	struct Seller rows[MAX_ROWS];
} Seller_db;

typedef struct Customer_db
{
	struct Customer rows[MAX_ROWS];

} Customer_db;

typedef struct ConnBook
{
	FILE *file;
	struct Book_db *db;
} ConnBook;

typedef struct ConnSeller
{
	FILE *file;
	struct Seller_db *db;
} ConnSeller;

typedef struct ConnCustomer
{
	FILE *file;
	struct Customer_db *db;
} ConnCustomer;

void die (const char *message)
{
	if (errno) {
		perror(message);
	} else {
		printf("ERROR: %s\n", message);
	}

	exit(1);
}

void Book_print (Book *book)
{

	printf("%d %s %d %d\n", book->id, book->title, book->pages, book->price);
}

void Seller_print (Seller *seller)
{

	printf("%d %s books:\n", seller->id, seller->name);
	
	int i = 0;
	for (i = 0 ; i < MAX_ROWS ; i++) {
		Book *book = &seller->books[i]; 
		if (book->set) {
			printf("\t");
			Book_print(book);
		}
	}
}

void Customer_print (Customer *customer)
{

	printf("%d %s book:\n", customer->id, customer->name);
	
	int i = 0;
	for (i = 0 ; i < MAX_ROWS ; i++) {
		Book *book = &customer->books[i];
		if (book->set) {
			printf("\t");
			Book_print(book);
		}
	}
}


void db_book_load (ConnBook *conn_book)
{
	int rc = fread(conn_book->db, sizeof(Book_db), 1, conn_book->file);

	if (rc != 1) {
		die("fail to load book database");
	}
}

void db_seller_load (ConnSeller *conn_seller)
{
	int rc = fread(conn_seller->db, sizeof(Seller_db), 1, conn_seller->file);

	if (rc != 1) {
		die("fail to load seller database");
	}
}

void db_customer_load (ConnCustomer *conn_customer)
{
	int rc = fread(conn_customer->db, sizeof(Customer_db), 1, conn_customer->file);

	if (rc != 1) {
		die("fail to load customer database");
	}
}















int main (int argc, char *argv[])
{

	return 0;
}
