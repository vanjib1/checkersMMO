
#include "stdafx.h"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <complex>
#include <WS2tcpip.h>
#include <minwinbase.h>
#pragma comment(lib, "ws2_32.lib")


class checkerPiece
{
public:
	checkerPiece();

	bool isRoyal;
	std::string color; //can be black, red, white, or null (white means it's a valid move but there is no valid checker in play, and null means it is not a valid move)

};
checkerPiece::checkerPiece()
{
	isRoyal = false;
	color = "null";
}

int redCount = 12; //if reaches 0 red lost
int blackCount = 12; // if reaches 0 black lost

std::string playingColor = "r";		

std::vector<std::vector<checkerPiece>> createBoard();
void setRed(std::vector<std::vector<checkerPiece>>& board);
void setBlack(std::vector<std::vector<checkerPiece>>& board);
std::vector<int> stringToPoint(std::string point);
bool isStringGood(std::string string);
void jump(std::vector<std::vector<checkerPiece>>& board, std::vector<int> move);
void move(std::vector<std::vector<checkerPiece>>& board, std::vector<int> move);
bool isValidMove(std::vector<int> move, std::vector<std::vector<checkerPiece>>& board);
bool canJump(std::vector<int> move, std::vector<std::vector<checkerPiece>>& board);
bool anyJumpable(std::vector<std::vector<checkerPiece>>& board);
void switchPlayingColor();
std::string whoIsTheWinner();
void kingMe(std::vector<int> move, std::vector<std::vector<checkerPiece>>& board);
void GameBoard(std::vector<std::vector<checkerPiece>>& board);
std::string printChar(std::vector<std::vector<checkerPiece>>& board, int x, int y);





void main()
{
	std::string ipAddress = "10.9.48.132"; //ip for server
	int port = 8888; //the listening port

	//initializing winsock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	//creating socket
	SOCKET sockPuppet = socket(AF_INET, SOCK_STREAM, 0);
	if (wsResult != 0)
	{
		std::cerr << "Server things broke";
		WSACleanup();
		return;
	}
	//hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	//connect to server
	int connResult = connect(sockPuppet, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		std::cerr << "cant connect to server, err#" << WSAGetLastError() << std::endl;
		closesocket(sockPuppet);
		WSACleanup();
		return;
	}
	char buf[4096];
	std::string userInput;
	do
	{
		std::cout << "Please enter your username: ";
		std::getline(std::cin, userInput);
		if (userInput.size() >0)
		{
			int sendResut = send(sockPuppet, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResut != SOCKET_ERROR)
			{
				ZeroMemory(buf, 4096);
				int bytesRecieved = recv(sockPuppet, buf, 4096, 0);
				if (bytesRecieved > 0)
				{
					std::cout << "SERVER> " << std::string(buf,0,bytesRecieved) << std::endl;
				}
			}
		}

	} while (userInput.size() > 0);
	closesocket(sockPuppet);
	WSACleanup();


	auto board = createBoard();
	//std::cout << "Please enter your username: ";
	std::string username;
	std::cin >> username;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	
	while ((redCount >0)| (blackCount > 0) )
	{
		GameBoard(board);
		std::string moveString;
		std::vector<int> movePoints;


		bool stringValid = false;
		bool moveValid = false;
		do
		{
			std::cout << "Your move:";
			std::cin >> moveString;
			stringValid = isStringGood(moveString);
			if (!stringValid)
			{
				std::cout << " \n Move not valid, please try again \n";
			}
			if (stringValid)
			{
				movePoints = stringToPoint(moveString);
				moveValid = isValidMove(movePoints, board);
				if (!moveValid)
				{
					std::cout << "Move not valid, please try again: \n";
				}
			}
		} while (!stringValid && !moveValid);

	move(board, movePoints);

	}
	
}

