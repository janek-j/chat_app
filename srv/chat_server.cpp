#include <iostream>
#include <winsock2.h>

#include "src/server.h"

int main() {
  ChatServer my_server(80);

  std::cout << "Testing chat server." << std::endl;

  return 0;
}