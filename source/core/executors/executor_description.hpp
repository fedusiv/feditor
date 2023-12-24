#ifndef __EXECUTOR_DESCRIPTIONS_HPP__
#define __EXECUTOR_DESCRIPTIONS_HPP__

#include <iostream>

namespace ExecutorsDescriptions{
  // This array is storage for all descriptios of executors and it's ordered in synchronization of OpCodes.
  // When adding new, you need to keep this order. On this order code is also based, for example in creating executor elements
  // TODO: need to decide where to keep this docs string. Inside executor binary or in some data obj file
  const std::string Descriptions[]  = {
    // TextInsert
    "Simple text insert operation",
    // TextInsertNewLine
    "Insert new line symbol, pressing Enter for example",
    //DeleteBeforeCursor,
    "Delete symbol before cursor, it's analogue to backspace functionality",
    //DeleteAfterCursor,
    "Delete symbol after cursor, it's analogue to delete key functionality",
    //MoveCursorUp,
    "It's obvious, move cursor up",
    //MoveCursorDown,
    "It's obvious, move cursor down",
    //MoveCursorLeft,
    "It's obvious, move cursor left",
    //MoveCursorRight,
    "It's obvious, move cursor right",
    //MoveCursorTo,
    "Move cursor to specific position in coordinates for current active buffer",
    //ScrollUp,
    "Scroll up, for example with mouse.",
    //ScrollDown,
    "Scroll down",
    //ScrollLeft,
    "Scroll left",
    //ScrollRight,
    "Scroll Right",

    //CreateBuffer,
    "CreateBuffer <filename>. It creates new buffer in current tab in vertical orientation. With the given name.",
    //CreateBufferHorizontal,
    "CreateBuffer <filename>. It creates new buffer in current tab in horizontal orientation. With the given name.",
    //SwitchBetweenEditorsInTabUp,
    "Switch between buffers in current tab, move up",
    //SwitchBetweenEditorsInTabDown,
    "Switch between buffers in current tab, move down",
    //SwitchBetweenEditorsInTabLeft,
    "Switch between buffers in current tab, move left",
    //SwitchBetweenEditorsInTabRight,
    "Switch between buffers in current tab, move right",
    //CreateNewTab,
    "Create new tab <tab-name>. Creates new tab with the given name, if no name is given, name will default",
    //SwitchToTab,
    "SwithToTab <id>, switching to tab id",

    //ExitApp,
    "Exit feditor",
    //GuiResize,
    "Resize window with given size",

    //ChangeEditorModeToInsert,
    "Change Editor mode to normal mode",
    //ChangeEditorModeToCmd,
    "Change Editor mode to Cmd mode",

    //ExecutorOpCodeMax
  };
}

#endif // EXECUTOR_DESCRIPTIONS_HPP__
