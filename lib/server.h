//
//  main.h
//  OLMS
//
//  Created by Pradyun Devarakonda on 15/05/24.
//

#ifndef main_h
#define main_h

#include "include.h"
#include "book.h"
#include "client.h"

#define LIBRARYSIZE 100
#define MAXLIBRARIANS 50
#define ADDEDBOOKFORCLIENT "added book for client"
#define COULDNTFINDCLIENT "couldn't find client"
struct librarian{
    char name[NAMESIZE];
    char password[PASSWORDSIZE];
};
typedef struct librarian librarian;
struct thread_librarian{
    librarian libn;
    bool verdict;
};
typedef struct thread_librarian thread_librarian;

struct arguments{
    int argc;
    int breaker;
    thread_librarian person;
    int choice;
    bool bound;
    char** argv;
};
typedef struct arguments arguments;

struct lib_transaction{
    scmessage msg;
    libook tbook;
};
typedef struct lib_transaction lib_transac;


book Library[LIBRARYSIZE];
librarian Librarians[MAXLIBRARIANS];
client all_clients_in_this_session[LIBRARYSIZE];
libook all_books[LIBRARYSIZE];
pthread_mutex_t library_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t librarian_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;


#endif /* main_h */
