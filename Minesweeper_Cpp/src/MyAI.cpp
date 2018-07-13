// ======================================================================
// FILE:        MyAI.cpp
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

#include "MyAI.hpp"
#include <vector>
#include <queue>
#include <pair>



MyAI::MyAI ( int _rowDimension, int _colDimension, int _totalMines, int _agentX, int _agentY ) : Agent()
{
    // ======================================================================
    // YOUR CODE BEGINS
    // ======================================================================
    rowDimension = _rowDimension;
    colDimension = _colDimension;
    totalMines = _totalMines;
    agentX = _agentX;
    agentY = _agentY;


    gameBoard = vector<vector<Tile>> v(rowDimension, vector<Tile>(colDimension,Tile()));


    // ======================================================================
    // YOUR CODE ENDS
    // ======================================================================
};

Agent::Action MyAI::getAction( int number )
{
    // ======================================================================
    // YOUR CODE BEGINS
    // ======================================================================
    gameBoard[agentX][agentY].number = number;
    gameBoard[agentX][agentY].uncovered = true;

    //Wei's code here
    pair<int,int> myPair;
   
    
    if (number==0)
    {
        if( agentX - 1 >= 0 && agentY -1 >=0)
        {
            myPair = make_pair(agentX-1,agentY-1); //1
            insertFutureMoves(myPair);
        }   
        if( agentX - 1 >= 0 && agentY + 1< rowDimension)
        {
            myPair = make_pair(agentX-1,agentY+1); //7
            insertFutureMoves(myPair);
        }      
        if( agentX + 1 < colDimension && agentY - 1 >= 0)
        {
            myPair = make_pair(agentX+1,agentY-1); //3
            insertFutureMoves(myPair);
        }   
        if( agentX + 1 < colDimension && agentY + 1< rowDimension)
        {
            myPair = make_pair(agentX+1,agentY+1); //9
            insertFutureMoves(myPair);
        }   
        if(agentY - 1 >= 0)
        {
            myPair = make_pair(agentX ,agentY-1); //2
            insertFutureMoves(myPair);
        } 
        if(agentY + 1 < rowDimension)
        {
            myPair = make_pair(agentX ,agentY+1); //8
            insertFutureMoves(myPair);
        } 
        if(agentX - 1 >= 0)
        {
            myPair = make_pair(agentX +1 ,agentY); //4
            insertFutureMoves(myPair);
        } 
        if(agentX + 1 < colDimension)
        {
            myPair = make_pair(agentX + 1,agentY); //6
            insertFutureMoves(myPair);
        } 
    }

    //




    if(!futureMoves.empty())
    {
        pair<int,int> myPair = myQueue.pop();

        agentX = myPair.first;
        agentY = myPair.second;

        return action(Action_type(UNCOVER), agentX, agentY);
    }



    return {LEAVE,-1,-1};
    // ======================================================================
    // YOUR CODE ENDS
    // ======================================================================

}


// ======================================================================
// YOUR CODE BEGINS
// ======================================================================


void MyAI::insertFutureMoves(pair myPair)
//This function takes a pair and a queue ,
//returns a new queue if the pair doesn't exist in the set(local)
{
    if(previousMoves.count(myPair) != 1)
    {
        previousMoves.insert(myPair)
        futureMoves.push(myPair);
    }

}


// ======================================================================
// YOUR CODE ENDS
// ======================================================================
