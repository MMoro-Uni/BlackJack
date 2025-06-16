/*!
* \file main.c
* \brief main file of the program, calls menu and game functions.
* Specifically, the file contains
* - initialization of OpenGL context through RayLb
* - definition of RAYGUI_IMPLEMENTATION, needed to use the library
* - call for the menu function
* - call for the game function and handling of repeated games.
*/

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include "blackjackclient.h"
#include "gamewindow.h"

#ifndef ONLINE_MODE_CALL
#include "gamemenu.h"
#define ONLINE_MODE_CALL
#endif

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>


int main(void)
{

    //text buffers need to be in main to be available for game functions to save results
    //it would be possible to do everything through gamemenu, but that would obfuscate the logical structure.
    
    //initial raylib setup
    InitWindow(1200, 625, "BlackJack");
    SetTargetFPS(60);

    int online_play = MainMenu();
    int retry = 0;

    if (online_play == 0)
    {
        exit(1);
    }
    else
    {
        do
        {
            retry = GameStart(online_play);
        } while (retry == 1);
    }

    return 0;
}