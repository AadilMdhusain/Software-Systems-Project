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
#include"loginlogout.h"

#define PORT 8080
#define BUFFER_SIZE 1024

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


void display_customers(int new_socket)
{
    int fd = open("customers", O_RDONLY, 0744);

    struct customer buffer;
    char output[2048] = "";

    while(read(fd, &buffer, sizeof(struct customer)) == sizeof(struct customer))
    {
        char temp[256];
        snprintf(temp, sizeof(temp),
                 "The username is : %s \n"
                 "The password for the customer is : %s \n"
                 "The age of the customer : %d \n"
                 "The balance of the customer : %d \n"
                 "The account number of the customer is : %d \n\n",
                 buffer.username, buffer.password, buffer.age, buffer.balance, buffer.account_number);

        strcat(output, temp);
    }

    int str_size;

    str_size = strlen(output);

    send(new_socket, &str_size, sizeof(str_size), 0);
    send(new_socket, output, str_size, 0);
    
    close(fd);
}

bool user_exists(char *str)
{
    bool variable = false;
    int fd = open("customers", O_RDONLY | O_CREAT, 0744);
    struct customer buffer;
    while(read(fd, &buffer, sizeof(struct customer)) == sizeof(struct customer))
    {
        if (strcmp(str, buffer.username) == 0)
        {
            variable = true;
        }
    }
    close(fd);
    return variable;

}


void add_new_customer(int new_socket)
{
    char username[20];
    char password[20];
    int age;
    send(new_socket,"\n............\nEnter the customer username:", sizeof("\n............\nEnter the customer username:"), 0);
    recv(new_socket, &username, sizeof(username), 0);
    username[strcspn(username, "\n")] = 0;
    printf("Username Entered :%s\n", username);

    send(new_socket, "Enter your password:", sizeof("Enter your password:"), 0);
    recv(new_socket, password, sizeof(password), 0);
    password[strcspn(password, "\n")] = 0;
    printf("Password Entered :%s\n", password);

    send(new_socket, "Enter your age:", sizeof("Enter your age:"), 0);
    recv(new_socket, &age, sizeof(age), 0);
    printf("The age of the customer Entered is : %d \n", age);
    
    if(!user_exists(username))
    {

         int fd2=open("temp",O_RDWR | O_CREAT, 0744);
         int prev;
         if(read(fd2,&prev,sizeof(int))==0)
         {
            prev=1000;
         }
         struct customer new;
         strcpy(new.username,username);
         strcpy(new.password,password);
         new.account_number= prev+1;
         prev=new.account_number;
         lseek(fd2, 0, SEEK_SET);
         write(fd2,&prev,sizeof(int));
         new.wp=0;
         new.dp=0;
         new.balance=0;
         new.islogged=0;
         new.activate=0;
         new.age=age;
         for(int i=0;i<5;i++)
         {
            new.withdrawn_history[i]=0;
            new.deposit_history[i]=0;
         }
        int fd = open("customers", O_RDWR | O_APPEND, 0744);
        write(fd,&new,sizeof(new));
         
    }
    else 
    {
        printf("Already Exist! try again:\n");
    }


}


void modify_cust_details(int new_socket)
{
    int account_no;
    int present =1;
    send(new_socket, "Enter the account no of the customer whose details you want to change:", sizeof("Enter the account no of the customer whose details you want to change:"), 0);
    recv(new_socket, &account_no, sizeof(account_no), 0);
    printf("The account number of the customer is : %d \n", account_no);
    int fd = open("customers", O_RDONLY | O_CREAT, 0744);
    struct customer buffer;
    while(read(fd, &buffer, sizeof(struct customer)) == sizeof(struct customer))
    {
        if (buffer.account_number == account_no)
        {
            present = 1;
            send(new_socket, &present, sizeof(present), 0);
            char username[20];
            char password[20];
            int age;
            send(new_socket,"\n............\nEnter the customer username:", sizeof("\n............\nEnter the customer username:"), 0);
            recv(new_socket, &username, sizeof(username), 0);
            username[strcspn(username, "\n")] = 0;
            printf("Username Entered :%s\n", username);

            send(new_socket, "Enter your password:", sizeof("Enter your password:"), 0);
            recv(new_socket, password, sizeof(password), 0);
            password[strcspn(password, "\n")] = 0;
            printf("Password Entered :%s\n", password);

            send(new_socket, "Enter your age:", sizeof("Enter your age:"), 0);
            recv(new_socket, &age, sizeof(age), 0);
            printf("The age of the customer Entered is : %d \n", age);


            strcpy(buffer.username,username);
            strcpy(buffer.password,password);
            buffer.age = age;
            lseek(fd, -sizeof(struct customer), SEEK_CUR);
            write(fd,&buffer,sizeof(buffer));
        }

    }
    close(fd);

}


