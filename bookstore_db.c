#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100


struct Book
{
	int id;
	char title[MAX_DATA];
	int pages;
	int price;
	int set;

};

struct Seller
{
	int id;
	char name[MAX_DATA];
	int set;
	struct Book books[MAX_ROWS];

};

struct Customer
{
	int id;
	char name[MAX_DATA];
	int set;
	struct Book books[MAX_ROWS];

};

struct Book_db
{
	struct Book rows[MAX_ROWS];
};

struct Seller_db
{
	struct Seller rows[MAX_ROWS];
};

struct Customer_db
{
	struct Customer rows[MAX_ROWS];

};

struct ConnectionBook
{
	FILE *fileBook;
	struct Book_db *db;
};

struct ConnectionSeller
{
	FILE *fileSeller;
	struct Seller_db *db;
};

struct ConnectionCustomer
{
	FILE *fileCustemor;
	struct Customer_db *db;
};

void die (const char *message)
{
	if (errno) {
		perror(message);
	} else {
		printf("ERROR: %s\n", message);
	}

	exit(1);
}

void Book_print (struct Book *book)
{

	printf("%d %s %d %d\n", book->id, book->title, book->pages, book->price);
}

void Seller_print (struct Seller *seller)
{

	printf("%d %s books:\n", seller->id, seller->name);
	
	int i = 0;
	for (i = 0 ; i < MAX_ROWS ; i++) {
		struct Book *book = &seller->books[i]; 
		if (book->set) {
			printf("\t");
			Book_print(book);
		}
	}
}

void Customer_print (struct Customer *customer)
{

	printf("%d %s book:\n", customer->id, customer->name);
	
	int i = 0;
	for (i = 0 ; i < MAX_ROWS ; i++) {
		struct Book *book = &customer->books[i];
		if (book->set) {
			printf("\t");
			Book_print(book);
		}
	}
}












int main (int argc, char *argv[])
{

	return 0;
}
