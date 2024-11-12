//
//  server.c
//  OLMS
//
//  Created by Pradyun Devarakonda on 15/05/24.
//

#include "../lib/server.h"

int num_books,num_librarians,num_clients,num_books;
int sockfd;
struct sockaddr_in serv, cli;
socklen_t size = sizeof(cli);

void* create_librarians(void*arg){
    FILE * fp = fopen("/Users/pradyundevarakonda/Documents/OLMS/OLMS/data/librarians.dat","wb");
    num_librarians=0;
    if(fp==NULL)perror("ISNULL\n");
    fwrite(&num_librarians, sizeof(int), 1, fp);
    fclose(fp);
    return NULL;
}
void* initialize_librarians(void*arg){
    char *argstr = (char*) (arg);
//    printf("hoyaa1");
    if(argstr==NULL || argstr[0]!='c'){
        FILE * fp = fopen("/Users/pradyundevarakonda/Documents/OLMS/OLMS/data/librarians.dat", "rb");
        if(fp==NULL)perror("ISNULL\n");
        fread(&num_librarians, sizeof(int), 1, fp);
//        printf("%d\n",num_librarians);
//        printf("hoyaa");
        if(num_librarians>0){
            fread(Librarians, sizeof(librarian), num_librarians, fp);
        }
//        for(int i = 0; i<num_librarians; i++){
//            printf("%s\n",Librarians[i].name);
//            fread(Librarians[i], <#size_t size#>, <#size_t nitems#>, <#FILE *restrict stream#>)
//        }
        fclose(fp);
    }
    else{
        return create_librarians(NULL);
    }
    return NULL;
}

void* create_clients(void*arg){
    FILE * fp = fopen("/Users/pradyundevarakonda/Documents/OLMS/OLMS/data/clients.dat","wb");
    num_clients=0;
    if(fp==NULL)perror("ISNULL\n");
    fwrite(&num_clients, sizeof(int), 1, fp);
    fclose(fp);
    return NULL;
    
}
void* initialize_clients(void*arg){
    char * argstr = (char*)(arg);
    printf("%s",argstr);
    if(argstr==NULL || argstr[2]!='c'){
        FILE * fp = fopen("/Users/pradyundevarakonda/Documents/OLMS/OLMS/data/clients.dat", "rb");
        if(fp==NULL)perror("ISNULL\n");
        fread(&num_clients, sizeof(int), 1, fp);
        if(num_clients>0){
            fread(all_clients_in_this_session, sizeof(client), num_clients, fp);
        }
        fclose(fp);
        return NULL;
    }
    else{
        return create_clients;
    }
}
bool client_compareer(client c1, client c2){
    if(!strcmp(c1.name,c2.name)&&(!strcmp(c1.password, c2.password))){
        return true;
    }
    return false;
}
void*book_name_checker(void*arg){
    pthread_mutex_lock(&library_mutex);
    libook* newlibook = (libook*)(arg);
    
    for(int i = 0; i<num_books; i++){
        if((!strcmp(newlibook->book.book_name,all_books[i].book.book_name)) && (!strcmp(newlibook->book.book_author,all_books[i].book.book_author))){
            all_books[i].num_copies+=newlibook->num_copies;
            pthread_mutex_unlock(&library_mutex);
            return NULL;
        }
    }
    strcpy(all_books[num_books].book.book_name,newlibook->book.book_name);
    strcpy(all_books[num_books].book.book_author,newlibook->book.book_author);
    all_books[num_books].num_copies = newlibook->num_copies;
    num_books++;
    pthread_mutex_unlock(&library_mutex);
    return NULL;
}
void* add_book(void*arg){
    libook* newlibook = (libook*)(malloc(sizeof(libook)));
//    printf("\nEnter the book's id: ");
//    scanf("%d",&newlibook->book.book_id);
    printf("\nEnter the name of the book: ");
    scanf("%s",newlibook->book.book_name);
    printf("\nEnter the name of the author: ");
    scanf("%s",newlibook->book.book_author);
    printf("\nEnter the number of copies you want to add: ");
    while(scanf("%d",&newlibook->num_copies)&&newlibook->num_copies<=0)printf("\nEnter a valid input(>0): ");
    return book_name_checker(newlibook);
}

