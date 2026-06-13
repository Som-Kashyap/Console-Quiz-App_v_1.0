#include <stdio.h>
#include <string.h>
#include "users.h"

void Load_Users(struct User user[], int *userCount)
{
    FILE *fp = fopen("users.txt", "r");

    if(fp == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    while(fscanf(fp,
                 "%49[^,],%d,%d\n",
                 user[*userCount].username,
                 &user[*userCount].attempts,
                 &user[*userCount].bestScore) == 3)
    {
        (*userCount)++;
    }

    fclose(fp);
}

void Save_Users ( struct User user[] , int userCount ) {

    FILE* fp = fopen( "users.txt" , "w" );

    if (!fp) {
    printf("ERROR opening file!\n");
    return;
}

    printf("Saving %d users\n", userCount);

    for ( int i = 0; i < userCount; i++ ) {

        fprintf( fp , "%s,%d,%d\n" , user[i].username , user[i].attempts , user[i].bestScore );

    }

    printf("Data written to users file successfully!");

    fclose(fp);

}

int Find_Users ( struct User user[] , int userCount , char name[] ) {

    for( int i = 0; i < userCount; i++ ) {

        if( strcmp( user[i].username , name ) == 0 ) return i; 
    }

    return -1;
}

void Add_Users ( struct User user[] , int *userCount , char name[] ) {

    strcpy( user[*userCount].username , name);
    user[*userCount].attempts = 0;
    user[*userCount].bestScore = 0;

    printf("Users Before increment: %d\n", *userCount);

    (*userCount)++;

    printf("Users After increment: %d\n", *userCount);

    Save_Users( user , *userCount);

}