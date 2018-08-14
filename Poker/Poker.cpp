
/****************************************************************************/
/*                A simple Texas Hold'em Poker simulartion.                 */
/*                Program runs fine, but gameplay still has some bugs.      */
/****************************************************************************/
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <ctime>
#ifdef __linux__
#include <unistd.h>
#elif _WIN32
#include <windows.h>
#endif

using namespace std;

const int suits_count = 4;
const int ranks_count = 13;
const int sleep_time = 2;

std::string suits[suits_count];
std::string ranks[ranks_count];

void press_Enter()
{
	printf("\nPress Enter to continue.\n");
	while (getchar() != '\n')
		;
}

void _sleep(int time)
{
#ifdef __linux__
	usleep(time * 100000);
#elif _WIN32
	Sleep(time);
#endif
}

class Card
{
  public:
	int suit;
	int rank;
};

int compareCards(const void *card1, const void *card2)
{
	return (*(Card *)card1).rank - (*(Card *)card2).rank;
}

class Deck
{

  private:
	int top;
	static const int card_tally = 52;

	Card cards[card_tally];

  public:
	Deck()
	{
		for (int i = 0; i < suits_count; i++)
		{
			for (int j = 0; j < ranks_count; j++)
			{
				cards[i * ranks_count + j].suit = i;
				cards[i * ranks_count + j].rank = j;
			}
		}
		suits[0] = "Diamonds";
		suits[1] = "Spades";
		suits[2] = "Hearts";
		suits[3] = "Clubs";

		ranks[0] = "2";
		ranks[1] = "3";
		ranks[2] = "4";
		ranks[3] = "5";
		ranks[4] = "6";
		ranks[5] = "7";
		ranks[6] = "8";
		ranks[7] = "9";
		ranks[8] = "10";
		ranks[9] = "Joker";
		ranks[10] = "Queen";
		ranks[11] = "King";
		ranks[12] = "Ace";
	}

	void print()
	{
		std::cout << "Printing the deck..." << std::endl;
		_sleep(1);
		for (int i = 0; i < card_tally; i++)
		{
			std::cout << ranks[cards[i].rank] << suits[cards[i].suit] << std::endl;
		}
		printf("\n");
	}

	void shuffle()
	{
		top = card_tally - 1;

		for (int i = 0; i < suits_count; i++)
		{
			for (int j = 0; j < ranks_count; j++)
			{
				cards[i * ranks_count + j].suit = i;
				cards[i * ranks_count + j].rank = j;
			}
		}

		int x;
		Card tempCard;
		for (int i = 0; i < card_tally; i++)
		{
			x = rand() % card_tally;
			tempCard = cards[i];
			cards[i] = cards[x];
			cards[x] = tempCard;
		}
	}

	Card hitme()
	{
		top--;
		return cards[top + 1];
	}
};

class Player
{
  public:
	std::string name;
	int money;
	Card cards[2];
	bool playing;
	int round;
	int goodToGo;
};

class PokerGame
{
  public:
	void start(const std::string &name)
	{
		for (int i = 0; i < players_count; i++)
		{
			players[i].money = 1000;
			players[i].playing = true;
		}

		player_index = 4;

		string names[12];
		names[0] = "Wojciech";
		names[1] = "Tristan";
		names[2] = "Michal";
		names[3] = "Edyta";
		names[4] = "Marcin";
		names[5] = "Kamil";
		names[6] = "Tymoteusz";
		names[7] = "Patryk";
		names[8] = "Norbert";
		names[9] = "Hubert";
		names[10] = "Krzysztof";
		names[11] = "Iga";

		for (int i = 0; i < players_count; i++)
			players[i].name = names[i % 12];

		players[player_index].name = name;

		startGame();
	}

