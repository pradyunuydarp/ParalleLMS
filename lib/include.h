//
//  include.h
//  OLMS
//
//  Created by Pradyun Devarakonda on 15/05/24.
//

#ifndef include_h
#define include_h
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#define MY_PRINTF(...) printf(__VA_ARGS__)
#define ADD_CLIENT_MSG "add client"
#define LOGIN_CLIENT_MSG "login client"
#define CHECK_NAME_MSG "check client name"
#define CHECK_BOOK_MSG "check book"
#define ADD_BOOK_MSG "add book"
#define BORROW_BOOK_MSG "borrow book"
#define RETURN_BOOK_MSG "return book"
#define DISPLAY_OWNED_BOOKS "display owned books"
#define DISPLAY_ALL_AVAILABLE_BOOKS "display all available books"
#endif /* include_h */
