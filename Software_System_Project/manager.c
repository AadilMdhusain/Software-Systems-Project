#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <arpa/inet.h>

struct manager
{
    char username[20];
    char password[20];
    int islogged;
};

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {

    struct manager mang;
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

    const char *identifier = "manager";
    send(sock, identifier, strlen(identifier), 0);


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