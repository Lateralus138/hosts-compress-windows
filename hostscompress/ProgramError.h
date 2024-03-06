#pragma once
std::string GetLastErrorAsString();
struct ErrorStruct
{
  int value;
  std::string message;
  ErrorStruct(int &value, std::string &message)
    : value(value), message(message) { };
  ErrorStruct()
  {
    value = 0;
    message = "Success";
  };
};
class ProgramError
{
private:
  std::map<int, std::string> errors;
  ErrorStruct currentError;
public:
  ProgramError(std::map<int, std::string> &errors, ErrorStruct &currentError)
    : errors(errors), currentError(currentError) { };
  ProgramError()
  {
    errors.insert({ 0, "Success" });
    currentError.value = 0;
    currentError.message = "Success";
  };
  void addError(int value, std::string message);
  void setError(int errorNumber);
  ErrorStruct getError();
  void print(bool color);
};
void SetCustomError(ProgramError& perror, int errorValue, std::string errorMessage);