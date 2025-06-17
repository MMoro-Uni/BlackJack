DECK fill_deck(int deck_number);
void shuffle_deck(DECK* to_shuffle);
CARD draw_card(DECK* deck_to_use);
int calculate_score(CARD hand_to_evaluate[], int card_number);