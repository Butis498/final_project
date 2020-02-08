#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <ctime>
#include <stdlib.h>
#include <queue>
#include "strcuts.h"
using namespace std;

#ifndef INSTRUCTIONSH
#define INSTRUCTIONSH
const int realMemorySize = 2048;
const int virtualMemorySize = 4096;
const int page_size = 16;
const int number_of_frames = realMemorySize / page_size;
queue<int> fifo_queue;
queue<int> lru_queue;
int total_swaps = 0;
int page_faults = 0;
int current_time = 0;
//true means fifo
//false means lru
bool ALGORITHM = true;

//vector for each line
vector<vector<string>> instructionsArg;
//vector of real memory
vector<swapMember> swappedPages;
vector<processPagesMember> process_pages;
memoria realMemory[realMemorySize];
memoria virtualMemory[virtualMemorySize];

int randomPage()
{
    return rand() % number_of_frames;
}

void update_lru(int page)
{

    vector<int> aux;

    int cont;

    while (!lru_queue.empty())
    {
        if (lru_queue.front() != page)
        {
            aux.push_back(lru_queue.front());
        }
        lru_queue.pop();
    }

    for (int i = 0; i < aux.size() - 1; i++)
    {
        lru_queue.push(aux.at(i));
    }

    lru_queue.push(page);
}

//return true if process exists already
bool existsProcess(int ID)
{

    for (int i = 0; i < process_pages.size(); i++)
    {
        if (process_pages.at(i).ID == ID)
        {
            return true;
        }
    }

    return false;
}

int asingnNextframe()
{

    int frameNum;
    queue<int> aux = fifo_queue;

    while (!aux.empty())
    {
        //cout << aux.front() << "  ";
        aux.pop();
    }
    cout << endl;

    if (ALGORITHM == true)
    {
        frameNum = fifo_queue.front();
        fifo_queue.pop();
        fifo_queue.push(frameNum);
    }
    else
    {

        frameNum = lru_queue.front();
        lru_queue.pop();
        lru_queue.push(frameNum);
    }
    return frameNum;
}

int AviliableSpaceFrameSwap()
{
    for (int i = 0; i < virtualMemorySize; i += page_size)
    {
        if (virtualMemory[i].proceso == -1)
        {
            return i;
        }
    }
    cout << " Virtual Memory is full , the operation can no be completed " << endl;
    return -1;
}

int AviliableSpaceRealMemory()
{
    for (int i = 0; i < realMemorySize; i += page_size)
    {
        if (realMemory[i].proceso == -1)
        {
            return i;
        }
    }
    return -1;
}

void loadPageToSwap(int space_aviliable, int last_process, int last_page)
{

    int value;
    if (last_process == -1 && last_page == -1)
    {
        value = -1;
    }
    else
    {
        value = last_process;
    }

    for (int i = 0; i < page_size; i++)
    {
        virtualMemory[space_aviliable + i].proceso = value;
    }
}

void loadPageToFrame(int space_aviliable, int last_process, int last_page)
{

    int value1, value2;
    if (last_process == -1 && last_page == -1)
    {
        value1 = -1;
        value2 = -1;
    }
    else
    {
        value1 = last_process;
        value2 = last_page;
    }

    for (int i = 0; i < page_size; i++)
    {
        realMemory[space_aviliable + i].proceso = value1;
        realMemory[space_aviliable + i].page = value2;
    }
}

bool existstInSwapped(int last_process)
{

    for (int i = 0; i < swappedPages.size(); i++)
    {
        if (last_process == swappedPages.at(i).ID)
        {
            return true;
        }
    }

    return false;
}

int getSwappedMemberPos(int ID)
{

    for (int i = 0; i < swappedPages.size(); i++)
    {
        if (ID == swappedPages.at(i).ID)
        {
            return i;
        }
    }
}

int getProcessedMemberPos(int ID)
{

    for (int i = 0; i < process_pages.size(); i++)
    {
        if (ID == process_pages.at(i).ID)
        {
            return i;
        }
    }
}

void printFrames(vector<int> vec)
{

    for (int i = 0; i < vec.size(); i++)
    {
        cout << vec.at(i) << ",";
    }
}

vector<int> freePagesofProcessfifo(vector<int> vec)
{

    queue<int> aux = fifo_queue, new_queue;
    vector<int> auxVec, released;
    for (int i = 0; i < aux.size(); i++)
    {
        auxVec.push_back(aux.front());
        aux.pop();
    }
    int size = auxVec.size();

    for (int j = 0; j < vec.size(); j++)
    {
        for (int i = 0; i < size; i++)
        {
            if (auxVec.at(i) == vec.at(j))
            {
                released.push_back(auxVec.at(i) / 16);
                auxVec.erase(auxVec.begin() + i);
                size = auxVec.size();
                break;
            }
        }
    }
    for (int i = 0; i < auxVec.size(); i++)
    {
        new_queue.push(auxVec.at(i));
    }

    fifo_queue = new_queue;

    return released;
}

