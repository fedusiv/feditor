#include <common.hpp>

void BufferLineSplit(BufferLine *bline, BufferData& data)
{
  BufferLine line;
  for(auto c: *bline){
    line.push_back(c);
    if(c == 0x20){ // 0x20 is 32 decimal is space symbpl in ASCII
      data.push_back(line);
      line.clear();
    }
  }
  data.push_back(line);
}

// This function is created in case BufferLine will be changed to something different than std::vector<int> and do not need to touch code in the application
bool BufferLinesCompare(BufferLine *line1, BufferLine *line2)
{
  return *line1 == *line2;
}

// Deletes or erase specific line from from BufferData
void BufferDataDeleteLine(BufferData& data, int lineId)
{
  data.erase(data.begin() + lineId);
}
