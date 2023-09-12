#pragma once
#ifndef CONSOLE_H
#define CONSOLE_H
// CodePage
class CodePage
{
private:
  unsigned int initCodePage;
  unsigned int initOutputCodePage;
  unsigned int currentCodePage;
  unsigned int currentOutputCodePage;
public:
  void setInitCodePage(ProgramError& perror, int errorValue, std::string errorMessage);
  unsigned int getInitCodePage();
  void getCodePage(ProgramError& perror, int errorValue, std::string errorMessage);
  unsigned int getCurrentCodePage();
  void setCodePage(unsigned int codePage, ProgramError& perror, int errorValue, std::string errorMessage);
  void setInitOutputCodePage(ProgramError& perror, int errorValue, std::string errorMessage);
  unsigned int getInitOutputCodePage();
  void getOutputCodePage(ProgramError& perror, int errorValue, std::string errorMessage);
  unsigned int getCurrentOutputCodePage();
  void setOutputCodePage(unsigned int outputCodePage, ProgramError& perror, int errorValue, std::string errorMessage);
  CodePage(const unsigned int& initCodePage, const unsigned int& initOutputCodePage, const unsigned int& currentCodePage, const unsigned int& currentOutputCodePage) : initCodePage(initCodePage), initOutputCodePage(initOutputCodePage), currentCodePage(currentCodePage), currentOutputCodePage(currentOutputCodePage) {};
  bool operator==(const CodePage& other) const = default;
  CodePage()
  {
    initCodePage = 0;
    initOutputCodePage = 0;
    currentCodePage = initCodePage;
    currentOutputCodePage = initOutputCodePage;
  };
};
class Handle
{
private:
  HANDLE inputHandle;
  HANDLE outputHandle;
  HANDLE errorHandle;
public:
  void setInputHandle(ProgramError& perror, int errorValue, std::string errorMessage);
  void setOutputHandle(ProgramError& perror, int errorValue, std::string errorMessage);
  void setErrorHandle(ProgramError& perror, int errorValue, std::string errorMessage);
  HANDLE getInputHandle();
  HANDLE getOutputHandle();
  HANDLE getErrorHandle();
};
class ConsoleMode
{
private:
  unsigned int initConsoleMode;
  unsigned int currentConsoleMode;
public:
  void setInitConsoleMode(HANDLE handle, ProgramError& perror, int errorValue, std::string errorMessage);
  unsigned int getInitConsoleMode();
  void getConsoleMode(HANDLE handle, ProgramError& perror, int errorValue, std::string errorMessage);
  unsigned int getCurrentConsoleMode();
  void setConsoleMode(HANDLE handle, unsigned int consoleMode, ProgramError& perror, int errorValue, std::string errorMessage);
  ConsoleMode(const unsigned int& initConsoleMode, const unsigned int& currentConsoleMode) : initConsoleMode(initConsoleMode), currentConsoleMode(currentConsoleMode) {};
  bool operator==(const ConsoleMode& other) const = default;
  ConsoleMode()
  {
    initConsoleMode = 0;
    currentConsoleMode = initConsoleMode;
  };
};
#endif // !CONSOLE_H