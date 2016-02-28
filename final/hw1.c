//-----------------------------------------------------------------------------
//	Michael Benson
//	cs1129
//	hw1.c
//	hw1 will play a game of rock, paper, scissor vegas style agains a player
//	Extra Credit:
//	The computer's betting strategy has been modified to increase its bet 
//		by a factor of 4 for each hand it wins during a wining streak 
//		(see compStrategy function for detials)
//-----------------------------------------------------------------------------

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Function Declarations

int coinToss();
int compStrategy(int playerMoney, int compMoney, int compStreak);
bool evenCheck(int numberCheck);
int showdown(char playerThrow, char compThrow);
char compSelect();
void stats(int playerRock, int playerPaper, int playerScissor, int compRock, int compPaper, int compScissor, int roundCount);
int betting(int playerBalance, int compBalance, int roundWinner, int compStreak);

// Global constants to identify player, computer, and draw

const int player = 0;
const int computer = 1;
const int draw = -1;

int main()
{

	// Variable Declarations
	
	int playerBalance = 0;
	int compBalance = 0;
	
	int maxBank = 1000000;
	bool evenNumber = false;
	
	int roundCount = 0;
	int coin = 1;
	
	int potValue = 0;
	int bet = 0;
	int compStreak = 0;
	
	char compThrow;
	char playerThrow;
	
	int roundWinner = -1; // 0 = Player, 1 = Computer, -1 = Draw
	
	// Variables for statistics tracking
	int playerRock = 0;
	int playerPaper = 0;
	int playerScissor = 0;
	int compRock = 0;
	int compPaper = 0;
	int compScissor = 0;
	
	// Seed the random generator 
	srand(time(0));
	
	// Splash Logo
	cout << "\n\n\n\n\n\n";
	cout << "\t*******************\n";
	cout << "\t WELCOME TO VEGAS! \n";
	cout << "\t*******************\n\n\n";
	cout << "How much money would you like to work with?\n";
	
	cout << "$";
	cin >> playerBalance;
	evenNumber = evenCheck(playerBalance);
	
	// Check to make sure input was an even number, between 0 and maxBank
	while (playerBalance >= maxBank || playerBalance == 0 || evenNumber == false)
	{
		cout << "\nPlease select an even amout below $1,000,000\n";
		cout << "$";
		cin >> playerBalance;
		evenNumber = evenCheck(playerBalance);
	}
		
	// Everyone gets the same amount to start with
	compBalance = playerBalance;

	
	// Decided who get to go first with a coin toss
	cout << "\n\n\t Coin toss! In the air!\n";
	roundWinner = coinToss();
	
	if (roundWinner == player)
		cout << "      Congradulations! You bet first!";
	else
		cout << "Computer wins the toss! It will bet first";
	
	
	// Let the games begin
	while (playerBalance != 0 && compBalance != 0)
	{
		// Increment the round counter every cycle
		roundCount++;
		
		// Round information
		cout << "\n\n\n";
		cout << "\t --------- Round " << roundCount << "---------\n";
		cout << "\n Your Balance: $" << playerBalance << "\t Computer Balance: $" << compBalance;
		cout << "\n\n\t      -----Betting-----\n\n";

		// Call the betting funcion to get the bet value for the round
		bet = betting(playerBalance, compBalance, roundWinner, compStreak);
		playerBalance -= bet;
		compBalance -= bet;
		potValue = bet*2;

		// Show the user the bet value and the new balances of the computer and player
		cout << "\n Your Balance: $" << playerBalance << "\t Computer Balance: $" << compBalance << endl;
		cout << "\n\t\tPot Value: $" << potValue << endl;
		
		// Call compThrow to randomly generate what the computer will use (r/p/s)
		cout << "\n\t  ----Chose Your Weapon!----";
		compThrow = compSelect();
		
		// Ask the player for what they would like to use
		cout << "\n\n    The computer has decided what to throw\n";
		cout << "\n\tRock = r   Paper = p   Scissor = s\n\n";
		cout << "Your selection: ";
		cin >> playerThrow;
		
		// Check that the player used a valid option
		while (playerThrow != 'r' && playerThrow != 'p' && playerThrow != 's')
		{
			cout << "\n\n    Please use one of the following selections:";
			cout << "\n\tRock = r   Paper = p   Scissor = s\n\n";
			cout << "Your selection: ";
			cin >> playerThrow;
		}
		
		// Call showdown to determine the winner of the round
		roundWinner = showdown(playerThrow,compThrow);
	
		// Show the player what each used to they can confirm that we are not cheating
		cout << "\n    Computer threw : " << compThrow << "\tYou threw : " << playerThrow << endl;
	
		// Case if computer wins
		if (roundWinner == computer)
		{
			cout << "\n\tThe computer won the round\n";
			compBalance += potValue;
			compStreak += 1;
		}
		// Case if player wins
		else if (roundWinner == player)
		{
			cout << "\n\t    You won the round!\n";
			playerBalance += potValue;
			compStreak = 0;
		}
		// Only remaining case is a draw
		else
		{
			cout << "\n\t\tThe round is a draw\n";
			playerBalance += potValue / 2;
			compBalance += potValue / 2;
			
			cout << "\n\t    Cointoss For Next To Bet...\n";
			roundWinner = coinToss();
			
			if (roundWinner == player)
				cout << "\t\tYou will be next to bet\n";
			else
				cout << "\t\tComputer will be next to bet\n";
		}

		// Increment statistics trackers
		if (playerThrow == 'r')
			playerRock++;
		else if (playerThrow == 'p')
			playerPaper++;
		else
			playerScissor++;
		
		if (compThrow == 'r')
			compRock++;
		else if (compThrow == 'p')
			compPaper++;
		else
			compScissor++;
	}

	// Someones balance reached 0
	cout << "\t\tGAME OVER";
	
	// Check who won the game
	if (playerBalance == 0)
		cout << "\n\n\n\tThe computer won this game";
	else
		cout << "\n\n\n\t\tYou won!";

	// Call stats function to calculate statistics for the game
	stats(playerRock, playerPaper, playerScissor, compRock, compPaper, compScissor, roundCount);

	return 0;

} // End of main()




