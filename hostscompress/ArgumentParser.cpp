#include "pch.h"
#include "ArgumentParser.h"
ArgumentParser::ArgumentParser(int& argc, const char** argv, int start)
{
  this->tokens = std::vector<std::string>(argv + start, argv + argc);
}
bool ArgumentParser::optionExists(const std::string& option) const
{
  return (std::find(this->tokens.begin(), this->tokens.end(), option) != this->tokens.end());
}
bool ArgumentParser::optionsExist(const std::vector<std::string>& options) const
{
  for (std::vector<std::string>::const_iterator iter = options.begin(); iter < options.end(); iter++)
  {
    if (this->optionExists(*iter)) return true;
  }
  return false;
}
const std::string& ArgumentParser::getOption(const std::string& option) const
{
  std::vector<std::string>::const_iterator iterator;
  iterator = std::find(this->tokens.begin(), this->tokens.end(), option);
  if ((iterator != this->tokens.end()) && (++iterator != this->tokens.end()))
  {
    return *iterator;
  }
  static const std::string empty("");
  return empty;
}
const std::string& ArgumentParser::getOptions(const std::vector<std::string>& options) const
{
  for (std::vector<std::string>::const_iterator iter = options.begin(); iter < options.end(); iter++)
  {
    const std::string &arg = this->getOption(*iter);
    if (!arg.empty()) return arg;
  }
  static const std::string empty("");
  return empty;
}