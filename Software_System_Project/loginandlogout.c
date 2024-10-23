#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>



struct administrator
{
    char username[30];
    char password[30];
    int islogged;
};

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

struct customer
{
    char username[20];
    char password[20];
    int age;
    int account_number;
    int wp;
    int dp;
    int balance;
    float withdrawn_history[5];
    float deposit_history[5];
    int islogged;
    int activate;
};

struct feedback
{
    char fb[1000];
};

struct loans
{
    int loanid;
    int amount;
    int account_number;
    char emp_username[20];
    int assigned;
};

struct manager
{
    char username[20];
    char password[20];
    int islogged;
};
void employee_logout(char *str, char *str1)
{   
    
    int fd = open("employee", O_RDWR, 0744);
    struct employee buffer;
    while (read(fd, &buffer, sizeof(struct employee)) == sizeof(struct employee))
    {
        if (strcmp(str, buffer.username) == 0)
        {
            buffer.islogged=0;
            lseek(fd, -sizeof(struct employee), SEEK_CUR);
            write(fd,&buffer,sizeof(buffer));
            close(fd);
            return;
        }
    }
}

void customer_logout(char *str, char *str1)
{   
    bool variable = false;
    int fd = open("customers", O_RDWR, 0744);
    struct customer buffer;
    while (read(fd, &buffer, sizeof(struct customer)) == sizeof(struct customer))
    {
        if (strcmp(str, buffer.username) == 0)
        {
            buffer.islogged=0;
            lseek(fd, -sizeof(struct customer), SEEK_CUR);
            write(fd,&buffer,sizeof(buffer));
            close(fd);
            return;
        }
    }
}

void manager_logout(char *str, char *str1)
{   
    bool variable = false;
    int fd = open("manager", O_RDWR, 0744);
    struct manager buffer;
    while (read(fd, &buffer, sizeof(struct manager)) == sizeof(struct manager))
    {
        if (strcmp(str, buffer.username) == 0)
        {
            buffer.islogged=0;
            lseek(fd, -sizeof(struct manager), SEEK_CUR);
            write(fd,&buffer,sizeof(buffer));
            close(fd);
            return;
        }
    }
}

void administrator_logout(char *str, char *str1)
{   
    bool variable = false;
    int fd = open("administrator", O_RDWR, 0744);
    struct administrator buffer;
    while (read(fd, &buffer, sizeof(struct administrator)) == sizeof(struct administrator))
    {
        if (strcmp(str, buffer.username) == 0)
        {
            buffer.islogged=0;
            lseek(fd, -sizeof(struct administrator), SEEK_CUR);
            write(fd,&buffer,sizeof(buffer));
            close(fd);
            return;
        }
    }
}

bool employee_exist(char *str, char *str1)
{   
    bool variable = false;
    int fd = open("employee", O_RDWR, 0744);
    struct employee buffer;
    while (read(fd, &buffer, sizeof(struct employee)) == sizeof(struct employee))
    {
        if (strcmp(str, buffer.username) == 0 && strcmp(str1, buffer.password) == 0 && buffer.islogged == 0)
        {
            variable = true;
            buffer.islogged=1;
            lseek(fd, -sizeof(struct employee), SEEK_CUR);
            write(fd,&buffer,sizeof(buffer));
            break;
        }
    }
    close(fd);
    return  variable;
}

bool customer_exist(char *str, char *str1)
{
    bool variable = false;
    int fd = open("customers", O_RDWR, 0744);
    struct customer buffer;
    while(read(fd, &buffer, sizeof(struct customer)) == sizeof(struct customer))
    {
        if (strcmp(str, buffer.username) == 0 && strcmp(str1, buffer.password) == 0 && buffer.islogged == 0)
        {
            variable = true;
            buffer.islogged = 1;
            lseek(fd, -sizeof(struct customer), SEEK_CUR);
            write(fd,&buffer,sizeof(buffer));
            break;
        }
    }
    close(fd);
    return variable;    
}
bool admin_verification(char *usrname, char *passwrd)
{ 
    int fd = open("administrator", O_RDWR, 0744);
    struct administrator admin;
    read(fd, &admin, sizeof(struct administrator));
    if( (strcmp(usrname, admin.username) == 0) && (strcmp(passwrd, admin.password) == 0) && admin.islogged == 0)
    {
        admin.islogged = 1;
        lseek(fd, -sizeof(struct administrator), SEEK_CUR);
        write(fd,&admin,sizeof(admin));
        return true;
    }
    else
    {
        return false;
    }
    close(fd);
}

bool manager_verification(char *usrname, char *passwrd)
{ 
    int fd = open("manager", O_RDWR, 0744);
    struct manager mang;
    read(fd, &mang, sizeof(struct manager));
    if( (strcmp(usrname, mang.username) == 0) && (strcmp(passwrd, mang.password) == 0) && mang.islogged == 0)
    {
        mang.islogged = 1;
        lseek(fd, -sizeof(struct manager), SEEK_CUR);
        write(fd,&mang,sizeof(mang));
        return true;
    }
    else
    {
        return false;
    }
    close(fd);
}




