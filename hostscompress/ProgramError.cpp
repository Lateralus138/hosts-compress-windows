#include "pch.h"
#include "ProgramError.h"
std::string GetLastErrorAsString()
{
  DWORD errorId = ::GetLastError();
  if (errorId == 0)
  {
    return std::string();
  }
  LPSTR buffer = nullptr;
  size_t messageSize =
    FormatMessageA
    (
      FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL, errorId,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      (LPSTR)&buffer, 0, NULL
    );
  std::string message(buffer, messageSize);
  LocalFree(buffer);
  return message;
}
void SetCustomError(ProgramError& perror, int errorValue, std::string errorMessage)
{
  std::string workingMessage = GetLastErrorAsString();
  workingMessage.append(errorMessage);
  perror.addError(errorValue, workingMessage);
  perror.setError(errorValue);
}
void ProgramError::addError(int value, std::string message)
{
  errors[value] = message;
}
void ProgramError::setError(int errorNumber)
{
  currentError = { errorNumber, errors[errorNumber] };
}
ErrorStruct ProgramError::getError()
{
  return currentError;
}
void ProgramError::print(bool color)
{
  std::string errorMessage = ((color) ? "\x1b[" : "");
  if (color)
  {
    errorMessage.append((currentError.value > 0) ? "91" : "92");
    errorMessage.push_back('m');
  }
  errorMessage.append(currentError.message);
  errorMessage.push_back(':');
  if (color)
  {
    errorMessage.append("\x1b[m");
  }
  errorMessage.append(" [");
  if (color)
  {
    errorMessage.append("\x1b[");
    errorMessage.append((currentError.value > 0) ? "91" : "92");
    errorMessage.push_back('m');
  }
  errorMessage.append(std::to_string(currentError.value));
  if (color)
  {
    errorMessage.append("\x1b[m");
  }
  errorMessage.push_back(']');
  if (currentError.value > 0) std::cerr << errorMessage << '\n';
  else std::cout << errorMessage << '\n';
}