void GameBoard(std::vector<std::vector<checkerPiece>>& board)
{ /*board is in x,y format
  A-7
  B-6
  C-5
  D-4
  E-3
  F-2
  G-1
  H-0

  1-0
  2-1
  3-2
  4-3
  5-4
  6-5
  7-6
  8-7
  */

	std::cout << '\n'
		<< "     | " << printChar(board, 1, 7) << " |   | " << printChar(board, 3, 7) << " |   | " << printChar(board, 7, 5) << " |   | " << printChar(board, 7, 7) << "\n"
		<< "A ___|___|___|___|___|___|___|___\n   "
		<< printChar(board, 0, 6) << " |   | " << printChar(board, 2, 6) << " |   | " << printChar(board, 4, 6) << " |   | " << printChar(board, 6, 6) << " | \n"
		<< "B ___|___|___|___|___|___|___|___\n"
		<< "     | " << printChar(board, 1, 5) << " |   | " << printChar(board, 3, 5) << " |   | " << printChar(board, 5, 5) << " |   | " << printChar(board, 7, 5) << "\n"
		<< "C ___|___|___|___|___|___|___|___\n"
		<< "     | " << printChar(board, 0, 4) << " |   | " << printChar(board, 2, 4) << " |   | " << printChar(board, 4, 4) << " |   | " << printChar(board, 6, 4) << "\n"
		<< "D ___|___|___|___|___|___|___|___\n"
		<< "     | " << printChar(board, 1, 3) << " |   | " << printChar(board, 3, 3) << " |   | " << printChar(board, 5, 3) << " |   | " << printChar(board, 7, 3) << "\n"
		<< "E ___|___|___|___|___|___|___|___\n   "
		<< printChar(board, 0, 2) << " |   | " << printChar(board, 2, 2) << " |   | " << printChar(board, 4, 2) << " |   | " << printChar(board, 6, 2) << " | \n"
		<< "F ___|___|___|___|___|___|___|___\n"
		<< "     | " << printChar(board, 1, 1) << " |   | " << printChar(board, 3, 1) << " |   | " << printChar(board, 5, 1) << " |   | " << printChar(board, 7, 1) << "\n"
		<< "G ___|___|___|___|___|___|___|___\n   "
		<< printChar(board, 0, 0) << " |   | " << printChar(board, 2, 0) << " |   | " << printChar(board, 4, 0) << " |   | " << printChar(board, 6, 0) << " | \n"
		<< "H ___|___|___|___|___|___|___|___\n"
		<< "   1   2   3   4   5   6   7   8 \n"
		<< "\n";
}

std::string printChar(std::vector<std::vector<checkerPiece>>& board, int x, int y)
{
	if (board[x][y].color == "b")
	{
		if (board[x][y].isRoyal == true)
		{
			return "B";
		}
		return "b";
	}

	else if (board[x][y].color == "r")
	{
		if (board[x][y].isRoyal == true)
		{
			return "R";
		}
		return "r";
	}

	else return " ";
}

std::vector<std::vector<checkerPiece>> createBoard()
/* creates the board (no duh) and sets all the valid pieces
* sample output:
*    go to google, look up an empty 8x8 checker board, and imagine it as a vector.
*/
{
	std::vector<std::vector<checkerPiece>> board(8);
	for (auto i = 0; i < 8; i++)
	{
		board[i].resize(8);
	} //creates a board of size 8*8 and sets everything to the default color= "null"

	for (auto i = 0; i < 8; i++)
	{
		if (i % 2 == 0)
		{
			for (auto j = 0; j < 8; j++)
			{
				if (j % 2 == 0)
				{
					board[i][j].color = "w";
				}
			}
		}
		else
		{
			for (auto j = 0; j < 8; j++)
			{
				if (j % 2 == 1)
				{
					board[i][j].color = "w";
				}
			}
		}
		//sets all valid places to "w"
	}
	setRed(board);
	setBlack(board);
	return board;
}
void setRed(std::vector<std::vector<checkerPiece>>&  board)
{
	//puts the red checkers in their appropriate places for new game
	for (auto i = 0; i < 8; i++)
	{
		for (auto j = 0; j < 3; j++)
		{
			if (board[i][j].color == "w")
			{
				board[i][j].color = "r";
			}
		}

	}
}

