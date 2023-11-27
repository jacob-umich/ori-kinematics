#include <string>
#include <map>
#include <vector>
#include <memory>
#include <fstream>
/* implementation from 
https://kishoreganesh.com/post/writing-a-json-parser-in-cplusplus/*/

class JSONNode;
using JSONObject = std::map<std::string, std::shared_ptr<JSONNode>>;
using JSONList = std::vector<std::shared_ptr<JSONNode>>;

class JSONNode {
    enum class Type { OBJECT, LIST, STRING, NUMBER, BOOLEAN, NULL_TYPE};
    union Values{
    
        JSONObject *object;
        JSONList *list;
        std::string *s;
        double dValue;
        bool bValue;
    } values;
    Type type;
    public:

        auto returnObject();
        auto returnList();
        auto returnString();
        auto returnNumber();
        auto returnBool();
        auto setObject(JSONObject *);
        auto setList(JSONList*);
        auto setString(std::string*);
        auto setNumber(double);
        auto setBool(bool);
        void setNull();
    
};

enum class TOKEN
{
    CURLY_OPEN,
    CURLY_CLOSE,
    COLON,
    STRING,
    NUMBER,
    ARRAY_OPEN,
    ARRAY_CLOSE,
    COMMA,
    BOOLEAN,
    NULL_TYPE
};

struct Token
{
    std::string value;
    TOKEN type;
    std::string toString(){
        switch (type)
        {
        case TOKEN::CURLY_OPEN:
        {
            return "Curly open";
        }
        case TOKEN::CURLY_CLOSE:
        {
            return "Curly close";
        }
        case TOKEN::COLON:
        {
            return "COLON";
        }
        case TOKEN::NUMBER:
        {
            return "Number: " + value;
        }
        case TOKEN::STRING:
        {
            return "String: " + value;
        }

        case TOKEN::ARRAY_OPEN:
        {
            return "Array open";
        }
        case TOKEN::ARRAY_CLOSE:
        {
            return "Array close";
        }
        case TOKEN::COMMA:
        {
            return "Comma";
        }
        case TOKEN::BOOLEAN:
        {
            return "Boolean: " + value;;
        }

        case TOKEN::NULL_TYPE:
        {
            return "Null";
        }
        default:{
            return "foo";
        }
               
        }
    }
};

class Tokenizer {
    std::fstream file;
    std::streampos prevPos;
    std::streampos currentPos;

public:
    Tokenizer(std::string fileName);
    auto getWithoutWhiteSpace();
    auto getToken();
    auto hasMoreTokens();
    void rollBackToken();
};

class JSONParser {
  std::shared_ptr<JSONNode> root;
  std::unique_ptr<JSONNode> current;
  Tokenizer tokenizer;

public:
  JSONParser(const std::string filename) : tokenizer(filename) {}

  void parse();
  std::shared_ptr<JSONNode> parseObject();
  std::shared_ptr<JSONNode> parseString();
  std::shared_ptr<JSONNode> parseNumber();
  std::shared_ptr<JSONNode> parseList();
  std::shared_ptr<JSONNode> parseBoolean();
  std::shared_ptr<JSONNode> parseNull();
};