vector<int> freePagesofProcesslru(vector<int> vec)
{

    queue<int> aux = lru_queue, new_queue;
    vector<int> auxVec, released;
    for (int i = 0; i < aux.size(); i++)
    {
        auxVec.push_back(aux.front());
        aux.pop();
    }

    int size = auxVec.size();

    for (int j = 0; j < vec.size(); j++)
    {
        for (int i = 0; i < size; i++)
        {
            if (auxVec.at(i) == vec.at(j))
            {
                released.push_back(auxVec.at(i) / 16);
                auxVec.erase(auxVec.begin() + i);
                size = auxVec.size();
                break;
            }
        }
    }
    for (int i = 0; i < auxVec.size(); i++)
    {
        new_queue.push(auxVec.at(i));
    }

    lru_queue = new_queue;

    return released;
}

void delProcPages(int page, int ID)
{

    for (int i = 0; i < process_pages.at(getProcessedMemberPos(ID)).frame.size(); i++)
    {

        if (process_pages.at(getProcessedMemberPos(ID)).frame.at(i) == page)
        {
            process_pages.at(getProcessedMemberPos(ID)).frame.erase(process_pages.at(getProcessedMemberPos(ID)).frame.begin() + i);
        }
    }
}
bool swap(int current_page, int ID, int nextFrame)
{
    //cout << "next frame : " << nextFrame << endl;
    int last_process = realMemory[nextFrame].proceso;
    int last_page = realMemory[nextFrame].page;

    int avilibleSpaceSwap = AviliableSpaceFrameSwap();

    if (avilibleSpaceSwap == -1)
    {
        return false;
    }

    // cout <<endl<< "avilable for swap " << avilibleSpaceSwap << endl<< endl;
    int new_frame = avilibleSpaceSwap / page_size;

    cout << "page " << last_page << " of process " << last_process << " swapped to frame " << new_frame << " from virtual memory " << endl;

    loadPageToSwap(avilibleSpaceSwap, last_process, last_page);

    if (!existstInSwapped(last_process))
    {
        swappedPages.push_back(last_process);
    }

    swappedPages.at(getSwappedMemberPos(last_process)).frame.push_back(avilibleSpaceSwap);

    delProcPages(last_page, last_process);

    loadPageToFrame(nextFrame, ID, current_page);

    process_pages.at(getProcessedMemberPos(ID)).frame.push_back(nextFrame);

    current_time += 20;

    return true;
}

vector<processPagesMember> getAllProcessPages(int ID)
{

    vector<processPagesMember> vec;

    for (int i = 0; i < process_pages.size(); i++)
    {
        if (process_pages.at(i).ID == ID)
        {
            vec.push_back(process_pages.at(i));
        }
    }

    return vec;
}

vector<swapMember> getAllProcessSwapped(int ID)
{

    vector<swapMember> vec;

    for (int i = 0; i < swappedPages.size(); i++)
    {
        if (swappedPages.at(i).ID == ID)
        {
            vec.push_back(swappedPages.at(i));
        }
    }

    return vec;
}

vector<int> delMemberSwap(int ID)
{

    vector<int> released;

    for (int i = 0; i < swappedPages.size(); i++)
    {
        if (swappedPages.at(getSwappedMemberPos(ID)).ID == ID)
        {
            for (int j = 0; j < swappedPages.at(getSwappedMemberPos(ID)).frame.size(); j++)
            {

                released.push_back(swappedPages.at(getSwappedMemberPos(ID)).frame.at(j));
            }

            swappedPages.erase(swappedPages.begin() + i);
        }
    }

    for (int i = 0; i < released.size(); i++)
    {
        for (int j = released.at(i); j < released.at(i) + 16; j++)
        {
            virtualMemory[j].proceso = -1;
        }
    }

    return released;
}

bool existsFrame(int ID, int page)
{

    for (int i = 0; i < process_pages.at(getProcessedMemberPos(ID)).frame.size(); i++)
    {
        if (process_pages.at(getProcessedMemberPos(ID)).frame.at(i) == page)
        {
            return true;
        }
    }
    return false;
}

bool existsFrameSwap(int ID, int page)
{

    if (swappedPages.empty())
    {
        return false;
    }

    for (int i = 0; i < swappedPages.at(getSwappedMemberPos(ID)).frame.size(); i++)
    {
        if (swappedPages.at(getSwappedMemberPos(ID)).frame.at(i) == page / page_size)
        {
            return true;
        }
    }
    return false;
}