void setBlack(std::vector<std::vector<checkerPiece>>& board)
{
	//puts the black checkers in their appropriate places for new game


	for (auto i = 0; i < 8; i++)
	{
		for (auto j = 7; j > 4; j--)
		{
			if (board[i][j].color == "w")
			{
				board[i][j].color = "b";
			}
		}

	}
}

void jump(std::vector<std::vector<checkerPiece>>& board, std::vector<int> move)
{
	//smh gang violence
	/* this function does no checking, just performs
	* sets the piece that was jumped to white (the default valid color) and then indicates that there was a jump by decrimenting *color* pieces by 1
	*/
	if (move[0] - 2 == move[2]) //checks the direction the piece is moving, first X then Y
	{
		if (move[1] - 2 == move[3])
		{
			board[move[0] - 1][move[1] - 1].color = "w"; //sets the piece that was jumped to white

		}
		if (move[1] + 2 == move[3])
		{
			board[move[0] - 1][move[1] + 1].color = "w";
		}
	}

	if (move[0] + 2 == move[2])
	{

		if (move[1] - 2 == move[3])
		{
			board[move[0] + 1][move[1] - 1].color = "w";
		}
		if (move[1] + 2 == move[3])
		{
			board[move[0] + 1][move[1] + 1].color = "w";
		}
	}

	board[move[2]][move[3]] = board[move[0]][move[1]]; //moves the piece
	board[move[0]][move[1]].color = "w";

	if (playingColor == "r")
	{
		blackCount--;
	}
	if (playingColor == "b")
	{
		redCount--;
	}
	switchPlayingColor();
}

void move(std::vector<std::vector<checkerPiece>>& board, std::vector<int> move)
{
	//if you're running this function you should have already checked isValid move
	//this does no checking, just moves pieces
	//TODO: check this function
	if (anyJumpable(board))
	{
		if (canJump(move, board))
		{
			jump(board, move);
			kingMe(move,board);
		}
	}
	else
	{
		 board[move[2]][move[3]] = board[move[0]][move[1]];
		kingMe(move, board);
		switchPlayingColor();
	}
	board[move[0]][move[1]].color = "w";

}

bool isValidMove(std::vector<int> move, std::vector<std::vector<checkerPiece>>& board)
{


	/*takes move already processed by stringToPoint and indicates if its a valid move
	*
	*invalid moves:
	*    if it lands on a null color ✓
	*    if there is a piece in that place ✓
	*    if you are in a position where you have to jump ✓
	*    if the move is within bounds ✓ (that was done in the string checking)
	*    if it moves backwards (unless you're a king)✓
	*        need to check colors for this
	*
	*
	*
	*/
	if (board[move[2]][move[3]].color != "w")
	{
		return false;
	}


	if (board[move[0]][move[2]].color == "r")
	{
		if (board[move[0]][move[2]].isRoyal)
		{
			if (!canJump(move, board))
			{
				if (std::abs(move[0] - move[2]) != 1)
				{
					return false;
				}
				if (std::abs(move[1] - move[3]) != 1)
				{
					return false;
				}
			}
			else
			{
				if (std::abs(move[0] - move[2]) != 2)
				{
					return false;
				}
				if (std::abs(move[1] - move[3]) != 2)
				{
					return false;
				}
			}
		}
		else
		{
			if (!canJump(move, board))
			{
				if (move[0] - move[2] != 1)
				{
					return false;
				}
				if (move[1] - move[3] != 1)
				{
					return false;
				}
			}
			else
			{
				if (move[0] - move[2] != 2)
				{
					return false;
				}
				if (move[1] - move[3] != 2)
				{
					return false;
				}
			}
		}
	}
	if (board[move[0]][move[2]].color == "b")
	{
		if (board[move[0]][move[2]].isRoyal)
		{
			if (!canJump(move, board))
			{
				if (std::abs(move[0] - move[2]) != 1)
				{
					return false;
				}
				if (std::abs(move[1] - move[3]) != 1)
				{
					return false;
				}
			}
			else
			{
				if (std::abs(move[0] - move[2]) != 2)
				{
					return false;
				}
				if (std::abs(move[1] - move[3]) != 2)
				{
					return false;
				}
			}
		}
		else
		{
			if (!canJump(move, board))
			{
				if (move[0] - move[2] != -1)
				{
					return false;
				}
				if (move[1] - move[3] != -1)
				{
					return false;
				}
			}
			else
			{
				if (move[0] - move[2] != -2)
				{
					return false;
				}
				if (move[1] - move[3] != -2)
				{
					return false;
				}
			}
		}
	}


	return true;
}

