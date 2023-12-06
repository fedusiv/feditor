#ifndef __BUFFER_HPP__
#define __BUFFER_HPP__

#include <vector>
#include <string>

#include "vec2.hpp"
#include "keymap.hpp"

typedef typename std::vector<int> BufferLine;
typedef typename std::vector<BufferLine> BufferData;

enum DeleteOperations
{
    BeforeCursor,
    AfterCursor
};

class Buffer
{
    public:
        Buffer(std::string filepath);
        int LinesNumber(void);
        int ColumnsNumber(void); // maximum amount of characters in one line
        int bufferId;
        void Append(KeysInsertedText data); // append text data on current cursor position
        void InsertNewLine(void);   // insert new line.
        BufferLine * LineData(int lineNumber);
        Vec2 CursorPosition(); // return cursor position
        void DeleteAtCursor(DeleteOperations operation);
        void MoveCursor(MoveCursorDirection direction);
        void SetCursorPosition(Vec2 position);  // explicit nomination of cursor position. Buffer will check if this position if possible to be reached. If not it will set to first appropriate place
        int GetLineSize(int lineNumber);    // get line size
        std::string FileName();
        void RequestToSetActive(bool status);  // this method called around buffer handler, so that's why it's request. Buffer handler in required time will change current active buffer
        bool RequestActive();   // return status to be active. If it's active it will set immidiately flag to false
        bool IsFake();          // here is logic: fake buffer does not have any output and will be deleted once normal will be created
        void MarkFake();

    private:
        static int _globalId;
        Vec2 _cursorPosition;
        BufferData _buffer;
        std::string _filename;
        std::string _filepath;
        int _largestLineSize;   // amount of characaters (symbols) in one line. Maximum amount for whole file
        bool _requestActive;    // flag is set when some widget or other object in around of bufferhandler wants to set this buffer active
        bool _isFake;           // this flag, which buffer as  fake, means, that no one expect any information from this buffer. Do not be sad my little buffer, you are very brave kiddo and we are glad to have you
};

#endif // __BUFFER_HPP__
