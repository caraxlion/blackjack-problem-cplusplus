#include <iostream>
#include <cstdlib>
#include <stdlib.h>

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
 * Should permute the deck of cards, effectively shuffling it.
 * Using the Fisher-Yates / Durstenfeld shuffle algorithm
 */
void shuffle(int cards[])
{
  //Fisher-Yates/Durstenfeld Shuffle
  for(int i = 51; i > 0; i--) //for each card in the deck
  {
    int rando = rand()%(i+1); // gives an index in range 0-51
    int temp = cards[i]; // temporary variable for i-card in order to suffle
    cards[i] = cards[rando]; // switch i card to rando card
    cards[rando] = temp; // switch rando card to i card
  }
}

/**
 * Prints the card in a "type-suit" format
 */
void printCard(int id)
{
  int cardsuit;

  // determine suit
  if (id >= 13)
  {
    cardsuit = id/13;
  }
  else
  {
    cardsuit = 0;
  }

  // determine type
  int cardnum;
  if (id >= 13)
  {
    cardnum = id%13;
  }
  else
  {
    cardnum = id;
  }


  // print cards
  cout << type[cardnum] << "-" << suit[cardsuit];

}

/**
 * Returns the numeric value of the card.
 *  Should return 11 for an ACE 
 */
int cardValue(int id)
{
  int cardnum;

  // look-up table
  if (id >= 13)
  {
    cardnum = id%13;
  }
  else
  {
    cardnum = id;
  }

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
  // if statement for dealer first hand
  if (numCards == 1)
  {
    printCard(hand[1]);
  } // end dealer first hand
  else
  {
    // for loop to go through each card in the hand
    for (int i = 0; i < numCards; i++)
    {
      printCard(hand[i]);
      cout << " "; //print card for i'th card in hand
    }
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
    score += points;
    if (points == 11) // add if not an ace
    {
        aces += 1;
    }
  }

  //add in aces
  if (score > 21)
  {
    for (int j = 0; j < aces; j++)
    {
      //cout << "Check me" << endl;
     score -= 10;
     if(score <= 21)
     {
       break;
     }
    }
  }


  return score; // return the score
}

/**
 * Main program logic for the game of 21
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

  //cout << "Check 1" << endl;
  int dblbrk = 0;

  //while loop for continuous play
  while (play == 'y')
  {
    //initialize the card deck
    for(int num = 0; num < 52; num++)
    {
      cards[num] = num;
    }

    // shuffle the deck before we begin
    shuffle(cards);

    //deal the cards back and forth between dealer and player ***************************************
    phand[0] = cards[0];
    dhand[0] = cards[1];
    phand[1] = cards[2];
    dhand[1] = cards[3];

    //print the dealer's hand 
    cout << "Dealer: ? ";
    printHand(dhand, 1);

    //cout << "Check 4" << endl;

    //print player's hand *****************
    cout << "Player: ";
    printHand(phand, 2);

    //cout << "Check 5" << endl;

    // index counters ********************
    int didx = 2;
    int pidx = 2;
    int deck = 4;

    // track the score to see if it busts
    bool bust = false;

    // evaluate the scores *******************************************************************************
    while (bust != true) // go til ya fail
    {
      // get initial scores
      pscore = getBestScore(phand, pidx);
      dscore = getBestScore(dhand, didx);

      if (pscore < 21) // player score under or 21 ***********************************************************************
      {
        // prompt for hit or stay
        char hitstay;
        cout << "Type 'h' to hit and 's' to stay: " << endl;
        cin >> hitstay;

        // evaluate and proceed
        if (hitstay == 'h') // proceed with hit
        {
          // deal
          phand[pidx] = cards[deck];
          // increment counters
          pidx += 1;
          deck += 1;
          // print the hand
          cout << "Player: ";
          printHand(phand, pidx);
          pscore = getBestScore(phand, pidx);
        } // end hit

        else if (hitstay == 's') // dealer draws ************************************************************************
        {
          dscore =  getBestScore(dhand, didx);
          // dealer draws until >16 *****************************************
          while (dscore < 16)
          {
            // deal
            dhand[didx] = cards[deck];
            dscore = getBestScore(dhand, didx);
            //increment counters
            didx += 1;
            deck += 1;
          } // end while dscore < 16

          // print dealer hand  > 16**************************
          cout << "Dealer: ";
          printHand(dhand, didx);
          dscore = getBestScore(dhand,didx);

          // evaluate both scores
          if (dscore <= 21) // 17<dealer<21 **********************
          {
            if ((dscore > pscore) | (dscore == 21))
            {
              cout << "Lose " << pscore << " " << dscore << endl;
              bust = true;
            }
            else if (dscore == pscore) // draw
            {
              cout << "Tie " << pscore << " " << dscore << endl;
              bust = true;
            }
            else // dealer's score is less than players
            {
              cout << "Win " << pscore << " " << dscore << endl;
              bust = true;
            }
          } // end if dealer score 17 < x < 21

          else // dealer bust
          {
            cout << "Dealer busts" << endl;
            cout << "Win " << pscore << " " << dscore << endl;
            bust = true;
          } // end dealer bust

        } // end stay

        else // non h or s
        {
          dblbrk += 1;
          break;
        } 

      } // end pscore <= 21

      else if (pscore == 21) // pscore 21 ***************************************************************************
      {
         dscore =  getBestScore(dhand, 2);
          // dealer draws until >16 *****************************************
          while (dscore < 16)
          {
            // deal
            dhand[didx] = cards[deck];
            dscore = getBestScore(dhand, didx);
            //increment counters
            didx += 1;
            deck += 1;
          } // end while dscore < 16

          // print dealer hand  > 16**************************
          cout << "Dealer: ";
          printHand(dhand, didx-1);
          dscore = getBestScore(dhand,didx-1);

          // evaluate both scores
          if (dscore <= 21) // 17<dealer<21 **********************
          {
            if ((dscore > pscore) | (dscore == 21))
            {
              cout << "Lose " << pscore << " " << dscore << endl;
              bust = true;
            }
            else if (dscore == pscore) // draw
            {
              cout << "Tie " << pscore << " " << dscore << endl;
              bust = true;
            }
            else // dealer's score is less than players
            {
              cout << "Win " << pscore << " " << dscore << endl;
              bust = true;
            }
          } 
          else // dealer bust
          {
            cout << "Dealer busts" << endl;
            cout << "Win " << pscore << " " << dscore << endl;
            bust = true;
          } // end dealer bust
      }// end pscore == 21

      else // Player Bust *****************************************************************************************
      {
        cout << "Player busts" << endl;
        cout << "Lose " << pscore << " " << dscore << endl;
        bust = true;
      } // end player bust

    } // end while not bust
    if (dblbrk != 0)
    {
      break;
    }

    // play again statements **************************************************************************************
    cout << endl;
    cout << "Play Again? [y/n]" << endl;
    cin >> play;

  } // end play while loop

  return 0;
}

