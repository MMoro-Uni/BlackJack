/*!
* \file dbfunctions.h
* \brief contains prototype functions for dbfunctions.c and definitions of db functions results
*/


#define DB_CONN_FAIL 10
#define NO_USER_FOUND 11
#define USER_FOUND 12
#define DB_ERROR 13
#define REG_DONE 14
#define UPDATE_DONE 15
#define OBTAIN_DONE 16

int GameLogin(char username[], char password[], sqlite3* ConnectedDB);
int GameRegister(char username[], char password[], sqlite3* ConnectedDB);
int AlterMoney(char username[], char password[], int value_modifier, sqlite3* ConnectedDB);
int GetMoney(char username[], char password[], char money_buffer[], sqlite3* ConnectedDB);