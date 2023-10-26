#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;
int gapCost;
int mismatchCost;
char dna[4] = {'A', 'T', 'G', 'C'};
typedef struct
{
    string s1;
    string s2;
    int cost;
} result;
result generateStrings(int n, int m);
int main(int argc, char const *argv[])
{

    if (argc < 3)
    {
        cout << "Not enough parameters";
        return 1;
    }

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);

    // Seed the random number generator
    srand(time(NULL));

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
    int substitutionMatrix[4][4];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            inputFile >> substitutionMatrix[i][j];
        }
    }
    inputFile >> mismatchCost;
    inputFile.close(); // Close the input file

    if (n < m)
    {
        // swap them
        int temp = n;
        n = m;
        m = temp;
    }
    result r = generateStrings(n, m);
    // print result
    cout << "string of length " << r.s1.length() << ": " << r.s1 << endl;
    cout << "string of length " << r.s2.length() << ": " << r.s2 << endl;
    cout << "Maximum cost: " << r.cost << endl;
}

result generateStrings(int n, int m)
{

    string s1 = "";
    string s2 = "";
    // Implement the various cases
    // n<4 && m<4 -> just one character per string
    if (n < 4 && m < 4)
    {
        s1 = string(n, dna[0]);
        s2 = string(m, dna[1]);
        return {s1, s2, (n - m) * gapCost + m * mismatchCost};
    }
    // n==m -> generate randomly the first string and then build the dual string to have max cost
    if (n == m)
    {
        // generate randomly the first string and it should have every character of the DNA if it's longer than 4
        if (n >= 4)
        {
            for (int i = 0; i < n; i++)
            {
                if (i < 4)
                {
                    s1 += dna[i];
                }
                else
                {
                    s1 += dna[rand() % 4];
                }
                switch (s1[i])
                {
                case 'A':
                    s2 += 'G';
                    break;
                case 'T':
                    s2 += 'C';
                    break;
                case 'G':
                    s2 += 'A';
                    break;
                case 'C':
                    s2 += 'T';
                    break;
                default:
                    break;
                }
            }
        }
        return {s1, s2, m * mismatchCost};
    }
    // cases where the length difference is 3
    if ((n - m) == 3)
    {
        if (n == 4)
        {
            s1 = "ATGC";
            s2 += dna[rand() % 4];
            // we only have one match
            return {s1, s2, (n - m) * gapCost + mismatchCost * (m - 1)};
        }
        if (m == 4)
        {
            s1 = string(n - 3, dna[0]) + "TGC";
            s2 = "TGCA";
            // we only have one match
            return {s1, s2, (n - m) * gapCost + mismatchCost * (m - 1)};
        }
        s1 = string(n - 3, dna[0]) + "TTGC";
        s2 = string(m - 3, dna[1]) + "GGCA";
        return {s1, s2, (n - m) * gapCost + m * mismatchCost};
    }
    // both for diff <= 4 and greater than 4 since it only change the cost
    s1 = string(n - 3, dna[0]) + "TGC";
    s2 = string(m - 3, dna[1]) + "GCA";
    if (n - m < 4)
    {
        return {s1, s2, (n - m) * gapCost + mismatchCost * m};
    }
    else
    {
        return {s1, s2, (n - m) * gapCost + (m - 1) * mismatchCost};
    }
}