bool canJump(std::vector<int> move, std::vector<std::vector<checkerPiece>>& board)
{


	//checks to see if selected piece can make a jump
	//if its not a king it only checks the direction it can move in ordinarily, if it is a king it checks all directions
	//does not check to see if move is valid (if there is a piece in the way) use isValidMove() for that


	if (board[move[0]][move[1]].color == "r")
	{
		if (!board[move[0]][move[1]].isRoyal)
		{
			if (move[0] != 0)
			{
				if (move[1] != 7)
				{
					if (board[move[0] - 1][move[1] + 1].color == "b")
					{
						if (move[0] != 1)
						{
							if (move[1] != 6)
							{
								if (board[move[0] - 2][move[1] + 2].color == "w")
								{
									return true;
								}
							}
						}
					}
				}
			}
			if (move[0] != 7)
			{
				if (board[move[0] + 1][move[1] + 1].color == "b")
				{
					if (move[0] != 6 & move[1] != 6)
					{

						if (board[move[0] + 2][move[1] + 2].color == "w")
						{
							return true;
						}
					}
				}
			}

		}
		else
		{
			if (move[0] != 0)
			{
				if (move[0] != 7)
				{

					if (board[move[0] - 1][move[1] + 1].color == "b")
					{
						if (move[0] != 1 & move[1] != 6)
						{

							if (board[move[0] - 2][move[1] + 2].color == "w")
							{
								return true;
							}
						}
					}
				}
			}
			if (move[0] != 7 & move[1] != 7)
			{

				if (board[move[0] + 1][move[1] + 1].color == "b")
				{
					if (move[0] != 6 & move[1] != 6)
					{


						if (board[move[0] + 2][move[1] + 2].color == "w")
						{
							return true;
						}
					}
				}
			}
			if (move[1] != 0 & move[0] != 0)
			{

				if (board[move[0] - 1][move[1] - 1].color == "b")
				{
					if (move[0] != 1 & move[1] != 1)
					{
						if (board[move[0] - 2][move[1] - 2].color == "w")
						{
							return true;
						}
					}
				}
			}
			if (move[0] != 0)
			{

				if (board[move[0] + 1][move[1] - 1].color == "b")
				{
					if (move[1] != 1)
					{
						if (board[move[0] + 2][move[1] - 2].color == "w")
						{
							return true;
						}
					}
				}
			}
		}
	}
	if (board[move[0]][move[1]].color == "b")
	{
		if (!board[move[0]][move[1]].isRoyal)
		{
			if (move[0] != 0 & move[1] != 0)
			{

				if (board[move[0] - 1][move[1] - 1].color == "r")
				{
					if (move[1] != 1 & move[1] != 1)
					{
						if (board[move[0] - 2][move[1] - 2].color == "w")
						{
							return true;
						}
					}
				}
			}
			if (move[0] != 7 & move[1] != 0)
			{

				if (board[move[0] + 1][move[1] - 1].color == "r")
				{
					if (move[1] != 1 & move[0] != 6)
					{

						if (board[move[0] + 2][move[1] - 2].color == "w")
						{
							return true;
						}
					}
				}
			}

		}

	}
	//else
	//{
	//    if (board[move[0] - 1][move[1] + 1].color == "r")
	//    {
	//        if (board[move[0] - 2][move[1] + 2].color == "w")
	//        {
	//            return true;
	//        }
	//    }
	//    if (board[move[0] + 1][move[1] + 1].color == "r")
	//    {
	//        if (board[move[0] + 2][move[1] + 2].color == "w")
	//        {
	//            return true;
	//        }
	//    }
	//    if (board[move[0] - 1][move[1] - 1].color == "r")
	//    {
	//        if (board[move[0] - 2][move[1] - 2].color == "w")
	//        {
	//            return true;
	//        }
	//    }
	//    if (board[move[0] + 1][move[1] - 1].color == "r")
	//    {
	//        if (board[move[0] + 2][move[1] - 2].color == "w")
	//        {
	//            return true;
	//        }
	//    }
	//}
	return false;
}

