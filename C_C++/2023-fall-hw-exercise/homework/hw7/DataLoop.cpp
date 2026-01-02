#include "DataLoop.h"

//Default Constructor:
DataLoop::DataLoop() : start(nullptr), count(0) {};

//Alternative Constructor
DataLoop::DataLoop(const int & num) : start(new _Node({num, nullptr, nullptr})), count(1) {
    start->next = start;
    start->prev = start;
  }

//Copy Constructor
DataLoop::DataLoop(const DataLoop & rhs) : DataLoop() {
    *this = rhs;
  }

//Overloaded operator= (makes deep copy)
DataLoop & DataLoop::operator=(const DataLoop & rhs) {
    _Node* cur = rhs.start;
    if (this != &rhs) {
        clear(); //clears dataloop on the LHS before creating a dataloop that contains all data values on RHS
        for (size_t i = 0; i < rhs.count; i++) {
            *this += cur->data;
            cur = cur->next;
        }
    }
    count = rhs.count;
    return *this;
}

//Destructor
DataLoop::~DataLoop() {
    clear();
}

//Overloaded operator==
bool DataLoop::operator==(const DataLoop & rhs) const {
    _Node* cur = start;
    _Node* other = rhs.start;

    //Assigns larger_count to be the longest length of the dataloop, between the one on the LHS and the one on the RHS
    size_t larger_count = count;
    if (count < rhs.count) {
        larger_count = rhs.count; 
    } else {
        larger_count = count;
    }

    if (larger_count != 0 && (count == 0 || rhs.count == 0)) { //If one is empty but the other is not
        return false;
    }

    //Looks through dataloops, returns false if anything doesn't match
    for (size_t i = 0; i < larger_count; i++) {
        if (other->data != cur->data) {
            return false;
        }
        cur = cur->next;
        other = other->next;
    }
    //If passes all, returns true
    return true;
}

//Overloaded operator+=
DataLoop & DataLoop::operator+=(const int & num) {
    _Node* new_node = new _Node({num, nullptr, nullptr});
    if (!new_node) {  //If allocation failed
	    return *this; 
    }

    if (start == nullptr) { //If current dataloop is empty
        start = new_node;
        start->prev = start;
        start->next = start;
        count++;
    } else if (start->prev != nullptr && start->next != nullptr) { //If not empty, added at end
        start->prev->next = new_node;
        new_node->prev = start->prev;
        start->prev = new_node;
        new_node->next = start;
        count++;
    }
    return *this;
  }

//Overloaded operator+
 DataLoop DataLoop::operator+(const DataLoop & rhs) const {
    DataLoop new_loop;
    _Node* cur1 = start;
    _Node* cur2 = rhs.start;

    //Add everything on LHS to new dataloop
    for (size_t i = 0; i < count; i++) {
        new_loop += cur1->data;
        cur1 = cur1->next;
    }
    
    //Add everything on RHS to new dataloop
    for (size_t i = 0; i < rhs.count; i++) {
        new_loop += cur2->data;
        cur2 = cur2->next;
    }
    
    new_loop.count = this->count + rhs.count;
    return new_loop;
}

//Overloaded operator^
DataLoop & DataLoop::operator^(int offset) {
    //Not changed
    if (offset == 0 || count == 0 || count == 1) {
        return *this;
    }

    if (offset > 0) {
        for (int i = 0; i < offset; i++) { //Moves start back
            _Node* temp = start;
            start = start->next;
            start->prev = temp;
        }
    } else if (offset < 0) { //Moves start forward
        for (int j = 0; j > offset; j--) {
            _Node* temp = start;
            start = start->prev;
            start->next = temp;
        }
    }
    return *this;
}

//Splice
DataLoop & DataLoop::splice(DataLoop & rhs, size_t pos) {

    if (rhs.count == 0) { //If adding empty dataloop
        return *this;
    } else if (count == 0) { //If the dataloop being added to is empty
        //Makes a copy of rhs
        *this = rhs;
        //Gets rid of rhs
        rhs.start = nullptr;
        rhs.count = 0;

        return *this;
    }
    //Shifts start to where we want to add the new dataloop
    *this ^ (pos - 1);

    //Add the dataloop on the RHS to position indicated
    _Node* this_temp = start->next;
    _Node* rhs_temp = rhs.start->prev;
    start->next = rhs.start;
    rhs.start->prev = start;
    this_temp->prev = rhs_temp;
    rhs_temp->next = this_temp;

    //Shifts start back into where it started from
    size_t position = pos % count;
    count += rhs.count;
    *this ^ (count - position + 1);

    //Gets rid of rhs
    rhs.start = nullptr;
    rhs.count = 0;

    return *this;
}

//Printing
std::ostream & operator<<(std::ostream & os, const DataLoop & dl) {
    if (dl.count == 0) { //If dataloop is empty
        os << ">no values<";
    } else {
        DataLoop::_Node* cur = dl.start;
        for (size_t i = 0; i < dl.count; i++) {
            os << "-> " << cur->data << " <-";
            cur = cur->next;
        }
    }
    return os;
}

//Clears all noode in dataloop
void DataLoop::clear() {
   _Node* temp;
    for (size_t i = 0; i < count; i++) {
        temp = start;
        if (start == nullptr) {
            break;
        }
        start = start->next;
        delete temp;
    }
    start = nullptr;
    count = 0;
}