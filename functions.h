#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include "instructions.h"
#include <vector>
#include <bits/stdc++.h>
using namespace std;

#ifndef FUNCTIONSH
#define FUNCTIONSH

void inicialize () {

    for (int i = 0; i < virtualMemorySize; i++)
    {
        virtualMemory[i].proceso = -1;
    }

    for (int i = 0; i < realMemorySize; i++)
    {
        realMemory[i].proceso = -1;
    }
    
    
}

bool setAlgorithm()
{

    string option;
    cout << "type (fifo or lru):" << endl;
    cin >> option;

    if (option != "fifo" && option != "lru")
    {

        cout << "Option is not valid , program ends" << endl;

        return false;
    }

    if (option == "fifo")
    {
        ALGORITHM = true;
    }
    else
    {
        ALGORITHM = false;
    }

    return true;
}

bool validateArg(vector<string> arguments, char Arg)
{

    switch (Arg)
    {
    case 'A':
        if (arguments.size() - 1 < 4)
        {
            cout << "Invalid number of arguments for instructions of " << Arg << " , " << Arg
                 << " expexts 3 arguments , instruction will be ignored" << endl;
                 return false;
        }

        break;

    case 'P':
        if (arguments.size() - 1 < 3)
        {
            cout << "Invalid number of arguments for instructions of " << Arg << " , " << Arg
                 << " expexts 3 arguments , instruction will be ignored" << endl;
                 return false;
        }
        break;
    case 'L':
        if (arguments.size() - 1 < 2)
        {
            cout << "Invalid number of arguments for instructions of " << Arg << " , " << Arg
                 << " expexts 3 arguments , instruction will be ignored" << endl;
                 return false;
        }
        break;

    case 'E':

        break;

    case 'F':

        break;
    case 'C':

        break;

    default:

        cout << "invalid argument " << Arg << endl;
        return false;
        break;
    }

    return true;
}

void openFile()
{

    ifstream test_file;
    string path;

    cout << "path followed by file name: " << endl;
   // cin >> path;
    //open file
    test_file.open("final_test.txt");

    while (!test_file.eof())
    {
        //vector for each word in line
        vector<string> arguments;
        string line;

        getline(test_file, line);

        istringstream ss(line);

        // Traverse through all words
        do
        {
            // Read a word
            string word;
            ss >> word;

            // save the read word
            arguments.push_back(word);

            // While there is more to read
        } while (ss);

        instructionsArg.push_back(arguments);
    }

    //file close
    test_file.close();
}

int stringToInt(string str)
{

    // object from the class stringstream
    stringstream geek(str);

    // The object has the value 12345 and stream
    // it to the integer x
    int x = 0;
    geek >> x;

    // Now the variable x holds the value of string
    return x;
}

#endif