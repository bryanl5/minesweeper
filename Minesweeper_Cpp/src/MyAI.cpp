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

    gameBoard = vector<vector<Tile>> ( colDimension, vector<Tile>(rowDimension,Tile()));


    // ======================================================================
    // YOUR CODE ENDS
    // ======================================================================
};

Agent::Action MyAI::getAction( int number )
{
    // ======================================================================
    // YOUR CODE BEGINS
    // ======================================================================

                                                                            
    updateGameBoard(number);
                                                                                
 

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

    //check for "1" tiles with one covered neighbor and flag as mine 
    
    for(int i=0; i<colDimension; i++)// X is col
    {
        for(int j =0; j<rowDimension; j++)// Y is row
        {

            int coverNei =0;
            vector<pair<int,int>> tempLocations;
            vector<pair<int,int>> temp;
            if (gameBoard[i][j].number > 0 )
            {

                temp = getNeighborsCoordinates(i, j);
                
                for(int k = 0; k < temp.size(); k ++)
                {

                    if (!gameBoard[temp[k].first][temp[k].second].uncovered )//thinking that all neighbor tiles are covered
                    {

                        myPair = make_pair(temp[k].first,temp[k].second);
                        tempLocations.push_back(myPair);
                        coverNei++;
                        
                    }
                    
                    
                }
                
            }
            
            
            if (coverNei == gameBoard[i][j].number)
            {
                
                for (int m =0; m<tempLocations.size();m++)//not entering this loop maybe temploactions.size = 0
                {

                    myPair = tempLocations[m];
                    
                    if(previousMoves.count(myPair)!=1)
                    {
                        
                        insertFutureMoves(myPair,2);
                        
                    }
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
    
    find11();
    find12();

    //if future moves is not empty, pop from it, change agent x and agent y, then return uncover action 
    if(!futureMoves.empty())
    {
        Action myAction = futureMoves.front();
        futureMoves.pop();
        agentX = myAction.location.first;
        agentY = myAction.location.second;
        return {actions[myAction.actionNumber], agentX, agentY};
    }

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

    return;
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

    return;
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

int MyAI::flaggedNeighborsCount(int x, int y)
{
    int count = 0;
    vector<pair<int,int>> temp;

    temp = getNeighborsCoordinates(x, y);
    for(int i  = 0 ; i < temp.size(); i++)
    {
        if(gameBoard[temp[i].first][temp[i].second].flag)
            count ++;
    }

    return count;
}

//this function updates the container game board based on the percept from the current getAction call
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
        gameBoard[agentX][agentY].uncovered = true;

    }
    if(num == -1)
    {
        gameBoard[agentX][agentY].number = num;
        gameBoard[agentX][agentY].flag = true;
    }

    return;

}

//helper functions that locate patterns and returns a vector of locations for actions if the coordinates are inbounds.
void MyAI::find11()
{   
    vector<pair<int,int>> temp;
    pair<int,int> tempPair;

    for(int i = 0 ; i < colDimension; i ++)
    {
        for(int j = 0; j < rowDimension; j++)
        {

            //vertical
            if( j + 1 < rowDimension && //if one up from y is in bounds
                (gameBoard[i][j].number - flaggedNeighborsCount(i, j)) == 1 && 
                (gameBoard[i][j + 1].number - flaggedNeighborsCount(i, j + 1)) == 1)//if two "1"s in a row upward ("1" after subtracting flagged tiles)
            {
                if(i - 1 >= 0 && gameBoard[i - 1][j].uncovered == false && gameBoard[i-1][j+1].uncovered == false)// if the two tiles on left are covered
                {
                    if( j - 1 < 0 || gameBoard[i - 1][j-1].uncovered == true)// if bottom left tile is out of bounds or is uncovered
                    {
                        if(j + 2 < rowDimension && gameBoard[i-1][j+2].uncovered == false)// if top left tile is covered
                        {
                            tempPair = make_pair(i-1, j+2);
                            insertFutureMoves(tempPair, 1); //uncover top left
                        }
                    }
                    if(j + 2 >= rowDimension || gameBoard[i-1][j+2].uncovered == true)// if top left tile is uncovered or out of bounds
                    {
                        if(j-1 >= 0 && gameBoard[i-1][j-1].uncovered == false)//if bottom left tile is covered
                        {
                            tempPair = make_pair(i-1, j-1);
                            insertFutureMoves(tempPair, 1);//uncover bottom left
                        }
                    }
                }
                if(i + 1 < colDimension && gameBoard[i + 1][j].uncovered == false && gameBoard[i+1][j+1].uncovered == false)// if the two tiles on right are covered
                {
                    if( j - 1 < 0 || gameBoard[i + 1][j-1].uncovered == true)// if bottom right tile is out of bounds or is uncovered
                    {
                        if(j + 2 < rowDimension && gameBoard[i+1][j+2].uncovered == false)// if top right tile is covered
                        {
                            tempPair = make_pair(i+1, j+2);
                            insertFutureMoves(tempPair, 1); //uncover top right
                        }
                    }
                    if(j + 2 >= rowDimension || gameBoard[i+1][j+2].uncovered == true)// if top right tile is uncovered or out of bounds
                    {
                        if(j-1 >= 0 && gameBoard[i+1][j-1].uncovered == false)//if bottom right tile is covered
                        {
                            tempPair = make_pair(i+1, j-1);
                            insertFutureMoves(tempPair, 1);//uncover bottom right
                        }
                    }
                }
            }

            //horizontal
            if( i + 1 < colDimension && //if one right from x is in bounds
                (gameBoard[i][j].number - flaggedNeighborsCount(i, j)) == 1 && 
                (gameBoard[i + 1][j].number - flaggedNeighborsCount(i + 1, j)) == 1)//if two "1"s in a row sideways ("1" after subtracting flagged tiles)
            {
                if(j - 1 >= 0 && gameBoard[i][j - 1].uncovered == false && gameBoard[i+1][j-1].uncovered == false)// if the two tiles on bottom are covered
                {
                    if( i - 1 < 0 || gameBoard[i - 1][j-1].uncovered == true)// if bottom left tile is out of bounds or is uncovered
                    {
                        if(i + 2 < colDimension && gameBoard[i+2][j-1].uncovered == false)// if bottom right tile is covered
                        {
                            tempPair = make_pair(i+2, j-1);
                            insertFutureMoves(tempPair, 1); //uncover bottom right
                        }
                    }
                    if(i + 2 >= colDimension || gameBoard[i+2][j-1].uncovered == true)// if bottom right tile is uncovered or out of bounds
                    {
                        if(i-1 >= 0 && gameBoard[i-1][j-1].uncovered == false)//if bottom left tile is covered
                        {
                            tempPair = make_pair(i-1, j-1);
                            insertFutureMoves(tempPair, 1);//uncover bottom left
                        }
                    }
                }
                if(j + 1 < rowDimension && gameBoard[i][j + 1].uncovered == false && gameBoard[i+1][j+1].uncovered == false)// if the two tiles on top are covered
                {
                    if( i - 1 < 0 || gameBoard[i - 1][j+1].uncovered == true)// if top left tile is out of bounds or is uncovered
                    {
                        if(i + 2 < colDimension && gameBoard[i+2][j+1].uncovered == false)// if top right tile is covered
                        {
                            tempPair = make_pair(i+2, j+1);
                            insertFutureMoves(tempPair, 1); //uncover top right
                        }
                    }
                    if(i + 2 >= colDimension || gameBoard[i+2][j+1].uncovered == true)// if top right tile is uncovered or out of bounds
                    {
                        if(i-1 >= 0 && gameBoard[i-1][j+1].uncovered == false)//if top left tile is covered
                        {
                            tempPair = make_pair(i-1, j+1);
                            insertFutureMoves(tempPair, 1);//uncover top left
                        }
                    }
                }
            }
        }
    }

    return;
}

//
void MyAI::find12()
{   
    vector<pair<int,int>> temp;
    pair<int,int> tempPair;

    for(int i = 0 ; i < colDimension; i ++)
    {
        for(int j = 0; j < rowDimension; j++)
        {

            //vertical
            if( j + 2 < rowDimension && //if one up from y is in bounds
                (gameBoard[i][j].number - flaggedNeighborsCount(i, j)) == 1 && 
                (gameBoard[i][j + 1].number - flaggedNeighborsCount(i, j + 1)) == 2 &&
                gameBoard[i][j + 2].uncovered)//if 12 pattern upward ("1" after subtracting flagged tiles)
            {
                if(i - 1 >= 0 && gameBoard[i - 1][j].uncovered == false && gameBoard[i-1][j+1].uncovered == false)// if the two tiles on left are covered
                {
                    if(gameBoard[i-1][j+2].flag == false)// if top left tile is unflagged
                    {
                        tempPair = make_pair(i-1, j+2);
                        insertFutureMoves(tempPair, 2); //flag top left
                    }
                }
                if(i + 1 < colDimension && gameBoard[i + 1][j].uncovered == false && gameBoard[i+1][j+1].uncovered == false)// if the two tiles on right are covered
                {
                    if(gameBoard[i+1][j+2].flag == false)// if top right tile is unflagged
                    {
                        tempPair = make_pair(i+1, j+2);
                        insertFutureMoves(tempPair, 2); //flag top right
                    }
                }
            }
            if( j + 2 < rowDimension && //if one up from y is in bounds
                gameBoard[i][j].uncovered && 
                (gameBoard[i][j + 1].number - flaggedNeighborsCount(i, j + 1)) == 2 &&
                (gameBoard[i][j + 2].number - flaggedNeighborsCount(i, j + 2)) == 1)//if 12 pattern downward ("1" after subtracting flagged tiles)
            {

                if(i - 1 >= 0 && gameBoard[i - 1][j + 1].uncovered == false && gameBoard[i-1][j+2].uncovered == false)// if the two tiles on left are covered
                {
                    if(gameBoard[i-1][j].flag == false)// if bottom left tile is unflagged
                    {
                        tempPair = make_pair(i-1, j);
                        insertFutureMoves(tempPair, 2); //flag bottom left
                    }
                }
                if(i + 1 < colDimension && gameBoard[i + 1][j + 1].uncovered == false && gameBoard[i+1][j+2].uncovered == false)// if the two tiles on right are covered
                {
                    if(gameBoard[i+1][j].flag == false)// if bottom right tile is unflagged
                    {
                        tempPair = make_pair(i+1, j);
                        insertFutureMoves(tempPair, 2); //flag bottom right
                    }
                }
            }

            //horizontal
            if( i + 2 < colDimension && //if two right from x is in bounds
                (gameBoard[i][j].number - flaggedNeighborsCount(i, j)) == 1 && 
                (gameBoard[i + 1][j].number - flaggedNeighborsCount(i + 1, j)) == 2 &&
                gameBoard[i + 2][j].uncovered)//if 12 patter to the right ("1" after subtracting flagged tiles)
            {
                if(j - 1 >= 0 && gameBoard[i][j - 1].uncovered == false && gameBoard[i+1][j-1].uncovered == false)// if the two tiles on bottom are covered
                {
                    if(gameBoard[i+2][j-1].flag == false)// if bottom right tile is unflagged
                    {
                        tempPair = make_pair(i+2, j-1);
                        insertFutureMoves(tempPair, 2); //flag bottom right
                    }
                }
                if(j + 1 < rowDimension && gameBoard[i][j + 1].uncovered == false && gameBoard[i+1][j+1].uncovered == false)// if the two tiles on top are covered
                {
                    if(gameBoard[i+2][j+1].flag == false)// if top right tile is unflagged
                    {
                        tempPair = make_pair(i+2, j+1);
                        insertFutureMoves(tempPair, 2); //flag top right
                    }
                }
            }

            if( i + 2 < colDimension && //if two right from x is in bounds
                gameBoard[i][j].uncovered && 
                (gameBoard[i + 1][j].number - flaggedNeighborsCount(i + 1, j)) == 2 &&
                gameBoard[i + 2][j].number - flaggedNeighborsCount(i+2, j) == 1)//if 12 pattern to the left ("1" after subtracting flagged tiles)
            {
                if(j - 1 >= 0 && gameBoard[i+1][j - 1].uncovered == false && gameBoard[i+2][j-1].uncovered == false)// if the two tiles on bottom are covered
                {
                    if(gameBoard[i][j-1].flag == false)// if bottom left tile is unflagged
                    {
                        tempPair = make_pair(i, j-1);
                        insertFutureMoves(tempPair, 2); //flag bottom left
                    }
                }
                if(j + 1 < rowDimension && gameBoard[i+1][j + 1].uncovered == false && gameBoard[i+2][j+1].uncovered == false)// if the two tiles on top are covered
                {
                    if(gameBoard[i][j+1].flag == false)// if top right left is unflagged
                    {
                        tempPair = make_pair(i, j+1);
                        insertFutureMoves(tempPair, 2); //flag top left
                    }
                }
            }
        }
    }

    return;
}


//debug functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