void* write_books(void*arg){
    pthread_mutex_lock(&library_mutex);
    FILE * fp = fopen("/Users/pradyundevarakonda/Documents/OLMS/OLMS/data/books.dat","wb");
    if(fp==NULL)perror("ISNULL\n");
    printf("%d\n",num_books);
    fwrite(&num_books, sizeof(int), 1, fp);
    fwrite(all_books, sizeof(libook), num_books, fp);
    fclose(fp);
    pthread_mutex_unlock(&library_mutex);
    return NULL;
}
void* create_books(void*arg){
    FILE * fp = fopen("/Users/pradyundevarakonda/Documents/OLMS/OLMS/data/books.dat","wb");
    num_books=0;
    if(fp==NULL)perror("ISNULL\n");
    fwrite(&num_books, sizeof(int), 1, fp);
    fclose(fp);
    return NULL;
}
void* initialize_books(void*arg){
    char * argstr = (char*)(arg);
//    printf("arg:%s\n",argstr);
    if(argstr==NULL || argstr[1]!='c'){
        FILE * fp = fopen("/Users/pradyundevarakonda/Documents/OLMS/OLMS/data/books.dat", "rb");
        if(fp==NULL)perror("ISNULL\n");
        fread(&num_books, sizeof(int), 1, fp);
        if(num_books>0){
            fread(all_books, sizeof(libook), num_books, fp);
        }
        fclose(fp);
        return NULL;
    }
    else{
        return create_books;
    }
}
void* display_books(void*arg){
    for(int i = 0; i<num_books; i++){
        printf("\nBook Name: %s, Author: %s, Available copies: %d\n",all_books[i].book.book_name,all_books[i].book.book_author,all_books[i].num_copies);
    }
    return NULL;
}
void* library_menu(void*arg){
    printf("\t\tWelcome to the Library\n");
    printf("\t0.Exit");
    printf("\n\t1.Create Librarian account");
    printf("\n\t2.Login as Librarian\n");
    int* library_choice = (int*)(arg);
    while(scanf("%d",library_choice)&&(((*library_choice)>3)||(*library_choice)<0))printf("\nEnter a valid choice:");
    return NULL;
}

