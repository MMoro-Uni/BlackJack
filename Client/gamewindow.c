/*!
* \file gamewindow.c
* \brief contains code for game window drawing and game logic.
* The code in this file is not meant for usage in other projects. Don't include without modifying first.
*/

/*!
* \fn void GameWindow_init(float gw_screen_width, float gw_screen_height)
* \brief init function for the game window
* Global variables are used to avoid having to pass window sizes to every function and to avoid updates of values when no changes were made to the window.
* This also avoids the deallocation and reallocation of the same size of memory for variables when the function gets called again for retry.
* Variables used for the game logic are initialized every time the function is called.
* Variables of which the values are tied to the screen size are only initialized if the screen size has changed from the last call.
* This is done through the global variables screen_width and screen_height, which store the size of the previous window and are compared to the passed values.
* 
* NOTE: Buttons are stored in an array.
* Current values: ButtonArray[0] = hit, ButtonArray[1] = stand, ButtonArray[2] = surrend, ButtonArray[3] = double down, ButtonArray[4] = bet
* this is done to facilitate adding new buttons if needed for modifications to the game.
*/

/*!
* \fn int GameStart(ONMODE online_mode);
* \brief main function for game logic and window drawing.
* online_mode is used to determine if the function should call client functions for server calls to handle the money amount the player has.
* return value is 1 if the game is supposed to restart, 0 if the game should end.
* Currently, the only way of closing the game is through closing the window, so 0 should not be returned, but the value is still returned in case of future added features.
* NOTE: this function is the only function in the file with a loop, so other functions are called during the loop for drawing the screen.
*/

/*!
* \fn void window_draw_card(CARD card_to_draw, Rectangle place_to_draw)
* \brief utility function for drawing a given card in the given position.
*/

/*!
* \fn void draw_end_screen_init()
* \brief init function for draw_end_screen
* see GameWindow_init() for reason of the usage of init functions
*/

/*!
* \fn void draw_end_screen(SIGNAL mode)
* \brief draws the end screen
*        mode is used to determine text to draw on the end screen
* This functions also changes the value of the global bool retry if the retry button is pressed.
*/

/*!
* \fn void draw_bet_screen_init()
* \brief init function for draw_bet_screen
* see GameWindow_init() for reason of usage of init functions
*/

/*
* \fn int draw_bet_screen()
* \brief draws the screen for placing bets.
*        return value is used to return the bet
*/



#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <raylib.h>
#include <raygui.h>
#include "cardstructs.h"
#include "cardfunctions.h"
#include "blackjackclient.h"
#include "gamewindow.h"

#define BUTTON_NUMBER 5
#define MAX_CARD_NUMBER 5
#define NUMBER_OF_STD_DECKS 8
#define START_CARD_NUMBER 2
#define BLACKJACK_SCORE 21
#define MIN_DEALER_STAND_SCORE 17
//suit symbols are done through a font. These are the characters that have those symbols in the font.
#define CLUBS "}"
#define DIAMONDS "{"
#define HEARTS "<"
#define SPADES ">"
#define DEFAULT_BUFLEN 20


void GameWindow_init(float gw_screen_width, float gw_screen_height);
void window_draw_card(CARD card_to_draw, Rectangle place_to_draw);
void draw_end_screen_init();
void draw_end_screen(SIGNAL mode);
void draw_bet_screen_init();
int draw_bet_screen();

//GameWindow and file wide global variables
//variables for other functions that had to be global for usage of init functions are declare before the init function
static CARD PlayerHand[MAX_CARD_NUMBER] = { 0 };
static CARD DealerHand[MAX_CARD_NUMBER] = { 0 };
static DECK GameDeck;
static SIGNAL endgame_code;

static int player_card_number;
static int player_hand_score;
static int dealer_card_number;
static int dealer_hand_score;
static int money_value;
static int money_bet;
static int rel_x;
static int rel_y;

static char money_value_string[DEFAULT_BUFLEN];
static char money_bet_string[DEFAULT_BUFLEN];

static float screen_width = 0;
static float screen_height = 0;

static Font CardFont;
static Font SuitFont;
static Font DefaultFont;

