#include "jsonParser.hpp"
#include <string>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <iomanip>

/* implementation from 
https://kishoreganesh.com/post/writing-a-json-parser-in-cplusplus/*/

bool to_bool(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    std::istringstream is(str);
    bool b;
    is >> std::boolalpha >> b;
    return b;
}

JSONObject JSONNode::returnObject(){
    if(type==Type::OBJECT){
        return *values.object;
    }
    throw std::logic_error("Improper return");
}
JSONObject* JSONNode::returnObjectPtr(){
    if(type==Type::OBJECT){
        return values.object;
    }
    throw std::logic_error("Improper return");
}
void JSONNode::setObject(JSONObject *obj){
    this->values.object = obj;
    type=Type::OBJECT;
}


JSONList JSONNode::returnList(){
    if(type==Type::LIST){
        return *values.list;
    }
    throw std::logic_error("Improper return");
}
JSONList *JSONNode::returnListPtr(){
    if(type==Type::LIST){
        return values.list;
    }
    throw std::logic_error("Improper return");
}
void JSONNode::setList(JSONList *list){
    this->values.list = list;
    type=Type::LIST;
}


std::string JSONNode::returnString(){
    if(type==Type::STRING){
        return *values.s;
    }
    throw std::logic_error("Improper return");
}
std::string* JSONNode::returnStringPtr(){
    if(type==Type::STRING){
        return values.s;
    }
    throw std::logic_error("Improper return");
}
void JSONNode::setString(std::string *s){
    this->values.s = s;
    type=Type::STRING;
}


double JSONNode::returnNumber(){
    if(type==Type::NUMBER){
        return values.dValue;
    }
    throw std::logic_error("Improper return");
}
int JSONNode::returnInt(){
    if(type==Type::NUMBER){
        return int(values.dValue);
    }
    throw std::logic_error("Improper return");
}
void JSONNode::setNumber(double d){
    this->values.dValue = d;
    type=Type::NUMBER;
}


bool JSONNode::returnBool(){
    if(type==Type::BOOLEAN){
        return values.bValue;
    }
    throw std::logic_error("Improper return");
}
void JSONNode::setBool(bool b){
    this->values.bValue = b;
    type=Type::BOOLEAN;
}
void JSONNode::setNull(){
    type = Type::NULL_TYPE;
}


std::string JSONNode::toString(int indent, bool followingObj, int level){
  std::string spaceString = std::string((indent*level), ' ');
  std::string spaceStringOld = std::string((indent*(level-1)), ' ');
    //sstreams
  std::string outputString = "";
  switch (type) {
  case Type::STRING: {
    outputString += "\""+*values.s+"\"";
    break;
  }
  case Type::NUMBER: {
    std::stringstream stream;
    stream<<std::fixed<<std::setprecision(3)<<values.dValue;
    outputString += stream.str();
    break;
  }
  case Type::BOOLEAN: {
    outputString +=  (values.bValue ? "true" : "false");
    break;
  }
  case Type::NULL_TYPE: {
    outputString +=  "null";
    break;

  }
  case Type::LIST: {
    std::string contStr ;
    bool followObj=false ;
    if((*values.list)[0]->type==Type::STRING||(*values.list)[0]->type==Type::BOOLEAN||(*values.list)[0]->type==Type::NUMBER){
      contStr="";
      followObj=true;
    }else{
      contStr="\n";
      followObj=false;

    }
    
    outputString += (followingObj? "":spaceStringOld)+ "["+contStr;
    size_t index = 0;
    for (auto node : (*values.list)) {
      outputString += node->toString(4,followObj,level+1);
      if(node->type==Type::STRING||node->type==Type::BOOLEAN||node->type==Type::NUMBER){
        contStr = ",";
      } else{
        contStr = ",\n";
      }
      if (index < (*values.list).size() - 1) {
        outputString += contStr;
      }
      index++;
    }\
    if(contStr==","){
      contStr = "";
    } else{
      contStr = "\n"+ spaceStringOld;
    }
    outputString += contStr + "]";
    break;
  }
  case Type::OBJECT: {
    outputString += (followingObj? "":spaceStringOld)+ "{\n";

    for (JSONObject::iterator i = (*values.object).begin();
          i != (*values.object).end(); i++) {
      outputString += spaceString + "\"" + i->first + "\"" + ":";
      outputString += i->second->toString(4,true,level+1);
      JSONObject::iterator next = i;
      next++;
      if ((next) != (*values.object).end()) {
        outputString += ",\n";
      }else{
        outputString +=  "\n";
      }
    }
    outputString += spaceStringOld + "}";
    break;
  }
  default:{
    break;
  }
  
  }
  return outputString;
}