void* librarian_login_checker(void*arg){
    pthread_mutex_lock(&librarian_mutex);
    thread_librarian* threadlibn = (thread_librarian*)(arg);
//    printf("%s %s\n",threadlibn->libn.name,threadlibn->libn.password);
    for(int i = 0; i<num_librarians; i++){
//        printf("%s\n",Librarians[i].name);
        if((!strcmp(threadlibn->libn.name, Librarians[i].name))){
            if(!strcmp(threadlibn->libn.password, Librarians[i].password)){
//                printf("%d:%s,%s",i,threadlibn->libn.name,threadlibn->libn.password);
                threadlibn->verdict = true;
                pthread_mutex_unlock(&librarian_mutex);
                return NULL;
            }
        }
    }
    threadlibn->verdict = false;
    pthread_mutex_unlock(&librarian_mutex);
    return NULL;
}
void* librarian_login(void*arg){
    char * libname2 = (char*)(malloc(NAMESIZE*sizeof(char)));
    char * libpassword2 = (char*)(malloc(PASSWORDSIZE*sizeof(char)));
    if (libname2 == NULL || libpassword2 == NULL ) {
            fprintf(stderr, "Memory allocation failed\n");
            return NULL;
        }
    thread_librarian* threadlibn = (thread_librarian*)(arg);
    MY_PRINTF("Enter your name(in less than %d characters):",NAMESIZE);
    scanf("%s",libname2);
    MY_PRINTF("Enter your password(in less than %d characters):",PASSWORDSIZE);
    scanf("%s",libpassword2);
//    threadlibn->libn.name = (char*)(malloc(NAMESIZE*sizeof(char)));
    strcpy(threadlibn->libn.name,libname2);
//    threadlibn->libn.password = (char*)(malloc(PASSWORDSIZE*sizeof(char)));
    strcpy(threadlibn->libn.password,libpassword2);
    printf("%s %s\n",threadlibn->libn.name,threadlibn->libn.password);
    pthread_t login_checker_thread;
    int login_checker_res = pthread_create(&login_checker_thread, NULL, librarian_login_checker, threadlibn);
    pthread_join(login_checker_thread, NULL);
    return NULL;
}
void* librarian_name_checker(void*arg){
    pthread_mutex_lock(&librarian_mutex);
    thread_librarian* threadlibn = (thread_librarian*)(arg);
    for(int i = 0; i<num_librarians; i++){
        if((!strcmp(threadlibn->libn.name, Librarians[i].name))){
            threadlibn->verdict = false;
            pthread_mutex_unlock(&librarian_mutex);
            return NULL;
        }
    }
    threadlibn->verdict = true;
    strcpy(Librarians[num_librarians].name,threadlibn->libn.name);
    strcpy(Librarians[num_librarians].password,threadlibn->libn.password);
    num_librarians++;
    pthread_mutex_unlock(&librarian_mutex);
    return NULL;
}
void* librarian_create_acc(void*arg){
    char * libname1 = (char*)(malloc(NAMESIZE*sizeof(char)));
    char * libpassword1 = (char*)(malloc(PASSWORDSIZE*sizeof(char)));
    if (libname1 == NULL || libpassword1 == NULL ) {
            fprintf(stderr, "Memory allocation failed\n");
            return NULL;
        }
    thread_librarian* threadlibn = (thread_librarian*)(arg);
    MY_PRINTF("Enter your name(in less than %d characters):",NAMESIZE);
    scanf("%s",libname1);
    MY_PRINTF("Enter your password(in less than %d characters):",PASSWORDSIZE);
    scanf("%s",libpassword1);
//    printf("chk0\n");
//    if(threadlibn->libn.name==NULL)printf("name is null\n");
//    threadlibn->libn.name = (char*)(malloc(NAMESIZE*sizeof(char)));
    strcpy(threadlibn->libn.name,libname1);
//    threadlibn->libn.password = (char*)(malloc(PASSWORDSIZE*sizeof(char)));
    strcpy(threadlibn->libn.password,libpassword1);
//    printf("chk1\n");
    printf("%s %s ;%s %s\n",libname1,libpassword1,threadlibn->libn.name,threadlibn->libn.password);
    pthread_t librarian_name_checker_thread;
    int librarian_name_checker_res = pthread_create(&librarian_name_checker_thread, NULL, librarian_name_checker, threadlibn);
    pthread_join(librarian_name_checker_thread, NULL);
//    printf("chk2\n");
    return NULL;
}
void* write_librarians(void*arg){
    pthread_mutex_lock(&librarian_mutex);
    FILE * fp = fopen("/Users/pradyundevarakonda/Documents/OLMS/OLMS/data/librarians.dat","wb");
    if(fp==NULL)perror("ISNULL\n");
//    printf("%d\n",num_librarians);
    fwrite(&num_librarians, sizeof(int), 1, fp);
    fwrite(Librarians, sizeof(librarian), num_librarians, fp);
    fclose(fp);
    pthread_mutex_unlock(&librarian_mutex);
    return NULL;
}
void* write_clients(void* arg){
    pthread_mutex_lock(&client_mutex);
    FILE * fp = fopen("/Users/pradyundevarakonda/Documents/OLMS/OLMS/data/clients.dat","wb");
    if(fp==NULL)perror("ISNULL\n");
//    printf("%dnumclients\n",num_clients);
    fwrite(&num_clients, sizeof(int), 1, fp);
    fwrite(all_clients_in_this_session, sizeof(client), num_clients, fp);
    fclose(fp);
    pthread_mutex_unlock(&client_mutex);
    return NULL;
}
bool check_book(libook b1, libook b2){
    if((!strcmp(b1.book.book_name,b2.book.book_name)) &&(!strcmp(b1.book.book_author,b2.book.book_author)))
       return true;
    return false;
}
bool copies_exist(libook b){
    bool v = false;
    (b.num_copies>0) ? (v=true) : (v=false);
    return v;
}
bool check_no_duplicate_books_client(book b, client c){
    for(int i =0; i<c.num_books_owned; i++){
        printf("%s, %s\n",b.book_name,b.book_author);
        if((!strcmp(b.book_name,c.books_owned[i].book_name))&&(!strcmp(b.book_author,c.books_owned[i].book_author))){
            printf("client already owns a copy\n");
            return false;
        }
    }
    return true;
}
int check_and_retrieve_book_client(book b, client c){
    
    for(int i = 0; i<c.num_books_owned; i++){
        if((!strcmp(b.book_name,c.books_owned[i].book_name))&&(!strcmp(b.book_author,c.books_owned[i].book_author))){
            return i;
        }
    }
    return -1;
}
void* find_client_and_lend_book(void*arg){
    lib_transac* newtransac = (lib_transac*)(arg);
    lib_transac* replytransac = (lib_transac*)(malloc(sizeof(lib_transac)));
    printf("%s,%s\n",newtransac->msg.msgclient.name,newtransac->msg.msgclient.password);
    for(int i = 0; i<num_clients; i++){
        if(client_compareer(newtransac->msg.msgclient, all_clients_in_this_session[i])){
            printf("found client\n");
            if(all_clients_in_this_session[i].num_books_owned<MAXOWNABLEBOOKS){
                if(check_no_duplicate_books_client(newtransac->tbook.book, all_clients_in_this_session[i])){
                    
                    all_clients_in_this_session[i].books_owned[all_clients_in_this_session[i].num_books_owned++] = newtransac->tbook.book;
                    strcpy(replytransac->msg.msgstr,ADDEDBOOKFORCLIENT);
                    printf("now num books owned: %d",all_clients_in_this_session[i].num_books_owned);
                    return replytransac;
                }
                else{
                    printf("no dup\n");
                    strcpy(replytransac->msg.msgstr,ALREADYHAVEACOPY);
                    return replytransac;
                }
            }
            else{
                strcpy(replytransac->msg.msgstr,BORROWLIMITREACHED);
                return replytransac;
            }
        }
    }
    strcpy(replytransac->msg.msgstr,COULDNTFINDCLIENT);
    
    return replytransac;
}
void* find_client_and_retrieve_book(void*arg){
    lib_transac* newtransac = (lib_transac*)(arg);
    lib_transac* replytransac = (lib_transac*)(malloc(sizeof(lib_transac)));
//    printf("requested client: %s,%s",newtransac->msg.msgclient.name,newtransac->msg.msgclient.password);
//    for(int i = 0; i<num_clients; i++){
//        printf("%s,%s\n",all_clients_in_this_session[i].name,all_clients_in_this_session[i].password);
//    }
    
    for(int i = 0; i<num_clients; i++){
        if(client_compareer(newtransac->msg.msgclient, all_clients_in_this_session[i])){
                int indx = check_and_retrieve_book_client(newtransac->tbook.book,all_clients_in_this_session[i]);
                if(indx ==-1){
                    strcpy(replytransac->msg.msgstr,BOOKNEVERTAKEN);
                    return replytransac;
                }
                else{
                    all_clients_in_this_session[i].books_owned[indx] = all_clients_in_this_session[i].books_owned[(all_clients_in_this_session[i].num_books_owned)-1];
                    all_clients_in_this_session[i].num_books_owned--;
                    strcpy(replytransac->msg.msgstr,BOOKRETRIEVED);
                    return replytransac;
                }
        }
    }
    
    strcpy(replytransac->msg.msgstr,COULDNTFINDCLIENT);
    
    return replytransac;
}
void* lend_book(void*arg){
//    printf("in lend book\n");
    pthread_mutex_lock(&library_mutex);
    printf("library locked\n");
    pthread_mutex_lock(&client_mutex);
    printf("clients locked\n");
    lib_transac* newtransac = (lib_transac*)(arg);
//    printf("%s,%s\n",newtransac->msg.msgclient.name,newtransac->msg.msgclient.password);
//    newtransac->tbook;
    scmessage* replymsg = (scmessage*)(malloc(sizeof(scmessage)));
    for(int i = 0; i<num_books; i++){
        if(check_book(newtransac->tbook, all_books[i])){
            if(copies_exist(all_books[i])){
//                printf("%s,%s\n",newtransac->msg.msgclient.name,newtransac->msg.msgclient.password);
                lib_transac* verdict_transac = (lib_transac*)(find_client_and_lend_book(newtransac));
//                printf("addedbook\n");
//                write_clients(NULL);
                if(!strcmp(verdict_transac->msg.msgstr,ADDEDBOOKFORCLIENT)){
                    all_books[i].num_copies--;
                    strcpy(replymsg->msgstr,BOOKCANBELENT);
                    pthread_mutex_unlock(&library_mutex);
                    pthread_mutex_unlock(&client_mutex);
                    return replymsg;
                }
                else if(!strcmp(verdict_transac->msg.msgstr,ALREADYHAVEACOPY)){
                    strcpy(replymsg->msgstr,ALREADYHAVEACOPY);
                    pthread_mutex_unlock(&library_mutex);
                    pthread_mutex_unlock(&client_mutex);
                    return replymsg;
                }
                else{
                    strcpy(replymsg->msgstr,ADDINGBOOKFAILED);
                    pthread_mutex_unlock(&library_mutex);
                    pthread_mutex_unlock(&client_mutex);
                    return replymsg;
                }
            }
            else{
                strcpy(replymsg->msgstr,BOOKNOCOPIES);
                pthread_mutex_unlock(&library_mutex);
                pthread_mutex_unlock(&client_mutex);
                return replymsg;
            }
        }
    }
    strcpy(replymsg->msgstr,BOOKNOTINLIB);
    pthread_mutex_unlock(&library_mutex);
    printf("library unlocked\n");
    pthread_mutex_unlock(&client_mutex);
    printf("clients unlocked\n");
    
    return replymsg;
}
void* retrieve_book(void*arg){
    pthread_mutex_lock(&library_mutex);
    pthread_mutex_lock(&client_mutex);

    lib_transac* newtransac = (lib_transac*)(arg);
    scmessage* replymsg = (scmessage*)(malloc(sizeof(scmessage)));
    for(int i = 0; i<num_books; i++){
        if(check_book(newtransac->tbook, all_books[i])){
            lib_transac* verdict_transac = (lib_transac*)(find_client_and_retrieve_book(newtransac));
            if(!strcmp(verdict_transac->msg.msgstr,BOOKRETRIEVED)){
                all_books[i].num_copies++;
                strcpy(replymsg->msgstr,BOOKRETRIEVED);
                pthread_mutex_unlock(&library_mutex);
                pthread_mutex_unlock(&client_mutex);
                return replymsg;
            }
            else if (!strcmp(verdict_transac->msg.msgstr,BOOKNEVERTAKEN)){
                strcpy(replymsg->msgstr,BOOKNEVERTAKEN);
                pthread_mutex_unlock(&library_mutex);
                pthread_mutex_unlock(&client_mutex);
                return replymsg;
            }
            else{
                strcpy(replymsg->msgstr,RETRIEVINGBOOKFAILED);
                pthread_mutex_unlock(&library_mutex);
                pthread_mutex_unlock(&client_mutex);
                return replymsg;
            }
            
        }
    }
    strcpy(replymsg->msgstr,BOOKNOTINLIB);
    pthread_mutex_unlock(&library_mutex);
    pthread_mutex_unlock(&client_mutex);
    
    return replymsg;
    
}
void* pre_login(void*arg){
    arguments * cliargs = (arguments*)(arg);
    int menuchoice = 0;
    pthread_t menuthread,init_librarian_thread,init_library_thread,login_thread,createacc_thread;
    if(cliargs->choice==-1){
        //    int menuchoice = 0;
        int menuthreadresult = pthread_create(&menuthread, NULL, library_menu, &menuchoice);
        if(cliargs->argc==1){
            int init_libratian_threadresult = pthread_create(&init_librarian_thread, NULL, initialize_librarians, NULL);
        }
        else if (cliargs->argc>1){
            int init_libratian_threadresult = pthread_create(&init_librarian_thread, NULL, initialize_librarians, cliargs->argv[1]);
        }
        else{
            perror("Wrong usage\n");
            exit(1);
        }
        pthread_join(init_librarian_thread, NULL);
        pthread_join(menuthread, NULL);
    }
    else{
        printf("HEYYO");
        int menuthreadresult = pthread_create(&menuthread, NULL, library_menu, &menuchoice);
        pthread_join(menuthread, NULL);
    }
    
    thread_librarian newlogin;newlogin.verdict=false;//newlogin.libn.name = NULL; newlogin.libn.password = NULL;
    librarian newlib;
    int breaker = 0;
    while(!newlogin.verdict){
        cliargs->choice = menuchoice;
        switch (menuchoice) {
            case 0:
                write_librarians(NULL);
                printf("\n\nThank You\n\n");
                exit(0);
                break;
        
            case 1:
                pthread_create(&createacc_thread, NULL, librarian_create_acc, &newlogin);
                pthread_join(createacc_thread, NULL);
                if(!newlogin.verdict)printf("A Librarian with that name already exists, please login, or use a different name \n");
                breaker = 1;
                
                break;
                
            case 2:
                pthread_create(&login_thread, NULL, librarian_login, &newlogin);
                pthread_join(login_thread, NULL);
                if(!newlogin.verdict)printf("No Librarian with that name/password exists \n ");
                breaker = 2;
                break;
                
            default:
                break;
        }
        if(!newlogin.verdict)library_menu((void*)(&menuchoice));
    }
    cliargs->breaker = breaker;
    cliargs->person = newlogin;
    
    write_librarians(NULL);
    
    return (void*)(cliargs);
}
void* pre_connection_choice_menu(void*arg){
    int* menuchoice = (int*)(arg);
    int verify=0;
    printf("\n\t0.Exit");
    printf("\n\t1.Logout");
    printf("\n\t2.Connect to a client");
    printf("\n\t3.Add a book");
    printf("\n\t4.Display books\n");
    while(scanf("%d",&verify)&&((verify>4)|(verify<0)))printf("\nEnter a valid input:");
    *menuchoice = verify;
    return NULL;
}
void* client_name_checker(void*arg){
    scmessage* msg = (scmessage*)(arg);
    scmessage* replymsg = (scmessage*)(malloc(sizeof(scmessage)));
    for(int i = 0; i<num_clients; i++){
        if(!strcmp(all_clients_in_this_session[i].name,msg->msgclient.name)){
            strcpy(replymsg->msgstr,NAMEALREADYUSED);
            replymsg->msgclient = msg->msgclient;
            return replymsg;
        }
    }
    strcpy(replymsg->msgstr,NAMEFREE);
    replymsg->msgclient = msg->msgclient;
    return replymsg;
}
void* add_client(void*arg){
    pthread_mutex_lock(&client_mutex);
    printf("In add client\n");
    scmessage* msg = (scmessage*)(arg);
    printf("clientname: %s\n",msg->msgclient.name);
    strcpy(all_clients_in_this_session[num_clients].name,msg->msgclient.name);
    strcpy(all_clients_in_this_session[num_clients].password,msg->msgclient.password);
    all_clients_in_this_session[num_clients].num_books_owned = msg->msgclient.num_books_owned;
    for(int i = 0; i<all_clients_in_this_session[num_clients].num_books_owned;i++){
        all_clients_in_this_session[num_clients].books_owned[i] = msg->msgclient.books_owned[i];
    }
    num_clients++;
    printf("now, numclients: %d\n",num_clients);
    scmessage* replymsg = (scmessage*)(malloc(sizeof(scmessage)));
//    pthread_mutex_t librarian_mutex = PTHREAD_MUTEX_INITIALIZER;
    strcpy(replymsg->msgstr,SUCCESSFULLYADDED);
    replymsg->msgclient = msg->msgclient;
    pthread_mutex_unlock(&client_mutex);
    return replymsg;
}
void* client_login_checker(void*arg){
    scmessage* msg = (scmessage*)(arg);
    bool *verdict = (bool*)(malloc(sizeof(bool)));
    printf("clientname: %s, password: %s\n",msg->msgclient.name,msg->msgclient.password);
    for(int i = 0; i<num_clients; i++){
        printf("%s:%s\n",all_clients_in_this_session[i].name,all_clients_in_this_session[i].password);
        if(!strcmp(msg->msgclient.name,all_clients_in_this_session[i].name) && !strcmp(msg->msgclient.password,all_clients_in_this_session[i].password)){
            printf("found a match\n");
            *verdict = true;
//            pthread_exit((void*)verdict);
            return verdict;
        }
    }
    *verdict = false;
//    pthread_exit((void*)verdict);
    return verdict;
}
void* client_login(void*arg){
    scmessage* msg = (scmessage*)(arg);
    scmessage* replymsg = (scmessage*)(malloc(sizeof(scmessage)));
    replymsg->msgclient = msg->msgclient;
    pthread_mutex_lock(&client_mutex);
    printf("In login_client\n");
    printf("clientname: %s, password: %s\n",msg->msgclient.name,msg->msgclient.password);
//    pthread_t login_checker_thread;
    bool* verdict = false;
//    int login_checker_res = pthread_create(&login_checker_thread, NULL, client_login_checker, msg);
//    pthread_join(login_checker_thread, (void**)(&verdict));
    verdict = (bool*)client_login_checker(msg);
    if (*verdict){
        strcpy(replymsg->msgstr,AUTHENTICATED);
    }
    else{
        strcpy(replymsg->msgstr,COULDNTLOGIN);
    }
    pthread_mutex_unlock(&client_mutex);
    return (void*)replymsg;
}