static Rectangle PlayerCardRecArray[MAX_CARD_NUMBER];
static Rectangle DealerCardRecArray[MAX_CARD_NUMBER];
static Rectangle ButtonArray[BUTTON_NUMBER];
static Rectangle ButtonAreaBGRec;

static Vector2 MoneyTextVec;
static Vector2 MoneyBetTextVec;

static bool hit_button_pressed;
static bool stand_button_pressed;
static bool surrend_button_pressed;
static bool doubledown_button_pressed;
static bool bet_button_pressed;
static bool end_game;
static bool retry;
static bool set_bet;
static bool updated_money;
static bool button_collision;
static bool sizes_changed;
static bool end_screen_changed;
static bool bet_screen_changed;


void GameWindow_init(float gw_screen_width, float gw_screen_height)
{
    sizes_changed = false;

    GameDeck = fill_deck(NUMBER_OF_STD_DECKS);
    shuffle_deck(&GameDeck);

    for (int i = 0; i < START_CARD_NUMBER; i++)
    {
        PlayerHand[i] = draw_card(&GameDeck);
        DealerHand[i] = draw_card(&GameDeck);
    }

    player_card_number = START_CARD_NUMBER;
    dealer_card_number = START_CARD_NUMBER;
    player_hand_score = calculate_score(PlayerHand, player_card_number);
    dealer_hand_score = calculate_score(DealerHand, dealer_card_number);

    money_value = 0;
    money_bet = 0;
    endgame_code = 0;

    strcpy(money_value_string, "");
    strcpy(money_bet_string, "");

    end_game = false;
    retry = false;
    set_bet = false;
    updated_money = false;

    CardFont = LoadFontEx("JqkasWild-w1YD6.ttf", 128, 0, 250);
    SuitFont = LoadFontEx("JqkasWild-w1YD6.ttf", 32, 0, 250);
    DefaultFont = GetFontDefault();

    hit_button_pressed = false;
    stand_button_pressed = false;
    surrend_button_pressed = false;
    doubledown_button_pressed = false;
    bet_button_pressed = false;
    button_collision = false;

    if (gw_screen_width != screen_width)
    {
        screen_width = gw_screen_width;

        for (int i = 0; i < MAX_CARD_NUMBER; i++)
        {
            PlayerCardRecArray[i].x = (0.03f + (i * 0.171f)) * screen_width;
            DealerCardRecArray[i].x = (0.03f + (i * 0.171f)) * screen_width;
            PlayerCardRecArray[i].width = screen_width * 0.156f;
            DealerCardRecArray[i].width = screen_width * 0.156f;
        }

        ButtonAreaBGRec.x = screen_width * 0.9f;
        ButtonAreaBGRec.width = screen_width * 0.1f;

        for (int i = 0; i < BUTTON_NUMBER; i++)
        {
            ButtonArray[i].x = screen_width * 0.91f;
            ButtonArray[i].width = screen_width * 0.08f;
        }

        MoneyTextVec.x = screen_width * 0.03f;
        MoneyBetTextVec.x = screen_width * 0.03f;

        sizes_changed = true;
    }

    if (gw_screen_height != screen_height)
    {
        screen_height = gw_screen_height;

        for (int i = 0; i < MAX_CARD_NUMBER; i++)
        {
            PlayerCardRecArray[i].y = screen_height * 0.55f;
            DealerCardRecArray[i].y = screen_height * 0.1f;
            PlayerCardRecArray[i].height = screen_height * 0.35f;
            DealerCardRecArray[i].height = screen_height * 0.35f;
        }

        ButtonAreaBGRec.y = 0;
        ButtonAreaBGRec.height = screen_height;

        for (int i = 0; i < BUTTON_NUMBER; i++)
        {
            ButtonArray[i].y = (0.01f + (i * 0.07f)) * screen_height;
            ButtonArray[i].height = screen_height * 0.05f;
        }

        MoneyTextVec.y = screen_height * 0.47f;
        MoneyBetTextVec.y = screen_height * 0.5f;

        sizes_changed = true;
    }

    if (sizes_changed)
    {
        //explanation of formula used at the end of the file
        float width = PlayerCardRecArray[0].width;
        float height = PlayerCardRecArray[0].height;
        float card_ratio = width / height;
        float temp_formula_value = 1 + (card_ratio * card_ratio);
        float diagonal_size = sqrtf((width * width) + (height * height));
        float diag_seg_size = diagonal_size * 0.1;

        rel_y = sqrtf((diag_seg_size * diag_seg_size) / temp_formula_value);
        rel_x = rel_y * card_ratio;
    }

    end_screen_changed = false;
    bet_screen_changed = false;



}



