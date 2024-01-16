#include <fstring.hpp>
#include <memory_resource>
#include <iostream>

void BufferLineSplit(FString *bline, BufferData& data)
{
  FString line;
  for(auto c: *bline){
    line.push_back(c);
    if(c == 0x20){ // 0x20 is 32 decimal is space symbpl in ASCII
      data.push_back(line);
      line.clear();
    }
  }
  data.push_back(line);
}

// Deletes or erase specific line from from BufferData
void BufferDataDeleteLine(BufferData& data, int lineId)
{
  data.erase(data.begin() + lineId);
}

FString FString::StringToUtf8(std::string string)
{
  FString line;
  for (auto it = string.begin(); it != string.end(); ++it) {
    unsigned int codepoint = static_cast<unsigned char>(*it);
    if ((codepoint & 0xF8) == 0xF0) {
      // 4-byte UTF-8 sequence
      codepoint = (codepoint & 0x07) << 18;
      ++it;
      codepoint |= (static_cast<unsigned char>(*it) & 0x3F) << 12;
      ++it;
      codepoint |= (static_cast<unsigned char>(*it) & 0x3F) << 6;
      ++it;
      codepoint |= static_cast<unsigned char>(*it) & 0x3F;
    } else if ((codepoint & 0xF0) == 0xE0) {
      // 3-byte UTF-8 sequence
      codepoint = (codepoint & 0x0F) << 12;
      ++it;
      codepoint |= (static_cast<unsigned char>(*it) & 0x3F) << 6;
      ++it;
      codepoint |= static_cast<unsigned char>(*it) & 0x3F;
    } else if ((codepoint & 0xE0) == 0xC0) {
      // 2-byte UTF-8 sequence
      codepoint = (codepoint & 0x1F) << 6;
      ++it;
      codepoint |= static_cast<unsigned char>(*it) & 0x3F;
    }
    line.push_back(static_cast<int>(codepoint));
  }
  return line;
}

std::string FString::Utf8ToString(FString fstring)
{
  std::string res;

  for (int codepoint : fstring) {
      if (codepoint <= 0x7F) {
          // 1-byte character (7 bits)
          res.push_back(static_cast<char>(codepoint));
      } else if (codepoint <= 0x7FF) {
          // 2-byte character (11 bits)
          res.push_back(static_cast<char>(0xC0 | ((codepoint >> 6) & 0x1F)));
          res.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
      } else if (codepoint <= 0xFFFF) {
          // 3-byte character (16 bits)
          res.push_back(static_cast<char>(0xE0 | ((codepoint >> 12) & 0x0F)));
          res.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
          res.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
      } else if (codepoint <= 0x10FFFF) {
          // 4-byte character (21 bits)
          res.push_back(static_cast<char>(0xF0 | ((codepoint >> 18) & 0x07)));
          res.push_back(static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F)));
          res.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
          res.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
      } else {
          // Invalid code point
          res.push_back('?');
      }
  }

  return res;  
}

#ifdef DEBUG_INFO
void PrintDataLine(FString line)
{
  for(auto c: line){
    std::cout << c << std::endl;    
  }
}

#endif // DEBUG_INFO
