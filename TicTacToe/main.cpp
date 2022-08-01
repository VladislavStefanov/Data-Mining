#include<iostream>

using namespace std;

int checkWinner(int** board)
{
    for(int i = 0; i < 3; i++)
    {
        if(board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != 0)
        {
            return board[i][0];
        }
    }

    for(int i = 0; i < 3; i++)
    {
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != 0)
        {
            return board[0][i];
        }
    }

    if(board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != 0)
    {
        return board[0][0];
    }

    if(board[0][2] == board[1][1] && board [1][1] == board[2][0] && board [0][2] != 0)
    {
        return board[0][2];
    }

    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            if(board[i][j] == 0)
            {
                return 2;
            }
        }
    }

    return 0;
}

void printBoard(int** board)
{
    cout << endl;
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            if(board[i][j] == 0)
            {
                cout << "- ";
            }

            if(board[i][j] == 1)
            {
                cout << "x ";
            }

            if(board[i][j] == -1)
            {
                cout << "o ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

int minimax(int** board, int depth, int a, int b, bool maxPlayer)
{
    int winner = checkWinner(board);
    if (winner != 2) return winner * (10 - depth);

    if (maxPlayer)
    {
        int maxValue = -100;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (board[i][j] == 0)
                {
                    board[i][j] = 1;
                    int childValue = minimax(board, depth + 1, a, b, false);
                    maxValue = max(maxValue, childValue);
                    a = max(a, childValue);
                    board[i][j] = 0;
                    if (b <= a)
                    {
                        break;
                    }
                }
            }
        }

        return maxValue;
    }
    else
    {
        int minValue = 100;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (board[i][j] == 0)
                {
                    board[i][j] = -1;
                    int childValue = minimax(board, depth + 1, a, b, true);
                    minValue = min(minValue, childValue);
                    b = min(b, childValue);
                    board[i][j] = 0;
                    if (b <= a)
                    {
                        break;
                    }
                }
            }
        }

        return minValue;
    }
}

int main()
{
    cout << "Computer(1) or Player(2) is first?" << endl;
    int first;
    cin >> first;

    int** board = new int*[3];
    for(int i = 0; i < 3; i++)
    {
        board[i] = new int[3];
        for(int j = 0; j < 3; j++)
        {
            board[i][j] = 0;
        }
    }

    bool computer = first == 1;
    if (computer)
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                board[i][j] = 1;
                int value = minimax(board, 0, -2, 2, !computer);
                if (value == 0)
                {
                    goto AFTER_LOOP;
                }
                board[i][j] = 0;
            }
        }

        AFTER_LOOP:
        printBoard(board);
    }



    while (true)
    {
        int x, y;
        cin >> y >> x;
        if (board[y - 1][x - 1] != 0)
        {
            cout << "Invalid move!" << endl;
            continue;
        }
        board[y - 1][x - 1] = !computer * 2 - 1;

        int winner = checkWinner(board);
        if (winner == !computer * 2 - 1)
        {
            printBoard(board);
            cout << "Player wins!" << endl;
            break;
        }
        if (winner == 0)
        {
            printBoard(board);
            cout << "Draw!" << endl;
            break;
        }

        int computerRow, computerColumn, bestValue = -100;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (board[i][j] == 0)
                {
                    board[i][j] = computer * 2 - 1;
                    int value = minimax(board, 0, -2, 2, !computer);

                    if (bestValue < (computer * 2 - 1) * value)
                    {
                        bestValue = (computer * 2 - 1) * value;
                        computerRow = i;
                        computerColumn = j;
                    }

                    board[i][j] = 0;

                }
            }
        }
        board[computerRow][computerColumn] = computer * 2 - 1;
        printBoard(board);

        winner = checkWinner(board);
        if (winner == computer * 2 - 1)
        {
            cout << "Computer wins!" << endl;
            break;
        }
        if (winner == 0)
        {
            cout << "Draw!" << endl;
            break;
        }

    }

    for(int i = 0; i < 3; i++)
    {
        delete[] board[i];
    }
    delete[] board;

    return 0;
}
