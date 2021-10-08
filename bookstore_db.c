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

ConnBook *db_book_open (const char *filename, char mode)
{
	ConnBook *conn_book = malloc(sizeof(ConnBook));
	if (!conn_book) {
		die("memory error");
	}

	conn_book->db = malloc(sizeof(Book_db));
	if (!conn_book->db) {
		die("memory error");
	}

	if (mode == 'c') {
		conn_book->file = fopen(filename, "w");
	} else {
		conn_book->file = fopen(filename, "r+");
		if (conn_book->file) {
			db_book_load(conn_book);
		}
	}

	if (!conn_book->file) {
		die("fail to open book file");
	}

	return conn_book;
}

ConnSeller *db_seller_open (const char *filename, char mode)
{
	ConnSeller *conn_seller = malloc(sizeof(ConnSeller));
	if (!conn_seller) {
		die("memory error");
	}

	conn_seller->db = malloc(sizeof(Seller_db));
	if (!conn_seller->db) {
		die("memory error");
	}

	if (mode == 'c') {
		conn_seller->file = fopen(filename, "w");
	} else {
		conn_seller->file = fopen(filename, "r+");
		if (conn_seller->file) {
			db_seller_load(conn_seller);
		}
	}

	if (!conn_seller->file) {
		die("fail to open seller file");
	}

	return conn_seller;
}

ConnSeller *db_customer_open (const char *filename, char mode)
{
	ConnCustomer *conn_customer = malloc(sizeof(ConnCustomer));
	if (!conn_customer) {
		die("memory error");
	}

	conn_customer->db = malloc(sizeof(Customer_db));
	if (conn_customer->db) {
		die("memory error");
	}

	if (mode == 'c') {
		conn_customer->file = fopen(filename, "w");
	} else {
		conn_customer->file = fopen(filename, "r+");
		if (conn_customer->file) {
			db_customer_load(conn_customer);
		}
	}

	if (!conn_customer->file) {
		die("fail to open customer file.");
	}
}















int main (int argc, char *argv[])
{

	return 0;
}
