// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Hosts Compress - Aggregate multiple domain names into single lines for local IP  ║
// ║ addresses of 0.0.0.0 and 127.0.0.1.                                              ║
// ║ © 2023 Ian Pride - New Pride Software / Services                                 ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝
#include "pch.h"
const std::regex RGX_ISURL_000("^0.0.0.0[\\s]+(?!(0.0.0.0|127.0.0.1|local$|localhost$|localhost.localdomain$)).*");
const std::regex RGX_URLS000_REPLACE("^(0.0.0.0)[\\s]+");
const std::regex RGX_ISURL_127("^127.0.0.1[\\s]+(?!(0.0.0.0|127.0.0.1|local$|localhost$|localhost.localdomain$)).*");
const std::regex RGX_URLS127_REPLACE("^(127.0.0.1)[\\s]+");
const std::regex RGX_UINT("[0-9]+");
struct Options
{
  std::regex RGX_UINT = std::regex("[0-9]+");
  int urlsPerLine = 9;
  std::string inputFile = "";
  std::string outputFile = "";
  bool isOutputColor = true;
  bool isDiscard = false;
  bool isOutputFile = false;
  int urlStartingLineIndex = 0;
  int url000StartingLineIndex = 0;
  int url127StartingLineIndex = 0;
  bool hasUrl000StartingLineIndex = false;
  bool hasUrl127StartingLineIndex = false;
  bool hasFooterAndHeader = false;
  bool isQuiet = false;
  bool isHelp = false;
  std::vector<std::string> urls000;
  std::vector<std::string> urls127;
  std::vector<std::string> headerContent;
  std::vector<std::string> footerContent;
};
std::string ColorString(std::string string, int color, Options& options)
{
  std::string result = std::string("");
  if (options.isOutputColor)
  {
    result.append("\x1b[");
    result.append(std::to_string(color));
    result.push_back('m');
    result.append(string);
    result.append("\x1b[");
    result.push_back('m');
  }
  else
  {
    result = string;
  }
  return result;
}
unsigned int ParseArguments(ArgumentParser &argumentParser, Options& options, ProgramError &perror)
{
  const std::vector<std::string> MONOCHROMEOPTIONS{ "/m", "/monochrome" };
  const std::vector<std::string> HELPOPTIONS{ "/h", "/help" };
  const std::vector<std::string> INPUTFILEOPTIONS{ "/i", "/input" };
  const std::vector<std::string> OUTPUTFILEOPTIONS{ "/o", "/output" };
  const std::vector<std::string> COUNTOPTIONS{ "/c", "/count" };
  const std::vector<std::string> DISCARDOPTIONS{ "/d", "/discard" };
  const std::vector<std::string> QUIETOPTIONS{ "/q", "/quiet" };
  if (argumentParser.optionsExist(MONOCHROMEOPTIONS))
  {
    options.isOutputColor = false;
  }
  if (argumentParser.optionsExist(DISCARDOPTIONS))
  {
    options.isDiscard = true;
  }
  if (argumentParser.optionsExist(QUIETOPTIONS))
  {
    options.isQuiet = true;
  }
  if (argumentParser.optionsExist(HELPOPTIONS))
  {
    std::cout <<
      "\n  Hosts Compress - Consolidate multiple blocked URLs to single IP lines in a"
      "\n  systems's hosts file with various options."
      "\n"
      "\n  @" << ColorString("USAGE", 95, options) <<
      "\n    hostscompress [" << ColorString("SWITCHES", 92, options) <<
      "] [[" << ColorString("OPTIONS", 94, options) <<
      "] <" << ColorString("PARAMS", 93, options) <<
      ">]"
      "\n"
      "\n  @" << ColorString("SWITCHES", 92, options) << ":"
      "\n    /h, /help       This help message."
      "\n    /m, /monochrome Verbose output is void of color."
      "\n    /q, /quiet      No verbosity; silences all errors and output with the errors"
      "\n                    and output with the exception of the resulting compression"
      "\n                    results if no output file is provided."
      "\n    /d, discard     Discard everything except the compressed lines from the"
      "\n                    resulting output. This is only recommended if the HOSTS file"
      "\n                    is only used for blocking URLs."
      "\n"
      "\n  @" << ColorString("OPTIONS", 94, options) << ":"
      "\n    /i, /input      Path to an optional " <<
      ColorString("INPUT FILE", 93, options) <<
      " to parse. Defaults to the"
      "\n                    default system hosts file location."
      "\n    /o, /output     Path to an optional " <<
      ColorString("OUTPUT FILE", 93, options) <<
      ". The default is to output"
      "\n                    to the console."
      "\n    /c, count       " <<
      ColorString("NUMBER", 93, options) <<
      " of URLs to compress to a single line. The default is"
      "\n                    9 (2-9)."
      "\n";
    options.isHelp = true;
  }
  if (argumentParser.optionsExist(INPUTFILEOPTIONS))
  {
    options.inputFile = argumentParser.getOptions(INPUTFILEOPTIONS);
    if (options.inputFile.empty())
    {
      perror.addError(1, "Option not provided for [/i, /input]");
      perror.setError(1);
      perror.print(options.isOutputColor);
      return perror.getError().value;
    }
  }
  if (argumentParser.optionsExist(OUTPUTFILEOPTIONS))
  {
    options.outputFile = argumentParser.getOptions(OUTPUTFILEOPTIONS);
    if (options.outputFile.empty())
    {
      perror.addError(2, "Option not provided for [/o, /output]");
      perror.setError(2);
      perror.print(options.isOutputColor);
      return perror.getError().value;
    }
    options.isOutputFile = true;
  }
  if (argumentParser.optionsExist(COUNTOPTIONS))
  {
    const std::string& option = argumentParser.getOptions(COUNTOPTIONS);
    if (!std::regex_match(option, options.RGX_UINT))
    {
      perror.addError(3, "Option provided for [/c, /count] is not a valid value.\nPlease provide a positive integer.");
      perror.setError(3);
      perror.print(options.isOutputColor);
      return perror.getError().value;
    }
    const int UPL = std::stoi(option);
    if ((UPL < 2) || (UPL > 9))
    {
      perror.addError(4, "Option provided for [/c, /count] exceeds url limit per line.\nPlease provide a positive integer from 2 - 9.");
      perror.setError(4);
      perror.print(options.isOutputColor);
      return perror.getError().value;
    }
    options.urlsPerLine = UPL;
  }
  return 0;
}
void ParseContent(std::vector<std::string>& inputFileData, Options& options, std::string variableName, std::vector<std::string>& urls, std::regex match, std::regex regexReplace)
{
  int parseDataIndex = 0;
  const bool isUrls000 = (variableName == "urls000");
  const bool isUrls127 = (variableName == "urls127");
  for (std::vector<std::string>::iterator iterator = inputFileData.begin(); iterator != inputFileData.end(); iterator++)
  {
    if (std::regex_match(*iterator, match))
    {
      if ((isUrls000) && (!options.hasUrl000StartingLineIndex))
      {
        options.hasUrl000StartingLineIndex = true;
        options.url000StartingLineIndex = parseDataIndex;
      }
      if ((isUrls127) && (!options.hasUrl127StartingLineIndex))
      {
        options.hasUrl127StartingLineIndex = true;
        options.url127StartingLineIndex = parseDataIndex;
      }
      *iterator = std::regex_replace(*iterator, regexReplace, "");
      urls.push_back(*iterator);
    }
    else
    {
      if (!options.hasFooterAndHeader)
      {
        const bool hasUrlsIndex = (options.hasUrl000StartingLineIndex || options.hasUrl127StartingLineIndex);
        if (!hasUrlsIndex) options.headerContent.push_back(*iterator);
        else options.footerContent.push_back(*iterator);
      }
    }
    parseDataIndex++;
  }
  options.hasFooterAndHeader = true;
}
std::string GetHostsOutput(std::vector<std::string> &compressed, Options &options)
{
  std::string output = std::string("");
  if ((!options.isDiscard) && ((int)options.headerContent.size()))
  {
    for (std::vector<std::string>::const_iterator iterator = options.headerContent.begin(); iterator != options.headerContent.end(); iterator++)
    {
      if (!output.empty()) output.push_back('\n');
      output.append(*iterator);
    }
  }
  for (std::vector<std::string>::const_iterator iterator = compressed.begin(); iterator != compressed.end(); iterator++)
  {
    if (!output.empty()) output.push_back('\n');
    output.append(*iterator);
  }
  if ((!options.isDiscard) && ((int)options.footerContent.size()))
  {
    for (std::vector<std::string>::const_iterator iterator = options.footerContent.begin(); iterator != options.footerContent.end(); iterator++)
    {
      if (!output.empty()) output.push_back('\n');
      output.append(*iterator);
    }
  }
  return output;
}
std::string IncrementString(std::string string, int increment)
{
  std::string incremented = std::string("");
  for (int index = 0; index < increment; index++)
    incremented.append(string);
  return incremented;
}
std::u8string IncrementString(std::u8string string, int increment)
{
  std::u8string incremented = std::u8string(u8"");
  for (int index = 0; index < increment; index++)
    incremented.append(string);
  return incremented;
}
void CompressUrls(Options& options, std::vector<std::string> &urls, std::vector < std::string> &output, std::string pre)
{
  const std::u8string BLOCK = u8"█";
  const std::u8string LBRACKET = u8"〘";
  const std::u8string RBRACKET = u8"〙";
  std::string spaces;
  std::u8string nextCharacter;
  int stepIndex{};
  int progressIndex{};
  if (!options.isQuiet)
  {
    stepIndex = 0;
    progressIndex = 0;
    spaces = IncrementString(" ", 100);
    nextCharacter = IncrementString(BLOCK, 0);
    std::cout << "\x1b[s" << (const char*)LBRACKET.c_str() << (const char*)nextCharacter.c_str() << spaces << (const char*)RBRACKET.c_str() << "0%";
  }
  for (int index = 0; index <= (int)(urls.size() - options.urlsPerLine); index += options.urlsPerLine)
  {
    if (!options.isQuiet) progressIndex = ((index * 100) / (int)(urls.size()));
    std::stringstream ss;
    ss << pre << ' ';
    for (int index2 = 0; index2 < options.urlsPerLine; index2++)
    {
      const int nIndex = (index + index2);
      ss << urls[nIndex];
      if (index2 < (options.urlsPerLine - 1)) ss << ' ';
    }
    output.push_back(ss.str());
    if ((!options.isQuiet) && (progressIndex > stepIndex))
    {
      stepIndex++;
      nextCharacter = IncrementString(u8"█", stepIndex);
      spaces = IncrementString(" ", (100 - stepIndex));
      std::cout << "\x1b[u" << (const char*)LBRACKET.c_str() << (const char*)nextCharacter.c_str() << spaces << (const char*)RBRACKET.c_str() << stepIndex << '%';
    }
  }
  int leftOver = (int)(urls.size() - (output.size() * options.urlsPerLine));
  if (leftOver > 0)
  {
    std::stringstream ss;
    for (int index = (int)(output.size() * options.urlsPerLine); index < (int)urls.size(); index++)
    {
      ss << urls[index];
      if (index < ((int)urls.size() - 1))
      {
        ss << ' ';
      }
    }
    output.push_back(ss.str());
  }
  if (!options.isQuiet)
  {
    stepIndex = 100;
    nextCharacter = IncrementString(u8"█", stepIndex);
    spaces = IncrementString(" ", (100 - stepIndex));
    std::cout << "\x1b[u" << (const char*)LBRACKET.c_str() << (const char*)nextCharacter.c_str() << spaces << (const char*)RBRACKET.c_str() << stepIndex << "%\n";
    std::cout
      <<"Compressed ["
      << (options.isOutputColor ? "\x1b[93m" : "")
      << std::to_string((int)urls.size())
      << (options.isOutputColor ? "\x1b[m" : "")
      << "] urls to ["
      << (options.isOutputColor ? "\x1b[92m" : "")
      << std::to_string((int)urls.size() / options.urlsPerLine)
      << (options.isOutputColor ? "\x1b[m" : "")
      << "] lines...\n";
  }
}
std::vector<std::string> ReadHostsToVector(std::filesystem::path &inputPath, ProgramError &perror)
{
  std::ifstream inputFileStream(inputPath);
  std::vector<std::string> inputFileData = {};
  if (inputFileStream.is_open())
  {
    for (std::string line; std::getline(inputFileStream, line);)
    {
      inputFileData.push_back(line);
    }
    inputFileStream.close();
  }
  else
  {
    std::string message = "Could not open file: ";
    message.append(inputPath.string());
    message.append(" for reading");
    perror.addError(15, message);
    perror.setError(15);
  }
  return inputFileData;
}
void CreateHostsFile(const std::string &OUTPUTDATA, Options& options, ProgramError &perror)
{
  std::ofstream ofs(options.outputFile, std::fstream::trunc);
  if (ofs.is_open())
  {
    ofs << OUTPUTDATA;
    ofs.close();
  }
  else
  {
    std::string message = "Could not open file: ";
    message.append(options.outputFile);
    message.append(" for writing");
    perror.addError(16, message);
    perror.setError(16);
  }
}
void PrintMessage(std::string message, Options& options)
{
  if (!options.isQuiet) std::cout << message;
}
void PrintMessage(std::vector<std::string> strings, Options& options)
{
  if (!options.isQuiet)
  {
    std::string message;
    for (std::vector<std::string>::const_iterator iterator = strings.begin(); iterator != strings.end(); iterator++)
    {
      message.append(*iterator);
    }
    std::cout << message;
  }
}
int main(int argc, const char* argv[])
{
  ProgramError perror;
  Options options;
  CodePage cp;
  Handle handle{};
  ConsoleMode
    inputConsoleMode,
    outputConsoleMode;
  auto errorTest = [options](ProgramError& perror)
  {
    if (perror.getError().value > 0)
    {
      if (!options.isQuiet) perror.print(options.isOutputColor);
      std::exit(perror.getError().value);
    }
  };
  ArgumentParser argumentParser(argc, argv, 1);
  const unsigned int PARSEARGUMENTSRESULT = ParseArguments(argumentParser, options, perror);
  if (PARSEARGUMENTSRESULT > 0)
  {
    return PARSEARGUMENTSRESULT;
  }
  if (options.isHelp) return EXIT_SUCCESS;
  const std::string WINDIR = GetWindowsDirectoryAsString(perror, 5, "Could not retrieve the Windows Directory");
  errorTest(perror);
  if (options.inputFile.empty())
  {
    options.inputFile.append(WINDIR);
    options.inputFile.append("\\System32\\drivers\\etc\\hosts");
  }
  std::filesystem::path inputPath(options.inputFile);
  std::filesystem::path outputPath(options.outputFile);
  std::error_code ec;
  const bool INPUTPATHEXISTS = std::filesystem::exists(inputPath, ec);
  if (ec.value() > 0)
  {
    perror.addError(6, ec.message());
    perror.setError(6);
    perror.print(options.isOutputColor);
    return perror.getError().value;
  }
  if (!INPUTPATHEXISTS)
  {
    std::string message(inputPath.string());
    message.append(" does not exist");
    perror.addError(7, message);
    perror.setError(7);
    perror.print(options.isOutputColor);
    return perror.getError().value;
  }
  cp.setInitCodePage(perror, 8, "Could not get the initial code page.");
  errorTest(perror);
  if (cp.getCurrentCodePage() != CP_UTF8)
  {
    cp.setCodePage(CP_UTF8, perror, 9, "Could not set the current code page.");
    errorTest(perror);
  }
  handle.setInputHandle(perror, 10, "Could not retrieve console input handle.");
  errorTest(perror);

  inputConsoleMode.setInitConsoleMode(handle.getInputHandle(), perror, 11, "");
  errorTest(perror);
  inputConsoleMode.setConsoleMode
  (
    handle.getInputHandle(),
    ENABLE_VIRTUAL_TERMINAL_INPUT | ENABLE_PROCESSED_INPUT,
    perror, 12, ""
  );
  errorTest(perror);
  handle.setOutputHandle(perror, 12, "Could not retrieve console input handle.");
  errorTest(perror);
  outputConsoleMode.setInitConsoleMode(handle.getOutputHandle(), perror, 13, "");
  errorTest(perror);
  outputConsoleMode.setConsoleMode
  (
    handle.getOutputHandle(),
    ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT,
    perror, 14, ""
  );
  errorTest(perror);
  SetConsoleTitle(L"Hosts Compress");
  PrintMessage
  (
    std::vector<std::string>
    {
      "Reading \"",
      std::string(options.isOutputColor ? "\x1b[93m" : ""),
      std::filesystem::absolute(inputPath).string(),
      std::string(options.isOutputColor ? "\x1b[m" : ""),
      "\" content...\n"
    },
    options
  );
  std::vector<std::string> inputFileData =
    ReadHostsToVector(inputPath, perror);
  errorTest(perror);
  std::vector<std::string> urls000;
  std::vector<std::string> urls127;
  std::vector<std::string> compressed;
  PrintMessage
  (
    std::vector<std::string>
    {
      "Compiling urls for [",
        std::string(options.isOutputColor ? "\x1b[93m" : ""),
        "0.0.0.0",
        std::string(options.isOutputColor ? "\x1b[m" : ""),
        "]...\n"
    },
    options
  );
  ParseContent(inputFileData, options, "urls000", urls000, RGX_ISURL_000, RGX_URLS000_REPLACE);
  if ((int)urls000.size() > 0) 
  {
    PrintMessage
    (
      std::vector<std::string>
      {
        "Found urls for [",
          std::string(options.isOutputColor ? "\x1b[92m" : ""),
          "0.0.0.0",
          std::string(options.isOutputColor ? "\x1b[m" : ""),
          "]; Compressing...\n"
      },
      options
    );
    CompressUrls(options, urls000, compressed, "0.0.0.0");
  }
  else
  {
    PrintMessage
    (
      std::vector<std::string>
      {
        "No urls found for [",
          std::string(options.isOutputColor ? "\x1b[91m" : ""),
          "0.0.0.0",
          std::string(options.isOutputColor ? "\x1b[m" : ""),
          "]...\n"
      },
      options
    );
  }
  PrintMessage
  (
    std::vector<std::string>
    {
      "Compiling urls for [",
        std::string(options.isOutputColor ? "\x1b[93m" : ""),
        "127.0.0.1",
        std::string(options.isOutputColor ? "\x1b[m" : ""),
        "]...\n"
    },
    options
  );
  ParseContent(inputFileData, options, "urls127", urls127, RGX_ISURL_127, RGX_URLS127_REPLACE);
  if ((int)urls127.size() > 0)
  {
    PrintMessage
    (
      std::vector<std::string>
      {
        "Found urls for [",
          std::string(options.isOutputColor ? "\x1b[92m" : ""),
          "127.0.0.1",
          std::string(options.isOutputColor ? "\x1b[m" : ""),
          "]; Compressing...\n"
      },
      options
    );
    CompressUrls(options, urls127, compressed, "127.0.0.1");
  }
  else
  {
    PrintMessage
    (
      std::vector<std::string>
      {
        "No urls found for [",
          std::string(options.isOutputColor ? "\x1b[91m" : ""),
          "127.0.0.1",
          std::string(options.isOutputColor ? "\x1b[m" : ""),
          "]...\n"
      },
      options
    );
  }
  const std::string OUTPUTDATA = GetHostsOutput(compressed, options);
  if (!options.isOutputFile)
  {
    std::cout << OUTPUTDATA;
  }
  else
  {
    CreateHostsFile(OUTPUTDATA, options, perror);
    errorTest(perror);
    PrintMessage
    (
      std::vector<std::string>
      {
        "Compressed data has been successfully written to:\n\"",
          std::string(options.isOutputColor ? "\x1b[92m" : ""),
          std::filesystem::absolute(outputPath).string(),
          std::string(options.isOutputColor ? "\x1b[m" : ""),
          "\"...\n"
      },
      options
    );
  }
  if (cp.getCurrentCodePage() != cp.getInitCodePage())
  {
    cp.setCodePage(cp.getInitCodePage(), perror, 17, "Could not set the code page to the initial value.");
    errorTest(perror);
  }
  if (inputConsoleMode.getCurrentConsoleMode() != inputConsoleMode.getInitConsoleMode())
  {
    inputConsoleMode.setConsoleMode(handle.getInputHandle(), inputConsoleMode.getInitConsoleMode(), perror, 18, "Could not set the console mode to the initial value.");
    errorTest(perror);
  }
  if (outputConsoleMode.getCurrentConsoleMode() != outputConsoleMode.getInitConsoleMode())
  {
    outputConsoleMode.setConsoleMode(handle.getOutputHandle(), outputConsoleMode.getInitConsoleMode(), perror, 19, "Could not set the console mode to the initial value.");
    errorTest(perror);
  }
  return EXIT_SUCCESS;
}