int GameStart(ONMODE online_mode)
{
    if (!IsWindowState(FLAG_BORDERLESS_WINDOWED_MODE))
    {
        SetWindowState(FLAG_BORDERLESS_WINDOWED_MODE);
    }
    GuiSetStyle(DEFAULT, TEXT_SIZE, 28);
    GuiSetStyle(DEFAULT, TEXT_SPACING, 1);
    
    
    float gw_screen_width = (float)GetScreenWidth();
    float gw_screen_height = (float)GetScreenHeight();


    GameWindow_init(gw_screen_width, gw_screen_height);

    //only needs to be called once since after retry the function gets called again
    if (online_mode == ONLINE_PLAY)
    {
        money_value = money_value_get();
    }

    while (!WindowShouldClose())
    {

        //checking button pressed and end game condition
        if (!end_game && !set_bet)
        {
            if (hit_button_pressed && player_card_number < MAX_CARD_NUMBER)
            {
                hit_button_pressed = false;
                player_card_number += 1;
                PlayerHand[player_card_number - 1] = draw_card(&GameDeck);
                player_hand_score = calculate_score(PlayerHand, player_card_number);
            }

            if (stand_button_pressed)
            {
                while (dealer_hand_score < MIN_DEALER_STAND_SCORE && dealer_card_number < MAX_CARD_NUMBER)
                {
                    dealer_card_number += 1;
                    DealerHand[dealer_card_number - 1] = draw_card(&GameDeck);
                    dealer_hand_score = calculate_score(DealerHand, dealer_card_number);
                }
            }

            if (doubledown_button_pressed)
            {
                player_card_number += 1;
                PlayerHand[player_card_number - 1] = draw_card(&GameDeck);
                player_hand_score = calculate_score(PlayerHand, player_card_number);

                while (dealer_hand_score < MIN_DEALER_STAND_SCORE && dealer_card_number < MAX_CARD_NUMBER)
                {
                    dealer_card_number += 1;
                    DealerHand[dealer_card_number - 1] = draw_card(&GameDeck);
                    dealer_hand_score = calculate_score(DealerHand, dealer_card_number);
                }

                money_bet *= 2;

                end_game = true;
            }

            if (surrend_button_pressed)
            {
                player_hand_score = 0;
                dealer_hand_score = 21;
                money_bet /= 2;
                end_game = true;
            }

            if (stand_button_pressed || player_hand_score > BLACKJACK_SCORE || player_card_number == MAX_CARD_NUMBER)
            {
                end_game = true;
            }

            if (bet_button_pressed && online_mode == ONLINE_PLAY)
            {
                set_bet = true;
            }
        }

        BeginDrawing();

        ClearBackground(DARKGREEN);

        for (int i = 0; i < player_card_number; i++)
        {
            window_draw_card(PlayerHand[i], PlayerCardRecArray[i]);
        }

        for (int i = player_card_number; i < MAX_CARD_NUMBER; i++)
        {
            DrawRectangleRec(PlayerCardRecArray[i], GREEN);
        }

        if (dealer_card_number == START_CARD_NUMBER && !stand_button_pressed && !end_game)
        {
            for (int i = 0; i < START_CARD_NUMBER - 1; i++)
            {
                window_draw_card(DealerHand[i], DealerCardRecArray[i]);
            }

            DrawRectangleRec(DealerCardRecArray[START_CARD_NUMBER - 1], DARKBLUE);

            for (int i = START_CARD_NUMBER; i < MAX_CARD_NUMBER; i++)
            {
                DrawRectangleRec(DealerCardRecArray[i], GREEN);
            }
        }
        else
        {
            for (int i = 0; i < dealer_card_number; i++)
            {
                window_draw_card(DealerHand[i], DealerCardRecArray[i]);
            }

            for (int i = dealer_card_number; i < MAX_CARD_NUMBER; i++)
            {
                DrawRectangleRec(DealerCardRecArray[i], GREEN);
            }
        }

        DrawRectangleRec(ButtonAreaBGRec, BROWN);
        hit_button_pressed = GuiButton(ButtonArray[0], "HIT!");
        stand_button_pressed = GuiButton(ButtonArray[1], "STAND");
        surrend_button_pressed = GuiButton(ButtonArray[2], "SURREND");
        doubledown_button_pressed = GuiButton(ButtonArray[3], "DOUBLE!");
        bet_button_pressed = GuiButton(ButtonArray[4], "BET!");

        if (online_mode == ONLINE_PLAY)
        {
  
            
            sprintf_s(money_value_string, DEFAULT_BUFLEN, "MONEY = %d", money_value);
            DrawTextEx(DefaultFont, money_value_string, MoneyTextVec, 32, 3, WHITE);
            sprintf_s(money_bet_string, DEFAULT_BUFLEN, "BET = %d", money_bet);
            DrawTextEx(DefaultFont, money_bet_string, MoneyBetTextVec, 32, 3, WHITE);
        }

        button_collision = false;

        for (int i = 0; i < BUTTON_NUMBER; i++)
        {
            if (CheckCollisionPointRec(GetMousePosition(), ButtonArray[i]))
            {
                button_collision = true;
            }
        }

        if (button_collision)
        {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        }
        else
        {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }

        if (set_bet)
        {
            money_bet = draw_bet_screen();
        }

        if (end_game)
        {
            if (dealer_hand_score < player_hand_score && player_hand_score < BLACKJACK_SCORE)
            {
                endgame_code = PLAYER_WIN;

                if ((!updated_money) && (online_mode == ONLINE_PLAY))
                {
                    money_value_update(money_bet);
                    updated_money = true;
                }
            }
            else if (dealer_hand_score > player_hand_score && dealer_hand_score <= BLACKJACK_SCORE)
            {
                endgame_code = PLAYER_LOSE;

                if ((!updated_money) && (online_mode == ONLINE_PLAY))
                {
                    money_value_update(-money_bet);
                    updated_money = true;
                }
            }
            else if (player_hand_score > BLACKJACK_SCORE)
            {
                endgame_code = PLAYER_BUST;

                if ((!updated_money) && (online_mode == ONLINE_PLAY))
                {
                    money_value_update(-money_bet);
                    updated_money = true;
                }
            }
            else if (dealer_hand_score > BLACKJACK_SCORE)
            {
                endgame_code = PLAYER_WIN;

                if ((!updated_money) && (online_mode == ONLINE_PLAY))
                {
                    money_value_update(money_bet);
                    updated_money = true;
                }
            }
            else if (player_card_number == MAX_CARD_NUMBER)
            {
                endgame_code = PLAYER_CHARLIE;

                if ((!updated_money) && (online_mode == ONLINE_PLAY))
                {
                    money_value_update(money_bet);
                    updated_money = true;
                }
            }
            else if (player_hand_score == BLACKJACK_SCORE)
            {
                endgame_code = PLAYER_BLACKJACK;

                if ((!updated_money) && (online_mode == ONLINE_PLAY))
                {
                    money_value_update(money_bet * 2);
                    updated_money = true;
                }
            }
            else if (player_hand_score == dealer_hand_score)
            {
                endgame_code = PLAYER_DRAW;
            }

            draw_end_screen(endgame_code);
        }

        EndDrawing();

        if (retry)
        {
            free(GameDeck.cards);
            return 1;
        }
    }

    free(GameDeck.cards);
    return 0;
}