void delSwappedPages(int page, int ID)
{

    for (int i = 0; i < swappedPages.at(getSwappedMemberPos(ID)).frame.size(); i++)
    {
        if (swappedPages.at(getSwappedMemberPos(ID)).frame.at(i) == page)
        {
            swappedPages.at(getSwappedMemberPos(ID)).frame.erase(swappedPages.at(getSwappedMemberPos(ID)).frame.begin() + i);
        }
    }
}

int getPagePos(int page, int ID)
{

    for (int i = 0; i < swappedPages.at(getSwappedMemberPos(ID)).frame.size(); i++)
    {
        if (swappedPages.at(getSwappedMemberPos(ID)).frame.at(i) == page)
        {
            return i;
        }
    }
}
//virtualAdress = is the virtual adress where in range
//ID = process ID
//mode = modification 1 or 0
void A(int virtualAdress, int ID, int mode)
{

    cout << "A " << virtualAdress << " " << ID << " " << mode << endl;
    if (mode == 0)
    {
        cout << "Get the real direction corresponding to the virtual adress  " << virtualAdress << " , of the process " << ID << endl;
    }
    else
    {
        if (mode == 1)
            cout << "Get the real direction corresponding to the virtual adress " << virtualAdress << " , of the process " << ID << " and modify that direction" << endl;
    }

    if (!existsProcess(ID))
    {
        cout << "The process does not exists" << endl;
        cout << "the instruction will not be executed" << endl;
    }

    if (virtualAdress < 0)
    {
        cout << " Virtual adress out of range " << endl;
    }

    if (mode != 0 && mode != 1)
    {
        cout << "mode out of range has a range of 0 and 1" << endl;
    }

    int page = floor(virtualAdress / double(page_size));
    int whole = virtualAdress / page_size;
    double fraction = (virtualAdress / double(page_size)) - whole;

    int disp = round(fraction) * 16;

    if (!existsFrame(ID, page))
    {

        if (!existsFrameSwap(ID, page))
        {

            cout << "No valid adress for this process" << endl;
            cout << " process will not be executed" << endl;
            return;
        }

        page_faults += 1;

        int nextFrame = AviliableSpaceRealMemory();

        if (nextFrame == -1)
        {
            nextFrame = asingnNextframe();
            bool swapped = swap(page, ID, nextFrame);

            if (!swapped)
            {
                return;
            }

            total_swaps += 2;
        }
        else
        {
            loadPageToFrame(nextFrame, ID, page);
            process_pages.at(getProcessedMemberPos(ID)).frame.push_back(nextFrame);
            current_time += 11;

            if (ALGORITHM)
            {
                fifo_queue.push(nextFrame);
            }
            else
            {

                lru_queue.push(nextFrame);
            }

            total_swaps += 1;

            cout << "the page " << page << " of the process " << ID << " exists and was mouved to the frame " << floor(nextFrame / double(page_size)) << endl;

            int page_in_swap = process_pages.at(getProcessedMemberPos(ID)).frame.at(getPagePos(page, ID));

            loadPageToSwap(page_in_swap, -1, -1);

            delSwappedPages(page, ID);
        }
    }
    else if (!ALGORITHM)
    {
        update_lru(process_pages.at(getProcessedMemberPos(ID)).frame.at(getPagePos(page, ID)));
    }

    current_time += 1;
    int frame = process_pages.at(getProcessedMemberPos(ID)).frame.at(getPagePos(page, ID));
    int address = frame + disp;

    cout << "Virtual adress: " << virtualAdress << endl;
    cout << "Real adress: " << address << endl
         << endl;
}

//ID = procces ID
//n = number of bytes
void P(int n, int ID)
{

    cout << endl;

    cout << "P " << n << " " << ID << endl;
    cout << "Asing " << n << " bytes to the process " << ID << endl;

    if (n <= 0)
    {
        cout << "Error process must be bigger than 0 " << endl;
        return;
    }
    else if (n > 2048)
    {
        cout << "Error process must be smaller than 2048 bytes" << endl;
        return;
    }
    else if (ID < 0)
    {
        cout << "Error process id must be bigger than 0" << endl;
        return;
    }
    else if (existsProcess(ID))
    {
        cout << "Error process already exists" << endl;
        return;
    }

    int number_of_pages = ceil(n / double(page_size));

    vector<int> frames;

    process_pages.push_back(ID);

    process_pages.at(getProcessedMemberPos(ID)).starttime = current_time;

    int curr_page = 0;
    int cont = 0;

    while (curr_page < number_of_pages)
    {

        if (cont >= realMemorySize && curr_page < number_of_pages)
        {

            int nextFrame = asingnNextframe();

            bool swappedPage = swap(curr_page, ID, nextFrame);

            if (!swappedPage)
            {
                return;
            }

            frames.push_back(floor(nextFrame / page_size));

            total_swaps += 1;
            curr_page += 1;
        }

        while (cont < realMemorySize)
        {
            if (realMemory[cont].proceso == -1)
            {

                frames.push_back(floor(cont / double(page_size)));
                process_pages.at(getProcessedMemberPos(ID)).frame.push_back(cont);

                if (ALGORITHM)
                {
                    fifo_queue.push(cont);
                }
                else
                {
                    lru_queue.push(cont);
                }

                loadPageToFrame(cont, ID, curr_page);

                current_time += 10;
                curr_page += 1;
                break;
            }
            cont += page_size;
        }
    }
    cout << "the frames ";
    printFrames(frames);
    cout << " had been asign to process  " << ID << endl
         << endl;
}

