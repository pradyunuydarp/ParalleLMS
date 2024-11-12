//
//  client.h
//  OLMS
//
//  Created by Pradyun Devarakonda on 15/05/24.
//

#ifndef client_h
#define client_h

#include "include.h"
#include "book.h"
#define MAX_RETRIES 10
#define RETRY_DELAY 5
#define NAMESIZE 16
#define PASSWORDSIZE 16
#define MAXOWNABLEBOOKS 15
#define MAXMSGLENGTH 10000
#define STARTCOMM "start communicating"
#define ENDCOMM "end communicating"
#define NAMEALREADYUSED "name already taken"
#define NAMEFREE "name free"
#define SUCCESSFULLYADDED "added client successfully"
#define COULDNTLOGIN "could not log in"
#define AUTHENTICATED "you can log in"
#define ACCEPTEDLOGIN "I logged in"
#define LOGGEDOUT "logged out"
#define BORROWLIMITREACHED "you can't borrow any more books"
#define ALREADYHAVEACOPY "you already have a copy of the book(no duplicate copies will be lent)"
#define BOOKNEVERTAKEN "you never borrowed this book in the first place"
#define BOOKRETRIEVED "book successfully retrieved"

struct client{
    char name[NAMESIZE];
    char password[PASSWORDSIZE];
    book books_owned[MAXOWNABLEBOOKS];
    int num_books_owned;
};
typedef struct client client;
struct scmessage{
    char msgstr[MAXMSGLENGTH];
    client msgclient;
};
typedef struct scmessage scmessage;
struct client_utility{
    scmessage msg;
    int sockfd;
};
typedef struct client_utility cliu;
#endif /* client_h */
