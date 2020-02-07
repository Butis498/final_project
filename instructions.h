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
int realMemory[realMemorySize];
int virtualMemory[virtualMemorySize];

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
        if (virtualMemory[i] == -1)
        {
            return i;
        }

        cout << " VirtualMemory is full , the operation can no be completed " << endl;
    }
    return -1;
}

int AviliableSpaceRealMemory()
{
    for (int i = 0; i < realMemorySize; i += page_size)
    {
        if (realMemory[i] == -1)
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
        virtualMemory[space_aviliable + i] = value;
    }
}

void loadPageToFrame(int space_aviliable, int last_process, int last_page)
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
        realMemory[space_aviliable + i] = value;
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

    return -1;
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

    return -1;
}

void printFrames(vector<int> vec)
{

    for (int i = 0; i < vec.size(); i++)
    {
        cout << vec.at(i) << ",";
    }
}

vector<int> freePagesofProcessfifo(int ID)
{

    vector<int> aux;
    vector<int> released;

    for (int i = 0; i < fifo_queue.size(); i++)
    {
        if (fifo_queue.front() == ID)
        {
            aux.push_back(fifo_queue.front());
        }
        else
        {
            released.push_back(fifo_queue.front());
        }

        fifo_queue.pop();
    }

    for (int i = 0; i < aux.size(); i++)
    {
        fifo_queue.push(aux.at(i));
    }

    return released;
}

vector<int> freePagesofProcesslru(int ID)
{

    vector<int> aux;
    vector<int> released;

    for (int i = 0; i < lru_queue.size(); i++)
    {
        if (lru_queue.front() == ID)
        {
            aux.push_back(lru_queue.front());
        }
        else
        {
            released.push_back(fifo_queue.front());
        }

        lru_queue.pop();
    }

    for (int i = 0; i < aux.size(); i++)
    {
        lru_queue.push(aux.at(i));
    }

    return released;
}
bool swap(int current_page, int ID, int nextFrame)
{

    int last_process = realMemory[nextFrame];
    int last_page = realMemory[nextFrame];

    int avilibleSpaceSwap = AviliableSpaceFrameSwap();

    if (avilibleSpaceSwap == -1)
    {
        return false;
    }

    int new_frame = floor(avilibleSpaceSwap / page_size);

    cout << "page " << last_page << " of process " << last_process << " swapped to frame " << new_frame << " from virtual memory " << endl;

    loadPageToSwap(avilibleSpaceSwap, last_process, last_page);

    if (!existstInSwapped(last_process))
    {
        swappedPages.push_back(last_process);
    }

    swappedPages.at(getSwappedMemberPos(last_process)).frame = avilibleSpaceSwap;

    process_pages.at(getProcessedMemberPos(last_process)).frame = -1;

    loadPageToFrame(nextFrame, ID, current_page);

    process_pages.at(getProcessedMemberPos(ID)).frame = nextFrame;

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

    if (virtualAdress < 0 || virtualAdress > process_pages.at(ID).frame * page_size)
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

    if (!existsProcess(ID))
    {
        if (!existstInSwapped(ID))
            ;
        {
            cout << "No valid adress fro this process" << endl;
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
            process_pages.at(getProcessedMemberPos(ID)).frame = nextFrame;
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

            cout << "the page " << page << "of the process " << ID << " exists and was mouved to the frame " << floor(nextFrame / double(page_size)) << endl;

            int page_in_swap = process_pages.at(getProcessedMemberPos(ID)).frame;

            loadPageToSwap(page_in_swap, -1, -1);

            process_pages.at(getProcessedMemberPos(ID)).frame = -1;
        }
    }
    else if (!ALGORITHM)
    {
        update_lru(process_pages.at(getProcessedMemberPos(ID)).frame);
    }

    current_time += 1;

    int frame = process_pages.at(getProcessedMemberPos(ID)).frame;
    int address = frame + disp;

    cout << "Virtual adress: " << virtualAdress << endl;
    cout << "Real adress: " << address << endl;
}

//ID = procces ID
//n = number of bytes
void P(int n, int ID)
{

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

    process_pages.at(getSwappedMemberPos(ID)).key = current_time;

    int curr_page = 0;
    int cont = 0;

    while (curr_page < number_of_pages)
    {

        if (cont >= realMemorySize && curr_page < number_of_pages)
        {

            int nextFrame = asingnNextframe();

            cout << endl
                 << endl
                 << nextFrame << endl
                 << endl;

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
            if (realMemory[cont] == -1)
            {

                frames.push_back(floor(cont / double(page_size)));
                process_pages.at(getProcessedMemberPos(ID)).frame = cont;

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
    cout << " had been asign to process  " << ID << endl;
}

//ID = process ID
void L(int ID)
{
    cout << "release frames of the process " << ID;
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
        if (pages.at(i).key != 0)
        {
            loadPageToFrame(pages.at(i).key, -1, -1);
        }
    }

    vector<int> released;
    if (ALGORITHM)
    {
        released = freePagesofProcessfifo(ID);
    }
    else
    {
        released = freePagesofProcesslru(ID);
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
            if (swapped.at(i).key != 0)
            {
                loadPageToSwap(pages.at(i).key, -1, -1);
            }
        }

        
    }
}

void E()
{

    cout << "End of instructions" << endl;
}

void F()
{

    cout << "F" << endl;
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