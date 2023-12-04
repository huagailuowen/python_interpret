#include "int2048.h"
namespace sjtu {
int getint(const int2048 &t) {
  int n = 0;
  for (int i = t.num.size() - 1; i >= 0; i--) {
    n = n * Digital + t.num[i];
  }
  n *= t.f;
  return n;
}
long long getlong(const int2048 &t) {
  long long n = 0;
  for (int i = t.num.size() - 1; i >= 0; i--) {
    n = n * Digital + t.num[i];
  }
  n *= t.f;
  return n;
}
void _add(std::vector<int> &a, std::vector<int> &b) {
  a.resize(std::max(a.size(), b.size()) + 1);
  for (int i = 0; i < a.size() - 1; i++) {
    if (i < b.size())
      a[i] += b[i];
    a[i + 1] += a[i] / Digital;
    a[i] %= Digital;
    // a[i]=(a[i]+Digital)%Digital;
  }
  while (a.size() > 1 && a[a.size() - 1] == 0)
    a.pop_back();
  return;
}
void _minus(std::vector<int> &a, std::vector<int> &b) {
  for (int i = 0; i < a.size(); i++) {
    if (i < b.size())
      a[i] -= b[i];
    if (a[i] < 0)
      a[i + 1]--;
    a[i] = (a[i] + Digital) % Digital;
  }
  while (a.size() > 1 && a[a.size() - 1] == 0)
    a.pop_back();
  return;
}

int2048::int2048() {
  f = 0;
  num.resize(1);
  num[0] = 0;
}

int2048::int2048(long long n) {
  f = 1;
  num.resize(0);
  if (n < 0) {
    n = -n;
    f = -1;
  }
  while (n > 0) {
    num.push_back(n % Digital);
    n /= Digital;
  }
  if (num.size() == 0) {
    f = 0;
    num.push_back(0);
  };
}
int2048::int2048(const std::string &str) {
  f = 1;
  int len = str.length();
  num.resize((len - 1) / Digital_length + 1);
  for (int i = 0; i < num.size(); i++)
    num[i] = 0;
  if (str[0] == '-') {
    f = -1;
    num.resize((len - 2) / Digital_length + 1);
  }
  for (int i = len - 1; i >= 0; i--) {
    if (str[i] == '-')
      break;
    num[(len - i - 1) / 4] += Pow10[(len - i - 1) % 4] * (str[i] - '0');
  }
  while (num.size() > 1 && num[num.size() - 1] == 0)
    num.pop_back();
  if (num[num.size() - 1] == 0)
    f = 0;
}
int2048::int2048(const int2048 &n) {
  f = n.f;
  num = n.num;
}
void int2048::read(const std::string &str) {
  f = 1;

  int len = str.length();
  num.resize(len / Digital_length + 1);
  for (int i = 0; i < num.size(); i++)
    num[i] = 0;
  if (str[0] == '+')
    exit(-10);
  if (str[0] == '-') {
    f = -1;
    num.resize((len - 1) / Digital_length + 1);
  }
  for (int i = len - 1; i >= 0; i--) {
    if (str[i] == '-')
      break;
    num[(len - i - 1) / 4] += Pow10[(len - i - 1) % 4] * (str[i] - '0');
  }
  while (num.size() > 1 && num[num.size() - 1] == 0)
    num.pop_back();
  if (num[num.size() - 1] == 0)
    f = 0;
}
void int2048::print() const {
  if (f == 0) {
    std::cout << '0';
    return;
  }
  if (f == -1) {
    std::cout << '-';
  }
  for (int i = num.size() - 1; i >= 0; i--) {
    if (i != num.size() - 1) {
      // if(num[i]<Pow10[0])std::cout<<'0';
      if (num[i] < Pow10[1])
        std::cout << '0';
      if (num[i] < Pow10[2])
        std::cout << '0';
      if (num[i] < Pow10[3])
        std::cout << '0';
    }
    std::cout << num[i];
  }
}

int2048 &int2048::add(int2048 b) {
  if (f == 0) {
    *this = b;
    return *this;
  }
  if (b.f == 0) {
    return *this;
  }
  if (f == b.f) {
    _add(num, b.num);
  } else {
    if (Absbigger(*this, b)) {
      f = b.f;
      swap(num, b.num);
    }
    _minus(num, b.num);
  }
  while (num.size() > 1 && num[num.size() - 1] == 0)
    num.pop_back();
  if (num[num.size() - 1] == 0)
    f = 0;
  return *this;
}
int2048 &int2048::minus(int2048 b) {
  if (f == 0) {
    swap(num, b.num);
    std::swap(f, b.f);
    f = -f;
    return *this;
  }
  if (b.f == 0) {
    return *this;
  }
  if (f == b.f) {
    if (Absbigger(*this, b)) {
      f = -b.f;
      swap(num, b.num);
    }
    _minus(num, b.num);

  } else {
    // f = b.f;
    _add(num, b.num);
  }
  while (num.size() > 1 && num[num.size() - 1] == 0)
    num.pop_back();
  if (num[num.size() - 1] == 0)
    f = 0;
  return *this;
}
int2048 add(int2048 a, const int2048 &b) { return a.add(b); }
int2048 minus(int2048 a, const int2048 &b) { return a.minus(b); }
bool int2048::Absbigger(const int2048 &a, const int2048 &b) {
  if (a.num.size() != b.num.size())
    return a.num.size() < b.num.size();
  for (int i = a.num.size() - 1; i >= 0; i--) {
    if (a.num[i] != b.num[i])
      return a.num[i] < b.num[i];
  }
  return 0;
}
int2048 int2048::operator+() const {
  int2048 a = *this;
  // a.f=f;
  return a;
}

int2048 int2048::operator-() const {
  int2048 a = *this;
  a.f = -f;
  return a;
}
int2048 &int2048::operator=(const int2048 &b) {
  f = b.f;
  num = b.num;
  return (*this);
}
int2048 &int2048::operator+=(const int2048 &b) {
  (*this).add(b);
  return (*this);
}
int2048 &int2048::operator-=(const int2048 &b) {
  (*this).minus(b);
  return (*this);
}
int2048 operator+(int2048 a, const int2048 &b) {
  a.add(b);
  return a;
}
int2048 operator-(int2048 a, const int2048 &b) {
  a.minus(b);
  return a;
}
std::istream &operator>>(std::istream &in, int2048 &b) {
  char str[MAXN];
  in >> str;
  b = int2048(str);
  return in;
}
std::ostream &operator<<(std::ostream &out, const int2048 &b) {
  // b.print();
  // return out;
  if (b.f == 0) {
    out << '0';
  } else {
    if (b.f == -1) {
      out << '-';
    }
    out << b.num[b.num.size() - 1];
    for (int i = (int)b.num.size() - 2; i >= 0; i--) {
      int tmp = b.num[i];
      if (tmp / Pow10[3] != 0 || i != b.num.size() - 1) {
        out << tmp / Pow10[3];
      }
      tmp %= Pow10[3];
      if (tmp / Pow10[2] != 0 || i != b.num.size() - 1) {
        out << tmp / Pow10[2];
      }
      tmp %= Pow10[2];
      if (tmp / Pow10[1] != 0 || i != b.num.size() - 1) {
        out << tmp / Pow10[1];
      }
      tmp %= Pow10[1];
      out << tmp / Pow10[0];
      // out<<'+';
      // if(i!=b.num.size()-1){
      //   // if(num[i]<Pow10[0])std::cout<<'0';
      //   if(b.num[i]<Pow10[1])s+='0';
      //   if(b.num[i]<Pow10[2])s+='0';
      //   if(b.num[i]<Pow10[3])s+='0';
      // }
    }
  }
  return out;
}
bool operator==(const int2048 &a, const int2048 &b) {
  if (a.f != b.f || a.num.size() != b.num.size())
    return 0;
  for (int i = 0; i < a.num.size(); i++)
    if (a.num[i] != b.num[i])
      return 0;
  return 1;
}
bool operator!=(const int2048 &a, const int2048 &b) { return !(a == b); }
bool operator<(const int2048 &a, const int2048 &b) {
  if (a.f != b.f)
    return a.f < b.f;
  if (a.num.size() != b.num.size())
    return a.f == 1 ? a.num.size() < b.num.size() : a.num.size() > b.num.size();
  for (int i = a.num.size() - 1; i >= 0; i--) {
    if (a.num[i] != b.num[i])
      return a.f == 1 ? a.num[i] < b.num[i] : a.num[i] > b.num[i];
  }
  return 0;
}
bool operator>(const int2048 &a, const int2048 &b) {
  if (a.f != b.f)
    return a.f > b.f;
  if (a.num.size() != b.num.size())
    return a.f == 1 ? a.num.size() > b.num.size() : a.num.size() < b.num.size();
  for (int i = a.num.size() - 1; i >= 0; i--) {
    if (a.num[i] != b.num[i])
      return a.f == 1 ? a.num[i] > b.num[i] : a.num[i] < b.num[i];
  }
  return 0;
}

bool operator<=(const int2048 &a, const int2048 &b) { return (!(a > b)); }
bool operator>=(const int2048 &a, const int2048 &b) { return (!(a < b)); }
const long long NttMode = 1945555039024054273;
// 4179340454199820289
std::vector<int> rev;
void Init(int n) {
  rev.resize(n);
  rev[0] = 0;
  for (int i = 1; i < n; i++)
    rev[i] = (rev[i >> 1] >> 1) | ((i & 1) ? n >> 1 : 0);
}
long long Myqpow(long long x, long long y = NttMode - 2) {
  long long ans = 1;
  for (; y; y >>= 1, x = (__int128)x * x % NttMode)
    if (y & 1)
      ans = (__int128)ans * x % NttMode;
  return ans;
}
std::vector<long long> NTT(int n, std::vector<long long> a, int f) {
  Init(n);
  a.resize(n);
  for (int i = 0; i < n; i++)
    if (rev[i] < i)
      std::swap(a[rev[i]], a[i]);
  for (int i = 1; i < n; i <<= 1) {
    long long wn = Myqpow(5, (NttMode - 1) / (long long)(i << 1));
    if (!f)
      wn = Myqpow(wn);
    for (int j = 0; j < n; j += (i << 1))
      for (long long k = 0, x, y, w = 1; k < i;
           k++, w = (__int128)w * wn % NttMode)
        x = a[j + k], y = (__int128)w * a[j + k + i] % NttMode,
        a[j + k] = (x + y) % NttMode,
        a[j + k + i] = (x - y + NttMode) % NttMode;
  }
  if (!f)
    for (long long i = 0, w = Myqpow(n); i < n; i++)
      a[i] = (__int128)w * a[i] % NttMode;
  return a;
}
int2048 &int2048::operator*=(int2048 b) {
  if (f == 0 || b.f == 0) {
    *this = int2048(0);
    return *this;
  }
  // if(b.)
  if (b.num.size() + num.size() <= 4) {
    (*this) = int2048(getlong(*this) * getlong(b));
    return *this;
  }
  f *= b.f;
  std::vector<long long> tmpnuma, tmpnumb;
  for (int i = 0; i < num.size(); i++)
    tmpnuma.push_back(num[i]);
  for (int i = 0; i < b.num.size(); i++)
    tmpnumb.push_back(b.num[i]);

  // for(int i=num.size()-1;i>=0;i--)
  //     std::cout<<num[i];
  // std::cout<<"\n";
  // for(int i=b.num.size()-1;i>=0;i--)
  //     std::cout<<b.num[i];
  // std::cout<<"\n";

  int len = 1;
  while (len < ((num.size() + b.num.size()))) {
    len <<= 1;
  }
  // std::cout<<len<<"\n";
  tmpnuma = NTT(len, tmpnuma, 1);
  tmpnumb = NTT(len, tmpnumb, 1);

  for (int i = 0; i < len; i++) {
    tmpnuma[i] = (__int128)tmpnuma[i] * tmpnumb[i] % NttMode;
  }
  tmpnuma = NTT(len, tmpnuma, 0);
  // for(int i=0;i<num.size()-1;i++){
  //     std::cout<<num[i];
  // }std::cout<<'\n';
  for (int i = 0; i < tmpnuma.size() - 1; i++) {
    tmpnuma[i + 1] += tmpnuma[i] / Digital;
    tmpnuma[i] %= Digital;
  }
  num.resize(tmpnuma.size());
  for (int i = 0; i < num.size(); i++) {
    num[i] = tmpnuma[i];
  }

  while (num.size() > 1 && num[num.size() - 1] == 0)
    num.pop_back();
  if (num[num.size() - 1] == 0)
    f = 0;
  return *this;
}
int2048 operator*(int2048 a, const int2048 b) {
  a *= b;
  return a;
}
std::vector<long long> InvMulti(std::vector<long long> a,
                                std::vector<long long> b) {
  int len = 1, tmp = a.size() + b.size() - 1;
  while (len < ((a.size() + b.size() - 1))) {
    len <<= 1;
  }
  // std::cout<<len<<"\n";
  a = NTT(len, a, 1);
  b = NTT(len, b, 1);
  for (int i = 0; i < len; i++) {
    a[i] = (__int128)a[i] * b[i] % NttMode;
  }
  a = NTT(len, a, 0);
  for (int i = len - 1; i > 0; i--) {
    a[i - 1] += a[i] / Digital;
    a[i] %= Digital;
  }
  a.resize(tmp);
  return a;
}
std::vector<long long> GetInv(std::vector<long long> a, int T, int TT) {
  std::vector<long long> tmp, inv;
  // 把除数变成一个1~10的数方便计算
  inv.resize(2);
  // T+=1;
  int tt = a[0] * Digital;
  if (a.size() > 1)
    tt += a[1];
  inv[1] = 100000000 / tt;
  for (int times = 1; times <= T; times++) {
    // tmp.resize(0);
    // if(T>10)exit(-1);
    tmp.resize(1 << std::min(TT, times));
    inv.resize(1 << std::min(TT, times));
    for (int i = 0; i < tmp.size(); i++) {
      if (i < a.size())
        tmp[i] = a[i];
      else
        tmp[i] = 0;
    }
    // std::cout<<inv[0]<<'|'<<inv[1]<<"|"<<inv[2]<<"+";
    // std::cout<<tmp[0]<<'|'<<tmp[1]<<"|"<<tmp[2]<<"+";
    tmp = InvMulti(tmp, inv);
    tmp.resize(1 << std::min(TT + 1, times));
    tmp[0] = 2LL - tmp[0];
    // std::cout<<tmp[0]<<"+";
    for (int i = 1; i < tmp.size(); i++) {
      tmp[i] = -tmp[i];
    }
    for (int i = tmp.size() - 1; i >= 1; i--) {
      tmp[i - 1] += ((tmp[i] - Digital + 1) / Digital);
      tmp[i] = (tmp[i] % Digital + Digital) % Digital;
    }
    inv = InvMulti(inv, tmp);
    inv.resize(1 << std::min(TT, times));
  }
  return inv;
}
int2048 &int2048::operator/=(int2048 b) {
  // exit(-1);
  if (b.num.size() <= 7) {
    // []unorderedmap
    __int128 B = 0, C = 0;
    for (int i = (int)b.num.size() - 1; i >= 0; i--) {
      B = B * Digital + b.num[i];
    }
    for (int i = (int)num.size() - 1; i >= 0; i--) {
      C = C * Digital + num[i];
      num[i] = C / B;
      C %= B;
    }
    f *= b.f;
    if (f == -1 && C != 0) {
      num[0] += 1;
      int d = 0;
      while (num[d] >= Digital) {
        num[d] -= Digital;
        num[d + 1] += 1;
        d++;
      }
    }
    while (num.size() > 1 && num[num.size() - 1] == 0) {
      num.pop_back();
    }
    if (num[num.size() - 1] == 0)
      f = 0;
    return *this;
    //
  }
  int F = f * b.f;
  // return (*this);
  f = b.f = 1;
  if (F == -1)
    (*this) += (b - 1);
  int2048 T(*this), TT(b);
  std::vector<long long> tmpnuma, tmpnumb;
  for (int i = 0; i < num.size(); i++)
    tmpnuma.push_back(num[num.size() - 1 - i]);
  for (int i = 0; i < b.num.size(); i++)
    tmpnumb.push_back(b.num[b.num.size() - 1 - i]);

  // std::reverse(num.begin(),num.end());
  // std::cout<<num.size();
  // return *this;
  // for(int i=0;i<tmpnuma.size()-1-i;i++)
  //   std::swap(tmpnuma[i],tmpnuma[tmpnuma.size()-1-i]);
  // for(int i=0;i<b.num.size()-1-i;i++)
  //   std::swap(b.num[i],b.num[b.num.size()-1-i]);
  // while(tmpnumb.size()>1&&tmpnumb[tmpnumb.size()-1]==0)tmpnumb.pop_back();
  // std::reverse(b.num.begin(),b.num.end());
  int L = (int)num.size() - (int)b.num.size() + 1;
  if (L <= 0) {
    num.resize(1);
    num[0] = 0;
    f = 0;
    return (*this);
  }
  // b.print();
  int lg = 1;
  // if(L>5000)exit(-1);
  //--------------------------------------------
  while ((1 << lg) < L)
    lg++;
  if (lg < 10) {
    // lg++;
    tmpnumb = GetInv(tmpnumb, lg + 4, lg);
  } else {
    tmpnumb = GetInv(tmpnumb, lg, lg);
  }
  // for(int i=0;i<std::min(10,(int)b.num.size());i++)std::cout<<b.num[i]<<' ';

  // std::cout<<'\n';
  // for(int i=0;i<10;i++)std::cout<<b.num[i];
  // return (*this);
  int len = 1;
  // exit(-1);
  while (len < ((tmpnuma.size() + tmpnumb.size()))) {
    len <<= 1;
  }

  // std::cout<<len<<"\n";
  tmpnuma = NTT(len, tmpnuma, 1);
  tmpnumb = NTT(len, tmpnumb, 1);
  for (int i = 0; i < len; i++) {
    tmpnumb[i] = (__int128)tmpnuma[i] * tmpnumb[i] % NttMode;
  }

  tmpnumb = NTT(len, tmpnumb, 0);
  for (int i = tmpnumb.size() - 1; i >= 1; i--) {
    tmpnumb[i - 1] += tmpnumb[i] / Digital;
    tmpnumb[i] %= Digital;
  }
  num.resize(L);
  for (int i = L - 1; i >= 0; i--)
    num[L - i - 1] = tmpnumb[i];
  // exit(0);

  while (num.size() > 1 && num[num.size() - 1] == 0)
    num.pop_back();
  // if(Absbigger(T, (*this)*TT))exit(4234324);
  // if(F==-1)T+=TT-1;//T.print();

  if (!Absbigger(T, (*this) * TT + TT))
    (*this) += int2048(1);
  if (!Absbigger(T, (*this) * TT + TT)) {
    (*this) += (T - (*this) * TT) / TT;
  }

  // if((int)it.num.size()-(int)TT.num.size()>200)exit(-1);
  if (Absbigger((*this) * TT + TT, T))
    exit(4234324);
  // int2048 it=T-(*this)*TT;it.print();std::cout<<"|\n";
  f = F;
  if (num[num.size() - 1] == 0)
    f = 0;
  return (*this);
}
int2048 operator/(int2048 a, const int2048 b) {
  a /= b;
  return a;
}
int2048 &int2048::operator%=(const int2048 b) {
  (*this) = (*this) - b * ((*this) / b);
  return (*this);
}
int2048 operator%(int2048 a, const int2048 b) {
  a %= b;
  return a;
}
}; // namespace sjtu
   // namespace sjtu

sjtu::int2048 a, b;
std::string s;