void process_loans(int new_socket, char *str)
{
    bool variable = false;
    int fd = open("loans", O_RDONLY | O_CREAT, 0744);
    struct loans buffer;
    char option[BUFFER_SIZE] = {0};
    int i=0;
    strcat(option,"These are the loans to be processed by the employee :\n");
    while (read(fd, &buffer, sizeof(struct loans)) == sizeof(struct loans))
    {
         if (strcmp(str, buffer.emp_username) == 0 && buffer.assigned == 0)
         {
            i++;
            char numStr[50];

            snprintf(numStr, sizeof(numStr), "%d. Loan Id: %d ", i, buffer.loanid);
            strcat(option,numStr);
            memset(numStr, 0, sizeof(numStr));
            snprintf(numStr, sizeof(numStr), " Amount %d", buffer.amount);
            strcat(option,numStr);
            strcat(option,"\n");
            memset(numStr, 0, sizeof(numStr));

         }

     }
     close(fd);
     if(i==0)
     {
        int present = 0, len;
        strcat(option, "No loans are assigned to you to be processed.\n");
        len=strlen(option);
        send(new_socket, &len, sizeof(len), 0);
        send(new_socket, option, len, 0);
        send(new_socket, &present, sizeof(present), 0);
     }
     else
     {
        int present = 1;
        int loanid;
        int assignment;
        int ammount;
        strcat(option, "Enter the loan id of the loan to be assigned : ");
        int len=strlen(option);
        send(new_socket, &len, sizeof(len), 0);
        send(new_socket, option, len, 0);
        send(new_socket, &present, sizeof(present), 0);
        recv(new_socket, &loanid, sizeof(loanid), 0);
        send(new_socket, "Do you wish to assign the loan(1 for accept) or reject the loan(2 for reject)? \n", sizeof("Do you wish to assign the loan(1 for accept) or reject the loan(2 for reject)? \n"), 0);
        recv(new_socket, &assignment, sizeof(assignment), 0);
        int fd = open("loans", O_RDWR | O_CREAT, 0744);
        while(read(fd, &buffer, sizeof(struct loans)) == sizeof(struct loans))
        {
            if (buffer.loanid == loanid)
            {
                buffer.assigned = assignment;
                lseek(fd, -sizeof(struct loans), SEEK_CUR);
                write(fd, &buffer, sizeof(buffer));
                break;
            }

        }
        send(new_socket, "Loan has been assigned", sizeof("Loan has been assigned"), 0);

        close(fd);
        if( assignment == 1)
        { int fd1;
        fd1 = open("employee", O_RDWR, 0744);
        struct employee emp;

        while(read(fd, &emp, sizeof(struct employee)) == sizeof(struct employee)){

            if(strcmp(str, emp.username) == 0)
            {
                int i= emp.l_assi;
                emp.loans_assigned[i] = loanid;
                emp.l_assi=i+1;
                int j= emp.l_amti;
                emp.loans_amount[j] = ammount;
                emp.l_amti=j+1;
                break;
            }
        }

        lseek(fd1, -sizeof(struct employee), SEEK_CUR);
        write(fd1, &emp, sizeof(emp));
        close(fd1);
        }
     }

     close(fd);
}


void view_assigned_loans(int new_socket, char *str)
{
    int fd = open("employee", O_RDWR, 0744);
    struct employee buffer;
    char option[BUFFER_SIZE] = {0};
    int i;
    char numstr[50];
    while (read(fd, &buffer, sizeof(struct employee)) == sizeof(struct employee))
    {
        if (strcmp(str, buffer.username) == 0)
        {
            strcat(option, "The loans assigned by the employee are : \n");
            i = buffer.l_assi;
            for(int j=0; j<i; j++)
            {
                snprintf(numstr, sizeof(numstr), "%d. Loan Id: %d Loan Amount: %d \n",j+1, buffer.loans_assigned[j], buffer.loans_amount[j]);
                strcat(option, "\n");
            }
        }
    }
    if(i==0)
        {
            strcat(option, "No loans has been assigned by the employee");
        }
    send(new_socket, option, sizeof(option), 0);
    close(fd);
}

void change_emp_password(int new_socket, char *str)
{
    int fd = open("employee", O_RDWR, 0744);
    int len;
    struct employee buffer;
    while (read(fd, &buffer, sizeof(struct employee)) == sizeof(struct employee))
    {
        if (strcmp(str, buffer.username) == 0)
        {
            send(new_socket, "Enter your new password:", sizeof("Enter your new password:"), 0);
            

            // char *password =  malloc(len + 1);
            char password[30];
            ssize_t bytes_read = recv(new_socket, password, sizeof(password), 0);
            password[bytes_read] = '\0';
            printf("String received : %s\n", password);
            // password[strcspn(password, "\n")] = 0;
            strcpy(buffer.password, password);
            lseek(fd, -sizeof(struct employee), SEEK_CUR);
            write(fd, &buffer, sizeof(buffer));
            break;
        }
    }
    close(fd);
}