void* send_all_books(void*arg){
    scmessage* msg = (scmessage*)(arg);
    scmessage* replymsg = (scmessage*)(malloc(sizeof(scmessage)));
    strcpy(replymsg->msgstr,"Available books:\n");
    for(int i = 0; i<num_books; i++){
        if(all_books[i].num_copies>0){
            char str[100];
            sprintf(str,"\tBookname: %s, Author: %s\n", all_books[i].book.book_name,all_books[i].book.book_author);
            strcat(replymsg->msgstr, str);
        }
    }
    
    return replymsg;
}
void* send_owned_books(void*arg){
    scmessage* msg = (scmessage*)(arg);
    scmessage* replymsg = (scmessage*)(malloc(sizeof(scmessage)));
    for(int i = 0; i<num_clients; i++){
        printf("\n\t%s,%s\n",msg->msgclient.name,all_clients_in_this_session[i].name);
        if(client_compareer(msg->msgclient, all_clients_in_this_session[i])){
            printf("num books owed bby client = %d\n",all_clients_in_this_session[i].num_books_owned);
            strcpy(replymsg->msgstr,"Books You Own:\n");
            for(int j = 0; j<all_clients_in_this_session[i].num_books_owned;j++){
                char str[100];
                sprintf(str,"\tBookname: %s, Author: %s\n",     all_clients_in_this_session[i].books_owned[j].book_name,all_clients_in_this_session[i].books_owned[j].book_author);
                strcat(replymsg->msgstr, str);
            }
            return replymsg;
        }
    }
    
    strcpy(replymsg->msgstr,COULDNTFINDCLIENT);
    return replymsg;
}

