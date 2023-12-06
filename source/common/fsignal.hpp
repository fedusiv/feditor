#ifndef __SIGNAL_HPP__
#define __SIGNAL_HPP__

#include <sys/types.h>
enum class FSignalOpCode{
    SwithTabListId,
};

typedef struct{
    FSignalOpCode opcode;
    int data;  // only for now it's uint. TODO:// mode something for the keeping
}FSignalCell;

void FSignalInit(void);
FSignalCell * FSignalGet(void);
void FSignalSend(FSignalCell * pnt);

#endif //__SIGNAL_HPP__
