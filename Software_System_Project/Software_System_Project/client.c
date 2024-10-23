#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <arpa/inet.h>

struct customer
{
    char username[20];
    char password[20];
    int age;
    int account_number;
    int wp;
    int dp;
    float balance;
    float withdrawn_history[5];
    float deposit_history[5];
    int islogged;
};
struct employee{
    char username[20];
    char password[20];
    int age;
    int empid;
    int loans_assigned[10];
    int l_assi;
    int loans_amount[10];
    int l_amti;
    int islogged;
};
struct manager
{
    char username[20];
    char password[20];
    int islogged;
};
struct administrator
{
    char username[30];
    char password[30];
};




#define PORT 8080
#define BUFFER_SIZE 1024



int customer(int sock) {
    struct customer cust;
    
    char buffer[BUFFER_SIZE] = {0};
    char username[BUFFER_SIZE] = {0};
    char password[BUFFER_SIZE]= {0};


    recv(sock, &username, sizeof(username), 0);
    printf("%s\n", username);
    fgets(cust.username, 20, stdin);
    send(sock, &cust.username, sizeof(cust.username), 0);

    
    recv(sock, &password, sizeof(password), 0);
    printf("%s\n", password);
    fgets(cust.password, 20, stdin);
    cust.password[strcspn(cust.password, "\n")] = 0;
    send(sock, &cust.password, sizeof(cust.password), 0);

    int present;
    recv(sock, &present, sizeof(present), 0);
    if(present==0)
    {
        printf("Wrong Credentials or Already Logged In \n");
        exit(EXIT_FAILURE);
    } 
    else
    {
        printf("\nLogin Successful \n");

        while(1)
        {

            int choice;
            char buffer[BUFFER_SIZE] = {0};
            recv(sock, &buffer, sizeof(buffer), 0);
            printf("%s", buffer);
            scanf("%d", &choice);
            send(sock, &choice, sizeof(choice), 0);

            if(choice == 1)
            {
                char buffer8[26];
                int balance;

                recv(sock, buffer8, sizeof(buffer8), 0);
                printf("%s", buffer8);
                recv(sock, &balance, sizeof(balance), 0);
                printf("%d", balance);

            }
            else if(choice ==2)
            {
                char buffer[41];
                char buffer1[34];
                int ammount;

                recv(sock, &buffer, sizeof(buffer), 0);
                printf("%s", buffer);
                scanf("%d", &ammount);
                send(sock, &ammount, sizeof(ammount), 0);
                recv(sock, &buffer1, sizeof(buffer1), 0);
                printf("%s\n", buffer1);

            }
            else if(choice == 3)
            {
                char buffer[43];
                char buffer1[34];
                int ammount;

                recv(sock, &buffer, sizeof(buffer), 0);
                printf("%s", buffer);
                scanf("%d", &ammount);
                send(sock, &ammount, sizeof(ammount), 0);

                while (getchar() != '\n');

                recv(sock, &buffer1, sizeof(buffer1), 0);
                printf("%s\n", buffer1);

            }
            else if(choice == 4)
            {
                int person;
                char buffer[62];
                char buffer1[50];
                char buffer2[34];
                int account_no;
                int ammount;
                recv(sock, buffer, sizeof(buffer), 0);
                printf("%s", buffer);
                scanf("%d", &account_no);
                send(sock, &account_no, sizeof(account_no), 0);

                recv(sock, &person, sizeof(person), 0);

                if(person == 0)
                {
                    printf("Account Number you have given is invalid\n");
                }
                else
                {
                    recv(sock, buffer1, sizeof(buffer1), 0);
                    printf("%s", buffer1);
                    scanf("%d", &ammount);
                    send(sock, &ammount, sizeof(ammount), 0);
                    recv(sock, buffer2, sizeof(buffer2), 0);
                    printf("%s\n", buffer2);
                }

            }
            else if(choice == 5)
            {
                char buffer[46];
                char buffer1[28];
                char buffer2[31];

                int amount, account_no;
                char username[20];

                recv(sock, buffer, sizeof(buffer), 0);
                printf("%s", buffer);
                scanf("%d", &amount);
                send(sock, &amount, sizeof(amount), 0);

                recv(sock, buffer1, sizeof(buffer1), 0);
                printf("%s", buffer1);
                scanf("%d", &account_no);
                send(sock, &account_no, sizeof(account_no), 0);
                
                while (getchar() != '\n');

            }
            else if(choice == 6)
            {
                int history_length;
                    
                recv(sock, &history_length, sizeof(history_length), 0);

                char buffer[history_length + 1]; 
                buffer[history_length] = '\0'; 

                recv(sock, buffer, history_length, 0);

                printf("%s\n", buffer);
            }

            else if(choice == 7)
            {
                char buffer[53];
                char feedback[513];
                
                recv(sock, buffer, sizeof(buffer), 0);

                printf("%s\n", buffer);
                while (getchar() != '\n');

                fgets(feedback, sizeof(feedback), stdin);

                send(sock, feedback, sizeof(feedback), 0);
            }
            else if(choice == 8)
            {
                char buffer[25];
                char password[30];
                recv(sock, buffer, sizeof(buffer), 0);
                printf("%s", buffer);
                
                scanf("%s", password);
                send(sock, password, sizeof(password), 0);

            }
            else if(choice == 9)
            {
                break;
            }

            memset(buffer, 0, sizeof(buffer));   
        }

    }

    printf("Thank You For Accessing Bank Portal \n");
    
    sleep(3);

    close(sock);
    return 0;

}