Tokenizer::Tokenizer(std::string filename){
    file.open(filename, std::ios::in|std::ios::binary);
    currentPos=file.tellg();
}

auto Tokenizer::hasMoreTokens(){
    return !file.eof();
}

void Tokenizer::rollBackToken()
    {
        if (file.eof())
        {
            file.clear();
        }
        file.seekg(prevPos);
    }

auto Tokenizer::getWithoutWhiteSpace(){
    char c = ' ';
    while ((c == ' ' || c == '\n'|| c=='\r'))
    {
        currentPos=file.tellg();
        file.get(c); // check
        currentPos=file.tellg();

        if ((c == ' ' || c == '\n'|| c == '\r') && !file.good())
        {
            // std::cout << file.eof() << " " << file.fail() << std::endl;
            throw std::logic_error("Ran out of tokens");
        }
        else if (!file.good())
        {
            return c;
        }
    }
    
    return c;
}

auto Tokenizer::getToken()
    {
        // string buf;
        char c;
        prevPos = file.tellg();
        c = getWithoutWhiteSpace();

        Token token;
        if (c == '"')
        {
            token.type = TOKEN::STRING;
            token.value = "";
            file.get(c);
            while (c != '"')
            {
                token.value += c;
                file.get(c);
            }
        }
        else if (c == '{')
        {
            token.type = TOKEN::CURLY_OPEN;
        }
        else if (c == '}')
        {
            token.type = TOKEN::CURLY_CLOSE;
        }
        else if (c=='-' || (c >= '0' && c <= '9'))
        {
            //Check if string is numeric
            token.type = TOKEN::NUMBER;
            token.value = "";
            token.value += c;
            std::streampos prevCharPos = file.tellg();
            while ((c=='-')||(c >= '0' && c <= '9') || c == '.')
            {
                prevCharPos = file.tellg();
                file.get(c);

                if (file.eof())
                {
                    break;
                }
                else
                {
                    if ((c=='-')||(c >= '0' && c <= '9')||(c=='.'))
                    {
                        token.value += c;
                    }
                    else
                    {
                        file.seekg(prevCharPos);
                    }
                }
            }
        }
        else if(c=='f'){
            token.type = TOKEN::BOOLEAN;
            token.value = "False";
            file.seekg(4, std::ios_base::cur);
        }
        else if(c=='t'){
            token.type = TOKEN::BOOLEAN;
            token.value = "True";
            file.seekg(3, std::ios_base::cur);
        }
        else if(c=='n'){
            token.type = TOKEN::NULL_TYPE;
            file.seekg(3, std::ios_base::cur);
        }
        else if (c == '[')
        {
            token.type = TOKEN::ARRAY_OPEN;
        }
        else if (c == ']')
        {
            token.type = TOKEN::ARRAY_CLOSE;
        }
        else if (c == ':')
        {
            token.type = TOKEN::COLON;
        }
        else if (c == ',')
        {
            token.type = TOKEN::COMMA;
        }
        return token;
    }




void JSONParser::parse() {
    std::string key = "";
    while (tokenizer.hasMoreTokens()) {
      Token token;
      try {
        token = tokenizer.getToken();
        switch (token.type) {
        case TOKEN::CURLY_OPEN: {
          std::shared_ptr<JSONNode> parsedObject = parseObject();
        //   parsedObject->printNode(0);
          if (!root) {
            root = parsedObject;
          }
          break;
        }
        case TOKEN::ARRAY_OPEN: {
          std::shared_ptr<JSONNode> parsedList = parseList();
          //parsedList->printNode(0);
          if (!root) {
            root = parsedList;
          }
          break;
        }

        case TOKEN::STRING: {
          tokenizer.rollBackToken();
          std::shared_ptr<JSONNode> parsedString = parseString();
          //parsedString->printNode(0);
          if (!root) {
            root = parsedString;
          }
          break;
        }
        case TOKEN::NUMBER: {
          tokenizer.rollBackToken();
          std::shared_ptr<JSONNode> parsedNumber = parseNumber();
          //parsedNumber->printNode(0);
          if (!root) {
            root = parsedNumber;
          }
          break;
        }
        
        case TOKEN::BOOLEAN: {
            tokenizer.rollBackToken();
            std::shared_ptr<JSONNode> parsedBoolean = parseBoolean();
            //parsedBoolean->printNode(0);
            break;
        }
        default:{
            break;
        }
        }
      }

      catch (std::logic_error& err2) {
        break;
      }
    }
    // assert token not valid
  }

