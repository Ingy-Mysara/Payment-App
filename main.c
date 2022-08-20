#include "Card/card.h"
#include "Server/server.h"
#include "Terminal/terminal.h"

int main() {
    ST_cardData_t UserCardData;
    printf("%d\n", getCardHolderName(&UserCardData));
    printf("%d\n", getCardExpiryDate(&UserCardData));
    printf("%d", getCardPAN(&UserCardData));
}