//------------------------------------------------
//	coinToss()
//	Pre-conditions
//	No input is needed for this function
//	Post-conditions
//	Returns 0 or 1 (Player or Computer)
//------------------------------------------------

int coinToss()
{
	// Variable Declaration	
	int coin = 0;
	
	// Upper bound for numbers generated from rand
	const int max = 2;
	
	// Keeps the result as 0 or 1 (player or comp)
	coin = rand() % max;
	
	return coin;
	
}

//-----------------------------------------------
//	compStrategy(int, int, int)
//	Pre-conditions
//	Needs the current balance for the player and computer
//	Post-conditions
//	Returns the amount of the computers bet
//-----------------------------------------------

int compStrategy(int playerMoney, int compMoney, int compStreak)
{
	// Variable Declaration
	int compBet;
	
	// Strategy if computer is winning
	if (compMoney >= playerMoney && playerMoney > 2)
		compBet = playerMoney / 2;
	// Strategy if computer is loseing
	else
		compBet = 2;
	
	// Aditional betting rules
	// If the computer has won more than 1 hand in a row,
	// 	increase the bet by a factor of 4
	if (compStreak > 0)	
		compBet += (compStreak - 1) * 4;
	
	// Make sure the computer is betting an even number
	compBet -= compBet % 2;

	// Check to make sure that the computers bet did not
	// 	exceed the amount the player or its self has
	if (compBet > playerMoney)
		compBet = playerMoney;
	if (compBet > compMoney)
		compBet = compMoney;	

	return compBet;
}

//----------------------------------------------
//	evenCheck(int)
//	Pre-conditions
//	Requires a numeric, integer input
//	Post-conditions
//	Returns bool isEven = true if the input was even
//	Returns bool isEven = false is the input was odd
//-----------------------------------------------

bool evenCheck(int numberCheck)
{
	// Variable Declarations
	int output = numberCheck % 2;
	bool isEven = true;
	
	// Checks numberCheck is even
	if (output == 1)
		isEven = false;
	
	return isEven;
}

//-----------------------------------------------
//	showdown(char, char)
//	Pre-conditions
//	Needs char's : what the player and computer selected
//	Post-conditions
//	Returns winStatus to reflect the winner of the round
//		based on the rules of RPS
//------------------------------------------------