void* initiate_connection(void*arg){
    arguments* cliargs = (arguments*)(arg);
    printf("initiateconn-arg: %s\n",cliargs->argv[0]);
    bool yes = true,no=false;
    pthread_t init_clients;
    if(cliargs->argc==1 || cliargs->argv[1][2]!='c')
        pthread_create(&init_clients, NULL, initialize_clients, NULL);
    else
        pthread_create(&init_clients, NULL, initialize_clients, cliargs->argv[1]);
    if(!cliargs->bound){
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        
        if (sockfd == -1){
            perror("Error: ");
            exit(1);
        }
        
        int opt = 1;
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
            perror("Error: Couldn't connect(couldn't set socket), Try later\n");
            close(sockfd);
            return NULL;
            //        exit(1);
        }
        memset(&serv, 0, sizeof(serv));
        serv.sin_family = AF_INET;
        serv.sin_addr.s_addr = INADDR_ANY;
        serv.sin_port = htons(5555);
        
        if (bind(sockfd, (struct sockaddr *)&serv, sizeof(serv)) == -1){
            perror("Error: Couldn't connect(socket binding failed), Try later\n");
            return NULL;
            //        exit(1);
        }
        
        if (listen(sockfd, 5) == -1){
            perror("Error: Couldn't connect(failed to listen), Try later\n");
            close(sockfd);
            return NULL;
            //        exit(1);
        }
        
        cliargs->bound = true;
        printf("Server set up successfully\n");
    }
    
    pthread_join(init_clients, NULL);
    while (true){
        
        int new_fd = accept(sockfd, (struct sockaddr *)&cli, &size);
        if (new_fd == -1){
            perror("Error: Couldn't connect, Try later\n");
            close(sockfd);
            return NULL;
        }
        int pipefd[2];
        pipe(pipefd);
        int x = fork();
        if (!x){
            
            printf("Connection with client successful\n");
            close(sockfd);
        //
        ////            int user;
            scmessage newmessage;
            //            read(new_fd, &newmessage, sizeof(scmessage*));
            //            printf("%s : \n",newmessage.msgstr);
            //            printf("%s : %s\n",newmessage.msgstr,newmessage.msgclient.name);
            ssize_t bytes_read = read(new_fd, &newmessage, sizeof(scmessage));
            printf("%s\n",newmessage.msgstr);
            if (bytes_read < 0) {
                perror("Error: Couldn't read data\n");
            }
            else if (bytes_read == 0) {
                printf("Client disconnected\n");
            } 
            else {
//                printf("Message: %s\n", newmessage.msgstr);
//                printf("Client Name: %s\n", newmessage.msgclient.name);
                if(!strcmp(newmessage.msgstr,STARTCOMM)){
                    printf("Communication Started\n");
                    while(strcmp(newmessage.msgstr,ENDCOMM)){
                        if (bytes_read < 0) {
                            perror("Error: Couldn't read data\n");
                            break;
                        }
                        else if (bytes_read == 0) {
                            printf("Client disconnected\n");
                            break;
                        }
                        else{
                            if(!strcmp(newmessage.msgstr,CHECK_NAME_MSG)){
                                scmessage* replymessage = (scmessage*)(client_name_checker(&newmessage));
                                write(new_fd, replymessage, sizeof(scmessage));
                            }
                            else if (!strcmp(newmessage.msgstr,ADD_CLIENT_MSG)){
                                printf("add client requested\n");
                                scmessage* replymessage = (scmessage*)(add_client(&newmessage));
                                write(new_fd, replymessage, sizeof(scmessage));
                            }
                            else if (!strcmp(newmessage.msgstr,LOGIN_CLIENT_MSG)){
                                printf("login client requested\n");
                                printf("%s,%s\n",newmessage.msgclient.name,newmessage.msgclient.password);
                                scmessage* replymessage = (scmessage*)(client_login(&newmessage));
                                printf("checked, %s\n",replymessage->msgstr);
                                write(new_fd, replymessage, sizeof(scmessage));
                            }
                            else if(!strcmp(newmessage.msgstr,BORROW_BOOK_MSG)){
                                book tolend;
                                scmessage* replymessage = (scmessage*)(send_all_books(&newmessage));
                                printf("%s",replymessage->msgstr);
                                write(new_fd, replymessage, sizeof(scmessage));
                                read(new_fd, &tolend, sizeof(book));
                                printf("requested book: %s, %s\n",tolend.book_name,tolend.book_author);
                                lib_transac* newborrow = (lib_transac*)(malloc(sizeof(lib_transac)));
                                newborrow->msg = newmessage;
                                printf("cname:%s\n",newborrow->msg.msgclient.name);
                                newborrow->tbook.book = tolend;
                                newborrow->tbook.num_copies = 1;
                                replymessage = lend_book(newborrow);
                                printf("\n%s\n",replymessage->msgstr);
                                write(new_fd, replymessage, sizeof(scmessage));
                            }
                            else if(!strcmp(newmessage.msgstr,RETURN_BOOK_MSG)){
                                book toretrieve;
                                scmessage* replymessage = (scmessage*)(send_owned_books(&newmessage));
                                printf("%s",replymessage->msgstr);
                                write(new_fd, replymessage, sizeof(scmessage));
                                read(new_fd, &toretrieve, sizeof(book));
                                printf("requested book: %s, %s\n",toretrieve.book_name,toretrieve.book_author);
                                lib_transac* newreturn = (lib_transac*)(malloc(sizeof(lib_transac)));
                                newreturn->msg = newmessage;
                                newreturn->tbook.book = toretrieve;
                                newreturn->tbook.num_copies = 1;
                                replymessage = retrieve_book(newreturn);
                                printf("\n%s\n",replymessage->msgstr);
                                write(new_fd, replymessage, sizeof(scmessage));
                            }
                            else if(!strcmp(newmessage.msgstr,DISPLAY_OWNED_BOOKS)){
                                scmessage* replymessage = (scmessage*)(send_owned_books(&newmessage));
                                printf("%s",replymessage->msgstr);
                                write(new_fd, replymessage, sizeof(scmessage));
                            }
                            else if(!strcmp(newmessage.msgstr,DISPLAY_ALL_AVAILABLE_BOOKS)){
                                scmessage* replymessage = (scmessage*)(send_all_books(&newmessage));
                                printf("%s",replymessage->msgstr);
                                write(new_fd, replymessage, sizeof(scmessage));
                            }
                        }
                        write_clients(NULL);
                        write_books(NULL);
                        bytes_read = read(new_fd, &newmessage, sizeof(scmessage));
                    }
                }
            }
            close(pipefd[0]);
            if(write(pipefd[1], &num_clients, sizeof(int))==-1){
                perror("Couldn't transfer info to the parent, rolling back data to previous client\n");
            }
            if(write(pipefd[1], all_clients_in_this_session, sizeof(all_clients_in_this_session))==-1){
                perror("Couldn't transfer info to the parent, rolling back data to previous client\n");
            }
            close(pipefd[1]);
            printf("Connection terminated\n");
            close(new_fd);
            exit(num_clients);

        }
        else{
            int retstatus = 0;
            close(pipefd[1]);
            if(read(pipefd[0], &num_clients, sizeof(int))==-1){
                perror("Couldn't get info from the child, rolling back data to previous client\n");
            }
            if(read(pipefd[0], all_clients_in_this_session, sizeof(all_clients_in_this_session))==-1){
                perror("Couldn't get info from the child, rolling back data to previous client\n");
            }
            close(pipefd[0]);
            waitpid(x,&retstatus,0);
//            printf("numclients by child:%d\n",retstatus);
            printf("child ended\n");
            close(new_fd);
            write_clients(NULL);
            printf("written client data to file\n");
        }
        
    }
    return NULL;
}


