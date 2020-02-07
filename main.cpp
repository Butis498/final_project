#include <iostream>
#include <string>
#include <math.h>
#include "functions.h"
#include "instructions.h"
#include <ctime>

using namespace std;


int main(int argc, char const *argv[])
{


   //set the fifo or the lru algorithm
   //if not a correct option program ends

   if (!setAlgorithm())
   {
      return 0;
   }

   openFile();
   inicialize();

   for (int i = 0; i < instructionsArg.size() ; i++)
   {
      //instruction to do
      char INST = instructionsArg.at(i).at(0)[0];
      vector<string> line = instructionsArg.at(i);
      //validate the instruction is correct

      if (validateArg(line, INST))
      {
         //instructions for each case

         switch (INST)
         {
         case 'A':

            A(stringToInt(line.at(1)), stringToInt(line.at(2)), stringToInt(line.at(3)));
            break;

         case 'P':
            P(stringToInt(line.at(1)), stringToInt(line.at(2)));
            break;
         case 'L':
            L(stringToInt(line.at(1)));
            break;

         case 'E':
            E();
            break;

         case 'F':
            F();
            break;
         case 'C':

            C(line);
            break;

         default:

            cout << "Error invalid arg in doc" << endl;
            break;
         }
      }
   }


   //end of instructions

   return 0;
}