int employee(int sock) {

    struct employee emp;
    char buffer[BUFFER_SIZE] = {0};
    char username[BUFFER_SIZE] = {0};
    char password[BUFFER_SIZE]= {0};
    

    recv(sock, &username, sizeof(username), 0);
    printf("%s\n", username);
    fgets(emp.username, 20, stdin);
    send(sock, &emp.username, sizeof(emp.username), 0);

    
    recv(sock, &password, sizeof(password), 0);
    printf("%s\n", password);
    fgets(emp.password, 20, stdin);
    emp.password[strcspn(emp.password, "\n")] = 0;
    send(sock, &emp.password, sizeof(emp.password), 0);

    int present;
    recv(sock, &present, sizeof(present), 0);
    if(present==0)
    {
        printf("Wrong Credentials or Already Logged In \n");
        exit(EXIT_FAILURE);
    } 
    else
    {
        printf("\nLogin Successful \n");

        while(1)
        {

            int choice;
            char buffer[BUFFER_SIZE] = {0};
            recv(sock, &buffer, BUFFER_SIZE, 0);
            printf("%s\n", buffer);
            scanf("%d", &choice);
            send(sock, &choice, sizeof(choice), 0);

            if(choice == 1)
            {
                while (getchar() != '\n');
                char buffer1[BUFFER_SIZE] = {0};
                char username1[BUFFER_SIZE] = {0};
                char password1[BUFFER_SIZE]= {0};

                char customername[20];
                char custpassword[20];
                int age;

                recv(sock, &username1, sizeof(username1), 0);
                printf("%s\n", username1);
                fgets(customername, 20, stdin);
                send(sock, &customername, sizeof(customername), 0);

                recv(sock, &password1, sizeof(password1), 0);
                printf("%s\n", password1);
                fgets(custpassword, 20, stdin);
                send(sock, &custpassword, sizeof(custpassword), 0);

                recv(sock, &buffer1, sizeof(buffer1), 0);
                printf("%s\n", buffer1);
                scanf("%d", &age);
                send(sock, &age, sizeof(age), 0);


            }
            else if(choice ==2)
            {
                char buffer[BUFFER_SIZE] = {0};
                int account_no;

                recv(sock, &buffer, sizeof(buffer), 0);
                printf("%s\n", buffer);
                scanf("%d", &account_no);
                send(sock, &account_no, sizeof(account_no), 0);

                while (getchar() != '\n');
                char buffer1[BUFFER_SIZE] = {0};
                char username1[BUFFER_SIZE] = {0};
                char password1[BUFFER_SIZE]= {0};

                char customername[20];
                char custpassword[20];
                int age;
                int present;

                recv(sock, &present, sizeof(present), 0);

                if(present==1)
                {
                
                recv(sock, &username1, sizeof(username1), 0);
                printf("%s\n", username1);
                fgets(customername, 20, stdin);
                send(sock, &customername, sizeof(customername), 0);

                recv(sock, &password1, sizeof(password1), 0);
                printf("%s\n", password1);
                fgets(custpassword, 20, stdin);
                send(sock, &custpassword, sizeof(custpassword), 0);

                recv(sock, &buffer1, sizeof(buffer1), 0);
                printf("%s\n", buffer1);
                scanf("%d", &age);
                send(sock, &age, sizeof(age), 0);

                }
                else
                {
                    printf("Invalid Account Number \n");
                }


            }
            else if(choice == 3)
            {
                
                char buffer1[82] = {0};
                char buffer2[24] = {0};
                int present;
                int length;
                recv(sock, &length, sizeof(length), 0);
                char *buffer = malloc(length+1);
                recv(sock, buffer, length, 0);
                printf("%s\n", buffer);
                recv(sock, &present, sizeof(present), 0);
                if(present==1)
                {
                    int loanid, loanasgmt;
                    scanf("%d", &loanid);
                    send(sock, &loanid, sizeof(loanid), 0);
                    recv(sock, buffer1, sizeof(buffer1), 0);
                    printf("%s", buffer1);
                    scanf("%d", &loanasgmt);
                    send(sock, &loanasgmt, sizeof(loanasgmt), 0);
                    recv(sock, buffer2, sizeof(buffer2), 0);
                    printf("%s\n", buffer2);
                }

            }
            else if(choice == 4)
            {
                char buffer[BUFFER_SIZE] = {0};
                recv(sock, &buffer, sizeof(buffer), 0);
                printf("%s\n", buffer);
            }
            else if(choice == 5)
            {
                int data_size;

                recv(sock, &data_size, sizeof(data_size), 0);

                char *employee_data = malloc(data_size + 1);

                recv(sock, employee_data, data_size, 0);
                employee_data[data_size] = '\0';
                printf("%s\n", employee_data);
            }
            else if(choice == 6)
            {
                char buffer[26];
                char password1[20];
                recv(sock, buffer, sizeof(buffer), 0);
                printf("%s", buffer);

                int len;
                scanf("%s",password1);
                send(sock, password1, sizeof(password1), 0);
            }
            else if(choice == 7)
            {
                break;
            }
            else
            {
                printf("Invalid choice! Try again.\n");
            }

            while (getchar() != '\n');
    
        }

    }
    printf("Thank You For Accessing Bank Portal \n");
    
    sleep(3);

    close(sock);
    return 0;
}


