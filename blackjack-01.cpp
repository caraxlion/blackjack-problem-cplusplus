#include <iostream>
#include <cstdlib>

using namespace std;

/* Prototypes */
void shuffle(int cards[]);
void printCard(int id);
int cardValue(int id);
void printHand(int hand[], int numCards);
int getBestScore(int hand[], int numCards);

const int NUM_CARDS = 52;

/**
 * Global arrays to be used as look-up tables.
 */
const char suit[4] = {'H','S','D','C'};
const char* type[13] = 
  {"2","3","4","5","6","7",
   "8","9","10","J","Q","K","A"};
const int value[13] = {2,3,4,5,6,7,8,9,10,10,10,10,11};

/**
 * Fisher-Yates / Durstenfeld shuffle algorithm
 */
void shuffle(int cards[])
{
  //Fisher-Yates/Durstenfeld Shuffle
  for(int i = 0; i < 52; i++) //for each card in the deck
  {
    int rando =  rand()%(i+1);
    int temp = cards[i]; // temporary variable for i-card in order to suffle
    cards[i] = cards[rando]; // switch i card to j card
    cards[rando] = temp; // switch j card to i card
  }
}

/**
 * Prints the card in a "type-suit" format
 */
void printCard(int id)
{
  // look-up table for suit and card #
  int cardsuit = id/13;
  int cardnum = id%13;

  // print cards
  cout << type[cardnum] << "-" << suit[cardsuit];

}

/**
 * Returns the numeric value of the card.
 *  Should return 11 for an ACE
 */
int cardValue(int id)
{
  // look-up table
  int cardnum = id%13;
  // find card num value
  int cardval = value[cardnum];

  return cardval;
}

/**
 * Should print out each card in the hand separated by a space and
 * then print a newline at the end.  
 */
void printHand(int hand[], int numCards)
{
  // for loop to go through each card in the hand
  for (int i = 0; i < numCards; i++)
  {
    printCard(hand[i]);
    cout << " "; //print card for i'th card in hand
  }

  cout << endl; // new line at the end of the hand

}

/**
 * Should return the total score of the provided hand.  
 * ACES should be treated as 11s to make the highest possible hand
 *  and only being reduced to 1s as needed to avoid busting.
 */
int getBestScore(int hand[], int numCards)
{
  // decalre and initialize score 
  int score = 0;
  int aces = 0;

  // for loop to sum score
  for(int i = 0; i < numCards; i++)
  {
    // get the card value
    int points = cardValue(hand[i]);
    if (points != value[12]) // add if not an ace
    {
        score += points;
    }
    else
    {
        aces++; // aces count
    }
  }

  //add in aces
  if (aces > 0)
  {
    for (int j = 1; j <= aces; j++)
    {
        if(score > 10) // add ace as 1
        {
            score += 1;
        }
        else // add ace as 11
        {
            score += 11;
        }
    }
  }

  return score; // return the score
}

/**
 * Main program logic for the game of blackjack
 */
int main(int argc, char* argv[])
{
  if(argc < 2){
    cout << "Error - Please provide the seed value." << endl;
    return 1;
  }
  int seed = atoi(argv[1]);
  srand(seed);

  int cards[52];
  int dhand[9];
  int phand[9];

  char play = 'y'; // does the user want to play
  int pscore;
  int dscore;

  cout << "Check 1" << endl;

  // initialize the card deck
  for (int num = 0; num < 52; num++)
  {
    cards[num] = num;
  }

  // while loop for continuous play
  while (play == 'y')
  {
    cout << "Check 2" << endl;

    // shuffle the deck before we begin
    shuffle(cards);

    cout << "Check 3" << endl;

    // deal the cards back and forth between dealer and player ***************************************
    phand[0] = cards[0];
    dhand[0] = cards[1];
    phand[1] = cards[2];
    dhand[1] = cards[3];

    // print the dealer's hand
    cout << "Dealer: ? ";
    printHand(dhand[1], 1);

    cout << "Check 4" << endl;

    // print player's hand
    cout << "Player: ";
    printHand(phand, 2);

    // calculate the score
    pscore = getBestScore(phand, 2);

    // index counters
    int didx = 2;
    int pidx = 2;
    int deck = 4;

    cout << "Check 5" << endl;

    bool bust = false; // track the score to see if it busts

    // evaluate the scores *******************************************************************************
    while (bust != true) // go til someone fails
    {
      // get initial scores
        pscore = getBestScore(phand, pidx);
        dscore = getBestScore(dhand, pidx);

        if (pscore <= 21) // player score under or 21 ***********************************************************************
        {
            // prompt for hits and stays
            char hitstay;
            cout << "Type 'h' to hit and 's' to stay: " << endl;
            cin >> hitstay;

            // evaluate and proceed
            if (hitstay == 'h') // proceed with hit ***********************************
            {
              // deal the card
              phand[pidx] = cards[deck];
              // increment counters
              numCards += 1;
              pidx += 1;
              deck += 1;
              // print the hand
              cout << "Player: ";
              printHand(phand, pidx);
              pscore = getBestScore(phand, pidx);
            }
            else if (hitstay == 's') // stay, dealer draws **********************************
            {
              // dealer draws until >16 *********************
              while (dscore <= 16)
              {
                // deal a card
                dhand[didx] = cards[deck];
                dscore = getBestScore(dhand, didx);
                // increment counters
                didx += 1;
                deck += 1;
              }

              // print dealer hand *****************************
              cout << "Dealer: ";
              printHand(dhand, didx);

              // evaluate scores
              if (dscore <= 21) // 17<dealer<21
              {
                if ((dscore > pscore) | (dscore == 21))
                {
                  cout << "Lose " << pscore << " " << dscore << endl;
                  bust = true;
                }
                else if (dscore == pscore)
                {
                  cout << "Tie " << pscore << " " << dscore << endl;
                  bust = true;
                }
                else
                {
                  cout << "Win " << pscore << " " << dscore << endl;
                  bust = true;
                }
              }
              else// dealer bust *******************************
              {
                cout << "Dealer busts" << endl;
                cout << "Win " << pscore << " " << dscore << endl;
                bust = true;
              }
            }
            else //  player bust *****************************************************
            {
              cout << "Player busts" << endl;
              cout << "Lose " << pscore << " " << dscore << endl;
              bust = true;
            }
        }
    }

    // play again statements **************************************************************************************
    cout << "Play Again? [y/n]" << endl;
    cin >> play;
  }

  return 0;
}
