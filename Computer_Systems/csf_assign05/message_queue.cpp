#include <cassert>
#include <ctime>
#include "message_queue.h"
#include "message.h"
#include "guard.h"

MessageQueue::MessageQueue() {
  // initialize the mutex and the semaphore
  pthread_mutex_init(&m_lock, nullptr);
  sem_init(&m_avail, 0 , 0);
}

MessageQueue::~MessageQueue() {
  // destroy the mutex and the semaphore
  Guard g(m_lock);
  while (!m_messages.empty()) { // clear all messages
    Message* msg = m_messages.front();
    m_messages.pop_front();
    delete msg;
  }
  pthread_mutex_destroy(&m_lock);
  sem_destroy(&m_avail);
}

void MessageQueue::enqueue(Message *msg) {
  // put the specified message on the queue
  Guard g(m_lock);
  m_messages.push_back(msg);
  sem_post(&m_avail); // notify any thread waiting for a message
}

Message *MessageQueue::dequeue() {
  // we don't check the return value because the only reason
  // this call would fail is if we specify a clock that doesn't
  // exist
  sem_wait(&m_avail);

  // lock it
  Guard g(m_lock);
  Message* msg = m_messages.front();
  m_messages.pop_front();
  return msg;
}
