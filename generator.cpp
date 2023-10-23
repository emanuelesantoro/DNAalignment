#include <iostream>
#include <string>
#include <stdio.h>
using namespace std;
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

int main(int argc, char const *argv[])
{
    int n, m;
    cout << "Insert length of the first string to be generated\n";
    cin >> n;
    cout << "Insert length of the second string to be generated\n";
    cin >> m;

    // Seed the random number generator
    srand(time(NULL));

    // Generate the string 1
    string str;
    for (int i = 0; i < n; i++) {
        int r = rand() % 2;
        if (r == 0) {
            str += "A";
        } else {
            str += "C";
        }
    }
    // Generate the string 2
    string str2;
    for (int i = 0; i < m; i++) {
        int r = rand() % 2;
        if (r == 0) {
            str2 += "G";
        } else {
            str2 += "T";
        }
    }

    cout << "Generated string with length equal to n: " << str << endl;
    cout << "Generated string with length equal to m: " << str2 << endl;
    return 0;
}