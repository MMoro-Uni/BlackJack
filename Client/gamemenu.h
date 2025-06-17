/*!
* \file gamemenu.h
* \brief header that contains declaration of MainMenu() from the .c file.
*        also contains enum online_mode for the current connection state.
*/

typedef enum online_mode
{
    ONLINE_PLAY = 10,
    OFFLINE_PLAY
} ONMODE;

ONMODE MainMenu();