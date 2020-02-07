#ifndef STRUCTSH
#define STRUCTSH

struct swapMember
{
    int ID;
    int frame;
    int key;
    swapMember(int);
};


swapMember::swapMember(int IDS){

    ID = IDS;
    key = 0;
}

struct processPagesMember
{
    int ID;
    int time;
    int frame;
    int key;
    processPagesMember(int);
};

processPagesMember::processPagesMember(int IDS){
    ID = IDS;
    key = 0;
    time = 0;
}


#endif