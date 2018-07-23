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

    firstMove = true;

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


    //temporary debug function DELETE BEFORE SUBMITION!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    

                                                                                    
    updateGameBoard(number);
                                                                                
                                                                                            cout << agentX + 1 << agentY + 1 << number <<endl;
    printMyWorldInfo(); 

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
    
                                                                                            cout<< "getAction  3"<<endl;
    for(int i=0; i<colDimension; i++)// X is col
    {
                                                                                            cout<< "getAction  4"<<endl;
        for(int j =0; j<rowDimension; j++)// Y is row
        {
                                                                                            cout<< "getAction  5"<<endl;

            int coverNei =0;
            vector<pair<int,int>> tempLocations;
            vector<pair<int,int>> temp;
                                                                                            cout<< i << j <<endl;
            if (gameBoard[i][j].number > 0 )
            {
                                                                                            cout<< "getAction  6"<<endl;

                temp = getNeighborsCoordinates(i, j);
                
                for(int k = 0; k < temp.size(); k ++)
                {
                                                                                            cout<< "getAction  7"<<endl;

                    if (!gameBoard[temp[k].first][temp[k].second].uncovered )//thinking that all neighbor tiles are covered
                    {
                                                                                            cout<< "getAction  8"<<endl;

                        myPair = make_pair(temp[k].first,temp[k].second);
                        tempLocations.push_back(myPair);
                        coverNei++;
                        
                    }
                    
                    
                }
                
            }
            
            
            if (coverNei == gameBoard[i][j].number)
            {
                                                                                            cout<< "getAction  9"<<endl;
                                                                                            cout<< tempLocations.size()<< endl;
                
                for (int m =0; m<tempLocations.size();m++)//not entering this loop maybe temploactions.size = 0
                {
                                                                                            cout<< "getAction  10"<<endl;

                    myPair = tempLocations[m];
                    
                    if(previousMoves.count(myPair)!=1)
                    {
                                                                                            cout<< "getAction  11"<<endl;

                                                                                            cout<< "the location of mine is:"<<myPair.first+1<<myPair.second+1<<endl;
                        
                        insertFutureMoves(myPair,2);
                        
                    }
                }
                
                
            }
            
        }
        
    }
    //
                                                                                                cout<< "getAction  12"<<endl;

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
                                                                                                cout<< "getAction  13"<<endl;

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

//This function takes a pair and int ,
//adds an action to futureMoves if the pair of coordinates doesn't already exist in previousMoves
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

//this function checks to see if the number of flagged neighbors = the tiles number, 
//if so add all covered neighbors to future moves
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

void MyAI::updateGameBoard(int num)
{

    if(firstMove == true)
    {
        gameBoard[agentX][agentY].uncovered = true;

        gameBoard[agentX][agentY].number = num;

        firstMove = false;
    }

    if(num > -1)
    {
        gameBoard[agentX][agentY].number = num;
        gameBoard[agentX][agentY].uncovered == true;
    }
    if(num == -1)
    {
        gameBoard[agentX][agentY].number = num;
        gameBoard[agentX][agentY].flag == true;
    }

}

//debug functions
void MyAI::printMyWorldInfo(     )
{
    printMyBoardInfo();
    printMyActionInfo();
}
void MyAI::printMyBoardInfo(     )
{
    cout << "=================== Container Game Board ------------------\n" << endl;
    for ( int r = rowDimension-1; r >= 0; --r )
    {
        printf("%-4d%c",r+1,'|');
        for ( int c = 0; c < colDimension; ++c )
            printMyTileInfo ( c, r );
        cout << endl << endl;
    }

    cout << "     ";
    for (int c = 0; c < colDimension; ++c)
        cout << setw(8) << "-" ;
    cout << endl << "     ";
    for (int c = 0; c < colDimension; ++c)
        cout << setw(8) << c + 1;
    cout << endl;
}
void MyAI::printMyTileInfo( int c, int r )
{

    string tileString;

    if ( gameBoard[c][r].uncovered )
        tileString.append(to_string(gameBoard[c][r].number));
    else if ( gameBoard[c][r].flag )
        tileString.append("#");
    else
        tileString.append(".");

    cout << setw(8) << tileString;
}
void MyAI::printMyActionInfo()
{
    switch ( futureMoves.front().actionNumber )
    {
        case 1:
            cout << "Next Action: Uncover";
            break;
        case 2:
            cout << "Next Action: Flag";
            break;
        case 3:
            cout << "Next Action: Unflag";
            break;
        case 0:
            cout << "Next Action: Leave" << endl;
            break;

        default:
            cout << "next action: Invalid" << endl;
    }

    if (futureMoves.front().actionNumber != 0)
        cout << " on tile " << futureMoves.front().location.first + 1 << " " << futureMoves.front().location.second + 1 << endl;
}

// ======================================================================
// YOUR CODE ENDS
// ======================================================================
