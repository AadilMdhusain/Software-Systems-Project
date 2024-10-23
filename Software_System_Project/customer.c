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
struct employees{
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



#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    struct customer cust;
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

    const char *identifier = "customer";
    send(sock, identifier, strlen(identifier), 0);


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
