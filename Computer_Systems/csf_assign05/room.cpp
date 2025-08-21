#include "guard.h"
#include "message.h"
#include "message_queue.h"
#include "user.h"
#include "room.h"
#include <sstream>

Room::Room(const std::string &room_name)
  : room_name(room_name) {
  // initialize the mutex
  pthread_mutex_init(&lock, nullptr);
}

Room::~Room() {
  // destroy the mutex
  pthread_mutex_destroy(&lock);
}

void Room::add_member(User *user) {
  // add User to the room
  Guard g(lock);
  members.insert(user);
}

void Room::remove_member(User *user) {
  // remove User from the room
  Guard g(lock);
  members.erase(user);
}

void Room::broadcast_message(const std::string &sender_username, const std::string &message_text) {
  // send a message to every (receiver) User in the room
  std::ostringstream oss;
  oss << room_name << ":" << sender_username << ":" << message_text;
  std::string payload = oss.str();
  Guard g(lock);
  for (User* user : members) {
    Message* msg = new Message(TAG_DELIVERY, payload);
    user->mqueue.enqueue(msg);
  }
}
