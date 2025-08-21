#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cerr << "Usage: ./sender [server_address] [port] [username]\n";
    return 1;
  }

  std::string server_hostname;
  int server_port;
  std::string username;

  server_hostname = argv[1];
  server_port = std::stoi(argv[2]);
  username = argv[3];

  // connect to server
  Connection conn;
  conn.connect(server_hostname, server_port);
  if (!conn.is_open()) {
    std::cerr << "Error: Failed to connect to server." << std::endl;
    return 1;
  }

  // send slogin message
  Message slogin_msg(TAG_SLOGIN, username);
  if (!conn.send(slogin_msg)) {
    std::cerr << "Error: Failed to send slogin message." << std::endl;
    return 1;
  }

  Message response;
  if (!conn.receive(response)) {
    std::cerr << "Error: Failed to receive login response." << std::endl;
    return 1;
  }

  if (response.tag == TAG_ERR) { // server reponded with error message 
    std::cerr << response.data << std::endl;
    return 1; // quit if login error
  }

  // loop reading commands from user, sending messages to server as appropriate
  Message msg;
  while (true) {
    std::cout << "> ";
    std::string line;
    if (!std::getline(std::cin, line)) {
      break;
    }
    if (line.compare(0, 6, "/join ") == 0) {
      msg = Message(TAG_JOIN, line.substr(6)); // get room name
    } else if (line.compare(0, 6, "/leave") == 0) {
      msg = Message(TAG_LEAVE, "");
    } else if (line.compare(0, 5, "/quit") == 0) {
      msg = Message(TAG_QUIT, "bye");
      conn.send(msg); // send message and break out of loop
      break;
    } else {
      msg = Message(TAG_SENDALL, line); // message to be sent to room
    }

    if (!conn.send(msg)) {
      std::cerr << "Error: Failed to send message to server." << std::endl;
      break;
    }

    if (!conn.receive(response)) {
      std::cerr << "Error: Failed to receive response from server." << std::endl;
      break;
    }

    if (response.tag == TAG_ERR) {
      std::cerr << response.data << std::endl;
    }
  } 

  conn.close();
  return 0;
}
