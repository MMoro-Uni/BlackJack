/*!
* \file cardfuntctions.c
* \brief contains functions for handling cards.
* This file is designed to be able to be used in other projects.
* When using this file in other projects, remember to include the custom structs from cardstructs.h
*/

/*!
* \fn DECK fill_deck(int deck_number)
* \brief creates a deck of cards with number of cards equal to the amount of cards in the given number of standard 52 card decks.
* Rank of created cards goes from 1 to 13, while their suit is either "Clubs", "Diamonds", "Hearts" or "Spades".
* For usage in card games that don't use standard cards, remember to change suit and rank value or map the values to other values somewhere else.
* The CARD* in the deck struct is initialized as an array of CARD through malloc, remember to dealloc in the code.
* Return value is the created deck.
*/

/*!
* \fn void shuffle_deck(DECK* to_shuffle)
* \brief shuffles the DECK pointed by the given pointer.
* Shuffling is done through the Fisher-Yates shuffle.
*/

/*!
* \fn card_swap(CARD* first, CARD* second)
* \brief util function for shuffle_deck, swaps two cards in a given position
*/

/*!
* \fn CARD draw_card(DECK* deck_to_use)
* \brief draws a card from the deck pointed by given pointer and returns it
* CARDs inside the given DECK are shiften forward after the draw, but memory is not reallocated.
* This allows refilling the deck without reallocating and allows function usage with DECKs not created through fill_deck
*/

/*!
* \fn int calculate_score(CARD hand_to_evaluate[], int card_number)
* \brief calculates the score of a given blackjack hand and returns it
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cardstructs.h"


void card_swap(CARD* first, CARD* second);


DECK fill_deck(int deck_number)
{
    int rank;
    int suit_number;

    DECK created_deck = { 0 };
    CARD card_to_add = { 0 };

    created_deck.cards = malloc(52 * deck_number * sizeof(CARD));

    if (created_deck.cards == NULL)
    {
        printf("No memory to write data. Program closing.\n");
        exit(1);
    }

    for (int i = 0; i < deck_number; i++)
    {
        for (int j = 0; j < 52; j++)
        {
            suit_number = j / 13;
            rank = j + 1;

            while (rank > 13)
            {
                rank -= 13;
            }

            card_to_add.rank = rank;

            switch (suit_number)
            {
            case 0:
                card_to_add.suit = "Clubs";
                break;
            case 1:
                card_to_add.suit = "Diamonds";
                break;
            case 2:
                card_to_add.suit = "Hearts";
                break;
            case 3:
                card_to_add.suit = "Spades";
                break;
            }

            created_deck.cards[j + (i * 52)] = card_to_add;
        }     
    }

    created_deck.current_card_number = deck_number * 52;

    return created_deck;
}

void shuffle_deck(DECK* to_shuffle)
{
    srand(time(NULL));

    int card_number = to_shuffle->current_card_number;

        for (int i = (card_number - 1); i > 0; i--)
        {
            int  j = rand() % (i + 1);
            card_swap(&to_shuffle->cards[i], &to_shuffle->cards[j]);
        }
}

void card_swap(CARD* first, CARD* second)
{
    CARD temp = *first;
    *first = *second;
    *second = temp;
}

CARD draw_card(DECK* deck_to_use)
{
    CARD card_to_return = { 0 };

    card_to_return.rank = deck_to_use->cards[0].rank;
    card_to_return.suit = deck_to_use->cards[0].suit;

    int card_number = deck_to_use->current_card_number;

    for (int i = 0; i < card_number - 1; i++)
    {
        deck_to_use->cards[i].rank = deck_to_use->cards[i + 1].rank;
        deck_to_use->cards[i].suit = deck_to_use->cards[i + 1].suit;
    }

    return card_to_return;

}

int calculate_score(CARD hand_to_evaluate[], int card_number)
{
    int score_to_return = 0;

    for (int i = 0; i < card_number; i++)
    {
        if (hand_to_evaluate[i].rank == 1)
        {
            score_to_return += 11;
        }
        else if (hand_to_evaluate[i].rank > 10)
        {
            score_to_return += 10;
        }
        else
        {
            score_to_return += hand_to_evaluate[i].rank;
        }
    }

    if (score_to_return > 21)
    {
        for (int i = 0; i < card_number && score_to_return > 21; i++)
        {
            if (hand_to_evaluate[i].rank == 1)
            {
                score_to_return -= 10;
            }
        }
    }

    return score_to_return;
}