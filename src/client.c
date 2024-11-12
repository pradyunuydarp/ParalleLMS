//
//  client.c
//  OLMS
//
//  Created by Pradyun Devarakonda on 15/05/24.
//

#include "../lib/client.h"


int sockfd,choice;
struct sockaddr_in serv;
client newclient;
scmessage newmsg,replymsg;
book borrow,giveback;
void client_menu1(void){
    printf("Welcome, choose one of the options:");
    printf("\n\t0.Exit");
    printf("\n\t1.Create Account");
    printf("\n\t2.Login");
    printf("\n");
}

void* initiate_server_connection(void* arg) {
    bool*verdict = (bool*)(malloc(sizeof(bool)));
    (*verdict)=false;
    int retries = 0;

    printf("Waiting for server to connect\n");
    while (retries < MAX_RETRIES) {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);

        if (sockfd == -1) {
            perror("Socket creation error");
            return NULL;
        }

        serv.sin_family = AF_INET;
        serv.sin_addr.s_addr = INADDR_ANY;
        serv.sin_port = htons(5555);

        if (connect(sockfd, (struct sockaddr *)&serv, sizeof(serv)) == -1) {
            perror("Connection error");
            close(sockfd);
            retries++;
            printf("Retrying in %d seconds... (%d/%d)\n", RETRY_DELAY, retries, MAX_RETRIES);
            sleep(RETRY_DELAY);
        } else {
            printf("Server connection established\n");
            strcpy(newclient.name, "");
            strcpy(newclient.password, "");
            strcpy(newmsg.msgstr, STARTCOMM);
            newmsg.msgclient = newclient;
            write(sockfd, &newmsg, sizeof(newmsg));
            (*verdict=true);
            return verdict;
        }
    }

    printf("Failed to connect to the server after %d retries\n", MAX_RETRIES);
    return verdict;
}
void post_login_menu(void*arg){
    printf("\t0.Exit");
    printf("\n\t1.Logout");
    printf("\n\t2.Borrow Book");
    printf("\n\t3.Return Book");
    printf("\n\t4.Display Books you have");
    printf("\n\t5.Display all available books\n");
//    return NULL;
}
void* post_login(void*arg){
    cliu *cl = (cliu*)(arg);
    printf("\nWelcome, %s\n",cl->msg.msgclient.name);
    post_login_menu(NULL);
    
    while(true){
        while(scanf("%d",&choice)&&(choice>5 || choice<0)){
            printf("\nEnter a valid choice:");
        }
        if(choice==0){
            printf("\n\nThank You\n\n");
            strcpy(cl->msg.msgstr,ENDCOMM);
            write(cl->sockfd, &(cl->msg), sizeof(scmessage));
            exit(0);
        }
        else if(choice==1){
            strcpy(cl->msg.msgstr,LOGGEDOUT);
            return cl;
        }
        else if (choice==2){
            strcpy(cl->msg.msgstr, BORROW_BOOK_MSG);
            write(cl->sockfd, &(cl->msg), sizeof(scmessage));
            read(cl->sockfd, &(replymsg), sizeof(scmessage));
            printf("%s",replymsg.msgstr);
            printf("\nEnter the name of the book you want to borrow: ");
            scanf("%s",borrow.book_name);
            printf("\nEnter the name of the author of that book: ");
            scanf("%s",borrow.book_author);
            write(cl->sockfd, &(borrow), sizeof(book));
            read(cl->sockfd, &(replymsg), sizeof(scmessage));
            printf("\nMessage from server: %s\n",replymsg.msgstr);
            if(!strcmp(replymsg.msgstr, BOOKCANBELENT)){
                printf("\n You have successfully borrowed the book %s authored by %s",borrow.book_name,borrow.book_author);
            }
            else if(!strcmp(replymsg.msgstr, BOOKNOTINLIB)){
                printf("\n A copy of such a book is not available\n");
            }
            else if(!strcmp(replymsg.msgstr, BORROWLIMITREACHED)){
                printf("\nYou have already borrowed the maximum books you can borrow\n");
            }
            else if(!strcmp(replymsg.msgstr, ALREADYHAVEACOPY)){
                printf("\nYou have already borrowed a copy of this book, a duplicate copy will not be issued\n");
            }
            else{
                printf("\nPlease try again\n");
            }
            
        }
        else if(choice==3){
            strcpy(cl->msg.msgstr, RETURN_BOOK_MSG);
            write(cl->sockfd, &(cl->msg), sizeof(scmessage));
            read(cl->sockfd, &(replymsg), sizeof(scmessage));
            printf("%s",replymsg.msgstr);
            printf("\nEnter the name of the book you want to return: ");
            scanf("%s",giveback.book_name);
            printf("\nEnter the name of the author of that book: ");
            scanf("%s",giveback.book_author);
            write(cl->sockfd, &(giveback), sizeof(book));
            read(cl->sockfd, &(replymsg), sizeof(scmessage));
            printf("\nMessage from server: %s\n",replymsg.msgstr);
            if(!strcmp(replymsg.msgstr, BOOKRETRIEVED)){
                printf("\n You have successfully returned the book %s authored by %s\n",giveback.book_name,giveback.book_author);
            }
//            else if(!strcmp(replymsg.msgstr, BOOKNOTINLIB)){
//                printf("\n A copy of such a book is not available\n");
//            }
            else if(!strcmp(replymsg.msgstr, BOOKNEVERTAKEN)){
                printf("\nOops, Seems like you have never borrowed that book\n");
            }
//            else if(!strcmp(replymsg.msgstr, ALREADYHAVEACOPY)){
//                printf("\nYou have already borrowed a copy of this book, a duplicate copy will not be issued\n");
//            }
            else{
                printf("\nPlease try again\n");
            }
        }
        else if(choice==4){
            strcpy(cl->msg.msgstr, DISPLAY_OWNED_BOOKS);
            write(cl->sockfd, &(cl->msg), sizeof(scmessage));
            read(cl->sockfd, &(replymsg), sizeof(scmessage));
            printf("%s",replymsg.msgstr);
        }
        else if(choice==5){
            strcpy(cl->msg.msgstr, DISPLAY_ALL_AVAILABLE_BOOKS);
            write(cl->sockfd, &(cl->msg), sizeof(scmessage));
            read(cl->sockfd, &(replymsg), sizeof(scmessage));
            printf("%s",replymsg.msgstr);
        }
        printf("\nWelcome, %s\n",cl->msg.msgclient.name);
        post_login_menu(NULL);
//        scanf("%d",&choice);
//        if(choice==)
    }
    
    return NULL;
}
void* pre_login(void*arg){
    printf("\nWelcome to The Library\n");
    bool* connected = (bool*)initiate_server_connection(NULL);
    if(!(*connected)){
        printf("Goodbye\n");
        exit(0);
    }
    client_menu1();
    choice = 0;
    scanf("%d",&choice);
    char * name = (char*)(malloc(NAMESIZE*sizeof(char)));
    char * password = (char*)(malloc(PASSWORDSIZE*sizeof(char)));
    if (name == NULL || password == NULL ) {
            fprintf(stderr, "Memory allocation failed\n");
            return NULL;
        }
    cliu* cl = (cliu*)(malloc(sizeof(cliu)));
    bool logged_in = false;
    if(choice==0){
        printf("\n\nThank You\n\n");
        exit(0);
    }
    while(choice!=0){
        switch (choice) {
            case 0:
                strcpy(newmsg.msgstr, ENDCOMM);
                write(sockfd,&newmsg,sizeof(newmsg));
                printf("\n\nThank You\n\n");
                exit(0);
                break;
            case 1:
                printf("Enter your name (in less than %d characters): ", NAMESIZE - 1);
                scanf("%s", name);
                strcpy(newclient.name, name);
                strcpy(newclient.password, "");
                strcpy(newmsg.msgstr, CHECK_NAME_MSG);
                newmsg.msgclient = newclient;
                write(sockfd, &newmsg, sizeof(newmsg));
                read(sockfd, &replymsg, sizeof(scmessage));
                if(!strcmp(replymsg.msgstr,NAMEALREADYUSED)){
                    printf("\nClient name already taken\n");
                    break;
                }
                printf("Enter a password (in less than %d characters): ", PASSWORDSIZE - 1);
                scanf("%s", password);
                strcpy(newclient.password, password);
                strcpy(newmsg.msgstr, ADD_CLIENT_MSG);
                newmsg.msgclient = newclient;
                write(sockfd, &newmsg, sizeof(newmsg));
                read(sockfd, &replymsg, sizeof(scmessage));
                printf("%s\n",replymsg.msgstr);
                logged_in = true;
                break;
                
            case 2:
                printf("Enter your name (in less than %d characters): ", NAMESIZE - 1);
                scanf("%s", name);
                printf("Enter your password (in less than %d characters): ", PASSWORDSIZE - 1);
                scanf("%s", password);
                strcpy(newclient.name, name);
                strcpy(newclient.password, password);
                strcpy(newmsg.msgstr, LOGIN_CLIENT_MSG);
                newmsg.msgclient = newclient;
                write(sockfd, &newmsg, sizeof(newmsg));
                read(sockfd, &replymsg, sizeof(scmessage));
                printf("%s\n",replymsg.msgstr);
                if(!strcmp(replymsg.msgstr,AUTHENTICATED))logged_in=true;
                if(!strcmp(replymsg.msgstr,COULDNTLOGIN)){
                    printf("Wrong name/password, please try again\n");
                    logged_in=false;
                }
                break;
                
            default:
                break;
        }
        if(logged_in){
            cl->sockfd = sockfd;
            cl->msg = newmsg;
            cliu* replycliu = (cliu*)post_login(cl);
            if(!strcmp(replycliu->msg.msgstr,LOGGEDOUT))
                logged_in = false;
        }
        client_menu1();
        while(scanf("%d",&choice)&&(choice>2 || choice<0)){
            printf("\nEnter a valid choice:");
        }
    }
    strcpy(newclient.name, "");
    strcpy(newclient.password, "");
    strcpy(newmsg.msgstr, ENDCOMM);
    newmsg.msgclient = newclient;
    write(sockfd, &newmsg, sizeof(newmsg));
    close(sockfd);
    
    return NULL;
}

int main(void){
    pre_login(NULL);
    return 0;
}
