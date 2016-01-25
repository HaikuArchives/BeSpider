#ifndef SPIDER_H
#define SPIDER_H

#define CARDS_IN_DECK 52
#define CARDS_IN_PLAY CARDS_IN_DECK*2


typedef struct card {
	short fValue;
	short fColor;
	bool fRevealed;
	short fEffect;
	bool fInPlay;
	card* fNextCard;
	card* fPrevCard;
} card;


class Spider {
public:
	Spider();
	~Spider();

	card* _FindLastUsed(short stock);
	card* _PickRandomCard();
	void _AddCardToPile(int pile, card* cardToAdd);
	void _RemoveCardFromPile(int pile, card* cardToRemove);

	card* fAllCards[CARDS_IN_PLAY]; // all cards
	card* fBoard[10]; // first card in each pile
};

#endif // SPIDER_H
