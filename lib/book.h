//
//  book.h
//  OLMS
//
//  Created by Pradyun Devarakonda on 15/05/24.
//

#ifndef book_h
#define book_h
#define MAXBOOKNAMELENGTH 26
#define MAXAUTHORNAMELENGTH 16
#define BOOKCANBELENT "book avaliable"
#define BOOKNOTINLIB "book doesn't exist"
#define BOOKNOCOPIES "no copies of book exist right now"
#define ADDINGBOOKFAILED "couldn't add book (probably client data missing)"
#define RETRIEVINGBOOKFAILED "couldn't add book (probably client data missing)"
struct book{
    char book_name[MAXBOOKNAMELENGTH];
    char book_author[MAXAUTHORNAMELENGTH];
};
typedef struct book book;

struct library_book{
    book book;
    int num_copies;
};
typedef struct library_book libook;
#endif /* book_h */
