
#include "int2048.h"
#include <iostream>
#include <map>
#include <any>
#include <string>
using namespace sjtu;
bool Getbool(std::any object);
int2048 Getint(std::any object);
double Getdouble(std::any object);
std::string Getstring(std::any object);
std::any Getdivdoubleright(std::any a, std::any b);
std::any operator+(std::any a, std::any  b);
std::any operator-(std::any a, std::any  b);
std::any addminussignal(std::any a);

std::any operator*(std::any a, std::any  b);
std::any operator/(std::any a, std::any  b);

std::any operator%(std::any a, std::any  b);

std::any &operator+=(std::any &a, std::any  &b);
std::any &operator-=(std::any &a, std::any  &b);
std::any &operator*=(std::any &a, std::any  &b);
std::any &operator/=(std::any &a, std::any  &b);
std::any &Getdivdouble(std::any &a, std::any  &b);
std::any &operator%=(std::any &a, std::any  &b);
bool operator<(std::any &a, std::any &b);
bool operator>(std::any &a, std::any &b);
bool operator==(std::any &a, std::any &b);
bool operator>=(std::any &a, std::any &b);
bool operator<=(std::any &a, std::any &b);
bool operator!=(std::any &a, std::any &b);

void print(std::any const &a);
std::string operator*(std::string str, int times);