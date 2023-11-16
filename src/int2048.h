#pragma once


// Integer 1:
// 实现一个有符号的大整数类，只需支持简单的加减
// Integer 2:
// 实现一个有符号的大整数类，支持加减乘除，并重载相关运算符

// 请不要使用除了以下头文件之外的其它头文件
#include <complex>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

// 请不要使用 using namespace std;

namespace sjtu {
const int MAXN = 1000000;
const int Digital = 10000;
const int Digital_length = 4;
const int Pow10[5] = {1, 10, 100, 1000, 10000};
class int2048 {
  // todo
public:
  int f = 0;
  std::vector<int> num;
  // 构造函数
  int2048();
  int2048(long long);
  int2048(const std::string &);
  int2048(const int2048 &);

  // 以下给定函数的形式参数类型仅供参考，可自行选择使用常量引用或者不使用引用
  // 如果需要，可以自行增加其他所需的函数
  // ===================================
  // Integer1
  // ===================================
  bool Absbigger(const int2048 &a, const int2048 &b);
  // 读入一个大整数
  void read(const std::string &);
  // 输出储存的大整数，无需换行
  void print() const;
  int getint() const;
  // 加上一个大整数
  int2048 &add(int2048);
  // 返回两个大整数之和
  friend int2048 add(int2048, const int2048 &);

  // 减去一个大整数
  int2048 &minus(int2048);
  // 返回两个大整数之差
  friend int2048 minus(int2048, const int2048 &);

  // ===================================
  // Integer2
  // ===================================

  int2048 operator+() const;
  int2048 operator-() const;

  int2048 &operator=(const int2048 &b);

  int2048 &operator+=(const int2048 &);
  friend int2048 operator+(int2048, const int2048 &);

  int2048 &operator-=(const int2048 &);
  friend int2048 operator-(int2048, const int2048 &);

  int2048 &operator*=(const int2048);
  friend int2048 operator*(int2048, const int2048);

  int2048 &operator/=(const int2048);
  friend int2048 operator/(int2048, const int2048);

  int2048 &operator%=(const int2048);
  friend int2048 operator%(int2048, const int2048);

  // friend std::vector<int> NTT(int n, std::vector<int> a, int f);

  friend std::istream &operator>>(std::istream &, int2048 &);
  friend std::ostream &operator<<(std::ostream &, const int2048 &);

  friend bool operator==(const int2048 &, const int2048 &);
  friend bool operator!=(const int2048 &, const int2048 &);
  friend bool operator<(const int2048 &, const int2048 &);
  friend bool operator>(const int2048 &, const int2048 &);
  friend bool operator<=(const int2048 &, const int2048 &);
  friend bool operator>=(const int2048 &, const int2048 &);
};
int getint(int2048 t);

} // namespace sjtu



