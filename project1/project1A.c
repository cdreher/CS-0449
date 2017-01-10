//Collin Dreher
#include <stdio.h>

int main()
{
	/*declare variables*/
	int playerCard1, playerCard2, playerTotal, dealerCard1, dealerCard2, dealerTotal;
	int acePresent;
	
	srand((unsigned int)time(NULL));
	
	/*
		Create random integers for dealer's cards.
		Between 2 and 14.
		In order to make the 10s more commons, treat 11-13 as face cards, 14 as ace.
	*/
	dealerCard1 = rand() % 13 + 2;
	dealerCard2 = rand() % 13 + 2;
	
	/*If ace is pulled, set the playerNewCard of the card to 11 for now.*/
	if(dealerCard1 == 14)
	{
		dealerCard1 = 11;
	}
	/*If 11, 12, or 13 is pulled, that represents a face card.*/
	if(dealerCard1 == 11 || dealerCard1 == 12 || dealerCard1 == 13)
	{
		dealerCard1 = 10;
	}
	if(dealerCard2 == 14)
	{
		dealerCard2 = 11;
	}
	if(dealerCard2 == 11 || dealerCard2 == 12 || dealerCard2 == 13)
	{
		dealerCard2 = 10;
	}
	
	/*Print the dealer's cards and add up the total.*/
	printf("The dealer:\n? + %d\n\n", dealerCard2);
	dealerTotal = dealerCard1 + dealerCard2;
	
	/*
		Create random integers for player's cards. 
		Same rules apply as above.
	*/
	playerCard1 = rand() % 13 + 2;
	playerCard2 = rand() % 13 + 2;
	
	/*Same action is taken as above with player's cards (if 11-14 is pulled).*/
	if(playerCard1 == 14)
	{
		playerCard1 = 11;
	}
	if(playerCard1 == 11 || playerCard1 == 12 || playerCard1 == 13)
	{
		playerCard1 = 10;
	}
	if(playerCard2 == 14)
	{
		playerCard2 = 11;
	}
	if(playerCard2 == 11 || playerCard2 == 12 || playerCard2 == 13)
	{
		playerCard2 = 10;
	}
	
	/*Add player's cards together.*/
	playerTotal = playerCard1 + playerCard2;
	
	/*Print the player's cards and total.*/
	printf("You:\n%d + %d = %d\n\n", playerCard1, playerCard2, playerTotal);
	
	/*Prompt user to draw another card or not, and store that response.*/
	char *hitOrStand[5] = {};
	printf("Would you like to \"hit\" or \"stand\"? ");
	scanf("%s", &hitOrStand);
	
	int dealerNewCard, playerNewCard;
	while(strcmp(hitOrStand, "hit") == 0 || dealerTotal < 17)
	{
		/*Check is the dealer is still in need of another card.*/
		if(dealerTotal < 17)
		{
			/*Hit dealer with new card.*/
			dealerNewCard = rand() % 13 + 2;
			
			/*If ace is pulled, decide whether to make it 11 or 1.*/
			if(dealerNewCard == 14)
			{
				if(dealerTotal + 11 > 21)
				{
					dealerNewCard = 1;
					dealerTotal += dealerNewCard;
				}
				else
				{
					dealerNewCard = 11;
					dealerTotal += dealerNewCard;
				}
			}
			/*If face card is pulled, make it a ten like before.*/
			else if(dealerNewCard == 11 || dealerNewCard == 12 || dealerNewCard == 13)
			{
				dealerNewCard = 10;
				dealerTotal += dealerNewCard;
			}
			else
			{
				/*Add dealer's cards together.*/
				dealerTotal += dealerNewCard;
			}	
			
			/*If dealer busts, user wins.*/
			if(dealerTotal > 21)
			{
				printf("The dealer BUSTED:\n? + %d = %d\n\nYOU WIN! Congrats!\n", dealerNewCard, dealerTotal);
				break;
			}
			else
			{
				printf("The dealer:\n? + %d\n\n", dealerNewCard);
			}
		}	
		
		/*Check to see if user wants another card.*/
		if(strcmp(hitOrStand, "hit") == 0)
		{
			/*Deal user another card.*/
			playerNewCard = rand() % 13 + 2;
			
			/*If ace is pulled, decide whether to make it 11 or 1.*/
			if(playerNewCard == 14)
			{
				if(playerTotal + 11 > 21)
				{
					playerNewCard = 1;
					playerTotal += playerNewCard;
				}
				else
				{
					playerNewCard = 11;
					playerTotal += playerNewCard;
				}
			}
			/*If face card is pulled, make it a ten.*/
			else if(playerNewCard == 11 || playerNewCard == 12 || playerNewCard == 13)
			{
				playerNewCard = 10;
				playerTotal += playerNewCard;
			}
			else
			{
				/*Add player's cards together.*/
				playerTotal += playerNewCard;
			}
			
			if(playerTotal > 21)
			{
				printf("You:\n%d + %d = %d BUSTED!\n\nYou busted. Dealer wins.\n", playerTotal - playerNewCard, playerNewCard, playerTotal);
				break;
			}
			else
			{
				printf("You:\n%d + %d = %d\n", playerTotal - playerNewCard, playerNewCard, playerTotal);
			}
			
			/*Prompt user to hit or stand again.*/
			printf("Would you like to \"hit\" or \"stand\"? ");
			scanf("%s", &hitOrStand);
		}
	}
	
	/*If both the dealer and user do not want anymore cards, decide the winner.*/
	if(dealerTotal <= 21 && playerTotal <= 21)
	{
		if(dealerTotal > playerTotal)
			printf("The dealer total: %d\nYour total: %d\nThe dealer wins.\n", dealerTotal, playerTotal);
		else
			printf("The dealer total: %d\nYour total: %d\nYOU WIN! Congrats.\n", dealerTotal, playerTotal);
	}
	
	return 0;

}
