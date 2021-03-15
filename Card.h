
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
};
