#ifndef __BUFFER_HPP__
#define __BUFFER_HPP__

#include <vector>
#include <string>

#include <common.hpp>
#include "vec2.hpp"
#include "keymap.hpp"


enum DeleteOperations
{
    BeforeCursor,
    AfterCursor
};

class Buffer
{
    public:
        Buffer(std::string filepath);
        Buffer(void);
        int LinesNumber(void);
        int ColumnsNumber(void); // maximum amount of characters in one line
        int bufferId;
        void Append(KeysInsertedText data); // append text data on current cursor position
        void Append(std::string data, int line); // append data to buffer. This is kind of debug function
        void InsertNewLine(void);   // insert new line.
        void CleanLine(int lineId); // delete content of whole line
        void DeleteLine(int lineId); // deletes whole line
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
        void MarkFake();        // set that this buffer is fake buffer. read logic under IsFake function
        void MarkOneLine();      // set, that this buffer is one line buffer. One line buffer is a logic, when only first line is managed by user, but other line are holding data

    private:
        void DefaultInit(void); // default init operation on creating buffer object
        void DeleteAtCursorOneLine(DeleteOperations operation); // just a wpar for one line logic of deleting
        void CopyOneLineToAnother(int srcId, int dstId); // copy content of one buffer line id to another. Hard copy with replace
        static int _globalId;
        Vec2 _cursorPosition;
        BufferData _buffer;
        std::string _filename;
        std::string _filepath;
        int _largestLineSize;   // amount of characaters (symbols) in one line. Maximum amount for whole file
        bool _requestActive;    // flag is set when some widget or other object in around of bufferhandler wants to set this buffer active
        bool _isFake;           // this flag, which buffer as  fake, means, that no one expect any information from this buffer. Do not be sad my little buffer, you are very brave kiddo and we are glad to have you
        bool _isOneLine;        // buffer can be one line, one line means, that user can manipulate  only with first line, other lines is used for keeping data
};

#endif // __BUFFER_HPP__
