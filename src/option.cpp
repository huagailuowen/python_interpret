#include "option.h"
#include "int2048.h"
#include <any>
#include <string>
// #include <any>
using namespace sjtu;
std::any Getdivdoubleright(std::any a, std::any b) {
  return Getdouble(a) / Getdouble(b);
}
bool Getbool(std::any object) {
  bool *pbool;
  int2048 *pint;
  std::string *pstring;
  double *pdouble;
  if ((pbool = std::any_cast<bool>(&object)) != nullptr) {
    return *pbool;
  } else if ((pint = std::any_cast<int2048>(&object)) != nullptr) {
    return !((*pint).f == 0);
  } else if ((pstring = std::any_cast<std::string>(&object)) != nullptr) {
    return !((*pstring) == "");
  } else if ((pdouble = std::any_cast<double>(&object)) != nullptr) {
    return !((*pdouble) == 0);
  } else {
    return false;
  }
}
std::any addminussignal(std::any a)
{
  int2048 *pint=std::any_cast<int2048>(&a);
  double *pdouble=std::any_cast<double>(&a);
  if(pint){
    return -(*pint);
  }
  if(pdouble){
    return -(*pdouble);
  }
  return a;
}
int2048 Getint(std::any object) {
  bool *pbool;
  int2048 *pint;
  std::string *pstring;
  double *pdouble;
  if ((pbool = std::any_cast<bool>(&object)) != nullptr) {
    if (*pbool == false)
      return int2048(0);
    else
      return int2048(1);
  } else if ((pint = std::any_cast<int2048>(&object)) != nullptr) {
    return *pint;
  } else if ((pstring = std::any_cast<std::string>(&object)) != nullptr) {
    return int2048(*pstring);
  } else if ((pdouble = std::any_cast<double>(&object)) != nullptr) {
    return int2048(int(*pdouble));
  } else {
    return int2048(0);
  }
}
double Getdouble(std::any object) {
  bool *pbool;
  int2048 *pint;
  std::string *pstring;
  double *pdouble;
  if ((pbool = std::any_cast<bool>(&object)) != nullptr) {
    if (*pbool == false)
      return 0.0;
    else
      return 1.0;
  } else if ((pint = std::any_cast<int2048>(&object)) != nullptr) {
    return (double)getint(*pint);
  } else if ((pstring = std::any_cast<std::string>(&object)) != nullptr) {
    return std::stod(*pstring);
  } else if ((pdouble = std::any_cast<double>(&object)) != nullptr) {
    return *pdouble;
  } else {
    return 0.0;
  }
}
std::string Getstring(std::any object) {
  bool *pbool;
  int2048 *pint;
  std::string *pstring;
  double *pdouble;
  if ((pbool = std::any_cast<bool>(&object)) != nullptr) {
    if (*pbool == false)
      return "False";
    else
      return "True";
  } else if ((pint = std::any_cast<int2048>(&object)) != nullptr) {
    return std::to_string(getint(*pint));
  } else if ((pstring = std::any_cast<std::string>(&object)) != nullptr) {
    return *pstring;
  } else if ((pdouble = std::any_cast<double>(&object)) != nullptr) {
    return std::to_string(*pdouble);
  } else {
    return "";
  }
}

bool operator<(std::any &a, std::any &b) {
  int2048 *pinta, *pintb;
  std::string *pstringa, *pstringb;
  double *pdoublea, *pdoubleb;
  bool *pboola, *pboolb;
  if ((pstringa = std::any_cast<std::string>(&a)) ||
      (pstringb = std::any_cast<std::string>(&b))) {
    return Getstring(a) < Getstring(b);
  } else if ((pdoublea = std::any_cast<double>(&a)) ||
             (pdoubleb = std::any_cast<double>(&b))) {
    return Getdouble(a) < Getdouble(b);
  } else if ((pinta = std::any_cast<int2048>(&a)) ||
             (pintb = std::any_cast<int2048>(&b))) {
    return Getint(a) < Getint(b);
  }
  if ((pboola = std::any_cast<bool>(&a)) ||
      (pboolb = std::any_cast<bool>(&b))) {
    return Getbool(a) < Getbool(b);
  } else {
    return 0;
  }
}
bool operator>(std::any &a, std::any &b) { return b < a; }
bool operator==(std::any &a, std::any &b) {
  if (!a.has_value()) {
    return b.has_value() ? false : true;
  }
  if (!b.has_value()) {
    return a.has_value() ? false : true;
  }
  int2048 *pinta, *pintb;
  std::string *pstringa, *pstringb;
  double *pdoublea, *pdoubleb;
  bool *pboola, *pboolb;
  if ((pstringa = std::any_cast<std::string>(&a)) ||
      (pstringb = std::any_cast<std::string>(&b))) {
    return Getstring(a) < Getstring(b);
  } else if ((pdoublea = std::any_cast<double>(&a)) ||
             (pdoubleb = std::any_cast<double>(&b))) {
    return Getdouble(a) < Getdouble(b);
  } else if ((pinta = std::any_cast<int2048>(&a)) ||
             (pintb = std::any_cast<int2048>(&b))) {
    return Getint(a) < Getint(b);
  }
  if ((pboola = std::any_cast<bool>(&a)) ||
      (pboolb = std::any_cast<bool>(&b))) {
    return Getbool(a) < Getbool(b);
  } else {
    return 0;
  }
}
bool operator>=(std::any &a, std::any  &b) { return !(a < b); }
bool operator<=(std::any &a, std::any  &b) { return !(a > b); }
bool operator!=(std::any &a, std::any  &b) { return !(a == b); }

