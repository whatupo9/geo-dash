#include "itos.h"
#include <sstream>

/**
 * Converts an int to a string
 *
 * @param n: The int to be converted
 *
 * @returns: The string representation of n
 */
std::string itos(int n)
{
  // Write the int to a stringstream
  std::stringstream ss;
  ss << n;
  
  // Return the contents of the stringstream
  return ss.str();
}
