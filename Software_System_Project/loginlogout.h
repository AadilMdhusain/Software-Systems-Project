#ifndef LOGINLOGOUT_H
#define LOGINLOGOUT_H
void employee_logout(char *str, char *str1);
void customer_logout(char *str, char *str1);
void manager_logout(char *str, char *str1);
void administrator_logout(char *str, char *str1);
bool employee_exist(char *str, char *str1);
bool customer_exist(char *str, char *str1);
bool admin_verification(char *usrname, char *passwrd);
bool manager_verification(char *usrname, char *passwrd);

#endif