bool anyJumpable(std::vector<std::vector<checkerPiece>>& board)
{
	//this function checks if there is any point where the current player can make a move

	for (auto x = 0; x < board.size(); x++)
	{
		for (auto y = 0; y < board[x].size(); y++)
		{
			if ((canJump({ x,y }, board)) & (board[x][y].color == playingColor))
			{
				return true;
			}

		}
	}
	return false;
}

void switchPlayingColor()
{
	//pretty self explanitory
	if (playingColor == "r")
	{
		playingColor = "b";
	}
	else
	{
		playingColor = "r";
	}
}

std::string whoIsTheWinner()
{
	//returns red if red won, black if black won, and an empty string if there is no winner yet.
	if (redCount == 0)
	{
		return "r";
	}
	if (blackCount == 0)
	{
		return "b";
	}
	return "";
}

std::vector<int> stringToPoint(std::string points)
{
	/*
	* Tested ✓
	assumes that input is valid and in correct format
	doesnt check if it's a valid move but does check if the points are within bounds
	sample input:
	"G6-F5"
	Sample output
	{5,1,4,2}
	(x1, y1, x2, y2) format
	*/
	points.erase(std::remove_if(points.begin(), points.end(), isspace), points.end());
	std::transform(points.begin(), points.end(), points.begin(), ::toupper);
	//now the string should be spacesless and all letters should be caps

	std::vector<int> vecInput(5);
	for (auto i = 0; i < points.length(); i++)
	{
		if ((static_cast<int> (points[i]) > 64) & (static_cast<int> (points[i]) < 73))
		{
			switch (points[i])
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
		else if ((static_cast<int> (points[i]) > 47) & (static_cast<int> (points[i]) < 57))
		{
			vecInput[i] = static_cast<int> (points[i]) - 48;
			vecInput[i] -= 1;

		}
	}
	vecInput.erase(vecInput.begin() + 2);
	std::iter_swap(vecInput.begin(), vecInput.begin() + 1);
	std::iter_swap(vecInput.begin() + 2, vecInput.begin() + 3);

	return vecInput;


}

bool isStringGood(std::string string)
{    // Tested ✓
	 /* this function simply checks to see if the string is correct, not if the move is valid
	 * string format should be -> letter number - letter number (5 characters in total)
	 * we remove spaces just in case
	 *
	 * sample output:
	 *         ...its a bool, what are you expecting exactly?
	 */

	string.erase(std::remove_if(string.begin(), string.end(), isspace), string.end());
	std::transform(string.begin(), string.end(), string.begin(), ::toupper);

	if (string.length() != 5)
	{
		return false;
	}
	for (auto i = 0; i < 5; i++)
	{
		if ((i == 0) | (i == 3))
		{
			if (!((static_cast<int> (string[i]) > 64) & (static_cast<int> (string[i]) < 73)))
			{
				return false;
			}
		}
		else if ((i == 1) | (i == 4))
		{
			if (!((static_cast<int> (string[i]) > 47) & (static_cast<int> (string[i]) < 57)))
			{
				return false;
			}
		}
		else
		{
			if (string[i] != '-')
			{
				return false;
			}
		}
	}
	return true;
}

void kingMe(std::vector<int> move, std::vector<std::vector<checkerPiece>>& board)
{
	//king the black at the top
	if ((playingColor == "b") & (move[3] == 0))
	{
		board[2][3].isRoyal = true;
	}
	if ((playingColor == "r") & (move[3] == 7))
	{
		board[2][3].isRoyal = true;
	}
}