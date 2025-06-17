/*!
* \file gamemenu.c
* \brief contains code for the main menu of the game
* the code in this file is not meant for usage outside of the project. Don't include in other projects without modifying first.
*/

/*!
* \fn ONMODE MainMenu()
* \brief draws the main menu, handles calling other functins for the menu and returns if login happened or not.
* Online mode is returned after the play button is pressed.
* Buttons are contained inside an array to simplify code reading, avoid giant if conditions and to make it easier to implement new buttons.
* A full implementation for new button creation hasn't been made, but the logic should help implementing it.
* Currently, ButtonArray[0] is the play button, ButtonArray[1] is the login button and ButtonArray[2] is the register button
*/

/*!
* \fn RESULT LoginRegisterInterface(MODE mode)
* \brief draws the login or register interface and calls client functions for server calls
* mode is used for choosing to login or to register.
* The return value is the result of the login or register process.
* Input Boxes and Buttons are contained in arrays for the same reason as MainMenu()
* Currently, InputBoxArray[0] is used for username and InputBoxArray[1] is used for password.
* ButtonArray[0] is the login/register button, ButtonArray[1] is the back button.
*/


#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <raygui.h>
#include <string.h>
#include "blackjackclient.h"
#include "gamemenu.h"

#define WINDOW_HEIGHT 625
#define WINDOW_WIDTH 1200
#define TEXT_BUFF_SIZE 65
#define BUTTON_AMOUNT 3
//when changing style for raygui through code, color needs to be given as a 32bit integer
#define WHITE_32 0xffffffff
#define BLUE_TRANSP_32 0x97E8FF80
#define RL_BUTTON_AMOUNT 2
#define RL_INPUT_AMOUNT 2

static Font TitleFont;
static Font DefaultFont;

RESULT LoginRegisterInterface(MODE mode);

ONMODE MainMenu()
{
    //variable declaration
    //variables are not static because they are currently only initialized once.

    Vector2 TitlePositionVec;
    Vector2 MoneyTextPositionVec;
    Rectangle ButtonArray[BUTTON_AMOUNT];

    bool play_button_pressed = false;
    bool login_button_pressed = false;
    bool register_button_pressed = false;
    bool cursor_colliding_with_buttons = false;

    ONMODE on_mode = OFFLINE_PLAY;
    int money_value = 0;
    char money_text[TEXT_BUFF_SIZE] = "";

    TitleFont = LoadFontEx("JqkasWild-w1YD6.ttf", 128, 0, 250);
    DefaultFont = GetFontDefault();

    TitlePositionVec.x = WINDOW_WIDTH * 0.03f;
    TitlePositionVec.y = WINDOW_HEIGHT * 0.03f;
    MoneyTextPositionVec.x = WINDOW_WIDTH * 0.4f;
    MoneyTextPositionVec.y = WINDOW_HEIGHT * 0.5f;

    for (int i = 0; i < BUTTON_AMOUNT; i++)
    {
        ButtonArray[i].x = WINDOW_WIDTH * 0.05f;
        ButtonArray[i].y = (0.3f + (i * 0.15f)) * WINDOW_HEIGHT;
        ButtonArray[i].width = WINDOW_WIDTH * 0.3f;
        ButtonArray[i].height = WINDOW_HEIGHT * 0.1f;
    }


    GuiSetStyle(DEFAULT, TEXT_SIZE, 40);
    GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, WHITE_32);
    GuiSetStyle(TEXTBOX, TEXT_COLOR_FOCUSED, WHITE_32);
    GuiSetStyle(TEXTBOX, TEXT_COLOR_PRESSED, WHITE_32);
    GuiSetStyle(TEXTBOX, BASE_COLOR_PRESSED, BLUE_TRANSP_32);

    //main loop
    //while in gamewindow.c only the main function of the file has a loop, in this file every function does
    //the reason for this is that this was easier to design, and once i noticed the mistake in gamewindow.c it was already working
    //it would be better to change gamewindow.c, but it would bring almost no performance gain and it wouldnt change the functioning of the program.

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(DARKGREEN);

        DrawTextEx(TitleFont, "BlackJack", TitlePositionVec, 128, 5, WHITE);

        play_button_pressed = GuiButton(ButtonArray[0], "PLAY");
        login_button_pressed = GuiButton(ButtonArray[1], "LOGIN");
        register_button_pressed = GuiButton(ButtonArray[2], "REGISTER");

        if (on_mode == ONLINE_PLAY)
        {
            sprintf_s(money_text, TEXT_BUFF_SIZE, "MONEY = %d", money_value);
            DrawTextEx(DefaultFont, money_text, MoneyTextPositionVec, 64, 3, WHITE);
        }

        for (int i = 0; i < BUTTON_AMOUNT; i++)
        {
            if (CheckCollisionPointRec(GetMousePosition(), ButtonArray[i]))
            {
                cursor_colliding_with_buttons = true;
            }
        }

        if (cursor_colliding_with_buttons)
        {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            cursor_colliding_with_buttons = false;
        }
        else
        {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }

        EndDrawing();

        if (play_button_pressed)
        {
            return on_mode;
        }
        else if (login_button_pressed)
        {
            int login_result = LoginRegisterInterface(LOGIN_MODE);

            if (login_result == LOGIN_SUCCESS)
            {
                on_mode = ONLINE_PLAY;
                money_value = money_value_get();
            }
            else
            {
                on_mode = OFFLINE_PLAY;
            }
        }
        else if (register_button_pressed)
        {
            LoginRegisterInterface(REGISTER_MODE);
        }

    }

    return 0;
}