void handle_employee(int new_socket) {

    struct employee em;
    int choice;
    char username[BUFFER_SIZE];
    char password[BUFFER_SIZE];
    char username1[BUFFER_SIZE];

    send(new_socket,"\nEnter your username:", sizeof("\nEnter your username:"), 0);
    recv(new_socket, username, sizeof(username), 0);
    username[strcspn(username, "\n")] = 0;
    printf("Username Entered :%s\n", username);


    send(new_socket, "Enter your password:", sizeof("Enter your password:"), 0);
    recv(new_socket, password, sizeof(password), 0);
    password[strcspn(password, "\n")] = 0;
    printf("Password Entered :%s\n", password);


    if(employee_exist(username, password))
    {

        int present=1;
        send(new_socket, &present, sizeof(present), 0);

        while(1){

        int choice;
        char *menu = "\n1.Add New Customer\n"
                 "2.Modify Customer Details\n"
                 "3.Process Loans\n"
                 "4.View Assigned Loans Applications\n"
                 "5.Display Customers\n"
                 "6.Change Password\n"
                 "7.Exit\n"
                 "Enter choice: ";

        send(new_socket, menu, strlen(menu), 0);
        printf("Menu sent to employee.\n");

        recv(new_socket, &choice, sizeof(choice), 0);
            
            switch (choice)
            {
            case 1:
                add_new_customer(new_socket);
                break;
            case 2:
                modify_cust_details(new_socket);
                 break;
            case 3:
                 process_loans(new_socket, username); 
                 break;
            case 4:
                view_assigned_loans(new_socket, username);
                break;
            case 5:
                display_customers(new_socket);
                break;
            case 6:         
                change_emp_password(new_socket, username);
                break;
            case 7:
                employee_logout(username, password);
                break;
            default:
                printf("Invalid choice! Try again.\n");
            }
        

        }
    }
    else
    {
        int present=0;
        send(new_socket, &present, sizeof(present), 0);
        printf("Invalid Credentials or Already Logged In \n");
    }


    sleep(20);

    close(new_socket);
    exit(0); 
}

//===============================================================================================================================
//===============================================================================================================================
//================================================================================================================================




void view_balance(int new_socket, char *str)
{
    int fd = open("customers", O_RDONLY, 0744);
    struct customer buffer;

    int fd1 = open("customerindex", O_RDONLY, 0744);
    int i=0;
    while(read(fd1, &buffer, sizeof(struct customer)) == sizeof(struct customer))
    {
        if(strcmp(str, buffer.username) == 0)
        {
            break;
        }
        i++;
    }
    struct flock lock;
    lock.l_type=F_RDLCK;
    lock.l_whence=SEEK_SET;
    lock.l_start=(i)*sizeof(buffer);
    lock.l_len=sizeof(buffer);
    lock.l_pid=getpid();

    fcntl(fd, F_SETLKW, &lock);

    printf("Lock Achieved\n");
    send(new_socket, "Total Available Balance :", sizeof("Total Available Balance :"), 0);
    while(read(fd, &buffer, sizeof(struct customer)) == sizeof(struct customer))
    {
        if (strcmp(str, buffer.username) == 0)
        {
            int balance = buffer.balance;
            send(new_socket, &balance, sizeof(balance), 0);
        }
    }

    lock.l_type=F_UNLCK;
    fcntl(fd,F_SETLK, &lock);
    close(fd);
}

//.........................................................

void deposit_money(int new_socket, char *str)
{
    int fd = open("customers", O_RDWR , 0744);
    struct customer buffer;

    int fd1 = open("customerindex", O_RDWR, 0744);
    int i=0;
    while(read(fd1, &buffer, sizeof(struct customer)) == sizeof(struct customer))
    {
        if(strcmp(str, buffer.username) == 0)
        {
            break;
        }
        i++;
    }
    struct flock lock;
    lock.l_type=F_WRLCK;
    lock.l_whence=SEEK_SET;
    lock.l_start=(i)*sizeof(buffer);
    lock.l_len=sizeof(buffer);
    lock.l_pid=getpid();

    fcntl(fd, F_SETLKW, &lock);

    while(read(fd, &buffer, sizeof(struct customer)) == sizeof(struct customer))
    {
        if (strcmp(str, buffer.username) == 0)
        {
            int deposit;
            send(new_socket,"How much amount do you wish to deposit \n", sizeof("How much amount do you wish to deposit \n"), 0);
            recv(new_socket, &deposit, sizeof(deposit), 0);

            buffer.balance+=deposit;
            buffer.deposit_history[buffer.dp]=deposit;
            buffer.dp++;

            lseek(fd, -sizeof(struct customer), SEEK_CUR);
            write(fd, &buffer, sizeof(buffer));
            break;
        }
    }
    send(new_socket,"Money transaction was successful\n", sizeof("Money transaction was successful\n"), 0);  

    sleep(15);

    lock.l_type=F_UNLCK;
    fcntl(fd,F_SETLK, &lock);
    close(fd);

}

//..................................................

