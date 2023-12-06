#include "fsignal.hpp"
#include "macros.h"

#define AMOUNT_OF_CELLS 100

FSignalCell *fSignalCells[AMOUNT_OF_CELLS];  // immediately allocate some hardcoded values for signaling
int receivedAmount;
int clearAmount;

void FSignalInit(void)
{
    for(auto c: fSignalCells)
    {
        c = nullptr;    // clear eveyrthing
        UNUSED(c);
    }
    receivedAmount = 0;
    clearAmount = 0;
}

FSignalCell * FSignalGet(void)
{
    FSignalCell * res = nullptr;
    if(receivedAmount){ // there is elements in buffer
        res = fSignalCells[clearAmount];
        clearAmount++;
        receivedAmount--;
    }else{
        // no elements in buffer
        if(clearAmount){
            // there were elements. clear buffer
            for(auto c: fSignalCells){
                delete c;
                c = nullptr;
                clearAmount--;
                if(!clearAmount){
                    break;  // when received zero - exit
                }
            }
        }
    }
    return res;
}

/*
    FSignal handles memory allocation. Do not worry. Create and send signals
*/
void FSignalSend(FSignalCell * pnt)
{
    fSignalCells[receivedAmount] = pnt;
    receivedAmount++;
}

