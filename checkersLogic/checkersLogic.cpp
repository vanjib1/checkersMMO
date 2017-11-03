// checkersLogic.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "checkerPiece.h"
#include <vector>
#include <iostream>
#include <string>

int redCount = 0;
int blackCount = 0;

std::vector<std::vector<checkerPiece>> createBoard();
void setRed(std::vector<std::vector<checkerPiece>>& board);
void setBlack(std::vector<std::vector<checkerPiece>>& board);
std::vector<int> stringToPoint(std::string point);
std::string stringChecking(std::string string);
void jump(std::vector<std::vector<checkerPiece>>& board);
void move(std::vector<std::vector<checkerPiece>>& board);




int main()
{
	auto board = createBoard();
	setRed(board);
	setBlack(board);

	//testing
	stringToPoint("A1-B2");

    return 0;
}

std::vector<std::vector<checkerPiece>> createBoard() //creates the board (no duh) and sets all the valid pieces
{
	std::vector<std::vector<checkerPiece>> board(8);
	for (auto i = 0; i < 8; i++)
	{
		board[i].resize(8);
	} //creates a board of size 8*8 and sets everything to the default color= "null"

	for(auto i =0; i < 8; i++)
	{
		if(i % 2 == 0)
		{
			for(auto j = 0; j < 8; j++)
			{
				if( j % 2 == 0)
				{
					board[i][j].color = "white";
				}
			}
		}
		else
		{
			for (auto j = 0; j < 8; j++)
			{
				if (j % 2 == 1)
				{
					board[i][j].color = "white";
				}
			}
		}
		//sets all valid places to "white"
	}

	return board;
}
void setRed(std::vector<std::vector<checkerPiece>>&  board)
{
	for(auto i = 0; i < 8; i++ )
	{
		for(auto j = 0; j < 3; j++)
		{
			if (board[i][j].color == "white")
			{
				board[i][j].color = "red";
			}
		}

	}
}

void setBlack(std::vector<std::vector<checkerPiece>>& board)
{
	for (auto i = 0; i < 8; i++)
	{
		for (auto j = 7; j > 4; j--)
		{
			if (board[i][j].color == "white")
			{
				board[i][j].color = "black";
			}
		}

	}
}

void jump(std::vector<std::vector<checkerPiece>>& board)
{
	//TODO: this function
}

void move(std::vector<std::vector<checkerPiece>>& board)
{
	//TODO: ...this
}

std::vector<int> stringToPoint(std::string points)
{
	/*
	assumes that input is valid and in correct format 
	doesnt check if it's a valid move
	sample input:
		"G6-F5"
	Sample output
		{1,6,2,5}
	*/
	std::vector<int> vecInput(5);
	for( auto i = 0; i < points.length(); i++)
	{
		if(static_cast<int> (points[i]) > 64 & static_cast<int> (points[i]) < 91 )
		{
			switch(points[i])
			{
			case('A'): vecInput[i] = 7;
					   break;
			case('B'): vecInput[i] = 6;
				       break;
			case('C'): vecInput[i] = 5;
				       break;
			case('D'): vecInput[i] = 4;
					   break;
			case('E'): vecInput[i] = 3;
					   break;
			case('F'): vecInput[i] = 2;
				       break;
			case('G'): vecInput[i] = 1;
					   break;
			case('H'): vecInput[i] = 0;
			}
		}
		else if(static_cast<int> (points[i]) > 47 & static_cast<int> (points[i]) < 57)
		{
			vecInput[i] = static_cast<int> (points[i]) - 48;
		}
	}
	vecInput.erase(vecInput.begin()+3);
	
	return vecInput;

	
}

std::string stringChecking(std::string string)
{
	//TODO: stuff
}