int manager(int sock) {

    struct manager mang;
    
    char buffer[BUFFER_SIZE] = {0};
    char username[BUFFER_SIZE] = {0};
    char password[BUFFER_SIZE]= {0};


    recv(sock, &username, sizeof(username), 0);
    printf("%s\n", username);
    fgets(mang.username, 20, stdin);
    send(sock, &mang.username, sizeof(mang.username), 0);

    
    recv(sock, &password, sizeof(password), 0);
    printf("%s\n", password);
    fgets(mang.password, 20, stdin);
    mang.password[strcspn(mang.password, "\n")] = 0;
    send(sock, &mang.password, sizeof(mang.password), 0);

    int present;
    recv(sock, &present, sizeof(present), 0);
    if(present==0)
    {
        printf("Wrong Credentials or Already Logged In \n");
        exit(EXIT_FAILURE);
    } 
    else
    {
        printf("\nLogin Successful \n");

        while(1)
        {

            int choice;
            char buffer[BUFFER_SIZE] = {0};
            recv(sock, &buffer, sizeof(buffer), 0);
            printf("%s", buffer);
            scanf("%d", &choice);
            send(sock, &choice, sizeof(choice), 0);

            if(choice == 1)
            {
                char buffer8[33];
                char username[20];
                char result[30];

                recv(sock, buffer8, sizeof(buffer8), 0);
                printf("%s", buffer8);
                while (getchar() != '\n');
                fgets(username, sizeof(username), stdin);
                send(sock, username, sizeof(username), 0);

                recv(sock, result, sizeof(result), 0);
                printf("%s", result);

            }
            else if (choice == 2)
            {
                int size;

                recv(sock, &size, sizeof(size), 0);
                char *loan_items = malloc(size + 1);
                recv(sock, loan_items, size, 0);

                printf("%s\n", loan_items);

                free(loan_items);
            }
            else if (choice == 3)
            {
                int loanid;

                int size;

                recv(sock, &size, sizeof(size), 0);
                char *loan_items = malloc(size + 1);
                recv(sock, loan_items, size, 0);

                printf("%s\n", loan_items);

                char enterloanid[28]={0};
                recv(sock, enterloanid, sizeof(enterloanid), 0);
                printf("%s\n", enterloanid);
                scanf("%d", &loanid);     
                send(sock, &loanid, sizeof(loanid), 0);

                char buffername[32]={0};
                recv(sock, buffername, sizeof(buffername), 0);
                printf("%s\n", buffername);
                char username[20];
                while (getchar() != '\n');
                fgets(username, sizeof(username), stdin);
                send(sock, username, sizeof(username), 0);

            }
            else if(choice == 4)
            {
                int feedback_length;

                recv(sock, &feedback_length, sizeof(feedback_length), 0);

                char *feedback = malloc(feedback_length + 1);
                if (feedback == NULL) {
                    perror("Failed to allocate memory");
                    exit(EXIT_FAILURE);
                }

                recv(sock, feedback, feedback_length, 0);
                feedback[feedback_length] = '\0'; 

                printf("Customer Feedback:\n%s\n", feedback);

                free(feedback);
            }

            else if(choice == 5)
            {
                char buffer[25];
                char buffer1[26];
                char username[20], password[20];


                while (getchar() != '\n');
                recv(sock, buffer, sizeof(buffer), 0);
                printf("%s", buffer);
                fgets(username, sizeof(username), stdin);
                send(sock, username, sizeof(username), 0);

                recv(sock, buffer1, sizeof(buffer1), 0);
                printf("%s", buffer1);
                fgets(password, sizeof(password), stdin);
                send(sock, password, sizeof(password), 0);

            }
            else if(choice == 6)
            {
                break;
                
            }
            memset(buffer, 0, sizeof(buffer));   
        }

    }


    printf("Thank You For Accessing Bank Portal \n");
    
    sleep(3);

    close(sock);
    return 0;

}