std::shared_ptr<JSONNode> JSONParser::parseList() {
    std::shared_ptr<JSONNode> node = std::make_shared<JSONNode>();
    JSONList *list =  new JSONList();
    bool hasCompleted = false;
    while (!hasCompleted) {
      if (!tokenizer.hasMoreTokens()) {
        throw std::logic_error("No more tokens");
      } else {
        Token nextToken = tokenizer.getToken();
        std::shared_ptr<JSONNode> listNode;
        switch (nextToken.type) {
        case TOKEN::ARRAY_OPEN: {
          listNode = parseList();
          break;
        }
        case TOKEN::CURLY_OPEN: {
          listNode = parseObject();
          break;
        }
        case TOKEN::STRING: {
          tokenizer.rollBackToken();
          listNode = parseString();
          break;
        }
        case TOKEN::NUMBER: {
          tokenizer.rollBackToken();
          listNode = parseNumber();
          break;
        }
        case TOKEN::BOOLEAN: {
            tokenizer.rollBackToken();
            listNode = parseBoolean();
            //parsedBoolean->printlistNode(0);
            break;

        }
        case TOKEN::NULL_TYPE: {
            listNode = parseNull();
            break;
        }
        default:{
            break;
        }
        }
        list->push_back(listNode);
        nextToken = tokenizer.getToken();
        if (nextToken.type == TOKEN::ARRAY_CLOSE) {
          hasCompleted = true;
        }
      }
    }
    node->setList(list);
    return node;
  }

std::shared_ptr<JSONNode> JSONParser::parseObject(){
    std::shared_ptr<JSONNode> node = std::make_shared<JSONNode>();
    JSONObject *keyObjectMap = new JSONObject();
    bool hasCompleted = false;
    while (!hasCompleted) {
      if (tokenizer.hasMoreTokens()) {
        Token nextToken = tokenizer.getToken();
        std::string key = nextToken.value;
        tokenizer.getToken();
        nextToken = tokenizer.getToken();
        // std::shared_ptr<JSONNode> node;
        //std::cout << nextToken.toString() << std::endl;
        switch (nextToken.type) {
        case TOKEN::STRING: {
          tokenizer.rollBackToken();
          (*keyObjectMap)[key] = parseString();
          break;
        }
        case TOKEN::ARRAY_OPEN: {
          (*keyObjectMap)[key] = parseList();
          break;
        }
        case TOKEN::NUMBER: {
          tokenizer.rollBackToken();
          (*keyObjectMap)[key] = parseNumber();
          break;
        }
        case TOKEN::CURLY_OPEN: {
          (*keyObjectMap)[key] = parseObject();
          break;
        }
        case TOKEN::BOOLEAN: {
            tokenizer.rollBackToken();
            (*keyObjectMap)[key] = parseBoolean();
            //parsedBoolean->printNode(0);
            break;

        }
        case TOKEN::NULL_TYPE: {
            (*keyObjectMap)[key] = parseNull();
            break;
        }
        default:{
            break;
        }
        }
        nextToken = tokenizer.getToken();
        if (nextToken.type == TOKEN::CURLY_CLOSE) {
          hasCompleted = true;
          break;
        }
      } else {
        throw std::logic_error("No more tokens");
      }
    }
    node->setObject(keyObjectMap);
    return node;
  }

std::shared_ptr<JSONNode> JSONParser::parseString() {
    std::shared_ptr<JSONNode> node = std::make_shared<JSONNode>();
    Token token = tokenizer.getToken();
    std::string *sValue = new std::string(token.value);
    node->setString(sValue);
    return node;
}

std::shared_ptr<JSONNode> JSONParser::parseNumber() {
    std::shared_ptr<JSONNode> node = std::make_shared<JSONNode>();
    Token token = tokenizer.getToken();
    double dValue = stod(token.value);
    node->setNumber(dValue);
    return node;
}

std::shared_ptr<JSONNode> JSONParser::parseBoolean() {
    std::shared_ptr<JSONNode> node = std::make_shared<JSONNode>();
    Token token = tokenizer.getToken();
    bool bValue = to_bool(token.value);
    node->setBool(bValue);
    return node;
}
std::shared_ptr<JSONNode> JSONParser::parseNull() {
    std::shared_ptr<JSONNode> node = std::make_shared<JSONNode>();
    Token token = tokenizer.getToken();
    node->setNull();
    return node;
}