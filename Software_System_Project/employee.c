#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <arpa/inet.h>


struct employee
{
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

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {

    struct employee emp;
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

    const char *identifier = "employee";
    send(sock, identifier, strlen(identifier), 0);

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