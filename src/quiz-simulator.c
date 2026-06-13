#include <stdio.h>
#include <string.h>
#include <time.h>
#include "users.h"

struct answer {

    float ans;
    char status[100];

};

enum operations {

    menu,
    signup,
    start,
    attempts,
    admin

};

void get_questions() {

    FILE* fp;
    char buffer[100];


    fp = fopen("questions.txt", "r");

    if (!fp) {

        printf("ERROR opening file for displaying question set!\n");
        return;

    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {

        printf("%s", buffer);

    }

    fclose(fp);

}

void check_answers(float answer_array[], struct answer ans[], int *score, int *correct, int *incorrect , int number_of_questions) {

    for (int i = 0; i < number_of_questions; i++) {

        if (ans[i].ans == answer_array[i]) {

            (*score)++;
            (*correct)++;
            strcpy(ans[i].status, "CORRECT");

        } else {

            (*incorrect)++;
            strcpy(ans[i].status, "INCORRECT");

        }
    }
}

void calculate_result(float answer_array[], struct answer ans[], int score, int correct, int incorrect, char name[] , int number_of_questions) {

    FILE* fp;
    time_t curr_time;
    time(&curr_time);

    fp = fopen("result.txt", "a");
    if (!fp) {

        printf("ERROR writing to result file!\n");
        return;

    }

    fprintf(fp, "\n----------------------------");

    fprintf(fp, "\nAttempted on :- %s", ctime(&curr_time));
    fprintf(fp, "Username: %s", name);

    for (int i = 0; i < number_of_questions; i++) {

        fprintf(fp, "\nanswer-%d : %s", i + 1, ans[i].status);

    }

    fprintf(fp, "\nOverall score: (%d/%d)", score , number_of_questions);
    fprintf(fp, "\nCorrect: %d | Incorrect: %d", correct, incorrect);

    float accuracy = ((float)correct / number_of_questions) * 100;
    fprintf(fp, "\nAccuracy: (%.2f%%)", accuracy);

    printf("Results written to file successfully!\n");

    fclose(fp);
}

void display_history() {

    FILE* fp;
    char buffer[100];

    fp = fopen("result.txt", "r");
    if (!fp) {

        printf("ERROR opening file for displaying results!\n");
        return;

    }

    printf("Attempt History Loading........\n");
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }

    fclose(fp);
}

void add_question() {

    FILE* fp;
    char buffer[100];

    printf("Enter question:-\n");

   if (!(fgets(buffer, sizeof(buffer), stdin))) {

    printf("ERROR- could not add questions!\n");
    return;

}

    fp = fopen("questions.txt", "a");

    if (!fp) {
        perror("ERROR opening file for adding questions!");
        return;
    }

    if (fprintf(fp, "%s", buffer) < 0) {
        perror("ERROR adding question!");
        fclose(fp);
        return;
    }

    printf("Question added successfully!\n");
    fclose(fp);
}

void delete_history() {

    FILE* fp = fopen("result.txt", "w");
    if (!fp) {
        perror("ERROR deleting contents of result file!");
        return;
    }

    fprintf(fp, "---The history was cleared by the admin!---");
    printf("History cleared successfully!\n");

    fclose(fp);

}

void User_Interface(enum operations *opt) {

    int option_selected;

    printf("\nQUIZ APP:\n");
    printf("1) START QUIZ\n");
    printf("2) SIGNUP\n");
    printf("3) VIEW PAST ATTEMPTS\n");
    printf("4) ADMIN LOGIN\n");

    printf("Enter choice:- ");
    
    if( scanf("%d", &option_selected) != 1){
        printf("Invalid Input!\n");
        *opt = menu;
        while ((getchar()) != '\n');
        return;
    }
    while ((getchar()) != '\n'); // clear buffer

    switch (option_selected) {
        case 1: *opt = start; break;
        case 2: *opt = signup; break;
        case 3: *opt = attempts; break;
        case 4: *opt = admin; break;
        default: *opt = menu; break;
        
    }
}

void logic(enum operations opt, char name[], char correct_password[], struct answer ans[], float answer_array[], int *score, int *correct, int *incorrect , int number_of_questions , struct User user[] , int *userCount ) {

    int admin_choice;

    if (opt == start) {

        printf("\nEnter your Name:- ");
        fgets(name, 50, stdin);

        name[strcspn(name, "\n")] = '\0'; // strip newline

        int user_idx = Find_Users( user , *userCount , name );

        if ( user_idx == -1 ) {
            printf( "No such user was found!");
            return;
        }

        get_questions();

        for (int i = 0; i < number_of_questions; i++) {

            printf("\nEnter answer of question-%d:- ", i + 1);
            scanf("%f", &ans[i].ans);

        }
        
        while ((getchar()) != '\n'); // clear buffer
        
        user[user_idx].attempts++;

        check_answers(answer_array, ans, score, correct, incorrect , number_of_questions);
        calculate_result(answer_array, ans, *score, *correct, *incorrect, name , number_of_questions);
        Save_Users( user , *userCount );

    } else if (opt == attempts) {
        display_history();

    } else if (opt == admin) {

        char password[50];
        printf("Enter password:- ");
        scanf("%s", password);

        if (strcmp(password, correct_password) == 0) {
            printf("ADMIN MENU:-\n");
            printf("1) Add question\n");
            printf("2) Delete attempt history\n");

            scanf("%d", &admin_choice);

            if (admin_choice == 1) {

                while ((getchar()) != '\n'); // clear buffer
                add_question();

            } else if (admin_choice == 2) {
                delete_history();
            } else {
                printf("Invalid Input!\n");
            }
        } else {
            printf("Incorrect password!\n");
        }
    }
    else if (opt == signup)
{
    printf("Creating new user.....\n");

    printf("\nEnter your Name:- ");
    fgets(name, 50, stdin);

    name[strcspn(name, "\n")] = '\0';

    printf("You entered: '%s'\n", name);

    Add_Users(user, userCount, name);

    printf("Signup finished\n");
}
}

int main() {

    struct User user[MAX_USERS];
    int userCount = 0;

    int number_of_questions = 3;

    enum operations opt = menu;
    float answer_array[] = {14, 12 , 0};
    char name[50];
    char correct_password[] = "somkashyap01";
    struct answer ans[number_of_questions];
    int score = 0, correct = 0, incorrect = 0;

    printf("Loading user/s data.....");
    Load_Users( user , &userCount );
    printf("\nLoaded %d users!" , userCount);
    
    while (1) {

    User_Interface(&opt);
    logic(opt, name, correct_password, ans, answer_array, &score, &correct, &incorrect , number_of_questions , user , &userCount);

    }

    return 0;

}
