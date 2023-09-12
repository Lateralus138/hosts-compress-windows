#include "pch.h"
#include "Console.h"
// CodePage
void CodePage::setInitCodePage(ProgramError& perror, int errorValue, std::string errorMessage)
{
  if (initCodePage == 0)
  {
    getCodePage(perror, errorValue, errorMessage);
    initCodePage = currentCodePage;
  }
}
unsigned int CodePage::getInitCodePage()
{
  return initCodePage;
}
void CodePage::getCodePage(ProgramError& perror, int errorValue, std::string errorMessage)
{
  const unsigned int CP = GetConsoleCP();
  if (CP == 0)
  {
    SetCustomError(perror, errorValue, errorMessage);
  }
  currentCodePage = CP;
}
unsigned int CodePage::getCurrentCodePage()
{
  return currentCodePage;
}
void CodePage::setCodePage(unsigned int codePage, ProgramError& perror, int errorValue, std::string errorMessage)
{
  const bool SET = SetConsoleCP(codePage);
  if (!SET)
  {
    SetCustomError(perror, errorValue, errorMessage);
  }
  else
  {
    currentCodePage = codePage;
  }
}
void CodePage::setInitOutputCodePage(ProgramError& perror, int errorValue, std::string errorMessage)
{
  if (initOutputCodePage == 0)
  {
    getOutputCodePage(perror, errorValue, errorMessage);
    initOutputCodePage = currentOutputCodePage;
  }
}
unsigned int CodePage::getInitOutputCodePage()
{
  return initOutputCodePage;
}
void CodePage::getOutputCodePage(ProgramError& perror, int errorValue, std::string errorMessage)
{
  const unsigned int CP = GetConsoleOutputCP();
  if (CP == 0)
  {
    SetCustomError(perror, errorValue, errorMessage);
  }
  currentOutputCodePage = CP;
}
unsigned int CodePage::getCurrentOutputCodePage()
{
  return currentOutputCodePage;
}
void CodePage::setOutputCodePage(unsigned int outputCodePage, ProgramError& perror, int errorValue, std::string errorMessage)
{
  const bool SET = SetConsoleOutputCP(outputCodePage);
  if (!SET)
  {
    SetCustomError(perror, errorValue, errorMessage);
  }
  else
  {
    currentOutputCodePage = outputCodePage;
  }
}
// Handle
void Handle::setInputHandle(ProgramError& perror, int errorValue, std::string errorMessage)
{
  inputHandle = GetStdHandle(STD_INPUT_HANDLE);
  if (inputHandle == INVALID_HANDLE_VALUE)
  {
    SetCustomError(perror, errorValue, errorMessage);
  }
}
void Handle::setOutputHandle(ProgramError& perror, int errorValue, std::string errorMessage)
{
  outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
  if (outputHandle == INVALID_HANDLE_VALUE)
  {
    SetCustomError(perror, errorValue, errorMessage);
  }
}
void Handle::setErrorHandle(ProgramError& perror, int errorValue, std::string errorMessage)
{
  errorHandle = GetStdHandle(STD_ERROR_HANDLE);
  if (errorHandle == INVALID_HANDLE_VALUE)
  {
    SetCustomError(perror, errorValue, errorMessage);
  }
}
HANDLE Handle::getInputHandle()
{
  return inputHandle;
}
HANDLE Handle::getOutputHandle()
{
  return outputHandle;
}
HANDLE Handle::getErrorHandle()
{
  return errorHandle;
}
// ConsoleMode
void ConsoleMode::setInitConsoleMode(HANDLE handle, ProgramError& perror, int errorValue, std::string errorMessage)
{
  if (initConsoleMode == 0)
  {
    getConsoleMode(handle, perror, errorValue, errorMessage);
    initConsoleMode = currentConsoleMode;
  }
}
unsigned int ConsoleMode::getInitConsoleMode()
{
  return initConsoleMode;
}
void ConsoleMode::getConsoleMode(HANDLE handle, ProgramError& perror, int errorValue, std::string errorMessage)
{
  DWORD buffer = { 0 };
  const bool GET = GetConsoleMode(handle, &buffer);
  if (!GET)
  {
    SetCustomError(perror, errorValue, errorMessage);
  }
  else
  {
    currentConsoleMode = buffer;
  }
}
unsigned int ConsoleMode::getCurrentConsoleMode()
{
  return currentConsoleMode;
}
void ConsoleMode::setConsoleMode(HANDLE handle, unsigned int consoleMode, ProgramError& perror, int errorValue, std::string errorMessage)
{
  bool SET = SetConsoleMode(handle, consoleMode);
  if (!SET)
  {
    SetCustomError(perror, errorValue, errorMessage);
  }
  else
  {
    currentConsoleMode = consoleMode;
  }
}
