/*!
* \file blackjackclient.h
* \brief contains structures for login and function declarations for blackjackclient.c.
* Function definitions are contained in the documentation for the .c file
*/

/*!
* \enum login_mode
* \brief enum for values to select login or register
*/

/*!
* \enum login_result
* \brief enum for results of login and register
*/

typedef enum login_mode
{
    LOGIN_MODE = 20,
    REGISTER_MODE
} MODE;

typedef enum login_result
{
    LOGIN_SUCCESS = 2,
    LOGIN_FAIL,
    REGISTER_SUCCESS,
    REGISTER_FAIL
} RESULT;

int server_login(char username[], char password[], MODE mode);
int money_value_update(int modify_value);
int money_value_get();