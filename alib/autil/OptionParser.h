#ifndef AUTIL_OPTIONPARSER_H
#define AUTIL_OPTIONPARSER_H

#include "autil/CommonMacros.h"
#include <string>
#include <vector>
#include <map>

AUTIL_BEGIN_NAMESPACE(autil);

//Note! this is not multi-thread safe!
class OptionParser
{
public:
    enum OptionType {
        OPT_STRING = 0,
        OPT_INT32,
        OPT_UINT32,
        OPT_BOOL,
        OPT_HELP,
    };
    enum OptionAction {
        STORE,
        STORE_TRUE,
        STORE_FALSE,
    };
public:
    typedef std::map<std::string, std::string> StrOptMap;
    typedef std::map<std::string, bool> BoolOptMap;
    typedef std::map<std::string, int32_t> IntOptMap;

public:
    OptionParser(const std::string &usageDescription = "");
    ~OptionParser();

    void addOption(const std::string &shortOpt, const std::string &longOpt, 
                   const std::string& optName, 
                   const OptionType type = OPT_STRING, bool isRequired = false);

    void addOption(const std::string &shortOpt, const std::string &longOpt, 
                   const std::string& optName, 
                   const OptionAction &action,
                   const OptionType type = OPT_BOOL, bool isRequired = false);

    //the following functions are for optional options
    void addOption(const std::string &shortOpt, const std::string &longOpt, 
                   const std::string& optName, 
                   const char* defaultValue); 

    void addOption(const std::string &shortOpt, const std::string &longOpt, 
                   const std::string& optName, 
                   const std::string& defaultValue); 

    void addOption(const std::string &shortOpt, const std::string &longOpt, 
                   const std::string& optName, 
                   const uint32_t defaultValue);

    void addOption(const std::string &shortOpt, const std::string &longOpt, 
                   const std::string& optName, 
                   const int32_t defaultValue);

    void addOption(const std::string &shortOpt, const std::string &longOpt, 
                   const std::string& optName, 
                   const bool defaultValue);


    bool parseArgs(int argc, char **argv);
    bool parseArgs(const std::string &commandString);
    bool getOptionValue(const std::string &optName, std::string &value) const;
    bool getOptionValue(const std::string &optName, bool &value) const;
    bool getOptionValue(const std::string &optName, int32_t &value) const;
    bool getOptionValue(const std::string &optName, uint32_t &value) const;
    StrOptMap getOptionValues() const;

private:
    class OptionInfo{
    public:
        OptionInfo(const std::string &optionName, OptionType type, 
                   OptionAction action, bool required,
                   const std::string &defaultValue = "") 
            : optionType(type), optionAction(action), 
              optionName(optionName), isRequired(required)
        {
            isSet = false;
        }
        ~OptionInfo() {}
        bool hasValue() {return optionAction == STORE;}
    public:
        OptionType optionType;
        OptionAction optionAction;
        std::string optionName;
        bool isRequired;
        bool isSet;
    };

private:
    void addOptionInfo(const OptionInfo &optionInfo, 
                       const std::string& shortOpt, const std::string& longOpt);

    // results:
private:
    StrOptMap _strOptMap;
    IntOptMap _intOptMap;
    BoolOptMap _boolOptMap;
    std::string _usageDescription;

    // infos:
private:
    typedef std::map<std::string, size_t> ShortOpt2InfoMap;
    typedef std::map<std::string, size_t> LongOpt2InfoMap;
    typedef std::vector<OptionInfo> OptionInfos;
    ShortOpt2InfoMap _shortOpt2InfoMap;
    LongOpt2InfoMap _longOpt2InfoMap;
    OptionInfos _optionInfos;
private:
    void init();
    bool isValidArgs();
private:
    friend class OptionParserTest;
};


AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_OPTIONPARSER_H
