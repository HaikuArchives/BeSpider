#ifndef SOLITARE_H
#define SOLITARE_H

#define CARDS_IN_DECK 52
#define SPIDER_CARDS_IN_PLAY CARDS_IN_DECK * 2
#define MAX_CARDS_IN_PLAY SPIDER_CARDS_IN_PLAY

typedef struct card {
	short fValue;
	short fColor;
	bool fRevealed;
	short fEffect;
	bool fInPlay;
	card* fNextCard;
	card* fPrevCard;
} card;


#define STARTING_WINDOW_WIDTH 900
#define STARTING_WINDOW_HEIGHT 490
#define CARD_WIDTH 80
#define CARD_HEIGHT 116
#define CARDS_IN_SUIT 13
#define CARD_IMAGE_BACK 52
#define CARD_IMAGE_EMPTY 53
#define CACHED_BACKS 6


enum effect { E_NONE, E_ALPHA25, E_ALPHA50, E_ALPHA75,
	E_HIDDEN, E_GREEN, E_RED };

class Solitare {
public:
	Solitare();
	~Solitare();

	card* _FindLastUsed(short stock);
	card* _PickRandomCard();
	void _AddCardToPile(int pile, card* cardToAdd);
	void _RemoveCardFromPile(int pile, card* cardToRemove);

	card* fAllCards[MAX_CARDS_IN_PLAY]; // all cards
	card* fBoard[10]; // first card in each pile
};

#endif // SOLITARE_H
