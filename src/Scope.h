#include "Evalvisitor.h"
#include "Python3Parser.h"
#include "Python3ParserBaseVisitor.h"
#include "int2048.h"
#include "option.h"
#include <any>
#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
typedef std::pair<std::string, std::any> variety;
// const int NO=0,ADD=1,SUB=2,MUL=3,DIV=4,DIVdouble=5,MOD=6,EQAL=7;
struct Flowcontrol {
  enum class controltype { break_, return_, continue_ };
  controltype controtype;
  std::any result;
  Flowcontrol(controltype a, std::any b) {
    controtype = a;
    result = b;
  }
};
namespace Scope {

std::map<std::string, std::any>::iterator find(std::any object);

}
struct function {
  std::any point;
  std::string s;
  std::vector<variety> varylist;
  function() {}
  function(int x) { point = int2048(x); }
};

std::any MYprint(std::any list);

std::any usefunction(function f, Python3Parser::ArglistContext *para);

void Changevalue(std::any &L, std::any R);

std::any Getvalue(std::any object);
