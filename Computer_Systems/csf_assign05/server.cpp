#include <pthread.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <set>
#include <vector>
#include <cctype>
#include <cassert>
#include <thread>
#include <chrono>
#include "message.h"
#include "connection.h"
#include "user.h"
#include "room.h"
#include "guard.h"
#include "server.h"


////////////////////////////////////////////////////////////////////////
// Server implementation data types
////////////////////////////////////////////////////////////////////////

// TODO: add any additional data types that might be helpful
//       for implementing the Server member functions
// Thread argument struct
typedef struct {
  Server* server;
  Connection* conn;
} ThreadArguments;

////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////

namespace {

void *worker(void *arg) {
  pthread_detach(pthread_self());

  // Use a static cast to convert arg from a void* to whatever pointer type describes the object(s) needed
  // to communicate with a client (sender or receiver)
  //Use the structure we defined above
  ThreadArguments* arguments = static_cast<ThreadArguments*>(arg);
  Server* server = arguments->server;
  Connection* conn = arguments->conn;
  delete arguments;

  // Check if we can login
  Message login_msg;
  if (!conn->receive(login_msg)) {
    delete conn;
    return nullptr;
  }
  std::string username = login_msg.data;

  // Check login type (if sender and receiver, then 
  // will send tag ok and open chat)
  if (login_msg.tag == TAG_SLOGIN || login_msg.tag == TAG_RLOGIN){
    conn->send(Message(TAG_OK, ""));
  } else {
    conn->send(Message(TAG_ERR, "Invalid login tag"));
    delete conn;
    return nullptr;
  }

  Room* room = nullptr;
  User recv_user(username);
  bool joined = false;
  Message msg;

  while (true) {
    if (login_msg.tag == TAG_SLOGIN ) {
      //Break condition to leave 
      if (!conn->receive(msg)) {
        break;
      }

      //Joins the server/creates one
      if (msg.tag == TAG_JOIN) {
        room = server->find_or_create_room(msg.data);
      
      //Sends the message to everyone
      } else if (msg.tag == TAG_SENDALL) {
        if (room) {
          room->broadcast_message(username, msg.data);
        } else { 
          conn->send(Message(TAG_ERR, "Not in a room")); continue; 
        }
      //Leave the room
      } else if (msg.tag == TAG_LEAVE) {
        room = nullptr;

      //Quit the room
      } else if (msg.tag == TAG_QUIT) {
        conn->send(Message(TAG_OK, ""));
        break;
      
      //Not a valid command 
      } else {
        conn->send(Message(TAG_ERR, "Invalid command"));
        continue;
      }
      conn->send(Message(TAG_OK, ""));
    } else {
      //Checks if we have joined a room
      if (!joined) {
        if (!conn->receive(msg) || msg.tag != TAG_JOIN) {
          conn->send(Message(TAG_ERR, "Must join a room first"));
          break;
        }

        //Creates a new one if aren't joined yet
        room = server->find_or_create_room(msg.data);
        room->add_member(&recv_user);
        conn->send(Message(TAG_OK, ""));
        joined = true;
        continue;
      }

      //Get message from the quote
      Message* out = recv_user.mqueue.dequeue();

      if (!out) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        continue;
      }

      if (!conn->send(*out)) { 
        delete out; 
        break; 
      }
      delete out;
    }
  }

  //remove the members for crashes
  if (login_msg.tag == TAG_RLOGIN && joined && room) {
    room->remove_member(&recv_user);
  }

  // Cleanup
  delete conn;
  return nullptr;
}

}

////////////////////////////////////////////////////////////////////////
// Server member function implementation
////////////////////////////////////////////////////////////////////////

Server::Server(int port)
  : m_port(port)
  , m_ssock(-1) {
  // Initialize mutex
  pthread_mutex_init(&m_lock, nullptr);
}

Server::~Server() {
  // Destroy mutex
  pthread_mutex_destroy(&m_lock);
  for (auto& pair : m_rooms) {
    delete pair.second;
  }
}

bool Server::listen() {
  // Use open_listenfd to create the server socket, return true if successful, false if not
  m_ssock = open_listenfd(std::to_string(m_port).c_str());
  if (m_ssock >= 0) {
    return true;
  } else {
    return false;
  }
}

void Server::handle_client_requests() {
  // TODO: infinite loop calling accept or Accept, starting a new
  //       pthread for each connected client
  //Infinite loop
  while (true) {
    //Tries to get the call accept
    int connFiguration = Accept(m_ssock, nullptr, nullptr);
    if (connFiguration < 0) {
      std::cerr << "No connection established" << std::endl;
      continue;
    }

    // Continues if we do have an accept and make new thread arguments
    Connection* conn = new Connection(connFiguration);
    pthread_t threads;
    ThreadArguments* argument = new ThreadArguments{this, conn};

    // If creating a thread fails
    if (pthread_create(&threads, nullptr, worker, argument) != 0) {
      std::cerr << "Failed to create thread" << std::endl;
      delete argument;
      delete conn;
    }
  }
}

Room *Server::find_or_create_room(const std::string &room_name) {
  // Find the room
  Guard g(m_lock);
  std::map<std::string, Room*>::iterator it = m_rooms.find(room_name);
  if (it != m_rooms.end()) {
    return it->second;
  }

  // Create a room if it doesn't exist
  Room* room = new Room(room_name);
  m_rooms[room_name] = room;
  return room;
}
