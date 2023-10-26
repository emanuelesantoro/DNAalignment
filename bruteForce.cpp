#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
using namespace std;
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
int gapCost;
int substitutionMatrix[4][4];
vector<vector<Cell>> table;
int stringCost(string xString, string yString)
{

    // put in yString the longest string
    bool isSwapped = false;
    if (xString.length() > yString.length())
    {
        string temp = xString;
        xString = yString;
        yString = temp;
        isSwapped = true;
    }

    int dimTableX = xString.length() + 1;
    int dimTableY = yString.length() + 1;

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
    return table[xString.length()][yString.length()].getValue();
}

#include <cstdlib> // for atoi function

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

typedef struct
{
    string s1;
    string s2;
    int cost;
} result;

bool containsACGT(const std::string &s)
{
    bool containsA = false;
    bool containsC = false;
    bool containsG = false;
    bool containsT = false;

    for (char c : s)
    {
        if (c == 'A')
        {
            containsA = true;
        }
        else if (c == 'C')
        {
            containsC = true;
        }
        else if (c == 'G')
        {
            containsG = true;
        }
        else if (c == 'T')
        {
            containsT = true;
        }
    }

    return containsA && containsC && containsG && containsT;
}

std::vector<std::string> generateAllPermutations(int n, bool checkACGT = false)
{
    std::string characters = "ACGT";
    std::vector<std::string> permutations;

    if (n == 0)
    {
        permutations.push_back("");
        return permutations;
    }

    std::vector<std::string> currentPerms;
    currentPerms.push_back("");

    for (int i = 0; i < n; i++)
    {
        std::vector<std::string> newPerms;
        for (const std::string &current : currentPerms)
        {
            for (char c : characters)
            {
                newPerms.push_back(current + c);
            }
        }
        currentPerms = newPerms;
    }

    std::vector<std::string> validPerms;
    for (const std::string &perm : currentPerms)
    {
        if (!checkACGT || containsACGT(perm))
        {
            validPerms.push_back(perm);
        }
    }

    return validPerms;
}

std::vector<result> generateCombinations(int n, int m)
{
    std::vector<result> results;
    vector<string> arrayS1 = generateAllPermutations(n,true);
    vector<string> arrayS2 = generateAllPermutations(m,true);
    int length = arrayS1.size();
    int counter = 0;
    std::ofstream outputFile("output.txt");
    for (string s1 : arrayS1)
    {
        // loading bar
        std::cout << "Progress: " << counter << "/" << length - 1 << "\r" << flush;
        // Open the output file for writing
        

        // Loop through arrayS2 and perform the given code
        for (const std::string &s2 : arrayS2)
        {
            result r;
            r.s1 = s1;
            r.s2 = s2;

            r.cost = stringCost(s1, s2);
            if (r.cost == ((n - m) * gapCost + m * substitutionMatrix[1][0]))
            {
                outputFile << "s1: " << r.s1 << std::endl;
                outputFile << "s2: " << r.s2 << std::endl;
                outputFile << "cost: " << r.cost << std::endl;
            }
            // You can also add r to the results vector, but that's not needed for file output.
            results.push_back(r);
        }

        // Close the output file
        counter++;
    }
    
    outputFile.close();
    return results;
}

int main(int argc, char *argv[])
{
    ifstream inputFile("input.txt"); // Open the input file

    if (!inputFile)
    {
        cerr << "Error opening the input file." << endl;
        return 1;
    }

    string s1, s2;

    // Read the strings from the file
    getline(inputFile, s1);
    getline(inputFile, s2);
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

    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " <n> <m>" << std::endl;
        return 1;
    }

    int n = std::atoi(argv[1]);
    int m = std::atoi(argv[2]);

    table = vector<vector<Cell>>(m + 1, vector<Cell>(n + 1));

    std::vector<result> results = generateCombinations(n, m);
    int counter = 0;
    result max = {"", "", 0};
    for (const result &r : results)
    {

        if (r.cost > max.cost)
        {
            max = r;
        }

        // std::cout << counter << " :s1=" << r.s1 << " s2=" << r.s2 << " cost=" << r.cost << std::endl;

        counter++;
    }
    cout << endl
         << "Max cost: " << max.cost << endl;
    cout << "s1: " << max.s1 << endl;
    cout << "s2: " << max.s2 << endl;
    return 0;
}
