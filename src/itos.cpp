#include "itos.h"
#include <sstream>

std::string itos(int n)
{
  std::stringstream ss;
  ss << n;
  return ss.str();
}
