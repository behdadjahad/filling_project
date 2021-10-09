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
	char writer[MAX_DATA];
	int pages;
	int price;
	int set;

} Book;

typedef struct Seller
{
	int id;
	char first_name[MAX_DATA];
	char last_name[MAX_DATA];
	int set;
	struct Book books[MAX_ROWS];

} Seller;

typedef struct Customer
{
	int id;
	char first_name[MAX_DATA];
	char last_name[MAX_DATA];
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

	printf("%d %s %s %d %d\n", book->id, book->title, book->writer, book->pages, book->price);
}

void Seller_print (Seller *seller)
{

	printf("%d %s %s books:\n", seller->id, seller->first_name, seller->last_name);
	
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

	printf("%d %s %s book:\n", customer->id, customer->first_name, customer->last_name);
	
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
		die("memory error conn");
	}

	conn_seller->db = malloc(sizeof(Seller_db));
	if (!conn_seller->db) {
		die("memory error db");
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

ConnCustomer *db_customer_open (const char *filename, char mode)
{
	ConnCustomer *conn_customer = malloc(sizeof(ConnCustomer));
	if (!conn_customer) {
		die("memory error conn");
	}

	conn_customer->db = malloc(sizeof(Customer_db));
	if (conn_customer->db) {
		die("memory error db");
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


void db_book_close (ConnBook *conn_book)
{
	if (conn_book) {
		if (conn_book->file) {
			fclose(conn_book->file);
		}
		if (conn_book->db) {
			free(conn_book->db);
		}
		free(conn_book);
	}
}

void db_seller_close (ConnSeller *conn_seller)
{
	if (conn_seller) {
		if (conn_seller->file) {
			fclose(conn_seller->file);
		}
		if (conn_seller->db) {
			free(conn_seller->db);
		}
		free(conn_seller);
	}
}


void db_customer_close (ConnCustomer *conn_customer)
{
	if (conn_customer) {
		if (conn_customer->file) {
			fclose(conn_customer->file);
		}
		if (conn_customer->db) {
			free(conn_customer->db);
		}
		free(conn_customer);
	}
}


void db_book_write (ConnBook *conn_book)
{
	rewind(conn_book->file);

	int rc = fwrite(conn_book->db, sizeof(Book_db), 1, conn_book->file);
	if (rc != 1) {
		die("failed to write databse");
	}

	rc = fflush(conn_book->file);
	if (rc == -1) {
		die("cannot flush database");
	}
}


void db_seller_write (ConnSeller *conn_seller)
{
	rewind(conn_seller->file);

	int rc = fwrite(conn_seller->db, sizeof(Seller_db), 1, conn_seller->file);
	if (rc != 1) {
		die("failed to write databse");
	}

	rc = fflush(conn_seller->file);
	if (rc == -1) {
		die("cannot flush database");
	}
}



void db_customer_write (ConnCustomer *conn_customer)
{
	rewind(conn_customer->file);

	int rc = fwrite(conn_customer->db, sizeof(Customer_db), 1, conn_customer->file);
	if (rc != 1) {
		die("failed to write databse");
	}

	rc = fflush(conn_customer->file);
	if (rc == -1) {
		die("cannot flush database");
	}
}

void db_book_create (ConnBook *conn_book)
{
	int i = 0;

	for (i = 0 ; i < MAX_ROWS ; i++) {
		Book book = {.id = i, .set = 0};
		conn_book->db->rows[i] = book;
	}
}


void db_seller_create (ConnSeller *conn_seller)
{
	int i = 0;

	for (i = 0 ; i < MAX_ROWS ; i++) {
		Seller seller = {.id = i, .set = 0};
		conn_seller->db->rows[i] = seller;

		int j = 0;
		for (int j = 0 ; j < MAX_ROWS ; j++) {
			Book book = {.id = j, .set = 0};
			conn_seller->db->rows[i].books[j] = book;
		}
	}
}


void db_customer_create (ConnCustomer *conn_customer)
{
	int i = 0;

	for (i = 0 ; i < MAX_ROWS ; i++) {
		Customer customer = {.id = i, .set = 0};
		conn_customer->db->rows[i] = customer;
		
		int j = 0;
		for (j = 0 ; j < MAX_ROWS ; j++) {
			Book book = {.id = j, .set = 0};
			conn_customer->db->rows[i].books[j] = book;
		}
	}
}


void db_book_set (ConnBook *conn_book, int id, const char *title, char *writer, int pages, int price)
{
	Book *book = &conn_book->db->rows[id];
	
	book->pages = pages;
	book->price = price;

	if (book->set) {
		die("already set, delete it first");
	}

	book->set = 1;
	
	char *res = strncpy(book->title, title, MAX_DATA);
	book->title[MAX_DATA - 1] = '\0';
	if (!res) {
		die("title copy failed");
	}
	
	res = strncpy(book->writer, writer, MAX_DATA);
	book->title[MAX_DATA - 1] = '\0';
	if (!res) {
		die("writer copy failed");
	}

}


void db_seller_set (ConnSeller *conn_seller, int id, const char *first_name, const char *last_name)
{
	Seller *seller = &conn_seller->db->rows[id];
	if (seller->set) {
		die("already set, delete it first");
	}

	seller->set = 1;

	char *res = strncpy(seller->first_name, first_name, MAX_DATA);
	seller->first_name[MAX_DATA] = '\0';
	if (!res) {
		die("first name copy failed");
	}

	res = strncpy(seller->last_name, last_name, MAX_DATA);
	seller->last_name[MAX_DATA] = '\0';
	if (!res) {
		die("last name copy failed");
	}

}

void db_sell_set_book (ConnSeller *conn_seller, ConnBook *conn_book, int seller_id, int book_id)
{
	Seller *seller = &conn_seller->db->rows[seller_id];
	Book *book = &conn_book->db->rows[book_id];
	Book book_seller;

	if (seller->set == 0) {
		die("no seller set, first set seller");
	}
	
	if (book->set == 0) {
		die("no book set, first set book");
	}

	if (seller->books[book_id].set == 1) {
		die("this book is already set to this seller");
	}

	book_seller.set = 1;
	book_seller.pages = book->pages;
	book_seller.price = book->price;
	
	char *res = strncpy(book_seller.title, book->title, MAX_DATA);
	if (!res) {
		die("copy failed");
	}

	res = strncpy(book_seller.writer, book->writer, MAX_DATA);
	if (!res) {
		die("copy failed");
	}

	conn_seller->db->rows[seller_id].books[book_id] = book_seller;

}

void db_customer_set (ConnCustomer *conn_customer, int id, const char *first_name, const char *last_name)
{
	Customer *customer = &conn_customer->db->rows[id];
	if (customer->set) {
		die("already set, delete it first");
	}

	customer->set = 1;

	char *res = strncpy(customer->first_name, first_name, MAX_DATA);
	customer->first_name[MAX_DATA - 1] = '\0';
	if (!res) {
		die("first name copy failed");
	}

	res = strncpy(customer->last_name, last_name, MAX_DATA);
	customer->first_name[MAX_DATA - 1] = '\0';
	if (!res) {
		die("last name copy failed");
	}

}

void db_cus_set_book (ConnCustomer *conn_customer, ConnBook *conn_book, int customer_id, int book_id)
{
	Customer *customer = &conn_customer->db->rows[customer_id];
	Book *book = &conn_book->db->rows[book_id];
	Book book_customer;

	if (customer->set == 0) {
		die("no seller set, first set customer");
	}
	
	if (book->set == 0) {
		die("no book set, first set book");
	}

	if (conn_customer->db->rows[customer_id].books[book_id].set == 1) {
		die("this book is already set to this customer");
	}

	book_customer.set = 1;
	book_customer.pages = book->pages;
	book_customer.price = book->price;
	
	char *res = strncpy(book_customer.title, book->title, MAX_DATA);
	if (!res) {
		die("copy failed");
	}

	res = strncpy(book_customer.writer, book->writer, MAX_DATA);
	if (!res) {
		die("copy failed");
	}

	conn_customer->db->rows[customer_id].books[book_id] = book_customer;

}

void db_book_get (ConnBook *conn_book, int id)
{
	Book *book = &conn_book->db->rows[id];

	if (book->set) {
		Book_print(book);
	} else {
		die("id is not set");
	}
}

void db_seller_get (ConnSeller *conn_seller, int id)
{
	Seller *seller = &conn_seller->db->rows[id];

	if (seller->set) {
		Seller_print(seller);
	} else {
		die("id is not set");
	}
}

void db_customer_get (ConnCustomer *conn_customer, int id)
{
	Customer *customer = &conn_customer->db->rows[id];

	if (customer->set) {
		Customer_print(customer);
	} else {
		die("id is not set");
	}
}


void db_book_delete (ConnBook *conn_book, int id)
{
	Book book = {.id = id, .set = 0};
	conn_book->db->rows[id] = book;
}

void db_seller_delete (ConnSeller *conn_seller, int id)
{
	Seller seller = {.id = id, .set = 0};
	conn_seller->db->rows[id] = seller;
}

void db_sell_del_book (ConnSeller *conn_seller, int seller_id, int book_id)
{
	Book book = {.id = book_id, .set = 0};
	conn_seller->db->rows[seller_id].books[book_id] = book;
}


void db_customer_delete (ConnCustomer *conn_customer, int id)
{
	Customer customer = {.id = id, .set = 0};
	conn_customer->db->rows[id] = customer;
}

void db_cus_del_book (ConnCustomer *conn_customer, int customer_id, int book_id)
{
	Book book = {.id = book_id, .set = 0};
	conn_customer->db->rows[customer_id].books[book_id] = book;
}

void db_book_list (ConnBook *conn_book)
{
	int i = 0;
	Book_db *db = conn_book->db;

	for (i = 0 ; i < MAX_ROWS ; i++) {
		Book *cur = &db->rows[i];
		if (cur->set) {
			Book_print(cur);
		}
	}
}

void db_seller_list (ConnSeller *conn_seller)
{
	int i = 0;
	Seller_db *db = conn_seller->db;

	for (i = 0 ; i < MAX_ROWS ; i++) {
		Seller *cur = &db->rows[i];
		if (cur->set) {
			Seller_print(cur);
		}
	}
}

void db_customer_list (ConnCustomer *conn_customer)
{
	int i = 0;
	Customer_db *db = conn_customer->db;

	for (i = 0 ; i < MAX_ROWS ; i++) {
		Customer *cur = &db->rows[i];
		if (cur->set) {
			Customer_print(cur);
		}
	}
}

int main (int argc, char *argv[])
{
	if (argc < 4) {
		die("USAGE: database <dbfile> <target> <action> [action params]");
	}
	
	char *filename = argv[1];
	char action = argv[3][0];
	char target = argv[2][0];
	
	
	int id = 0;
	int book_id = 0;
	
	if (argc > 4) {
		id = atoi(argv[4]);
	}

	if (id >= MAX_ROWS) {
		die("there's not that many records.");
	}
	
	switch (target) {
		case 'B':
			;
			ConnBook *conn_book = db_book_open(filename, action);
			switch (action) {
				case 'c':
					db_book_create(conn_book);
					db_book_write(conn_book);
					break;
				case 'g':
					if (argc != 5) {
						die("need an id to get");
					}
					db_book_get(conn_book, id);
					break;
				case 's':
					if (argc != 9) {
						die("need id, title, writer, pages, price, to set");
					}
					int pages = atoi(argv[7]);
					int price = atoi(argv[8]);
					db_book_set(conn_book, id, argv[5], argv[6], pages, price);
					db_book_write(conn_book);
					break;
				case 'l':
					db_book_list(conn_book);
					break;
				case 'd':
					if (argc != 5) {
						die("need id to delete");
					}
					db_book_delete(conn_book, id);
					db_book_write(conn_book);
				default:
					die("invalid action: c=create, g=get, s=set, d=del, l=list");
			}
			db_book_close(conn_book);
			break;
		case 'S':
			;
			ConnSeller *conn_seller = db_seller_open(filename, action);
			switch (action) {
				case 'c':
					db_seller_create(conn_seller);
					db_seller_write(conn_seller);
					break;
				case 'g':
					if (argc != 5) {
						die("need an id to get");
					}
					db_seller_get(conn_seller, id);
					break;
				case 's':
					if (argc != 7) {
						die("need id, first name, last name, to set");
					}
					db_seller_set(conn_seller, id, argv[5], argv[6]);
					db_seller_write(conn_seller);
					break;
				case 'l':
					db_seller_list(conn_seller);
					break;
				case 'a':
					if (argc != 7) {
						die("need id, book db file and book id");
					}
					book_id = atoi(argv[6]);
					ConnBook *conn_book = db_book_open(argv[5], 'g');
					db_sell_set_book(conn_seller, conn_book, id, book_id);
					db_book_write(conn_book);
					db_seller_write(conn_seller);
					db_book_close(conn_book);
					break;
				case 'r':
					if (argc != 6) {
						die("need id, book id");
					}
					book_id = atoi(argv[5]);
					db_sell_del_book(conn_seller, id, book_id);
					db_seller_write(conn_seller);
					break;
				case 'd':
					if (argc != 5) {
						die("need id to delete");
					}
					db_seller_delete(conn_seller, id);
					db_seller_write(conn_seller);
					break;
				default:
					die("invalid action: c=create, g=get, s=set, a=add book, r=remove book, d=del, l=list");
			}
			db_seller_close(conn_seller);
			break;
		case 'C':
			;
			ConnCustomer *conn_customer = db_customer_open(filename, action);
			switch (action) {
				case 'c':
					db_customer_create(conn_customer);
					db_customer_write(conn_customer);
					break;
				case 'g':
					if (argc != 5) {
						die("need an id to get");
					}
					db_customer_get(conn_customer, id);
					break;
				case 's':
					if (argc != 7) {
						die("need id, first name, last name, to set");
					}
					db_customer_set(conn_customer, id, argv[5], argv[6]);
					db_customer_write(conn_customer);
					break;
				case 'a':
					if (argc != 7) {
						die("need id, book filename, book id");
					}
					book_id = atoi(argv[6]);
					ConnBook *conn_book = db_book_open(argv[5], 'g');
					db_cus_set_book(conn_customer, conn_book, id, book_id);
					db_customer_write(conn_customer);
					db_book_close(conn_book);
					break;
				case 'r':
					if (argc != 6) {
						die("need id  book id");
					}
					book_id = atoi(argv[5]);
					db_cus_del_book(conn_customer, id, book_id);
					db_customer_write(conn_customer);
					break;
				case 'l':
					db_customer_list(conn_customer);
					break;
				case 'd':
					if (argc != 5) {
						die("need id to delete");
					}
					db_customer_delete(conn_customer, id);
					db_customer_write(conn_customer);
					break;
				default:
					die("invalid action: c=create, g=get, s=set, a=add book, r=remove book, d=del, l=list");
			}
			db_customer_close(conn_customer);
			break;
		default:
			break;
	}
	
	return 0;
}
