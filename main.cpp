#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>
#include <cstring>
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
    int value;

public:
    Cell()
    {
        for (int i = 0; i < 3; i++)
        {
            flags[i] = false;
        }
        value = 0;
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

    int getValue() const
    {
        return value;
    }

    void setValue(int data)
    {
        value = data;
    }
};

int main(int argc, char const *argv[])
{
    bool parametricFlag = false;

    // Iterate through the command-line arguments
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-p") == 0)
        {
            parametricFlag = true; // Set the flag to true if "-p" is found
            break;                 // No need to continue searching
        }
    }

    int substitutionMatrix[4][4];
    int gapCost;
    if (!parametricFlag)
    {

        cout << "Default parameters are being used. Launch with -p to change them" << endl;
        gapCost = 2;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j <= i; j++)
            {
                if (i == j)
                {
                    substitutionMatrix[i][j] = 0;
                }
                else
                {
                    substitutionMatrix[i][j] = 10;
                    // Mirror the value to the other half of the matrix for simmetricity
                    substitutionMatrix[j][i] = 10;
                }
            }
        }
    }
    else
    {
        cout << "Insert cost for gap: " << endl;
        cin >> gapCost;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j <= i; j++)
            {
                if (i == j)
                {
                    substitutionMatrix[i][j] = 0;
                }
                else
                {
                    cout << "Enter substitution cost for " << enumToString(i) << " and " << enumToString(j) << ": ";
                    cin >> substitutionMatrix[i][j];
                    // Mirror the value to the other half of the matrix for simmetricity
                    substitutionMatrix[j][i] = substitutionMatrix[i][j];
                }
            }
        }
    }
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
    string xString, yString;
    cout << "Insert first string:\n";
    //TODO change this for input 
    //getline(cin, xString); 
    xString = "AGGCA";
    cout << "Insert second string:\n";
    //getline(cin, yString);
    yString = "AGGGCT";

    // The plus one is needed because we also need to consider the empty string
    int dimTableX = xString.length() + 1;
    int dimTableY = yString.length() + 1;

    vector<vector<Cell>> table(dimTableX, vector<Cell>(dimTableY));

    // loop for computing the table.
    // remember that STRINGS and INDEXES of table are SHIFTED of 1
    for (int i = 0; i < dimTableX; i++)
    {
        for (int j = 0; j < dimTableY; j++)
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
            // Initialization of the first column
            else if (j == 0)
            {
                if (i >= 1)
                {
                    // Initialize the first column with the number of gap cost
                    table[i][j].setValue(gapCost * i);
                    table[i][j].setBackTrackTop();
                }
            }
            else
            {
                /// improve the code below
                int valueDiag = table[i - 1][j - 1].getValue() + substitutionMatrix[charToInt(xString[i - 1])][charToInt(yString[j - 1])];
                int valueTop = table[i - 1][j].getValue() + gapCost;
                int valueLeft = table[i][j - 1].getValue() + gapCost;

                int minValue = min(valueDiag, min(valueTop, valueLeft));
                Cell currentCell = table[i][j];
                // Set the backtrack flags
                if (minValue == valueDiag)
                {
                    currentCell.setBackTrackDiag();
                }
                if (minValue == valueTop)
                {
                    currentCell.setBackTrackTop();
                }
                if (minValue == valueLeft)
                {
                    currentCell.setBackTrackLeft();
                }

                currentCell.setValue(minValue);
                table[i][j] = currentCell;
            }
        }
    }
    cout << "Table:" << endl;
    for (int i = 0; i < dimTableX; i++)
    {
        for (int j = 0; j < dimTableY; j++)
        {
            cout << (table[i][j].getValue() < 10 ? "| " : "|") << table[i][j].getValue() << "|";
        }
        cout << endl;
    }
}
