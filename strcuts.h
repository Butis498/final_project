#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <ctime>
#include <stdlib.h>
#include <queue>

using namespace std;

#ifndef STRUCTSH
#define STRUCTSH


struct swapMember
{
    int ID;
    vector<int> frame;
    int key;
    swapMember(int);
};

struct memoria
{
    int proceso;
    int page;
};



swapMember::swapMember(int IDS){

    ID = IDS;
    key = 0;
}

struct processPagesMember
{
    int ID;
    int time;
    int endtime;
    int starttime;
    vector<int> frame;
    int key;
    processPagesMember(int);
};

processPagesMember::processPagesMember(int IDS){
    ID = IDS;
    key = 0;
    time = 0;
    starttime = -1;
    endtime = -1;
}


#endif