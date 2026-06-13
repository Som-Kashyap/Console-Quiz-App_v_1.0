#ifndef USER_H
#define USER_H

#define MAX_USERS 100

struct User {

    char username[50];
    int attempts;
    int bestScore;

};

void Load_Users ( struct User user[] , int *userCount );
void Save_Users ( struct User user[] , int userCount );
int Find_Users ( struct User user[] , int userCount , char name[] );
void Add_Users ( struct User user[] , int *userCount , char name[] );

#endif