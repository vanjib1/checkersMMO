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
void jump(std::vector<std::vector<checkerPiece>>& board);
std::vector<int> stringToPoint(std::string point);

int main()
{
	auto board = createBoard();
	setRed(board);
	setBlack(board);

	std::cout << "hell0";
    return 0;
}

std::vector<std::vector<checkerPiece>> createBoard() //creates the board (no duh) and sets all the valid pieces
{
	std::vector<std::vector<checkerPiece>> board(8);
	for (auto i = 0; i < 8; i++)
	{
		board[i].resize(8);
	} //creates a board of size 8*8 and sets everything to the default color=null

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

}

std::vector<int> stringToPoint(std::string point)
{
}


