Distribution:

Bella:
    connection.cpp
    sender.cpp
    room.cpp
    message_queue.cpp
Kyle:
    receiver.cpp
    server.cpp
    Report


Eventually your report about how you implemented thread synchronization
in the server should go here

In our chat server, synchronization is enforced in three concrete code sections to prevent race conditions, deadlocks, and undue contention.
First, in server.cpp, within the Server::find_or_create_room() method, we wrap the calls to m_rooms.find(room_name) and m_rooms[room_name] = new Room(...) inside a Guard(m_lock) critical section protecting the pthread_mutex_t m_lock. This ensures that lookup and insertion into the global m_rooms map are performed atomically, avoiding races when multiple threads create or find rooms concurrently.
Second, in room.cpp, the Room class protects its members set using its own pthread_mutex_t lock. In Room::add_member(), Room::remove_member(), and Room::broadcast_message(), we use Guard(lock) to safely modify or iterate over members. During broadcast_message(), we briefly hold the lock to iterate over the set and call MessageQueue::enqueue() on each member. Since each MessageQueue handles its own locking internally, and no other locks are acquired while iterating, this design avoids blocking join/leave operations during broadcasts.
Third, in message_queue.cpp, the MessageQueue class protects its m_messages deque with m_lock. enqueue() acquires m_lock to push a new message and then calls sem_post(&m_avail) to signal availability. dequeue() first blocks on sem_wait(&m_avail), then acquires m_lock to safely pop a message from the queue. This ensures correct message ordering without busy-waiting or contention.
We determined these critical sections by identifying places where shared mutable data structures (m_rooms, members, and m_messages) could be concurrently accessed. We chose pthread_mutex_t for short, fast mutual exclusion within classes, and sem_t for producer-consumer synchronization between sender and receiver threads.
By scoping each mutex to a single class, never holding more than one mutex at a time, and detaching each worker thread (pthread_detach) immediately, we prevent cyclic locking patterns and guarantee deadlock freedom. Overall, the synchronization design ensures safe concurrent access, correct message delivery ordering, and high throughput under multithreaded load.