	void deal()
	{
		for (int i = 0; i < players_count; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				if (players[i].playing)
				{
					players[i].cards[j] = deck1.hitme();
				}
			}
		}
		for (int i = 0; i < 5; i++)
			tableCards[i].rank = -1;
	}

	void flop()
	{
		for (int i = 0; i < 3; i++)
			tableCards[i] = deck1.hitme();
	}

	void turn()
	{
		tableCards[3] = deck1.hitme();
	}

	void river()
	{
		tableCards[4] = deck1.hitme();
	}

	void printName(char namesRow[], int position, string name)
	{
		const char *cName = name.c_str();
		for (int i = 0; i < name.length(); i++)
			namesRow[position + i] = cName[i];
	}

	void printMoney(char moneyRow[], int position, int money)
	{
		char cMoney[8];
		sprintf(cMoney, "$ %d|", money);
		for (int i = 0; i < 8; i++)
		{
			if (cMoney[i] != '|')
				moneyRow[position + i] = cMoney[i];
			else
				break;
		}
	}

	void printBindToken(char namesRow[], int position)
	{
		namesRow[position] = '@';
	}

	void printHalfOfThePlayers(char namesRow[], char moneyRow[], bool printFirstHalf)
	{
		int position = 0;
		if (printFirstHalf)
			for (int i = 0; i < ((players_count + 1) / 2); i++)
			{
				if (players[i].playing)
				{
					position = ((80 / ((players_count + 1) / 2)) * i) + 3;
					printName(namesRow, position, players[i].name);
					if(bind==i) printBindToken(namesRow,position-1);
					printMoney(moneyRow, position, players[i].money);
				}
			}
		else
			for (int i = ((players_count + 1) / 2); i < players_count; i++)
			{
				if (players[i].playing)
				{
					position = ((80 / ((players_count + 1) / 2)) * (players_count - i - 1)) + 3;
					printName(namesRow, position, players[i].name);
					if(bind==i) printBindToken(namesRow,position-1);
					printMoney(moneyRow, position, players[i].money);
				}
			}
	}

	int checkCardWidth(Card card)
	{
		string cardRank = ranks[card.rank];
		string cardSuit = suits[card.suit];
		int rankLength = cardRank.length();
		int suitLength = cardSuit.length();

		return ((rankLength > suitLength) ? rankLength : suitLength) + 3; //card width
	}

	int checkTableWidth()
	{
		int cardsTotalWidth = 0;
		for (int i = 0; i < 5; i++)
		{
			int cardWidth = 0;
			if (tableCards[i].rank >= 0)
			{
				cardWidth = checkCardWidth(tableCards[i]);
				cardsTotalWidth += cardWidth + 2;
			}
		}
		return cardsTotalWidth + 2;
	}

	int printCard(char table[][81], int XPosition, int YPosition, Card card)
	{
		string cardRank = ranks[card.rank];
		string cardSuit = suits[card.suit];

		int cardWidth = checkCardWidth(card); //plus padding

		//upper and lower card edges
		for (int i = 1; i < cardWidth; i++)
		{
			table[YPosition][XPosition + i] = '_';
			table[YPosition + 3][XPosition + i] = '_';
		}
		//left/right edges
		for (int i = 1; i <= 3; i++)
		{
			table[YPosition + i][XPosition] = '|';
			table[YPosition + i][XPosition + cardWidth] = '|';
		}

		printName(table[YPosition + 1], XPosition + 2, cardRank);
		printName(table[YPosition + 2], XPosition + 2, cardSuit);

		return cardWidth;
	}

	int checkActiveTableWidth(char table[][81])
	{
		int activeGameWidth = 80;
		for (int i = 79; i >= 0; i--)
			if (table[0][i] == ' ')
				activeGameWidth--;
			else
				break;

		return activeGameWidth;
	}

	void printMainTable(char table[][81], int YPosition)
	{
		int tableWidth = checkTableWidth();
		int activeGameWidth = checkActiveTableWidth(table);
		int XPosition = (activeGameWidth / 2) - (tableWidth / 2) + 3;
		XPosition = (XPosition > 2) ? XPosition : 3;

		//print cards to table
		int cardsTotalWidth = 0;
		for (int i = 0; i < 5; i++)
		{
			int cardWidth = 0;
			if (tableCards[i].rank >= 0)
			{
				cardWidth = printCard(table, XPosition + cardsTotalWidth, YPosition + 2, tableCards[i]);
				cardsTotalWidth += cardWidth + 2;
			}
		}

		cardsTotalWidth -= 2;
		cardsTotalWidth = cardsTotalWidth >= 10 ? cardsTotalWidth : 10; //min width 10

		//printing table around cards
		for (int i = 0; i <= cardsTotalWidth; i++)
		{
			table[YPosition][XPosition + i] = '_';
			table[YPosition + 8][XPosition + i] = '_';
		}

		for (int i = 0; i < 6; i++)
		{
			table[YPosition + i + 2][XPosition - 2] = '|';
			table[YPosition + i + 2][XPosition + cardsTotalWidth + 2] = '|';
		}
		table[YPosition + 1][XPosition - 1] = '/';
		table[YPosition + 1][XPosition + cardsTotalWidth + 1] = '\\';
		table[YPosition + 8][XPosition + cardsTotalWidth + 1] = '/';
		table[YPosition + 8][XPosition - 1] = '\\';

		//printing pot
		char potCString[10];
		sprintf(potCString, "%d", pot);
		string potString = string("Pot = $ ") + string(potCString);
		int potStart = ((cardsTotalWidth + 2) / 2) - (potString.length() / 2);
		printName(table[9], XPosition + potStart, potString);
	}

	void printPlayersTwoCards(char table[][81], int YPosition)
	{
		printName(table[YPosition],3,"Your hand:");

		int cardXPosition = 3;
		for(int i=0;i<2;i++)
		{
			Card actualCard = players[player_index].cards[i];
			int cardsWidth = checkCardWidth(actualCard);
			printCard(table,cardXPosition,17,actualCard);
			cardXPosition += cardsWidth + 3;
		}
	}

	void printTable()
	{
		//creating table
		char table[22][81];
		for (int i = 0; i < 22; i++)
			for (int j = 0; j <= 80; j++)
				if (j == 80)
					table[i][j] = '\n';
				else
					table[i][j] = ' ';

		printHalfOfThePlayers(table[0], table[1], true);
		printMainTable(table, 2);
		printHalfOfThePlayers(table[13], table[14], false);
		if(players[player_index].round)
			printPlayersTwoCards(table,16);

		//printing table
		for (int i = 0; i < 22; i++)
			for (int j = 0; j <= 80; j++)
				cout << table[i][j];
	}

  private:
	static const int players_count = 13;
	Player players[players_count];
	Deck deck1;
	int bind = 0;
	Card tableCards[5];
	enum actions
	{
		Flop = 1,
		Check,
		Bet,
		Raise
	};
	int pot, bet, rational, betOn, winner, maxPoints, roundWinner;
	enum actions action;
	int handPoints[players_count];
	int bestHand[players_count][3];
	int player_index;

	int playersLeft()
	{
		int count = 0;
		for (int i = 0; i < players_count; i++)
			if (players[i].money > 0)
				count++;
		return count;
	}

	actions computerAction(int playerNum)
	{
		if (players[playerNum].cards[0].rank < 8 && players[playerNum].cards[1].rank < 8)
		{
			if (players[playerNum].cards[0].rank != players[playerNum].cards[1].rank)
				return Flop;
			else
				return Check;
		}
		else if (players[playerNum].cards[0].rank < 10 && players[playerNum].cards[1].rank < 10)
		{
			if (players[playerNum].cards[0].rank != players[playerNum].cards[1].rank)
				return Check;
			else
				return Bet;
		}
		else
		{
			return Bet;
		}
	}

	/*checks if someone still got bet/call*/
	bool playersToBet()
	{
		for (int i = 0; i < players_count; i++)
			if (players[i].round == 1 && players[i].goodToGo == 0)
				return true;

		return false;
	}

	actions readAction()
	{
		int iAction;
		cin >> iAction;
		return (actions)iAction;
	}

	void takeBets()
	{
		using std::cin;
		using std::cout;
		using std::endl;

		betOn = 0;
		for (int k = 0; k < players_count; k++)
			players[k].goodToGo = 0;

		for (int k = bind + 1; k < bind + players_count + 1; k++)
		{
			/* human player actions */
			if (k % players_count == player_index && players[player_index].round)
			{
				if (players[player_index].money > 0)
					if (betOn)
					{
						cout << "\t\t\t\tYour action: (1) FLOP (3) BET/CALL (4) RAISE ";
						action = readAction();
						while (action != Flop && action != Bet && action != Raise)
						{
							cout << "Invalid number pressed." << endl;
							cout << "\t\t\t\tYour action: (1) FLOP (3) BET/CALL (4) RAISE ";
							action = readAction();
						}
					}
					else
					{
						cout << "\t\t\t\tYour action: (1) FLOP (2) CHECK (3) BET/CALL ";
						action = readAction();
						while (action < Flop || action > Bet)
						{
							cout << "Invalid number pressed." << endl;
							cout << "\t\t\t\tYour action: (1) FLOP (2) CHECK (3) BET/CALL ";
							action = readAction();
						}
					}
				else if (betOn)
				{
					cout << "\t\t\t\tYour action: (1) FLOP ";
					action = readAction();
					while (action != Flop)
					{
						cout << "Invalid number pressed." << endl;
						cout << "\t\t\t\tYour action: (1) FLOP ";
						action = readAction();
					}
				}
				else
				{
					cout << "\t\t\t\tYour action: (1) FLOP (2) CHECK ";
					action = readAction();
					while (action < Flop || action > Check)
					{
						cout << "Invalid number pressed." << endl;
						cout << "\t\t\t\tYour action: (1) FLOP (2) CHECK ";
						action = readAction();
					}
				}

				cout << endl;

				if (action == Flop)
				{
					players[player_index].round = 0;
					cout << "\t- " << players[player_index].name << " flops...\n";
				}
				else if (action == Check)
				{
					cout << "\t+ " << players[player_index].name << " checks.\n";
					continue;
				}
				else if (action == Bet)
				{
					if (betOn)
					{
						if (betOn > players[player_index].money)
						{
							pot += players[player_index].money;
							players[player_index].money -= players[player_index].money;
						}
						else
						{
							pot += betOn;
							players[player_index].money -= betOn;
						}
						players[player_index].goodToGo = 1;
						cout << "\t+ " << players[player_index].name << " bets " << betOn << "$\n";
					}
					else
					{
						cout << "How much do you want to bet: ";
						cin >> bet;
						while (bet > players[player_index].money || bet < 1)
						{
							cout << "Invalid number to bet." << endl;
							cout << "How much do you want to bet: ";
							cin >> bet;
							cout << endl
								 << endl;
						}
						pot += bet;
						players[player_index].money -= bet;
						betOn = bet;
						players[player_index].goodToGo = 1;

						cout << "\t+ " << players[player_index].name << " bets " << bet << "$\n";
					}
				}
				else
				{
					if (betOn > players[player_index].money)
					{
						cout << "You can't raise, bet is already bigger than your money: ";
						pot += players[player_index].money;
						players[player_index].money -= players[player_index].money;
						players[player_index].goodToGo = 1;
						cout << "\t+ " << players[player_index].name << " bets " << players[player_index].money << "$\n";
					}
					else
					{
						int raise;
						cout << "How much do you want to raise: ";
						cin >> raise;
						while ((bet + raise) > players[player_index].money || raise < 0)
						{
							cout << "Invalid number to raise." << endl;
							cout << "How much do you want to raise: ";
							cin >> raise;
							cout << endl
								 << endl;
						}
						bet += raise;
						pot += bet;
						players[player_index].money -= bet;
						betOn = bet;
						players[player_index].goodToGo = 1;
						cout << "\t+ " << players[player_index].name << " raises " << raise << ". New bet: " << betOn << "$\n";
					}
				}
			}
			/* computers actions */
			else
			{
				if (players[k % players_count].round == 0)
				{
					continue;
				}
				rational = rand() % 2;
				if (rational)
				{
					action = computerAction(k % players_count);
				}
				else
				{
					action = (actions)(rand() % 4 + 1);
				}
				if (action == Flop)
				{
					players[k % players_count].round = 0;
					cout << "\t- " << players[k % players_count].name << " flops..." << endl;
				}
				else if (action == Check && betOn == 0)
				{
					cout << "\t+ " << players[k % players_count].name << " checks." << endl;
					continue;
				}
				else if (action == Bet || betOn == 0)
				{
					if (betOn)
					{
						if (betOn > players[k % players_count].money)
						{
							pot += players[k % players_count].money;
							players[k % players_count].money -= players[k % players_count].money;
						}
						else
						{
							pot += betOn;
							players[k % players_count].money -= betOn;
						}

						cout << "\t++ " << players[k % players_count].name << " calls!" << endl;
						players[k % players_count].goodToGo = 1;
					}
					else
					{
						if (players[k % players_count].money == 1)
							bet = 1;
						else
							bet = (rand() % (players[k % players_count].money - 1) + 1);
						pot += bet;
						players[k % players_count].money -= bet;
						cout << '\a';
						cout << "\t+ " << players[k % players_count].name << " bets " << bet << "$" << endl;
						betOn = bet;
						players[k % players_count].goodToGo = 1;
					}
				}
				else
				{
					if (betOn > players[k % players_count].money)
					{
						pot += players[k % players_count].money;
						players[k % players_count].money -= players[k % players_count].money;
					}
					else
					{
						int raise = rand() % (players[k % players_count].money + 1 - betOn);
						betOn += raise;
						pot += betOn;
						players[k % players_count].money -= betOn;
					}

					cout << "\t+++ " << players[k % players_count].name << " raises! New bet:" << betOn << endl;
					players[k % players_count].goodToGo = 1;
				}
				_sleep(1);
			}
		}

		if (betOn && playersToBet())
		{
			for (int k = bind + 1; k < bind + 7; k++)
			{
				if (k % players_count == player_index)
				{
					if (players[player_index].round && players[player_index].goodToGo == 0)
					{
						cout << "\t\t\t\t\tYour action: (1) FLOP (3) BET/CALL ";
						action = readAction();
						while (action != Flop && action != Bet)
						{
							cout << "Invalid number pressed." << endl;
							cout << "\t\t\t\t\tYour action: (1) FLOP (3) BET/CALL ";
							action = readAction();
							cout << endl
								 << endl;
						}
						if (action == Flop)
						{
							cout << "\t- " << players[player_index].name << " flops...\n";
							players[player_index].round = 0;
						}
						else
						{
							if (betOn > players[k % players_count].money)
							{
								pot += players[k % players_count].money;
								players[k % players_count].money -= players[k % players_count].money;
							}
							else
							{
								pot += betOn;
								players[k % players_count].money -= betOn;
							}
							players[player_index].goodToGo = 1;

							cout << "\t+ " << players[player_index].name << " bets " << betOn << "$\n";
						}
					}
				}

				else
				{
					if (players[k % players_count].round == 0 || players[k % players_count].goodToGo == 1)
						continue;
					action = (actions)(rand() % 2 + 1);
					if (action == Flop)
					{
						players[k % players_count].round = 0;
						cout << "\t- " << players[k % players_count].name << " flops..." << endl;
					}
					else
					{

						if (betOn > players[k % players_count].money)
						{
							pot += players[k % players_count].money;
							players[k % players_count].money -= players[k % players_count].money;
						}
						else
						{
							pot += betOn;
							players[k % players_count].money -= betOn;
						}
						cout << "\t++ " << players[k % players_count].name << " calls!" << endl;
						players[k % players_count].goodToGo = 1;
					}
				}
			}
		}
	}

	bool oneLeft()
	{
		int count = 0;
		for (int k = 0; k < players_count; k++)
			if (players[k].round)
				count++;
		if (count == 1)
			return true;
		else
			return false;
	}

	int getWinner()
	{
		int winner;
		for (int k = 0; k < players_count; k++)
			if (players[k].round)
				winner = k;
		return winner;
	}

	int getScore(Card hand[])
	{
		qsort(hand, 5, sizeof(Card), compareCards);
		int straight, flush, three, four, full, pairs, high;
		int k;

		straight = flush = three = four = full = pairs = high = 0;
		k = 0;

		/*checks for flush*/
		while (k < 4 && hand[k].suit == hand[k + 1].suit)
			k++;
		if (k == 4)
			flush = 1;

		/* checks for straight*/
		k = 0;
		while (k < 4 && hand[k].rank == hand[k + 1].rank - 1)
			k++;
		if (k == 4)
			straight = 1;

		/* checks for fours */
		for (int i = 0; i < 2; i++)
		{
			k = i;
			while (k < i + 3 && hand[k].rank == hand[k + 1].rank)
				k++;
			if (k == i + 3)
			{
				four = 1;
				high = hand[i].rank;
			}
		}

		/*checks for threes and fullhouse*/
		if (!four)
		{
			for (int i = 0; i < 3; i++)
			{
				k = i;
				while (k < i + 2 && hand[k].rank == hand[k + 1].rank)
					k++;
				if (k == i + 2)
				{
					three = 1;
					high = hand[i].rank;
					if (i == 0)
					{
						if (hand[3].rank == hand[4].rank)
							full = 1;
					}
					else if (i == 1)
					{
						if (hand[0].rank == hand[4].rank)
							full = 1;
					}
					else
					{
						if (hand[0].rank == hand[1].rank)
							full = 1;
					}
				}
			}
		}

		if (straight && flush)
			return 170 + hand[4].rank;
		else if (four)
			return 150 + high;
		else if (full)
			return 130 + high;
		else if (flush)
			return 110;
		else if (straight)
			return 90 + hand[4].rank;
		else if (three)
			return 70 + high;

		/* checks for pairs*/
		for (k = 0; k < 4; k++)
			if (hand[k].rank == hand[k + 1].rank)
			{
				pairs++;
				if (hand[k].rank > high)
					high = hand[k].rank;
			}

		if (pairs == 2)
			return 50 + high;
		else if (pairs)
			return 30 + high;
		else
			return hand[4].rank;
	}

	int tryHand(int array[], int player)
	{
		Card hand[5];

		/* get cards from table and player */
		for (int i = 1; i < 4; i++)
			hand[i - 1] = tableCards[array[i]];

		for (int i = 0; i < 2; i++)
			hand[i + 3] = players[player].cards[i];

		return getScore(hand);
	}

	void evaluateHands()
	{
		int stack[10], k;
		int currentPoints;

		for (int q = 0; q < players_count; q++)
		{
			if (players[q].round)
			{
				stack[0] = -1; /* -1 is not considered as part of the set */
				k = 0;
				while (1)
				{
					if (stack[k] < 4)
					{
						stack[k + 1] = stack[k] + 1;
						k++;
					}

					else
					{
						stack[k - 1]++;
						k--;
					}

					if (k == 0)
						break;

					if (k == 3)
					{
						currentPoints = tryHand(stack, q);
						if (currentPoints > handPoints[q])
						{
							handPoints[q] = currentPoints;
							for (int x = 0; x < 3; x++)
								bestHand[q][x] = stack[x + 1];
						}
					}
				}
			}
		}
	} /*end of evaluateHands() */

	void printPlayersHand(int player, int roundWinner)
	{
		//creating cards table
		char table[5][81];
		for (int i = 0; i < 5; i++)
			for (int j = 0; j <= 80; j++)
				if (j == 80)
					table[i][j] = '\n';
				else
					table[i][j] = ' ';

		//Get cards
		Card playerHand[5];
		for (int i = 0; i < 3; i++)
			playerHand[i] = tableCards[bestHand[roundWinner][i]];

		for (int i = 0; i < 2; i++)
			playerHand[i + 3] = players[player].cards[i];

		//print hand owner name
		printName(table[0], 3, players[player].name);
		//print cards to table
		int position = 3;
		for (int i = 0; i < 5; i++)
		{
			int cardWidth = printCard(table, position, 1, playerHand[i]);
			position += cardWidth + 2;
		}
		//printing cards table
		for (int i = 0; i < 5; i++)
			for (int j = 0; j <= 80; j++)
				cout << table[i][j];

		_sleep(3);
	}

	/* main gameplay function*/
	void startGame()
	{
		int i = 0;

		for (int i = 0; i < players_count; i++)
		{
			if (players[i].money <= 0)
				players[i].playing = 0;
		}

		while (playersLeft() > 1)
		{
			/* starting default values*/
			for (int z = 0; z < players_count; z++)
				if (players[z].money < 1)
				{
					players[z].playing = 0;
					players[z].round = 0;
				}
			for (int z = 0; z < players_count; z++)
			{
				if (players[z].playing)
					players[z].round = 1;
				handPoints[z] = -1;
			}
			for (int x = 0; x < players_count; x++)
			{
				for (int y = 0; y < 3; y++)
				{
					bestHand[x][y] = -1;
				}
			}

			/* checking for game over*/
			if (players[player_index].playing == 0)
			{
				std::cout << "You are out of money, sorry." << std::endl;
				std::cout << "Game over." << std::endl;
				break;
			}

			bind = i % players_count;
			while (players[bind].playing == false)
			{
				i++;
				bind = i % players_count;
			}

			/* paying bind */
			pot = 20;
			if (players[bind].money >= 20)
				players[bind].money -= 20;
			else
				players[bind].playing = 0;

			std::cout << "\n\n\n\n\n\n\n";
			std::cout << "\t\t\t\t\t ------ ROUND " << i + 1 << " ------\n\n\n";
			_sleep(1);
			deck1.shuffle();

			/* pre-flop */
			deal();
			_sleep(sleep_time);
			printTable();
			takeBets();
			if (oneLeft())
			{
				winner = getWinner();
				std::cout << players[winner].name << " wins $" << pot << "\n\n";
				i++;
				players[winner].money += pot;
				continue;
			}

			/* flop */
			flop();
			std::cout << std::endl;
			_sleep(sleep_time);
			printTable();
			takeBets();
			if (oneLeft())
			{
				winner = getWinner();
				std::cout << players[winner].name << " wins $" << pot << "\n\n";
				i++;
				players[winner].money += pot;
				continue;
			}

			/* turn */
			turn();
			std::cout << std::endl;
			_sleep(sleep_time);
			printTable();
			takeBets();
			if (oneLeft())
			{
				winner = getWinner();
				std::cout << players[winner].name << " wins $" << pot << "\n\n";
				i++;
				players[winner].money += pot;
				continue;
			}

			/* river */
			river();
			std::cout << std::endl;
			_sleep(sleep_time);
			printTable();
			takeBets();

			evaluateHands();

			/* find and declare round winner */
			maxPoints = 0;
			for (int q = 0; q < players_count; q++)
			{
				if (players[q].round)
				{
					if (handPoints[q] > maxPoints)
					{
						maxPoints = handPoints[q];
						roundWinner = q;
					}
				}
			}
			std::cout << std::endl;
			std::cout << players[roundWinner].name << " wins $" << pot << " with ";
			if (maxPoints < 30)
				std::cout << "HIGH CARD";
			else if (maxPoints < 50)
				std::cout << "SINGLE PAIR";
			else if (maxPoints < 70)
				std::cout << "TWO PAIRS";
			else if (maxPoints < 90)
				std::cout << "THREE OF A KIND";
			else if (maxPoints < 110)
				std::cout << "STRAIGHT";
			else if (maxPoints < 130)
				std::cout << "FLUSH";
			else if (maxPoints < 150)
				std::cout << "FULL HOUSE";
			else if (maxPoints < 170)
				std::cout << "FOUR OF A KIND";
			else
				std::cout << "STRAIGHT FLUSH";
			std::cout << "\n\n";

			std::cout << "Winners hand\n";
			printPlayersHand(roundWinner, roundWinner);

			std::cout << "Other hands\n";
			for (int i = 0; i < players_count; i++)
			{
				if (i == roundWinner || !players[i].playing)
					continue;
				printPlayersHand(i, roundWinner);
				std::cout << "\n";
			}

			players[roundWinner].money += pot;

			i++;
		}
	}
};

int main()
{
	std::string name;
	PokerGame game1;

	srand(time(NULL));

	using std::cout;
	using std::endl;

	cout << "Welcome to..." << endl
		 << endl;
	cout << "#######                        ###### " << endl;
	cout << "   #    ###### #    # #####    #     #  ####  #    # ###### #####" << endl;
	cout << "   #    #       #  #    #      #     # #    # #   #  #      #    #" << endl;
	cout << "   #    #####    ##     #      ######  #    # ####   #####  #    #" << endl;
	cout << "   #    #        ##     #      #       #    # #  #   #      #####" << endl;
	cout << "   #    #       #  #    #      #       #    # #   #  #      #   #" << endl;
	cout << "   #    ###### #    #   #      #        ####  #    # ###### #    #" << endl
		 << endl;

	cout << "Please type your name: ";
	//std::cin >> name;
	name = "Karolek";

	cout << "OK " << name << " let's play some poker!" << endl
		 << endl;

	game1.start(name);

	sleep(3);
	return 0;
}
