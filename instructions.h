#include<iostream>
#include<iostream>
#include<string>
#include<math.h>
#include<vector>
using namespace std;
#ifndef INSTRUCTIONS.H
#define INSTRUCTIONS.H

//true means fifo 
//false means lru
bool ALGORITHM = true;

//vector for each line 
vector<vector<string>> instructionsArg; 

//virtualAdress = is the virtual adress where in range
//ID = process ID
//mode = modification 1 or 0
void A (int virtualAdress,int ID,int mode){
 
 cout << "A " << virtualAdress << " " << ID << " " << mode << endl;

}

//ID = procces ID
//n = number of bytes
void P (int n , int ID){

    cout << "P " << n << " " << ID << endl;

}

//ID = process ID
void L (int ID){

    cout << "L " << ID << endl;

}

void E () {

    cout << "End of instructions" << endl;
}

void F () {

    cout << "F" << endl;
}

void C (vector<string> str) {

    for (int i = 0; i < str.size() -1; i++)
    {
        cout << str.at(i) << " ";
    }
    cout << endl;
}

#endif 