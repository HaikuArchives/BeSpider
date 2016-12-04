#include "Solitare.h"

#include <stdlib.h>
#include <stdio.h>


Solitare::Solitare()
{
	for(short i = 0; i < MAX_CARDS_IN_PLAY; i++)
		fAllCards[i] = NULL;
}


Solitare::~Solitare()
{
	for(short i = 0; i < MAX_CARDS_IN_PLAY; i++)
		delete fAllCards[i];
}


card* Solitare::_PickRandomCard()
{
	for(short i = 0; i < MAX_CARDS_IN_PLAY; i++) {
		if(fAllCards[i]->fInPlay == false) { // if card not in play
			fAllCards[i]->fInPlay = true; // move card into play
			return fAllCards[i]; // return that card
		}
	}
	printf("Error: ran out of cards to deal!\n");
	return NULL; // no card found
}


void Solitare::_AddCardToPile(int pile, card* cardToAdd) {
	if(fBoard[pile] == NULL) {
		fBoard[pile] = cardToAdd;
	} else {
		cardToAdd->fPrevCard = _FindLastUsed(pile);
		cardToAdd->fPrevCard->fNextCard = cardToAdd;
	}
}


void Solitare::_RemoveCardFromPile(int pile, card* cardToRemove) {
	if(fBoard[pile] == cardToRemove) { // first in pile
		fBoard[pile] = NULL;
	} else { // second or later in pile
		cardToRemove->fPrevCard->fNextCard = NULL;
		cardToRemove->fPrevCard = NULL;
	}
}


card* Solitare::_FindLastUsed(short stock) {
	card* currentCard = fBoard[stock];
	if(currentCard == NULL)
		return NULL;
	while(currentCard->fNextCard != NULL)
		currentCard = currentCard->fNextCard;

	return currentCard;
}
