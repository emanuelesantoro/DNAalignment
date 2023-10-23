#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
// ANSI escape codes for colors
#define ANSI_RESET "\033[0m"
#define ANSI_YELLOW "\033[33m" //Yellow color
#define ANSI_BLACK "\033[30m" // Black color

using namespace std;

std::string getArrowChar(bool flags[], bool isBacktrack)
{
    const std::string &color = isBacktrack ? ANSI_YELLOW : ANSI_BLACK; // Use green for isBacktrack, black for default
    if (flags[0])
    {
        return color + "↖" + ANSI_RESET;
    }
    else if (flags[1])
    {
        return color + "↑" + ANSI_RESET;
    }
    else if (flags[2])
    {
        return color + "←" + ANSI_RESET;
    }
    else
    {
        return " ";
    }
}
using namespace std;
enum DNA
{
    A,
    T,
    G,
    C
};
// Function to convert an enum value to a character representation
std::string enumToString(DNA dna)
{
    switch (dna)
    {
    case A:
        return "A";
    case T:
        return "T";
    case G:
        return "G";
    case C:
        return "C";
    default:
        return "Unknown";
    }
}
int charToInt(char c)
{
    switch (c)
    {
    case 'A':
        return 0;
    case 'T':
        return 1;
    case 'G':
        return 2;
    case 'C':
        return 3;
    }
}
std::string enumToString(int intValue)
{
    return enumToString(static_cast<DNA>(intValue));
}
class Cell
{
private:
    bool flags[3]; // Array of three flags that represent the three backtrack pointer
    bool isBacktrack;
    int value;

public:
    Cell()
    {
        for (int i = 0; i < 3; i++)
        {
            flags[i] = false;
        }
        value = 0;
        isBacktrack = false;
    }
    // Method to set the backtrack diagonal flag to true
    void setBackTrackDiag()
    {
        flags[0] = true;
    }

    // Method to set the backtrack top flag to true
    void setBackTrackTop()
    {
        flags[1] = true;
    }

    // Method to set the backtrack left flag to true
    void setBackTrackLeft()
    {
        flags[2] = true;
    }

    // Method to get the state of the backtrack diagonal flag
    bool isBackTrackDiag() const
    {
        return flags[0];
    }

    // Method to get the state of the backtrack top flag
    bool isBackTrackTop() const
    {
        return flags[1];
    }

    // Method to get the state of the backtrack left flag
    bool isBackTrackLeft() const
    {
        return flags[2];
    }
    bool *getFlags()
    {
        return flags;
    }
    void setFlags(bool diag, bool top, bool left)
    {
        flags[0] = diag;
        flags[1] = top;
        flags[2] = left;
    }
    int getValue() const
    {
        return value;
    }
    void setValue(int data)
    {
        value = data;
    }
    void setBacktrack()
    {
        isBacktrack = true;
    }
    bool getBacktrack()
    {
        return isBacktrack;
    }
};

