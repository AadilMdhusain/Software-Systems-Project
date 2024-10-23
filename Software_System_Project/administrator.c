#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <arpa/inet.h>

struct administrator
{
    char username[30];
    char password[30];
};

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {

    struct administrator admin;
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

    const char *identifier = "administrator";
    send(sock, identifier, strlen(identifier), 0);


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