//ID = process ID
void L(int ID)
{

    cout << "L " << ID << endl;
    cout << "release frames of the process " << ID << endl;

    if (!process_pages.at(getProcessedMemberPos(ID)).time == 0)
    {
        cout << " process has not been executed yet " << endl;
        return;
    }

    if (!process_pages.at(getProcessedMemberPos(ID)).time == -1)
    {

        cout << "Process " << ID << " has been already releases" << endl;
        return;
    }

    vector<processPagesMember> pages = getAllProcessPages(ID);

    for (int i = 0; i < pages.size(); i++)
    {
        if (pages.at(i).starttime != -1)
        {
            for (int j = 0; j < pages.at(i).frame.size(); j++)
            {
                loadPageToFrame(pages.at(i).frame.at(j), -1, -1);
            }
        }
    }

    int index;
    for (int i = 0; i < pages.size(); i++)
    {
        if (ID == pages.at(i).ID)
        {
            index = i;
        }
    }

    vector<int> released;
    if (ALGORITHM)
    {
        released = freePagesofProcessfifo(pages.at(index).frame);
    }
    else
    {
        released = freePagesofProcesslru(pages.at(index).frame);
    }

    cout << "the frames ";
    printFrames(released);
    cout << " had been released " << endl;

    vector<swapMember> swapped;

    if (existstInSwapped(ID))
    {
        swapped = getAllProcessSwapped(ID);

        for (int i = 0; i < swapped.size(); i++)
        {
            for (int j = 0; j < swapped.at(i).frame.size(); j++)
            {
                loadPageToSwap(swapped.at(i).frame.at(j), -1, -1);
            }
        }

        vector<int> vecS = delMemberSwap(ID);

        cout << "the frames ";
        printFrames(vecS);
        cout << " are released from the virtual Memory" << endl;
    }

    current_time += (pages.size() + swapped.size() - 1);

    process_pages.at(getProcessedMemberPos(ID)).endtime = current_time;

    cout << endl;
}

void E()
{

    cout << "End of instructions" << endl;
}

void F()
{

    int processes = 0;

    double average_turn_around = 0;

    if (process_pages.size() == 0)
    {

        cout << "No process in memory" << endl;
        return;
    }
    vector<processPagesMember> check_values;

    for (int i = 0; i < process_pages.size(); i++)
    {
        if (process_pages.at(i).endtime != -1)
        {
            check_values.push_back(process_pages.at(i));
        }
    }

    priority_queue<int> myQueue;

    if (check_values.size() > 0)
    {
        cout << " Calculiting report" << endl;


        for (int i = 0; i < check_values.size(); i++)
        {
            if (process_pages.at(i).endtime != -1)
            {
                L(process_pages.at(i).ID);
                
            }
            
        }
        
        
    }


    cout << "Report : " << endl;

    for (int i = 0; i < process_pages.size(); i++)
    {
        processes++;

        int current_turn_arund = (process_pages.at(i).endtime - process_pages.at(i).starttime) / 10;

        cout << "process :" << process_pages.at(i).ID << " " << current_turn_arund << endl;

        average_turn_around += current_turn_arund;
        
    }
    
    average_turn_around += average_turn_around / processes;

    cout << "turnaround average; " << average_turn_around << endl;

    cout << "page fault " << page_faults << endl;

    cout << "swap count " << total_swaps << endl;


    if (ALGORITHM)
    {
        while (!fifo_queue.empty())
        {
            fifo_queue.pop();
        }
        
    }else
    {
        while (!lru_queue.empty())
        {
            lru_queue.pop();
        }
        
    }
    
    
    for (int i = 0; i < realMemorySize; i++)
    {
        realMemory[i].proceso = -1;
        realMemory[i].page = -1;

        
    }
    
    for (int i = 0; i < virtualMemorySize; i++)
    {
        virtualMemory[i].proceso = -1;
        virtualMemory[i].page = -1;
    }
    
    current_time = 0;
    page_faults=0;
    total_swaps = 0;
    
    
    
}

void C(vector<string> str)
{

    for (int i = 0; i < str.size() - 1; i++)
    {
        cout << str.at(i) << " ";
    }
    cout << endl;
}

#endif