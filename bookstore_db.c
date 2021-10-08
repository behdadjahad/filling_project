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
	Book bts[MAX_ROWS];

} Seller;

typedef struct Customer
{
	int id;
	char name[MAX_DATA];
	int set;
	Book btb[MAX_ROWS];

} Customer;

typedef struct Book_db
{
	Book rows[MAX_ROWS];
} Book_db;


typedef struct Seller_db
{
	Seller rowa[MAX_ROWS];
} Seller_db;

typedef struct Customer_db
{
	Customer rows[MAX_ROWS];

} Customer_db;

typedef struct ConnectionBook
{
	FILE *fileBook;
	Book_db *db;
} ConnectionBook;

typedef struct ConnectionSeller
{
	FILE *fileSeller;
	Seller_db *db;
} ConnectionSeller;

typedef struct ConnectionCustomer
{
	FILE *fileCustemor;
	Customer_db *db;
} ConnectionCustomer;

















int main (int argc, char *argv[])
{

	return 0;
}
