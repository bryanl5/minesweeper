// ======================================================================
// FILE:        MyAI.hpp
//
// AUTHOR:      Jian Li
//
// DESCRIPTION: This file contains your agent class, which you will
//              implement. You are responsible for implementing the
//              'getAction' function and any helper methods you feel you
//              need.
//
// NOTES:       - If you are having trouble understanding how the shell
//                works, look at the other parts of the code, as well as
//                the documentation.
//
//              - You are only allowed to make changes to this portion of
//                the code. Any changes to other portions of the code will
//                be lost when the tournament runs your code.
// ======================================================================

#ifndef MINE_SWEEPER_CPP_SHELL_MYAI_HPP
#define MINE_SWEEPER_CPP_SHELL_MYAI_HPP

#include "Agent.hpp"
#include <iostream> // for debug
#include <iomanip>  //
#include <string>   //
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <queue>
#include <utility>

using namespace std;

class MyAI : public Agent
{
public:
    MyAI ( int _rowDimension, int _colDimension, int _totalMines, int _agentX, int _agentY );

    Action getAction ( int number ) override;


    // ======================================================================
    // YOUR CODE BEGINS
    // ======================================================================
private:   

	struct Tile
	{
  								// the tile has Bomb or not (deleted since we dont know where mines are)
        bool uncovered  = false; // the tile uncovered or not
        bool flag       = false; // the tile has been flagged as mine or not
        int  number     = -1;     // records number of bombs around
    };

    struct Action
    {
        pair<int,int> location;
        int actionNumber = -1;
    };

    const Action_type actions[4] =
    {
    	LEAVE,
    	UNCOVER, 
    	FLAG,
    	UNFLAG,
    };


    //member variables
    bool firstMove;

    vector<vector<Tile>> gameBoard;

    queue<Action> futureMoves;

    set<pair<int,int>> previousMoves;  


    //member functinos

    //this function checks to see if the number of flagged neighbors = the tiles number, 
    //if so add all covered neighbors to future moves
	void uncoverAllPossible(int x, int y);

    //This function takes a pair and int ,
    //adds an action to futureMoves if the pair of coordinates doesn't already exist in previousMoves
	void insertFutureMoves(pair<int,int> myPair, int actionNum);

    //this funciton returns a vector of pairs of coordinates of all in bounds neighbors of the given x,y
    vector<pair<int,int>> getNeighborsCoordinates(int x, int y);

    //returns the number of flagged neighbors for a given tile coordinate
    int flaggedNeighborsCount(int x, int y);

    //this function updates the container game board based on the percept from the current getAction call
    void updateGameBoard(int num);

    //helper functions that locate patterns and uncovers tiles based on patters
    void find11();
    void find12();




    //debug functions
    void  printMyWorldInfo(     );
    void  printMyBoardInfo();
    void printMyTileInfo( int c, int r );
    void printMyActionInfo();


    // ======================================================================
    // YOUR CODE ENDS
    // ======================================================================
};

#endif //MINE_SWEEPER_CPP_SHELL_MYAI_HPP