void backtrackingAndPrinting(vector<vector<Cell>> table, string xString, string yString, int dimTableX, int dimTableY, bool isSwapped);
int main(int argc, char const *argv[])
{
    ifstream inputFile("input.txt"); // Open the input file

    if (!inputFile)
    {
        cerr << "Error opening the input file." << endl;
        return 1;
    }

    string xString, yString;
    int gapCost;
    int substitutionMatrix[4][4];

    // Read the strings from the file
    getline(inputFile, xString);
    xString.erase(xString.find_last_not_of("\r") + 1); // Remove carriage return characters

    getline(inputFile, yString);
    yString.erase(yString.find_last_not_of("\r") + 1); // Remove carriage return characters

    // Read the gap cost from the file
    inputFile >> gapCost;

    // Read the substitution matrix from the file
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            inputFile >> substitutionMatrix[i][j];
        }
    }

    inputFile.close(); // Close the input file

    cout << "Substitution Matrix:" << endl;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            cout << substitutionMatrix[i][j] << "\t";
        }
        cout << endl;
    }
    cout << "Gap cost:" << gapCost << endl;

    cout << "First string: " << xString << endl;
    cout << "Second string: " << yString << endl;

    // put in yString the longest string
    bool isSwapped = false;
    if (xString.length() > yString.length())
    {
        string temp = xString;
        xString = yString;
        yString = temp;
        isSwapped = true;
    }

    // The plus one is needed because we also need to consider the empty string
    int dimTableX = xString.length() + 1;
    int dimTableY = yString.length() + 1;

    vector<vector<Cell>> table(dimTableX, vector<Cell>(dimTableY));

    // loop for computing the table.
    // remember that STRINGS and INDEXES of table are SHIFTED of 1
    for (int i = 0; i < dimTableX; i++)
    {
        // j start from i because we only need to compute the upper half of the table
        for (int j = i; j < dimTableY; j++)
        {
            // Initialization of the first row
            if (i == 0)
            {
                if (j >= 1)
                {
                    // Initialize the first row with the number of gap cost
                    table[i][j].setValue(gapCost * j);
                    table[i][j].setBackTrackLeft();
                }
                // if j = 0 we don't need to do anything because in i=0 and j=0 the table is already initialized with 0
            }
            else
            {
                Cell currentCell = table[i][j];
                int valueDiag = table[i - 1][j - 1].getValue() + substitutionMatrix[charToInt(xString[i - 1])][charToInt(yString[j - 1])];
                int minValue = valueDiag;
                // calculating the value of the cells in the diagonals
                if (i == j)
                {
                    currentCell.setBackTrackDiag();
                }
                if (i < j)
                {

                    int valueLeft = table[i][j - 1].getValue() + gapCost;
                    minValue = min(valueLeft, valueDiag);
                    if (minValue == valueDiag)
                    {
                        currentCell.setBackTrackDiag();
                    }
                    if (minValue == valueLeft)
                    {
                        currentCell.setBackTrackLeft();
                    }
                }

                currentCell.setValue(minValue);
                table[i][j] = currentCell;
            }
        }
    }
    backtrackingAndPrinting(table, xString, yString, dimTableX, dimTableY, isSwapped);
    cout << endl
         << "Final cost : " << table[dimTableX - 1][dimTableY - 1].getValue() << endl;
    return 0;
}

void backtrackingAndPrinting(vector<vector<Cell>> table, string xString, string yString, int dimTableX, int dimTableY, bool isSwapped)
{
    // Backtracking
    int i = dimTableX - 1;
    int j = dimTableY - 1;
    string xAlignment = "";
    string yAlignment = "";
    while (i > 0 || j > 0)
    {
        Cell currentCell = table[i][j];
        if (currentCell.isBackTrackDiag())
        {
            bool isMatch = xString[i - 1] == yString[j - 1];
            xAlignment = (isMatch ? string(1, xString[i - 1]) : "*") + xAlignment;
            yAlignment = (isMatch ? string(1, yString[j - 1]) : "*") + yAlignment;
            currentCell.setFlags(true, false, false);
            currentCell.setBacktrack();
            table[i][j] = currentCell;
            i--;
            j--;
        }
        else if (currentCell.isBackTrackTop())
        {
            xAlignment = xString[i - 1] + xAlignment;
            yAlignment = "_" + yAlignment;
            currentCell.setFlags(false, true, false);
            currentCell.setBacktrack();
            table[i][j] = currentCell;
            i--;
        }
        else if (currentCell.isBackTrackLeft())
        {
            xAlignment = "_" + xAlignment;
            yAlignment = yString[j - 1] + yAlignment;
            currentCell.setFlags(false, false, true);
            currentCell.setBacktrack();
            table[i][j] = currentCell;
            j--;
        }
    }
    // print table
    cout << endl;
    cout << " ";
    for (int j = 0; j < dimTableY; j++)
    {
        if (j == 0)
            // we have to print the epsilon character
            cout << "+-"
                 << "ε"
                 << "-";
        else
            cout << "+-" << yString[j - 1] << "-";
    }
    cout << "+" << endl;
    for (int i = 0; i < dimTableX; i++)
    {
        cout << "+";
        for (int j = 0; j < dimTableY; j++)
        {
            cout << "+---";
        }
        cout << "+" << endl;

        if (i == 0)
            cout << "ε";
        else
            cout << xString[i - 1];

        for (int j = 0; j < dimTableY; j++)
        {
            cout << "|" << getArrowChar(table[i][j].getFlags(), table[i][j].getBacktrack()) << table[i][j].getValue() << (table[i][j].getValue() < 10 ? " " : "");
        }
        cout << "|" << endl;
    }
    cout << endl;
    cout << (isSwapped ? "Y" : "X") << " alignment: " << xAlignment << endl;
    cout << (isSwapped ? "X" : "Y") << " alignment: " << yAlignment << endl;
}
