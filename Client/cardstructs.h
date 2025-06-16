/*!
* \file cardstructs.h
* \brief contains structs to be used with cardfunctions.c and cardfunctions.h
* the header file is not included in the cardfunctions.h header, so it needs to be included when using the header
*/
typedef struct card
{
    int rank;
    char* suit;
} CARD;

typedef struct deck
{
    CARD* cards;
    int current_card_number;
} DECK;