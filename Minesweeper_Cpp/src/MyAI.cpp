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
#include <set>
#include <utility>
#include <iostream>



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


    gameBoard = vector<vector<Tile>> (rowDimension, vector<Tile>(colDimension,Tile()));


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

    pair<int,int> myPair;
   
    //if the tile number is 0 add its neighbors to futureMoves as long as they are inbounds
    if (number==0)
    {
        vector<pair<int,int>> temp = getNeighborsCoordinates(agentX, agentY);

        for(int i = 0; i < temp.size(); i ++)
        {
            insertFutureMoves(temp[i], 1);
        }
    }

    //as long as futureMoves is not empty, pop the coordinates from queue and uncover that location
    if(!futureMoves.empty())
    {
        Action myAction = futureMoves.front();

        futureMoves.pop();

        agentX = myAction.location.first;
        agentY = myAction.location.second;



        return {actions[myAction.actionNumber], agentX, agentY};
    }

    //check for "1" tiles with one covered neighbor and flag as mine as well as insert mine location into mineLocations list
    for(int i=0; i<colDimension; i++)// X is col
    {
        int coverNei;
        coverNei =0;
        for(int j =0; j<rowDimension; j++)// Y is row
        {
            if (gameBoard[i][j].number==1)
            {
                if(  i- 1 >= 0 && j -1 >=0) //1
                {
                    if (!gameBoard[i-1][j-1].uncovered)
                    {
                        myPair = make_pair(i-1,j-1);
                        coverNei++;
                    }
                }
                if( i - 1 >= 0 && j + 1< rowDimension)
                {
                    if (!gameBoard[i-1][j+1].uncovered) //7
                    {
                        myPair = make_pair(i-1,j+1);
                        coverNei++;
                    }

                }
                if(i + 1 < colDimension && j - 1 >= 0)
                {
                    if (!gameBoard[i+1][j-1].uncovered)//3
                    {
                        myPair = make_pair(i+1,j-1);
                        coverNei++;
                    }

                }
                if( i + 1 < colDimension && j + 1< rowDimension)
                {
                    if (!gameBoard[i+1][j+1].uncovered) //9
                    {
                        myPair = make_pair(i+1,j+1);
                        coverNei++;
                    }

                }
                if(j - 1 >= 0)
                {
                    if (!gameBoard[i][j-1].uncovered)//2
                    {
                         myPair = make_pair(i,j-1);
                        
                        coverNei++;
                    }

                }
                if(j + 1 < rowDimension)
                {
                    if (!gameBoard[i][j+1].uncovered) //8
                    {
                         myPair = make_pair(i,j+1);
                        coverNei++;
                    }

                }
                if(i - 1 >= 0)
                {
                    if (!gameBoard[i-1][j].uncovered)//4
                    {
                        myPair = make_pair(i-1,j);
                        coverNei++;
                    }

                }
                if(i + 1 < colDimension)
                {
                    if (!gameBoard[i+1][j].uncovered)//6
                    {
                        myPair = make_pair(i+1,j);
                        coverNei++;
                    }
                }
            }
            
            if (coverNei ==1)
            {
                if(previousMoves.count(myPair)!=1)
                {
                    previousMoves.insert(myPair);
                    mineLocations.push_back(myPair);
                    gameBoard[agentX][agentY].flag = true;
                    
                    return {actions[2], myPair.first, myPair.second};

                    
                }

             
            }
            
                      
        }
        
    }
    //

    //scan for x-number tiles and check to see if x neighbors are flagged as a mine, if so, put all covered and unflagged neighbors into future moves
    for(int i = 0; i < colDimension; i++)
    {
        for(int j = 0; j < rowDimension; j++)
        {
            if(gameBoard[i][j].uncovered == true && gameBoard[i][j].number > 0)
                uncoverAllPossible(i, j);
        }
    }

    //if future moves is not empty, pop from it, change agent x and agent y, then return uncover action 
    if(!futureMoves.empty())
    {
        Action myAction = futureMoves.front();

        futureMoves.pop();

        agentX = myAction.location.first;
        agentY = myAction.location.second;



        return {actions[myAction.actionNumber], agentX, agentY};
    }
    //


    return {LEAVE,-1,-1};
    // ======================================================================
    // YOUR CODE ENDS
    // ======================================================================

}


// ======================================================================
// YOUR CODE BEGINS
// ======================================================================

//This function takes a pair and a queue ,
//returns a new queue if the pair doesn't exist in the set(local)
void MyAI::insertFutureMoves(pair<int,int> myPair, int actionNum)
{
    if(previousMoves.count(myPair) != 1)
    {
        previousMoves.insert(myPair);

        //create struct action push action instead of myPair
        Action action;

        action.location = myPair;
        action.actionNumber = actionNum;

        futureMoves.push(action);
    }

}

//this function checks to see if the number of flagged neighbors = the tiles number, if so add all covered neighbors to future moves
void MyAI::uncoverAllPossible(int x, int y)
{
    vector<pair<int,int>> temp = getNeighborsCoordinates(x,y);
    int flaggedCount = 0;

    for(int i = 0 ; i < temp.size(); i ++)
    {
        if(gameBoard[temp[i].first][temp[i].second].flag == true)
            flaggedCount ++;
    }

    if(flaggedCount == gameBoard[x][y].number)
    {
        for(int i = 0; i < temp.size(); i ++)
        {
            if(gameBoard[temp[i].first][temp[i].second].flag == false && gameBoard[temp[i].first][temp[i].second].uncovered == false)
                insertFutureMoves(temp[i], 1);
        }
    }
}

//this funciton returns a vector of pairs of coordinates of all in bounds neighbors of the given x,y
vector<pair<int,int>> MyAI::getNeighborsCoordinates(int x, int y)
{
    vector<pair<int,int>> neighborCoordinates;

    pair<int,int> myPair;

    if( x - 1 >= 0 && y -1 >=0)
    {
        myPair = make_pair(x-1,y-1); 
        neighborCoordinates.push_back(myPair);
    }   
    if( x - 1 >= 0 && y + 1< rowDimension)
    {
        myPair = make_pair(x-1,y+1); 
        neighborCoordinates.push_back(myPair);
    }      
    if( x + 1 < colDimension && y - 1 >= 0)
    {
        myPair = make_pair(x+1,y-1); 
        neighborCoordinates.push_back(myPair);
    }   
    if( x + 1 < colDimension && y + 1< rowDimension)
    {
        myPair = make_pair(x+1,y+1); 
        neighborCoordinates.push_back(myPair);
    }   
    if(y - 1 >= 0)
    {
        myPair = make_pair(x,y-1); 
        neighborCoordinates.push_back(myPair);
    } 
    if(y + 1 < rowDimension)
    {
        myPair = make_pair(x,y+1); 
        neighborCoordinates.push_back(myPair);
    } 
    if(x - 1 >= 0)
    {
        myPair = make_pair(x-1,y); 
        neighborCoordinates.push_back(myPair);
    } 
    if(x + 1 < colDimension)
    {
        myPair = make_pair(x+1,y); 
        neighborCoordinates.push_back(myPair);
    } 

    return neighborCoordinates;
}

// ======================================================================
// YOUR CODE ENDS
// ======================================================================