std::any operator+(std::any a, std::any b) {
  int2048 *pinta, *pintb;
  std::string *pstringa, *pstringb;
  double *pdoublea, *pdoubleb;
  if ((pstringa = std::any_cast<std::string>(&a)) ||
      (pstringb = std::any_cast<std::string>(&b))) {
    return Getstring(a) + Getstring(b);
  } else if ((pdoublea = std::any_cast<double>(&a)) ||
             (pdoubleb = std::any_cast<double>(&b))) {
    return Getdouble(a) + Getdouble(b);
  } else if ((pinta = std::any_cast<int2048>(&a)) ||
             (pintb = std::any_cast<int2048>(&b))) {
    return Getint(a) + Getint(b);
  } else {
    return 0;
  }
}
std::any operator-(std::any a, std::any b) {
  int2048 *pinta, *pintb;
  double *pdoublea, *pdoubleb;
  if ((pdoublea = std::any_cast<double>(&a)) ||
      (pdoubleb = std::any_cast<double>(&b))) {
    return Getdouble(a) - Getdouble(b);
  } else if ((pinta = std::any_cast<int2048>(&a)) ||
             (pintb = std::any_cast<int2048>(&b))) {
    return Getint(a) - Getint(b);
  } else {
    return 0;
  }
}
std::any operator*(std::any a, std::any b) {
  int2048 *pinta, *pintb;
  std::string *pstringa, *pstringb;
  double *pdoublea, *pdoubleb;
  if ((pdoublea = std::any_cast<double>(&a)) ||
      (pdoubleb = std::any_cast<double>(&b))) {
    return Getdouble(a) * Getdouble(b);
  } else if ((pstringa = std::any_cast<std::string>(&a)) ||
             (pstringb = std::any_cast<std::string>(&b))) {
    if ((pinta = std::any_cast<int2048>(&a))) {
      int times = getint(*pinta);
      pstringb = std::any_cast<std::string>(&b);
      std::string s = *pstringb, ans = "";
      while (times--) {
        ans += s;
      }
      return ans;
    } else {
      int times = getint(*pintb);
      pstringa = std::any_cast<std::string>(&a);
      std::string s = *pstringa, ans = "";
      while (times--) {
        ans += s;
      }
      return ans;
    }
  } else if ((pinta = std::any_cast<int2048>(&a)) ||
             (pintb = std::any_cast<int2048>(&b))) {
    return Getint(a) * Getint(b);
  } else {
    return 0;
  }
}
std::any operator/(std::any a, std::any b) {
  int2048 *pinta, *pintb;
  double *pdoublea, *pdoubleb;
  if ((pdoublea = std::any_cast<double>(&a)) ||
      (pdoubleb = std::any_cast<double>(&b))) {
    double ta = Getdouble(a), tb = Getdouble(b);
    return int(ta / tb);
  } else if ((pinta = std::any_cast<int2048>(&a)) ||
             (pintb = std::any_cast<int2048>(&b))) {
    return Getint(a) / Getint(b);
  } else {
    return 0;
  }
}

std::any operator%(std::any a, std::any b) { return Getint(a) % Getint(b); }
std::any &operator+=(std::any &a, std::any  &b) { return a = a + b; }
std::any &operator-=(std::any &a, std::any  &b) { return a = a - b; }
std::any &operator*=(std::any &a, std::any  &b) { return a = a * b; }
std::any &operator/=(std::any &a, std::any  &b) { return a = a / b; }
std::any &Getdivdouble(std::any &a, std::any  &b) {
  return a = Getdivdoubleright(a, b);
}
std::any &operator%=(std::any &a, std::any  &b) { return a = a % b; }

void print(std::any const &a) { std::cout << Getstring(a); }

std::string operator*(std::string str, int times);