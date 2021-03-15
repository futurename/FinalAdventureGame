//
// Created by Frank on 03/15/2021.
//

#ifndef FINALGAMEDESIGN_CARD_H
#define FINALGAMEDESIGN_CARD_H


class Card{
    enum PlayerCard { INFANTRY, CALVARY, artillery };

    private:
        PlayerCard cardType;
    public:
        Card(PlayerCard card);
        PlayerCard getCard();
        void setCard(PlayerCard newCardType);
};




#endif //FINALGAMEDESIGN_CARD_H
