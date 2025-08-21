#include <sstream>
#include <cctype>
#include <cassert>
#include "csapp.h"
#include "message.h"
#include "connection.h"

Connection::Connection()
  : m_fd(-1)
  , m_last_result(SUCCESS) {
}

Connection::Connection(int fd)
  : m_fd(fd)
  , m_last_result(SUCCESS) {
  // initialize the rio_t object
  rio_readinitb(&m_fdbuf, fd);
}

void Connection::connect(const std::string &hostname, int port) {
  // connect to the server
  int fd = open_clientfd(hostname.c_str(), std::to_string(port).c_str());
  if (fd < 0) { // not valid
    m_last_result = EOF_OR_ERROR;
    m_fd = -1;
    return;
  }
  m_fd = fd;
  rio_readinitb(&m_fdbuf, fd); // initialize the rio_t object
  m_last_result = SUCCESS;
}

Connection::~Connection() {
  if (is_open()) { // check if its open
    close();
  }
}

bool Connection::is_open() const {
  // return true if the connection is open
  if (m_fd < 0) {
    return false;
  }
  return true;
}

void Connection::close() {
  // close the connection if it is open
  if (is_open()){
    Close(m_fd);
    m_fd = -1; // mark it closed
  }
}

bool Connection::send(const Message &msg) {
  std::ostringstream oss;
  oss << msg.tag << ":" << msg.data << "\n";
  std::string msg_str = oss.str();

  if (msg_str.length() > Message::MAX_LEN) { // check if message is within length
    m_last_result = INVALID_MSG;
    return false;
  }

  if (rio_writen(m_fd, msg_str.c_str(), msg_str.size()) != (ssize_t)msg_str.size()) { // check if eturn value equals the number of bytes I asked to send
    m_last_result = EOF_OR_ERROR;
    return false;
  }

  m_last_result = SUCCESS;
  return true;
}

bool Connection::receive(Message &msg) {
  char buffer[Message::MAX_LEN];

  if (rio_readlineb(&m_fdbuf, buffer, Message::MAX_LEN) <= 0) {
    m_last_result = EOF_OR_ERROR;
    return false;
  }

  // remove newline character if present
  std::string line(buffer);
  if (!line.empty() && line.back() == '\n') {
    line.pop_back();
  }

  // find first colon to split tag and data
  size_t colon_pos = line.find(':');
  if (colon_pos == std::string::npos) {
    m_last_result = INVALID_MSG;
    return false;
  }

  msg.tag = line.substr(0, colon_pos);
  msg.data = line.substr(colon_pos + 1);

  m_last_result = SUCCESS;
  return true;
}