void window_draw_card(CARD card_to_draw, Rectangle place_to_draw)
{
    static Vector2 suit_text_size;
    static Vector2 suit_pos_1;
    static Vector2 suit_pos_2;
    static Vector2 suit_pos_3;
    static Vector2 suit_pos_4;

    suit_text_size = MeasureTextEx(SuitFont, CLUBS, (float)SuitFont.baseSize, 0);

    suit_pos_1.x = (place_to_draw.x + rel_x) - (suit_text_size.x / 2.0f);
    suit_pos_1.y = (place_to_draw.y + rel_y) - (suit_text_size.y / 2.0f);
    suit_pos_2.x = (place_to_draw.x + place_to_draw.width - rel_x) - (suit_text_size.x / 2.0f);
    suit_pos_2.y = (place_to_draw.y + rel_y) - (suit_text_size.y / 2.0f);
    suit_pos_3.x = (place_to_draw.x + rel_x) - (suit_text_size.x / 2.0f);
    suit_pos_3.y = (place_to_draw.y + place_to_draw.height - rel_y) - (suit_text_size.y / 2.0f);
    suit_pos_4.x = (place_to_draw.x + place_to_draw.width - rel_x) - (suit_text_size.x / 2.0f);
    suit_pos_4.y = (place_to_draw.y + place_to_draw.height - rel_y) - (suit_text_size.y / 2.0f);

    static char suit_string[2] = "";

    if (strcmp(card_to_draw.suit, "Clubs") == 0)
    {
        strcpy(suit_string, CLUBS);
    }
    else if (strcmp(card_to_draw.suit, "Diamonds") == 0)
    {
        strcpy(suit_string, DIAMONDS);
    }
    else if (strcmp(card_to_draw.suit, "Hearts") == 0)
    {
        strcpy(suit_string, HEARTS);
    }
    else
    {
        strcpy(suit_string, SPADES);
    }

    static Color text_color;
    text_color = (strcmp(suit_string, CLUBS) == 0 || strcmp(suit_string, SPADES)) ? BLACK : RED;

    DrawRectangleRec(place_to_draw, RAYWHITE);

    DrawTextEx(SuitFont, suit_string, suit_pos_1, (float)SuitFont.baseSize, 0, text_color);
    DrawTextEx(SuitFont, suit_string, suit_pos_2, (float)SuitFont.baseSize, 0, text_color);
    DrawTextEx(SuitFont, suit_string, suit_pos_3, (float)SuitFont.baseSize, 0, text_color);
    DrawTextEx(SuitFont, suit_string, suit_pos_4, (float)SuitFont.baseSize, 0, text_color);


    static char rank_str_array[][3] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
    static char rank_str[3] = "";

    strcpy(rank_str, rank_str_array[card_to_draw.rank - 1]);

    static Vector2 rank_text_size;
    rank_text_size = MeasureTextEx(CardFont, rank_str, (float)CardFont.baseSize, 2);

    static Vector2 rank_position;
    rank_position.x = (place_to_draw.x + (place_to_draw.width / 2.0f)) - (rank_text_size.x / 2.0f);
    rank_position.y = (place_to_draw.y + (place_to_draw.height / 2.0f)) - (rank_text_size.y / 2.0f);

    DrawTextEx(CardFont, rank_str, rank_position, (float)CardFont.baseSize, 2, text_color);
}