void withdraw_money(int new_socket, char *str)
{
    int fd = open("customers", O_RDWR | O_CREAT, 0744);
    struct customer buffer;

    int fd1 = open("customerindex", O_RDWR, 0744);
    int i=0;
    while(read(fd1, &buffer, sizeof(struct customer)) == sizeof(struct customer))
    {
        if(strcmp(str, buffer.username) == 0)
        {
            break;
        }
        i++;
    }
    struct flock lock;
    lock.l_type=F_WRLCK;
    lock.l_whence=SEEK_SET;
    lock.l_start=(i)*sizeof(buffer);
    lock.l_len=sizeof(buffer);
    lock.l_pid=getpid();

    fcntl(fd, F_SETLKW, &lock);

    while(read(fd, &buffer, sizeof(struct customer)) == sizeof(struct customer))
    {
        if (strcmp(str, buffer.username) == 0)
        {
            int withdraw;
            send(new_socket,"How much amount do you wish to withdraw :\n", sizeof("How much amount do you wish to withdraw :\n"), 0);
            recv(new_socket, &withdraw, sizeof(withdraw), 0);

            buffer.balance-=withdraw;
            buffer.withdrawn_history[buffer.wp]=withdraw;
            buffer.wp++;

            lseek(fd, -sizeof(struct customer), SEEK_CUR);
            write(fd, &buffer, sizeof(buffer));
            break;
        }

    }
    send(new_socket,"Money transaction was successful\n", sizeof("Money transaction was successful\n"), 0);

    sleep(15);

    lock.l_type=F_UNLCK;
    fcntl(fd,F_SETLK, &lock);

    close(fd);
}

//.........................................................

void transfer_funds(int new_socket, char *str)
{
    int fd = open("customers", O_RDWR | O_CREAT, 0744);
    struct customer buffer;
    int valid=0;
    int amount;
    int i=0;
    while(read(fd, &buffer, sizeof(struct customer)) == sizeof(struct customer))
    {
        if(strcmp(str, buffer.username) == 0)
        {
            int account_no;
            send(new_socket, "Which account number do you wish to transfer the amount to :\n", sizeof("Which account number do you wish to transfer the amount to :\n"), 0);
            recv(new_socket, &account_no, sizeof(account_no), 0);

            int fd1 = open("customers", O_RDWR | O_CREAT, 0744);
            struct customer bufferlock;

            //==================FOR ACHIEVING RECORD LOCKING=======================================================
            int fd2 = open("customerindex", O_RDWR, 0744);
            int i=0;
            while(read(fd2, &bufferlock, sizeof(struct customer)) == sizeof(struct customer))
            {
                if(bufferlock.account_number == account_no)
                {
                    break;
                }
                i++;
            }
            struct flock lock;
            lock.l_type=F_WRLCK;
            lock.l_whence=SEEK_SET;
            lock.l_start=(i)*sizeof(bufferlock);
            lock.l_len=sizeof(bufferlock);
            lock.l_pid=getpid();

            fcntl(fd, F_SETLKW, &lock);
            //====================================================================================================

            struct customer buffer1;
            while(read(fd1, &buffer1, sizeof(struct customer)) == sizeof(struct customer))
            {
                if(buffer1.account_number == account_no)
                {
                    valid=1;
                    send(new_socket, &valid, sizeof(valid), 0);
                    send(new_socket, "Amount you want to transfer to the person : \n", sizeof("Amount you want to transfer to the person : \n"), 0);
                    recv(new_socket, &amount, sizeof(amount), 0);

                    buffer.withdrawn_history[buffer.wp++]=amount;
                    buffer1.deposit_history[buffer.dp++]=amount;

                    buffer1.balance+=amount;
                    lseek(fd1, -sizeof(struct customer), SEEK_CUR);
                    write(fd1, &buffer1, sizeof(buffer1));

                    buffer.balance-=amount;
                    lseek(fd, -sizeof(struct customer), SEEK_CUR);
                    write(fd, &buffer, sizeof(buffer));
                    break;

                }
                i++;
            }
            if(valid == 1)
            {
                send(new_socket, "Transfer of funds successfully. \n", sizeof("Transfer of funds successfully. \n"), 0);
                break;

            }
            else
            {
             send(new_socket, &valid, sizeof(valid), 0);   
            }
            close(fd1);

            lock.l_type=F_UNLCK;
            fcntl(fd,F_SETLK, &lock);
        }

    }
    close(fd);
}

//................................................


void apply_for_loan(int new_socket, char *str)
{
    int amount, account_no;
    char username[20];
    struct loans l;
    send(new_socket, "Enter the amount of loan you would require : ", sizeof("Enter the amount of loan you would require : "), 0);
    recv(new_socket, &amount, sizeof(amount), 0);
    send(new_socket, "Enter your account number: ", sizeof("Enter your account number: "), 0);
    recv(new_socket, &account_no, sizeof(account_no), 0);

    int fd = open("loans",O_RDWR | O_APPEND | O_CREAT, 0744);
    
    int fd2=open("temp1",O_RDWR | O_CREAT, 0744);
    int prev;
    if(read(fd2,&prev,sizeof(int))==0)
    {
        prev=1000;
    }

    l.amount = amount;
    l.account_number=account_no;
    strcpy(l.emp_username, "unassigned");
    l.loanid= prev+1;
    prev = prev + 1;
    l.assigned=0;
    lseek(fd2, 0, SEEK_SET);
    write(fd2,&prev,sizeof(int));
    close(fd2);

    write(fd,&l,sizeof(l));
    close(fd);
}

