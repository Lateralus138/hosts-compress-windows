#include "pch.h"
#include "Functions.h"
const std::string GetWindowsDirectoryAsString(ProgramError& perror, int errorValue, std::string errorMessage)
{
  char path[MAX_PATH];
  unsigned int size = GetWindowsDirectoryA(path, sizeof(path));
  if (size == 0)
  {
    std::string message = GetLastErrorAsString();
    message.append(errorMessage);
    perror.addError(errorValue, message);
    return std::string("");
  }
  return std::string(path);
}