RESULT LoginRegisterInterface(MODE mode)
{
    //variable declaration
    //function can be called multiple times, so variables are made static to avoid useless allocations and deallocations of memory
    
    static bool username_edit_mode = false;
    static bool password_edit_mode = false;
    static bool log_reg_button_pressed = false;
    static bool back_button_pressed = false;
    static bool button_collision = false;
    static bool input_collision = false;

    static Rectangle InputBoxArray[RL_INPUT_AMOUNT];
    static Rectangle ButtonArray[RL_BUTTON_AMOUNT];
    static Vector2 UsernameLabelPos;
    static Vector2 PasswordLabelPos;

    static char button_label_text[10] = "";
    static char username_string[TEXT_BUFF_SIZE] = "";
    static char password_string[TEXT_BUFF_SIZE] = "";

    UsernameLabelPos.x = WINDOW_WIDTH * 0.1f;
    PasswordLabelPos.x = WINDOW_WIDTH * 0.1f;
    UsernameLabelPos.y = WINDOW_HEIGHT * 0.15f;
    PasswordLabelPos.y = WINDOW_HEIGHT * 0.4f;

    for (int i = 0; i < RL_INPUT_AMOUNT; i++)
    {
        InputBoxArray[i].x = WINDOW_WIDTH * 0.1f;
        InputBoxArray[i].y = (0.2f + (0.25f * i)) * WINDOW_HEIGHT;
        InputBoxArray[i].width = WINDOW_WIDTH * 0.5f;
        InputBoxArray[i].height = WINDOW_HEIGHT * 0.15f;
    }

    for (int i = 0; i < RL_BUTTON_AMOUNT; i++)
    {
        ButtonArray[i].x = (0.12f + (0.23f * i)) * WINDOW_WIDTH;
        ButtonArray[i].y = WINDOW_HEIGHT * 0.75f;
        ButtonArray[i].width = WINDOW_WIDTH * 0.2f;
        ButtonArray[i].height = WINDOW_HEIGHT * 0.1f;
    }

    if (mode == REGISTER_MODE)
    {
        strcpy(button_label_text, "REGISTER");
    }
    else if (mode == LOGIN_MODE)
    {
        strcpy(button_label_text, "LOGIN");
    }

    //function loop

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(DARKGREEN);

        if (GuiTextBox(InputBoxArray[0], username_string, TEXT_BUFF_SIZE - 1, username_edit_mode))
        {
            username_edit_mode = !username_edit_mode;
        }

        if (GuiTextBox(InputBoxArray[1], password_string, TEXT_BUFF_SIZE - 1, password_edit_mode))
        {
            password_edit_mode = !password_edit_mode;
        }

        DrawTextEx(DefaultFont, "USERNAME", UsernameLabelPos, 32, 5, WHITE);
        DrawTextEx(DefaultFont, "PASSWORD", PasswordLabelPos, 32, 5, WHITE);

        log_reg_button_pressed = GuiButton(ButtonArray[0], button_label_text);
        back_button_pressed = GuiButton(ButtonArray[1], "BACK");

        for (int i = 0; i < RL_INPUT_AMOUNT; i++)
        {
            if (CheckCollisionPointRec(GetMousePosition(), InputBoxArray[i]))
            {
                input_collision = true;
            }
        }

        if (!input_collision)
        {
            for (int i = 0; i < RL_BUTTON_AMOUNT; i++)
            {
                if (CheckCollisionPointRec(GetMousePosition(), ButtonArray[i]))
                {
                    button_collision = true;
                }
            }
        }

        if (input_collision)
        {
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
        }
        else if (button_collision)
        {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        }
        else
        {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }

        EndDrawing();

        if (back_button_pressed)
        {
            return 0;
        }
        else if (log_reg_button_pressed)
        {
            if (mode == LOGIN_MODE)
            {
                int server_query_result = server_login(username_string, password_string, LOGIN_MODE);
                return server_query_result;
            }
            else if (mode == REGISTER_MODE)
            {
                int server_query_result = server_login(username_string, password_string, REGISTER_MODE);
                return server_query_result;
            }
        }
    }


}