void* post_login_choice(void*arg){
    arguments * cliargs = (arguments*)(arg);
//    printf("args: %s\n",cliargs->argv[1]);
    printf("\t\tWelcome, %s",cliargs->person.libn.name);
    pthread_t menuthread,init_library_thread;
    int pre_connection_choice;
    int menuthreadresult = pthread_create(&menuthread, NULL, pre_connection_choice_menu, &pre_connection_choice);
    if(cliargs->argc==1){
        int init_library_threadresult = pthread_create(&init_library_thread, NULL, initialize_books, NULL);
    }
    else if (cliargs->argc>=2){
        int init_library_threadresult = pthread_create(&init_library_thread, NULL, initialize_books, cliargs->argv[1]);
    }
//    else if (cliargs->argc>=3){
//        int init_library_threadresult = pthread_create(&init_library_thread, NULL, initialize_books, cliargs->argv[2]);
//    }
    else{
        perror("Wrong usage\n");
        exit(1);
    }
    
    pthread_join(init_library_thread, NULL);
    //    printf("l:%d b:%d\n",num_librarians,num_books);
    pthread_join(menuthread, NULL);
    while(true){
        cliargs->choice = pre_connection_choice;
        if(pre_connection_choice==0){
            printf("\n\nThank You\n\n");
            write_books(NULL);
            exit(0);
        }
        else if(pre_connection_choice==1){
            cliargs->person.verdict = false;
            printf("\n..Logging out\n\n");
            write_books(NULL);
            return NULL;
        }
        else if (pre_connection_choice==2){
            printf("postlogin args:%s\n",cliargs->argv[1]);
            initiate_connection(cliargs);
        }
        else if (pre_connection_choice==3){
            add_book(NULL);
        }
        else if (pre_connection_choice==4){
            display_books(NULL);
        }
        printf("\t\tWelcome, %s",cliargs->person.libn.name);
        pre_connection_choice_menu((void*)(&pre_connection_choice));
    }
    
    return NULL;
}
int main(int argc, char *argv[]){
    arguments cliargs;
    cliargs.argc = argc;
    cliargs.argv = argv;
    thread_librarian loginperson;
    pthread_t pre_login_thread,pre_connection_thread;
    cliargs.choice = -1;
    cliargs.bound = false;
    while(cliargs.choice!=0){
        pre_login((void*)(&cliargs));
        post_login_choice((void*)(&cliargs));
    }
    return 0;
}
