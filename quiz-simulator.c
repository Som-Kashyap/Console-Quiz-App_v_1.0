#include<stdio.h>
#include<string.h>
#include<time.h>

struct answer {

    float ans;

    char status[100];

};

void get_questions() {

    FILE* fp;

    char Buffer[100];

    fp = fopen ("questions.txt" , "r");

    if ( !fp ) printf("ERROR opening file for displaying question set!");

    while ( fgets ( Buffer , 100 , fp )!=NULL ) {
        printf ( "%s" , Buffer );
    }

    fclose ( fp );
}

void check_answers( float answer_array[] , struct answer ans[] , int *score ) {

    for ( int i = 0; i < 2; i++ ) {


        if ( ans[i].ans == answer_array[i] ) {
            
            *score++;
            strcpy( ans[i].status , "CORRECT" );

        }

        else{

            strcpy( ans[i].status , "INCORRECT" );

        }
    }
}

void calculate_result( float answer_array[] , struct answer ans[] , int *score ) {

    FILE* fp;
    char buffer[100];

    time_t curr_time;
    time(&curr_time);

    fp = fopen ( "result.txt" , "a" );

    if (!fp) printf("ERROR- writing to result file!");

    fprintf(fp ,"\n----------------------------");
    fprintf( fp , "\nAttempted on :- %s" , ctime(&curr_time) );

    for ( int i = 0; i < 2; i++ ) {

        fprintf ( fp , "\nanswer-%d : %s" , i+1 ,ans[i].status );

    }

    fprintf (fp,"\n overall score: (%p/2)" , *score);

    printf ("Result written to file succesfully!");

    fclose(fp);

}

int main () {

    float answer_array[] = { 14 , 12 };

    get_questions();

    struct answer ans[2];

    int score = 0;
    
    for ( int i = 0; i < 2; i++ ) {

        printf ( "\nEnter answer of question-%d:- " , i+1 );

        scanf("%f" , &ans[i].ans);

    }

    check_answers(  answer_array , ans , &score );
    calculate_result( answer_array , ans , &score );

    return 0;
}

