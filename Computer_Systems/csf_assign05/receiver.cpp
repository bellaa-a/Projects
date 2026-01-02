#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int main(int argc, char **argv) {
  if (argc != 5) {
    std::cerr << "Usage: ./receiver [server_address] [port] [username] [room]\n";
    return 1;
  }

  std::string server_hostname = argv[1];
  int server_port = std::stoi(argv[2]);
  std::string username = argv[3];
  std::string room_name = argv[4];

  // Connect to server
  Connection conn;
  conn.connect(server_hostname, server_port);
  if (!conn.is_open()) {
    std::cerr << "Error: Failed to connect to server." << std::endl;
    return 1;
  }

  Message resp;
  // Send rlogin and join messages (expect a response from the server for each one)

  // rlogin messages
  Message rlogin_msg(TAG_RLOGIN, username);
  if (!conn.send(rlogin_msg)) {
    std::cerr << "Error: Failed to send rlogin message." << std::endl;
    return 1;
  }

  // Server doesn't receive login
  Message response;
  if (!conn.receive(response)) {
    std::cerr << "Error: Failed to receive login response." << std::endl;
    return 1;
  }

  // server reponded with error message 
  if (response.tag == TAG_ERR) { 
    std::cerr << response.data << std::endl;
    return 1;
  }


  // Send join message
  Message join_msg(TAG_JOIN, room_name);
  if (!conn.send(join_msg)) {
    std::cerr << "Error: Failed to send join message\n";
    return 1;
  }

  // Server doesn't receive response
  if (!conn.receive(resp)) {
    std::cerr << "Error: No response after join\n";
    return 1;
  }

  // Server reponded with error message 
  if (resp.tag == TAG_ERR) {
    std::cerr << resp.data << std::endl;
    return 1;
  }

  // Loop waiting for messages from server (which should be tagged with TAG_DELIVERY)
  Message msg;
  while (conn.receive(msg)) {
    if (msg.tag == TAG_DELIVERY) {
      // parse it correctly as room:sender:message_text
      size_t p1 = msg.data.find(':');
      size_t p2 = msg.data.find(':', p1 + 1);
      std::string sender = msg.data.substr(p1 + 1, p2 - p1 - 1);
      std::string text = msg.data.substr(p2 + 1);
      std::cout << sender << ": " << text << std::endl;
    } else if (msg.tag == TAG_ERR) {
      std::cerr << msg.data << std::endl;
    } 
  }

  return 0;
}