int showdown(char playerThrow, char compThrow)
{
	// Variable Declarations
	int winStatus = player;
	
	// Computer win conditions
	if ( (compThrow == 'r'  && playerThrow == 's') || (compThrow == 's' && playerThrow == 'p') || (compThrow == 'p' && playerThrow == 'r') )
		winStatus = computer;
	// Player win conditions
	else if( (playerThrow == 'r' && compThrow == 's') || (playerThrow == 's' && compThrow == 'p') || (playerThrow == 'p' && compThrow == 'r') )
		winStatus = player;
	// If comp and player both do not win, it is a draw
	else
		winStatus = draw;
	
	return winStatus;
}

//-------------------------------------------------
//	compSelect()
//	Pre-conditions
//	No inputs
//	Post-conditions
//	Creates and returns the char. call based on a random
//		number for what the computer will throw
//--------------------------------------------------

char compSelect()
{
	// Variable declarations
	char call;
	int select = 0;
	const int max = 3; // upper bound for numbers
	
	// Selects 0-2
	select = rand() % max;

	// Based on select, asign a letter
	if (select == 0)
		call = 'r';
	else if (select == 1)
		call = 'p';
	else
		call = 's';
	
	return call;
}

//-------------------------------------------------------
//	stats(int, int, int, int, int, int, int)
//	stats has been moved out of the main function
//		to save clutter
//	Pre-conditions
//	Called at end of game
//		Requires counts of r, p, and s from player and computer
//	Post-conditions
//	Displays the number of times and percentage of the number
//		of each item thrown by the person and computer
//--------------------------------------------------------

void stats(int playerRock, int playerPaper, int playerScissor, int compRock, int compPaper, int compScissor, int roundCount)
{
	// Variable Declarations and calculations
	// Calculate the percentages directly in there declarations
	double playerPRock = (static_cast<double>(playerRock) / static_cast<double>(roundCount)) * 100;
	double playerPPaper = (static_cast<double>(playerPaper) / static_cast<double>(roundCount)) * 100;
	double playerPScissor = (static_cast<double>(playerScissor) / static_cast<double>(roundCount)) * 100;
	double compPRock = (static_cast<double>(compRock) / static_cast<double>(roundCount)) * 100;
	double compPPaper = (static_cast<double>(compPaper) / static_cast<double>(roundCount)) * 100;
	double compPScissor = (static_cast<double>(compScissor) / static_cast<double>(roundCount)) * 100;

	// Output the players stats
	cout << "\n\nYou threw:";
	cout << "\n\tNumber\tPercent\n";
	cout << "Rock\t" << playerRock << "\t" << playerPRock << endl;
	cout << "Paper\t" << playerPaper << "\t" << playerPPaper << endl;
	cout << "Scissor\t" << playerScissor << "\t" << playerPScissor << endl;

	// Output the computers stats
	cout << "\n\nThe computer threw:";
	cout << "\n\tNumber\tPercent\n";
	cout << "Rock\t" << compRock << "\t" << compPRock << endl;
	cout << "Paper\t" << compPaper << "\t" << compPPaper << endl;
	cout << "Scissor\t" << compScissor << "\t" << compPScissor << endl;
}

//-----------------------------------------------------
//	betting(int, int, int, int)
//	The betting funcion has been moved out of the main function
//		to save clutter.  
//	Pre-conditions
//	Must have the computer and player balances, the round winner, and the
//		current computer streak values
//	Post-conditions
//	Returns the value of bet, either input by the user or from the
//		compStrategy function
//--------------------------------------------------

int betting(int playerBalance, int compBalance, int roundWinner, int compStreak)
{
	// Variable Declarations
	int bet;
	bool evenNumber;

	// Check who won the last round, they get to bet
	// Player won
	if (roundWinner == player)
	{
		// Get the players bet
		cout << "\t\tYour bet: $";
		cin >> bet;
		
		// Call evenNumber to check if there input was even
		evenNumber = evenCheck(bet);

		// Check that the player used an even number 
		// 	and that its within there and the comp's balances
		while (evenNumber == false || bet > playerBalance || bet > compBalance)
		{
			cout << "\n Please select and even number within your and the computer's balance\n";
			cout << "\n Your Balance: $" << playerBalance;
			cout << "\t Computer Balance: $" << compBalance;
			cout << "\n$";
			cin >> bet;
			evenNumber = evenCheck(bet);
		}
			
		cout << endl;
	}
	// Computer won
	else
	{
		// Call compStrategy to get the computers bet
		bet = compStrategy(playerBalance, compBalance, compStreak);
		
		// Display the bet to the player
		cout << "\t     Computer bets: $" << bet;
		cout << "\n";
	}

	return bet;
}