//..............................

void view_transaction_history(int new_socket, char *str)
{
    int fd = open("customers", O_RDONLY | O_CREAT, 0744);
    struct customer buffer;
    char history[BUFFER_SIZE] = ""; 
    while(read(fd, &buffer, sizeof(struct customer)) == sizeof(struct customer))
    {
        if (strcmp(str, buffer.username) == 0)
        {
            strcat(history, "The Deposit History of the Customer will be : \n");
            for(int i=0; i<buffer.dp; i++)
            {
                char numStr[100];
                snprintf(numStr, sizeof(numStr), "%d. %f\n ", i+1, buffer.deposit_history[i]);
                strcat(history, numStr);
            }
            strcat(history, "The Withdrawal History of the Customer will be : \n");
            for(int i=0; i<buffer.wp; i++)
            {
                char numStr[100];
                snprintf(numStr, sizeof(numStr), "%d. %f\n ", i+1, buffer.withdrawn_history[i]);
                strcat(history, numStr);
            }
        }
    }

    int history_length = strlen(history);
    send(new_socket, &history_length, sizeof(history_length), 0);
    
    send(new_socket, history, history_length, 0);
    
    close(fd);
}


//.............................

void add_feedback(int new_socket, char *str)
{
    int fd = open("feedback", O_WRONLY | O_CREAT | O_APPEND, 0744);
    struct feedback fb;
    char fback[513];
    send(new_socket, "Please Enter Your Feedback on the Banking System. \n", sizeof("Please Enter Your Feedback on the Banking System. \n"), 0);
    recv(new_socket, fback, sizeof(fback), 0);

    strcpy(fb.fb, fback);
    write(fd,&fb,sizeof(fb));

    close(fd);

}

//....................................

void change_cust_password(int new_socket, char *str)
{
    int fd = open("customers", O_RDWR, 0744);
    char password[30];
    struct customer buffer;
    while (read(fd, &buffer, sizeof(struct customer)) == sizeof(struct customer))
    {
        if (strcmp(str, buffer.username) == 0)
        {
            send(new_socket, "Enter your new password:", sizeof("Enter your new password:"), 0);
            ssize_t bytes_read = recv(new_socket, password, sizeof(password), 0);
            password[bytes_read] = '\0';
            // password[strcspn(password, "\n")] = 0;
            strcpy(buffer.password, password);
            lseek(fd, -sizeof(struct customer), SEEK_CUR);
            write(fd,&buffer,sizeof(buffer));
        }


    }
    close(fd);
}


void handle_customer(int new_socket){

    struct employee em;
    int choice;
    char username[BUFFER_SIZE];
    char password[BUFFER_SIZE];
    char response[BUFFER_SIZE];

    send(new_socket,"\nEnter your username:", sizeof("\nEnter your username:"), 0);
    recv(new_socket, &username, sizeof(username), 0);
    username[strcspn(username, "\n")] = 0;
    printf("Username Entered :%s\n", username);


    send(new_socket, "Enter your password:", sizeof("Enter your password:"), 0);
    recv(new_socket, password, sizeof(password), 0);
    password[strcspn(password, "\n")] = 0;
    printf("Password Entered :%s\n", password);

    if(customer_exist(username, password))
    {

        remove("customerindex");
        int sourceFd = open("customers", O_RDONLY, 0744);
        int destFd = open("customerindex", O_RDWR | O_CREAT, 0744);
        char buffer[BUFFER_SIZE];
        ssize_t bytesRead;
        while ((bytesRead = read(sourceFd, buffer, BUFFER_SIZE)) > 0) {
        if (write(destFd, buffer, bytesRead) != bytesRead) {
            perror("Error writing to destination file");
            close(sourceFd);
            close(destFd);
            return;
            }
        }

        int present=1;
        send(new_socket, &present, sizeof(present), 0);

        while(1){

            int choice;
            char *menu = "\n1. View Account Balance\n"
                        "2. Deposit Money\n"
                        "3. Withdraw Money\n"
                        "4. Transfer Funds\n"
                        "5. Apply for a Loan\n"
                        "6. View Transaction History\n"
                        "7. Add Feedback\n"
                        "8. Change Password\n"
                        "9. Exit\n"
                        "Enter choice: ";

            send(new_socket, menu, strlen(menu), 0);
            printf("Menu sent to customer.\n");

            recv(new_socket, &choice, sizeof(choice), 0);
                
                switch (choice)
                {
                case 1:
                    view_balance(new_socket, username);
                    break;
                case 2:
                    deposit_money(new_socket, username);
                    break;
                case 3:
                    withdraw_money(new_socket, username);
                    break;
                case 4:
                    transfer_funds(new_socket, username);
                    break;
                case 5:
                    apply_for_loan(new_socket, username);
                    break;
                case 6:
                    view_transaction_history(new_socket, username);
                    break;
                case 7:
                    add_feedback(new_socket, username);
                    break;
                case 8:
                    change_cust_password(new_socket, username);
                    break;
                case 9:
                    customer_logout(username, password);
                    return;
                    break;
                default:
                    printf("Invalid choice! Try again.\n");
                    break;

                }


        }
    }
    else
    {
        int present=0;
        send(new_socket, &present, sizeof(present), 0);
        printf("Invalid Credentials or Already Logged In\n");
    }


    sleep(20);

    close(new_socket);
    exit(0);


}

