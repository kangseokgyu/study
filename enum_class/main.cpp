#include <arpa/inet.h>
#include <iostream>

using namespace std;

enum class TEST : uint8_t { TYPE_1 = 0x01, TYPE_2 = 0x02, TYPE_3 = 0x03 };

struct test_st {
  TEST t;
  uint32_t i;
} __attribute__((packed));

std::ostream &operator<<(std::ostream &os, TEST t) {
  return os << static_cast<int>(t);
}

int main() {
  uint8_t data[] = {0x02, 0x00, 0x00, 0x00, 0x01};

  struct test_st *s = (struct test_st *)data;

  cout << "t:" << s->t << "\n";
  cout << "i:" << ntohl(s->i) << "\n";

  return 0;
}