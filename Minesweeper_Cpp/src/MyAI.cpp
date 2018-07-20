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

    //Wei's code here
    pair<int,int> myPair;
   
    //if the tile number is 0 add its neighbors to futureMoves as long as they are inbounds
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
            myPair = make_pair(agentX -1 ,agentY); //4
            insertFutureMoves(myPair);
        } 
        if(agentX + 1 < colDimension)
        {
            myPair = make_pair(agentX + 1,agentY); //6
            insertFutureMoves(myPair);
        } 
    }



    //as long as futureMoves is not empty, pop the coordinates from queue and uncover that location
    if(!futureMoves.empty())
    {
        pair<int,int> myPair = futureMoves.front();

        futureMoves.pop();

        agentX = myPair.first;
        agentY = myPair.second;



        return {actions[1], agentX, agentY};
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

    //temp minAI feature, if mine, then uncover all covered tiles around mine
    for(int i = 0; i < colDimension; i++)
    {
        for(int j = 0; j < rowDimension; j++)
        {
            if(gameBoard[i][j].flag == false && gameBoard[i][j].uncovered == false)
            {
                myPair = make_pair(i, j);
                insertFutureMoves(myPair);
            }
        }
    }
    if(!futureMoves.empty())
    {
        pair<int,int> myPair = futureMoves.front();

        futureMoves.pop();

        agentX = myPair.first;
        agentY = myPair.second;



        return {actions[1], agentX, agentY};
    }
    //

    //scan for x-number tiles and check to see if x neighbors are flagged as a mine, if so, uncover all possible other tiles
    for(int i = 0; i < colDimension; i++)
    {
        for(int j = 0; j < rowDimension; j++)
        {
            if(gameBoard[i][j].uncovered == true && gameBoard[i][j].number > 0)
                uncoverAllPossible(i, j);
        }
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
void MyAI::insertFutureMoves(pair<int,int> myPair)
{
    if(previousMoves.count(myPair) != 1)
    {
        previousMoves.insert(myPair);
        futureMoves.push(myPair);
    }

}

//this function checks to see if the number of flagged neighbors = the tiles number, if so add all covered neighbors to future moves
void MyAI::uncoverAllPossible(int x, int y)
{

}

vector<pair<int,int>> MyAI::getNeighborsCoordinates(int x, int y)
{
    vector<pair<int,int>> neighborCoordinates;

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