//==============================================================================================================================================
//==============================================================================================================================================
//===============================================================================================================================================

void add_new_employee( int new_socket)
{
    char username[20];
    char password[20];
    int age;
    send(new_socket,"\n............\nEnter the employee username:", sizeof("\n............\nEnter the employee username:"), 0);
    recv(new_socket, &username, sizeof(username), 0);
    username[strcspn(username, "\n")] = 0;
    printf("Username Entered :%s\n", username);

    send(new_socket, "Enter employee's password:", sizeof("Enter employee's password:"), 0);
    recv(new_socket, password, sizeof(password), 0);
    password[strcspn(password, "\n")] = 0;
    printf("Password Entered :%s\n", password);

    send(new_socket, "Enter employee's age:", sizeof("Enter employee's age:"), 0);
    recv(new_socket, &age, sizeof(age), 0);
    printf("The age of the employee is : %d \n", age);
    
    if(!user_exists(username))
    {

         int fd2=open("temp2",O_RDWR | O_CREAT, 0744);
         int prev;
         if(read(fd2,&prev,sizeof(int))==0)
         {
            prev=5000;
         }
         struct employee new;
         strcpy(new.username,username);
         strcpy(new.password,password);
         new.empid= prev+1;
         prev=new.empid;
         lseek(fd2, 0, SEEK_SET);
         write(fd2,&prev,sizeof(int));
         close(fd2);
         new.islogged=0;
         new.age=age;
         new.l_amti=0;
         new.l_assi=0;
         for(int i=0;i<10;i++)
         {
            new.loans_amount[i]=0;
            new.loans_assigned[i]=0;
         }
        int fd = open("employee", O_RDWR | O_APPEND | O_CREAT, 0744);
        write(fd,&new,sizeof(new));
        close(fd);
         
    }
    else 
    {
        printf("Already Exist! try again:\n");
    }

}


void modify_employee_detail(int new_socket)
{
    char username[30];
    send(new_socket,"\n............\nEnter the employee username:", sizeof("\n............\nEnter the employee username:"), 0);
    recv(new_socket, &username, sizeof(username), 0);
    username[strcspn(username, "\n")] = 0;
    printf("Username Entered :%s\n", username);
    int fd = open("employee", O_RDWR, 0744);
    char password[30];
    struct employee buffer;
    while (read(fd, &buffer, sizeof(struct employee)) == sizeof(struct employee))
    {
        if (strcmp(username, buffer.username) == 0)
        {
            send(new_socket, "Enter your new password:", sizeof("Enter your new password:"), 0);

            recv(new_socket, password, sizeof(password), 0);
            password[strcspn(password, "\n")] = 0;
            strcpy(buffer.password, password);
            lseek(fd, -sizeof(struct employee), SEEK_CUR);
            write(fd,&buffer,sizeof(buffer));
        }

    }

    close(fd);
}

void disp_emp_detail(int new_socket)
{
    int fd = open("employee", O_RDONLY, 0744);
    struct employee buffer;
    char option[BUFFER_SIZE] = ""; 
    char employee_detail[256]; 

    while(read(fd, &buffer, sizeof(struct employee)) == sizeof(struct employee))
    {
        snprintf(employee_detail, sizeof(employee_detail),
                 "Username: %s\nPassword: %s\nAge: %d\nEmployee ID: %d\n\n",
                 buffer.username, buffer.password, buffer.age, buffer.empid);
        
        strcat(option, employee_detail);
    }

    int details_length = strlen(option);
    send(new_socket, &details_length, sizeof(details_length), 0);
    
    send(new_socket, option, details_length, 0);
    
    close(fd);
}

void change_password(int new_socket)
{
    int fd = open("administrator", O_WRONLY | O_CREAT, 0744);
    char password[20];
    char username[20];
    struct administrator buffer;

    send(new_socket, "Enter your new username:", sizeof("Enter your new username:"), 0);
    recv(new_socket, username, sizeof(username), 0);
    username[strcspn(username, "\n")] = 0;
    strcpy(buffer.username, username);

    send(new_socket, "Enter your new password:", sizeof("Enter your new password:"), 0);
    recv(new_socket, password, sizeof(password), 0);
    password[strcspn(password, "\n")] = 0;
    strcpy(buffer.password, password);

    buffer.islogged = 0;

    lseek(fd, 0, SEEK_SET);

    write(fd, &buffer, sizeof(buffer));

    close(fd);
}


