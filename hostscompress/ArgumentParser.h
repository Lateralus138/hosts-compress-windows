#pragma once
class ArgumentParser
{
private:
  std::vector<std::string> tokens;
public:
  ArgumentParser(int& argc, const char** argv, int start);
  bool optionExists(const std::string& option) const;
  bool optionsExist(const std::vector<std::string>& options) const;
  const std::string& getOption(const std::string& option) const;
  const std::string& getOptions(const std::vector<std::string>& options) const;
};

