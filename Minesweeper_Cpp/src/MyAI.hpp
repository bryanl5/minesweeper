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
#include <iostream> // temporary use
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
        bool flag       = false; // the tile has been flag or not
        int  number     = -1;     // records number of bombs around
    };

    const Action_type actions[4] =
    {
    	LEAVE,
    	UNCOVER, 
    	FLAG,
    	UNFLAG,
    };

    vector<vector<Tile>> gameBoard;

    queue<pair<int,int>> futureMoves;

    set<pair<int,int>> previousMoves;  

	void uncoverAllPossible();

	void insertFutureMoves(pair<int,int> myPair);



    // ======================================================================
    // YOUR CODE ENDS
    // ======================================================================
};

#endif //MINE_SWEEPER_CPP_SHELL_MYAI_HPP