void handle_administrator(int new_socket){

    int choice;
    char username[BUFFER_SIZE];
    char password[BUFFER_SIZE];
    char response[BUFFER_SIZE];

    send(new_socket,"\nEnter your username:", sizeof("\nEnter your username:"), 0);
    recv(new_socket, &username, sizeof(username), 0);
    username[strcspn(username, "\n")] = 0;
    printf("Username Entered :%s\n", username);


    send(new_socket, "Enter your password:", sizeof("Enter your password:"), 0);
    recv(new_socket, password, sizeof(password), 0);
    password[strcspn(password, "\n")] = 0;
    printf("Password Entered :%s\n", password);


    if(admin_verification(username, password))
    {

        int present=1;
        send(new_socket, &present, sizeof(present), 0);

        while(1){

            int choice;
            char *menu = "\n1. Add New Bank Employee\n"
                        "2. Modify Employee Details\n"
                        "3. Display all Employees Details\n"
                        "4. Change Password\n"
                        "5. Exit\n"
                        "Enter choice: ";

            send(new_socket, menu, strlen(menu), 0);
            printf("Menu sent to administrator.\n");

            recv(new_socket, &choice, sizeof(choice), 0);
                
                switch (choice)
                {
                case 1:
                    add_new_employee(new_socket);
                    break;
                case 2:
                    modify_employee_detail(new_socket);
                    break;
                case 3:
                    disp_emp_detail(new_socket);
                    break;
                case 4:
                    change_password(new_socket);
                    break;
                case 5:
                    administrator_logout(username, password);
                    break;
                default:
                    printf("Invalid choice! Try again.\n");
                    break;

                }


        }
    }
    else
    {
        int present=0;
        send(new_socket, &present, sizeof(present), 0);
        printf("Invalid Credentials or Already Logged In\n");
    }


    sleep(5);

    close(new_socket);
    exit(0);


}

//=====================================================================================================================================
//====================================================================================================================================
//=====================================================================================================================================



void deactivate_customer(int new_socket) {
    char username[BUFFER_SIZE];
    send(new_socket, "\nEnter the customer's username:", sizeof("\nEnter the customer's username:"), 0);
    recv(new_socket, username, sizeof(username), 0);
    username[strcspn(username, "\n")] = 0; 
    printf("Customer Username Entered: %s\n", username);

    int fd = open("customers", O_RDWR, 0744);
    if (fd < 0) {
        perror("Failed to open customers file");
        return;
    }

    int temp_fd = open("temp_customers", O_WRONLY | O_CREAT | O_TRUNC, 0744);
    if (temp_fd < 0) {
        perror("Failed to open temporary file");
        close(fd);
        return;
    }

    struct customer cust;
    int found = 0; 

    while (read(fd, &cust, sizeof(struct customer)) == sizeof(struct customer)) {
        if (strcmp(username, cust.username) != 0) {
            write(temp_fd, &cust, sizeof(struct customer));
        } else {
            found = 1; 
        }
    }

    close(fd);
    close(temp_fd);

    if (found) {
        remove("customers"); 
        rename("temp_customers", "customers"); 
        send(new_socket, "Customer record deleted successfully.\n", 39, 0);
    } else {
        remove("temp_customers"); 
        send(new_socket, "Customer not found.\n", 20, 0);
    }
}

void display_loans(int new_socket){
    int fd = open("loans", O_RDONLY);

    struct loans l;
    char all_loans[5000] = "";

    while(read(fd, &l, sizeof(struct loans)) == sizeof(struct loans))
    {
        if(strcmp(l.emp_username, "unassigned") == 0)
        {
            char loan_item[500] = ""; 
            snprintf(loan_item, sizeof(loan_item),
             "Loan id: %d\nLoan amount: %d\nCustomer Account Number: %d\n",
             l.loanid, l.amount, l.account_number);

             strcat(all_loans, loan_item);
        }

    }

    int loanlength = strlen(all_loans);

    send(new_socket, &loanlength, sizeof(loanlength), 0);
    send(new_socket, all_loans, loanlength, 0);
}

void display_all_loans(int new_socket){
    int fd = open("loans", O_RDONLY);

    struct loans l;
    char all_loans[5000] = "";

    while(read(fd, &l, sizeof(struct loans)) == sizeof(struct loans))
    {
        // if(strcmp(l.emp_username, "unassigned") == 0)
        {
            char loan_item[500] = ""; 
            snprintf(loan_item, sizeof(loan_item),
             "Loan id: %d\nLoan amount: %d\nCustomer Account Number: %d\nEmployee Assigned to : %s\n",
             l.loanid, l.amount, l.account_number, l.emp_username);

             strcat(all_loans, loan_item);
        }

    }

    int loanlength = strlen(all_loans);

    send(new_socket, &loanlength, sizeof(loanlength), 0);
    send(new_socket, all_loans, loanlength, 0);
}



