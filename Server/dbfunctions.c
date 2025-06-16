/*!
* \file dbfunctions.c
* \brief Contains code for managing the database.
*		 Code inside this file and its header is not meant for usage in other projects.
* 
* \fn int login_callback(void* data, int columns, char** row_fields, char** row_names)
* \brief callback function for SQLite SELECT
* the function doesn't do much with the data it gets in input, but returns the amount of columns matching details given.
* if the return value is >0 that means something was found with the given credentials, meaning login was successful.
* 
* \fn int GameLogin(char username[], char password[], sqlite3* ConnectedDB)
* \brief function to login into database
* Tries to log into databse using given username and password.
* Data is looked for in a LoginTable table containing usernames and their associated passwords.
* LoginTable must be inside the sqlite3 object passed to the function.
* 
* \fn int GameRegister(char username[], char password[], sqlite3* ConnectedDB)
* \brief function to register into database
* Creates a new row in the login table and creates a new row in the money table for given user.
* Data is created in LoginTable and MoneyTable.
* Tables must be inside the sqlite3 object passes to the function.
* 
* \fn int AlterMoney(char username[], char password[], int value_modifier, sqlite3* ConnectedDB)
* \brief alters the amount of money the given user has by value_modifier
* The function first checks if the user exists in LoginTable through GameLogin()
* If user exists, the username is used to modify the correct value in MoneyTable.
* value_modifier is added to the Value field in the selected MoneyTable row.
* LoginTable and MoneyTable must be inside the given sqlite3 object.
* 
* \fn int GetMoney(char username[], char password[], char money_buffer[],  sqlite3* ConnectedDB)
* \brief gets the amount of money the given user has and puts it into money_buffer
* The function first checks if the user exists in LoginTable through GameLogin()
* If user exists, the username is used to get the correct value from MoneyTable.
* The obtained value is then copied as a string into money_buffer[].
* Tables must be inside the sqlite3 object passed to the function.
* 
* \int get_money_callback(void* var_pointer, int columns, char** row_fields, char** row_names)
* \brief callback function for GetMoney()
* After the value to put into money_buffer has been obtained by the callback function, it is put into var_pointer
* For correct functioning, var_pointer must be a pointer to money_buffer.
* 
* 
* Possible return values for functions:
* DB_CONN_FAIL: could not connect to database
* DB_ERROR: error in writing data to the database
* NO_USER_FOUND: no user was found with the given credentials
* USER_FOUND: an user was found with the given credentials
* REG_DONE: registration process was completed successfully
* UPDATE_DONE: the amount of money the player has was updated successfully
* OBTAIN_DONE: the amount of money the player has was obtained successfully
* 
*/


#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include "dbfunctions.h"


//function prototypes
int login_callback(void* data, int columns, char** row_fields, char** row_names);
int get_money_callback(char* var_pointer, int columns, char** row_fields, char** row_names);

//function declaration
int GameLogin(char username[], char password[], sqlite3* ConnectedDB)
{

	char sql_query[200] = "";
	int sql_returned; 

	sprintf_s(sql_query, 200, "SELECT * FROM LoginTable WHERE Username = '%s' AND Password = '%s';", username, password);
	sql_returned = sqlite3_exec(ConnectedDB, sql_query, login_callback, 0, 0);

	if (sql_returned == SQLITE_OK)
	{
		return NO_USER_FOUND;
	}
	else if (sql_returned == SQLITE_ABORT)
	{
		return USER_FOUND;
	}
	else
	{
		return DB_CONN_FAIL;
	}
}

int login_callback(void* data, int columns, char** row_fields, char** row_names)
{
	int login_found_columns = columns;
	return login_found_columns;
}

int GameRegister(char username[], char password[], sqlite3* ConnectedDB)
{
	char sql_query[200] = "";
	int sql_returned;

	sprintf_s(sql_query, 200, "INSERT INTO LoginTable VALUES('%s','%s');", username, password);
	sql_returned = sqlite3_exec(ConnectedDB, sql_query, 0, 0, 0);

	if (sql_returned == SQLITE_OK)
	{
		sprintf_s(sql_query, 200, "INSERT INTO MoneyTable VALUES('%s',1000);", username);
		sql_returned = sqlite3_exec(ConnectedDB, sql_query, 0, 0, 0);

		if (sql_returned == SQLITE_OK)
		{
			return REG_DONE;
		}
		
	}
	
	return DB_ERROR;

}

int AlterMoney(char username[], char password[], int value_modifier, sqlite3* ConnectedDB)
{

	char sql_query[200] = "";
	int sql_returned;
	int user_exists = GameLogin(username, password, ConnectedDB);

	if (user_exists == USER_FOUND)
	{
		sprintf_s(sql_query, 200, "UPDATE MoneyTable SET Value = Value + %d WHERE Username = '%s';", value_modifier, username);
		sql_returned = sqlite3_exec(ConnectedDB, sql_query, 0, 0, 0);

		if (sql_returned == SQLITE_OK)
		{
			return UPDATE_DONE;
		}

	}
	
	return DB_ERROR;
}

int get_money_callback(char* var_pointer, int columns, char** row_fields, char** row_names)
{
	strcpy(var_pointer, row_fields[0]);
	return 0;
}

int GetMoney(char username[], char password[], char money_buffer[], sqlite3* ConnectedDB)
{
	char sql_query[200] = "";
	int sql_returned;
	int user_exists = GameLogin(username, password, ConnectedDB);

	if (user_exists == USER_FOUND)
	{
		sprintf_s(sql_query, 200, "SELECT Value FROM MoneyTable WHERE Username = '%s';", username);
		sql_returned = sqlite3_exec(ConnectedDB, sql_query, get_money_callback, money_buffer, 0);

		if (sql_returned == SQLITE_OK)
		{
			return OBTAIN_DONE;
		}
	}

	return DB_ERROR;
}