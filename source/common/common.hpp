#ifndef __COMMON_HPP__
#define __COMMON_HPP__

#include <vector>

typedef typename std::vector<int> BufferLine;
typedef typename std::vector<BufferLine> BufferData;

void BufferLineSplit(BufferLine *bline, BufferData& data);
bool BufferLinesCompare(BufferLine *line1, BufferLine *line2);
void BufferDataDeleteLine(BufferData& data, int lineId);

#endif // __COMMON_HPP__