void assign_employee(int new_socket)
{
    char username[20];
    int loanid;
    struct loans l;
    display_loans(new_socket);
    send(new_socket, "Enter the id of the loan :", sizeof("Enter the id of the loan :"), 0);
    recv(new_socket, &loanid, sizeof(loanid), 0);

    int fd = open("loans",O_RDWR | O_CREAT, 0744);
    while( read(fd, &l, sizeof(struct loans)) == sizeof(struct loans) )
    {
        if(l.loanid == loanid)
        {
            send(new_socket, "Enter the employee user name: ", sizeof("Enter the employee user name: "), 0);
            recv(new_socket, username, sizeof(username), 0);
            username[strcspn(username,"\n")];

            strcpy(l.emp_username, username);

            lseek(fd, -sizeof(struct loans), SEEK_CUR);
            write(fd,&l,sizeof(l));
            break;
        }
    }

    close(fd);
}


void review_customer_feedback(int new_socket) {
    int fd = open("feedback", O_RDONLY);
    if (fd < 0) {
        perror("Failed to open feedback file");
        return;
    }

    struct feedback fb_record;
    char all_feedback[5000] = ""; 

    while (read(fd, &fb_record, sizeof(struct feedback)) == sizeof(struct feedback)) {
        strcat(all_feedback, fb_record.fb);
        strcat(all_feedback, "\n"); 
    }

    int fblength = strlen(all_feedback);
    
    send(new_socket, &fblength, sizeof(fblength), 0);
    send(new_socket, all_feedback, fblength, 0);

    close(fd);
}


void change_password_manager(int new_socket)
{
    int fd = open("manager", O_WRONLY | O_CREAT, 0744);
    char password[20];
    char username[20];
    struct manager buffer;

    send(new_socket, "Enter your new username:", sizeof("Enter your new username:"), 0);
    recv(new_socket, username, sizeof(username), 0);
    username[strcspn(username, "\n")] = 0;
    strcpy(buffer.username, username);

    send(new_socket, "Enter your new password:", sizeof("Enter your new password:"), 0);
    recv(new_socket, password, sizeof(password), 0);
    password[strcspn(password, "\n")] = 0;
    strcpy(buffer.password, password);

    buffer.islogged = 0;

    lseek(fd, 0, SEEK_SET);

    write(fd, &buffer, sizeof(buffer));

    close(fd);
}


void handle_manager(int new_socket){

    int choice;
    char username[BUFFER_SIZE];
    char password[BUFFER_SIZE];

    send(new_socket,"\nEnter your username:", sizeof("\nEnter your username:"), 0);
    recv(new_socket, &username, sizeof(username), 0);
    username[strcspn(username, "\n")] = 0;
    printf("Username Entered :%s\n", username);


    send(new_socket, "Enter your password:", sizeof("Enter your password:"), 0);
    recv(new_socket, password, sizeof(password), 0);
    password[strcspn(password, "\n")] = 0;
    printf("Password Entered :%s\n", password);


    if(manager_verification(username, password))
    {

        int present=1;
        send(new_socket, &present, sizeof(present), 0);

        while(1){

            int choice;
            char *menu = "\n1. Deactivate Customer Accounts\n"
                        "2. View all the Loans Applications\n"
                        "3. Assigning Loan Applications to Employees\n"
                        "4. Review Customer Feedback\n"
                        "5. Change Password\n"
                        "6. Exit\n"
                        "Enter choice: ";

            send(new_socket, menu, strlen(menu), 0);
            printf("Menu sent to administrator.\n");

            recv(new_socket, &choice, sizeof(choice), 0);
                
                switch (choice)
                {
                case 1:
                    deactivate_customer(new_socket);
                    break;
                case 2:
                    display_all_loans(new_socket);
                    break;
                case 3:
                    assign_employee(new_socket);
                    break;
                case 4:
                    review_customer_feedback(new_socket);
                    break;
                case 5:
                    change_password_manager(new_socket);
                    break;
                case 6:
                    manager_logout(username, password);
                    break;
                default:
                    printf("Invalid choice! Try again.\n");
                    break;

                }

        }
    }
    else
    {
        int present=0;
        send(new_socket, &present, sizeof(present), 0);
        printf("Invalid Credentials or Already Logged In\n");
    }


    sleep(5);

    close(new_socket);
    exit(0);


}



//=====================================================================================================================
//=====================================================================================================================
//=====================================================================================================================


int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            continue; 
        }

        memset(buffer, 0, BUFFER_SIZE);
        recv(new_socket, buffer, sizeof(buffer), 0);

        if (fork() == 0) {
           close(server_fd); 
            
            if (strcmp(buffer, "employee") == 0) {
                handle_employee(new_socket);
            } else if (strcmp(buffer, "customer") == 0) {
                handle_customer(new_socket);
            } else if (strcmp(buffer, "administrator") == 0){
                handle_administrator(new_socket);
            } else if (strcmp(buffer, "manager") == 0){
                handle_manager(new_socket);
            } else {
                printf("Unknown client type\n");
                close(new_socket);
            }
            exit(0);
        }

        close(new_socket); 
        waitpid(-1, NULL, WNOHANG); 
    }

    close(server_fd);
    return 0;
}
