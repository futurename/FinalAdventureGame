
enum CardType {
    INFANTRY, CALVARY, ARTILLERY
};

class Card {

private:
    CardType cardType;
public:

    Card(CardType card);

    CardType getCard();

    void setCard(CardType newCardType);

    static const int CARD_EXCHANGE_BASE = 5;
};