static Rectangle EndMessageBGRec;
static Rectangle RetryButtonRec;

static Vector2 EndscreenTextPos;
static Vector2 EndTextSize;
static Vector2 EndTextFinalPos;

static char EndMessage[DEFAULT_BUFLEN];


void draw_end_screen_init()
{
    if (sizes_changed && !end_screen_changed)
    {
        EndMessageBGRec.x = screen_width * 0.2f;
        EndMessageBGRec.y = screen_height * 0.3f;
        EndMessageBGRec.width = screen_width * 0.5f;
        EndMessageBGRec.height = screen_height * 0.4f;

        EndscreenTextPos.x = screen_width * 0.45f;
        EndscreenTextPos.y = screen_height * 0.4f;

        RetryButtonRec.x = screen_width * 0.4f;
        RetryButtonRec.y = screen_height * 0.5f;
        RetryButtonRec.width = screen_width * 0.1f;
        RetryButtonRec.height = screen_height * 0.05f;

        end_screen_changed = true;
    }
}

void draw_end_screen(SIGNAL mode)
{
    draw_end_screen_init();

    DrawRectangleRec(EndMessageBGRec, GOLD);

    switch (mode)
    {
    case PLAYER_WIN:
        strcpy(EndMessage, "You Win!");
        break;
    case PLAYER_LOSE:
        strcpy(EndMessage, "You Lose!");
        break;
    case PLAYER_BUST:
        strcpy(EndMessage, "Bust! You Lose!");
        break;
    case PLAYER_CHARLIE:
        strcpy(EndMessage, "Charlie! You Win!");
        break;
    case PLAYER_BLACKJACK:
        strcpy(EndMessage, "BlackJack!");
        break;
    case PLAYER_DRAW:
        strcpy(EndMessage, "It's a Draw!");
        break;
    }

    EndTextSize = MeasureTextEx(DefaultFont, EndMessage, 96, 2);

    EndTextFinalPos.x = EndscreenTextPos.x - (EndTextSize.x / 2.0f);
    EndTextFinalPos.y = EndscreenTextPos.y - (EndTextSize.y / 2.0f);

    DrawTextEx(DefaultFont, EndMessage, EndTextFinalPos, 96, 2,  BROWN);

    static bool retry_button_pressed;

    retry_button_pressed = GuiButton(RetryButtonRec, "RETRY");

    if (CheckCollisionPointRec(GetMousePosition(), RetryButtonRec))
    {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }
    else
    {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    if (retry_button_pressed)
    {
        retry = true;
    }

}

static Rectangle BetWindowBGRec;
static Rectangle BetInputBoxRec;
static Rectangle BetButtonRec;

static Vector2 BetLabelPositionVec;



void draw_bet_screen_init()
{
    
    if (sizes_changed && !bet_screen_changed)
    {
        BetWindowBGRec.x = screen_width * 0.2f;
        BetWindowBGRec.y = screen_height * 0.3f;
        BetWindowBGRec.width = screen_width * 0.5f;
        BetWindowBGRec.height = screen_height * 0.4f;

        BetInputBoxRec.x = screen_width * 0.25f;
        BetInputBoxRec.y = screen_height * 0.5f;
        BetInputBoxRec.width = screen_width * 0.2f;
        BetInputBoxRec.height = screen_height * 0.1f;

        BetButtonRec.x = screen_width * 0.47f;
        BetButtonRec.y = BetInputBoxRec.y;
        BetButtonRec.width = screen_width * 0.1f;
        BetButtonRec.height = BetInputBoxRec.height;

        Vector2 BetLabelSize = MeasureTextEx(DefaultFont, "INSERT BET", 32, 3);

        BetLabelPositionVec.x = (screen_width * 0.45f) - (BetLabelSize.x / 2.0f);
        BetLabelPositionVec.y = (screen_height * 0.35f) - (BetLabelSize.y / 2.0f);

        bet_screen_changed = true;
    }
}

int draw_bet_screen()
{
    draw_bet_screen_init();
    
    DrawRectangleRec(BetWindowBGRec, DARKGRAY);
    DrawTextEx(DefaultFont, "INSERT BET", BetLabelPositionVec, 32, 3, WHITE);

    static bool bet_button_pressed;
    bet_button_pressed = GuiButton(BetButtonRec, "BET!");

    static bool Gui_edit_state = false;
    static char money_buffer[10] = "";

    if (GuiTextBox(BetInputBoxRec, money_buffer, 9, Gui_edit_state))
    {
        Gui_edit_state = !Gui_edit_state;
    }

    if (CheckCollisionPointRec(GetMousePosition(), BetInputBoxRec))
    {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    }
    else if (CheckCollisionPointRec(GetMousePosition(), BetButtonRec))
    {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }
    else
    {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    if (bet_button_pressed)
    {
        static int return_value;
        if (strlen(money_buffer) > 0)
        {
            return_value = (int)strtol(money_buffer, NULL, 10);

            if (return_value < 0)
            {
                return_value = 0;
            }
        }
        else
        {
            return_value = 0;
        }

        set_bet = false;
        return return_value;
    }
}




/*
* EXPLANATION OF FORMULA USED IN GameWindow_init
* 
* the goal of the formula is to find a point on the diagonal of a rectangle given its height and its width
* 
* We can think of the segment of the diagonal that corresponds to a given size (in this case 10% of its size) as the diagonal of a smaller rectangle of unknow height and width.
* From the height and width of the card rectangle we can ovtain the ratio of the two sizes, which will remain the same in the smaller rectangle we are considering.
* By substituting in the formula for the diagonal the x with y * ratio we can then obtain that the diagonal is equal to the square root of y^2 * (1 + ratio^2).
* By taking the square of this formula (so diagonal^2 and remove the square root) we can then divide the square of the diagonal by (1 + ratio^2) to obtain y^2.
* After finding the square root of the obtained y, we can multiply it by the ratio to find the value of x, finding the point on the diagonal of the caard square.
*/