int administrator(int sock) {

    struct administrator admin;
   
    char buffer[BUFFER_SIZE] = {0};
    char username[BUFFER_SIZE] = {0};
    char password[BUFFER_SIZE]= {0};
    size_t len=0;


    recv(sock, &username, sizeof(username), 0);
    printf("%s\n", username);
    fgets(admin.username, 20, stdin);
    admin.username[strcspn(admin.username,"\n")] = 0;
    send(sock, &admin.username, sizeof(admin.username), 0);

    
    recv(sock, &password, sizeof(password), 0);
    printf("%s\n", password);
    fgets(admin.password, 20, stdin);
    admin.password[strcspn(admin.password, "\n")] = 0;
    send(sock, &admin.password, sizeof(admin.password), 0);

    int present;
    recv(sock, &present, sizeof(present), 0);
    if(present==0)
    {
        printf("Wrong Credentials or Already Logged In\n");
        exit(EXIT_FAILURE);
    } 
    else
    {
        printf("\nLogin Successful \n");

        while(1)
        {

            int choice;
            char buffer[BUFFER_SIZE] = {0};
            recv(sock, &buffer, sizeof(buffer), 0);
            printf("%s", buffer);
            scanf("%d", &choice);
            send(sock, &choice, sizeof(choice), 0);

            if(choice == 1)
            {
                while (getchar() != '\n');
                char buffer1[BUFFER_SIZE] = {0};
                char username1[BUFFER_SIZE] = {0};
                char password1[BUFFER_SIZE]= {0};

                char customername[20];
                char custpassword[20];
                int age;

                recv(sock, &username1, sizeof(username1), 0);
                printf("%s\n", username1);
                fgets(customername, 20, stdin);
                send(sock, &customername, sizeof(customername), 0);

                recv(sock, &password1, sizeof(password1), 0);
                printf("%s\n", password1);
                fgets(custpassword, 20, stdin);
                send(sock, &custpassword, sizeof(custpassword), 0);

                recv(sock, &buffer1, sizeof(buffer1), 0);
                printf("%s\n", buffer1);
                scanf("%d", &age);
                send(sock, &age, sizeof(age), 0);

            }
            else if(choice ==2)
            {
                char buffer[43];
                char buffer1[26];
                char password[30];
                char username[30];

                recv(sock, &buffer, sizeof(buffer), 0);
                printf("%s", buffer);
                while (getchar() != '\n');
                fgets(username, sizeof(username), stdin);
                send(sock, username, sizeof(username), 0);

                recv(sock, &buffer1, sizeof(buffer1), 0);
                printf("%s", buffer1);
                fgets(password, sizeof(password), stdin);
                send(sock, password, sizeof(password), 0);

            }
            else if(choice == 3)
            {
                int length;
                char buffer[BUFFER_SIZE];
                
                recv(sock, &length, sizeof(length), 0);
                recv(sock, buffer, length, 0);
                printf("%s\n", buffer);

            }
            else if(choice == 4)
            {
                char buffer[26];
                char buffer1[26];
                char username[20], password[20];

                recv(sock, buffer, sizeof(buffer), 0);
                printf("%s", buffer);
                while (getchar() != '\n');
                fgets(username, sizeof(username), stdin);
                send(sock, username, sizeof(username), 0);

                recv(sock, buffer1, sizeof(buffer1), 0);
                printf("%s", buffer1);
                fgets(password, sizeof(password), stdin);
                send(sock, password, sizeof(password), 0);

            }
            else if(choice == 5)
            {
                break;
            }

            memset(buffer, 0, sizeof(buffer));   
        }

    }

    printf("Thank You For Accessing Bank Portal \n");
    
    sleep(3);

    close(sock);
    return 0;

}


int main() {

    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    char username[BUFFER_SIZE] = {0};
    char password[BUFFER_SIZE]= {0};
    size_t len=0;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    while(1)

{
    printf("1)Customer\n");
    printf("2)Employee\n");
    printf("3)Manager\n");
    printf("4)Admin\n");
    printf("5)Logout\n");
    int choice;
    printf("Enter your choice: ");
    scanf("%d",&choice);

    if(choice == 1)
    {
        const char *identifier = "customer";
        send(sock, identifier, strlen(identifier), 0);
        customer(sock);
    }
    else if(choice == 2)
    {
        const char *identifier = "employee";
        send(sock, identifier, strlen(identifier), 0);
        employee(sock);

    }
    else if(choice == 3)
    {
        const char *identifier = "manager";
        send(sock, identifier, strlen(identifier), 0);
        manager(sock);
    }
    else if(choice == 4)
    {
        const char *identifier = "administrator";
        send(sock, identifier, strlen(identifier), 0);
        administrator(sock);
    }
    else if(choice == 5)
    {
        break; 
    }

}

    
}