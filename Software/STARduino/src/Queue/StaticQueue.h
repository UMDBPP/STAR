#ifndef _QUEUELIST_H
#define _QUEUELIST_H

#ifndef QUEUE_LEN
// define a default size of 10, but allow user to override
#define QUEUE_LEN 10
#endif

/*
 * Definition of a statically allocated queued-list for use 
 * in the command queue of the STAR payload. Class is templated
 * to allow using it with arbitrary data types (except arrays).
 */
template <class T>
class Queue{
public:
	Queue(); 
	bool push(const T); 
	bool pop(T&);  
  bool peek(T&);
  bool peek(T&,uint16_t);
	bool isEmpty();
	bool isFull();
  uint16_t getSize();
private:
	uint16_t bottom_; 
	uint16_t top_;  
	T queue_[QUEUE_LEN];  
};

/*
 * Constructor for Queue class. 
 * Initializes counters which track position in the queue.
 *
 * Inputs:
 * none
 *
 * Output:
 * none
 *
 * Return:
 * none
 */
template <class T>
Queue<T>::Queue(){
  top_ = 0;
  bottom_ = 0;
}

/*
 * Function which returns if the queue is empty
 *
 * Inputs:
 * none
 *
 * Output:
 * none
 *
 * Return:
 * boolean [1 if empty, 0 if not]
 */
template <class T>
bool Queue<T>::isEmpty(){
  
	return top_ == bottom_;
}

/*
 * Function which returns if the queue is full
 *
 * Inputs:
 * none
 *
 * Output:
 * none
 *
 * Return:
 * boolean [1 if full, 0 if not]
 */
template <class T>
bool Queue<T>::isFull(){
  
	return top_ - bottom_ == QUEUE_LEN;
}

/*
 * Function which returns the size of the queue
 *
 * Inputs:
 * none
 *
 * Output:
 * none
 *
 * Return:
 * uint16_t - number of elements in queue
 */
template <class T> 
uint16_t Queue<T>::getSize(){

		return top_ - bottom_;
}

/*
 * Pushes an element onto the end of the queue
 *
 * Inputs:
 * T - the object (type matches type of queue) to be added
 *
 * Output:
 * none
 *
 * Return:
 * bool [1 if successful, 0 if not]
 */
template <class T>
bool Queue<T>::push(const T _item){
	if (!isFull()){
    //printf("push: %d\n",item);
    memcpy(&queue_[top_++],&_item,sizeof(T));
    //printf("queue: %d at %d\n",queue[top-1], top-1);
		return 1 ;  // push successful
	}
	return 0 ;  // push unsuccessful
}

/*
 * Pop's an element off the front of the queue
 *
 * Inputs:
 * none
 *
 * Output:
 * T - the object (type matches type of queue) which was removed
 *
 * Return:
 * bool [1 if successful, 0 if not]
 */
template <class T> 
bool Queue<T>::pop(T& _popValue) {
	if (!isEmpty()){
    //printf("%d\n",bottom);
    memcpy(&_popValue,&queue_[bottom_++],sizeof(T));
    //printf("%d\n",bottom);
		return 1 ;  // pop successful
	}
	return 0 ;  // pop unsuccessful
}

/*
 * Return the element at the front of the queue
 *
 * Inputs:
 * none
 *
 * Output:
 * T - the object (type matches type of queue) which was removed
 *
 * Return:
 * bool [1 if successful, 0 if not]
 */
template <class T> 
bool Queue<T>::peek(T& _peekValue) {
  
  // peek at the next element in the queue
	return peek(_peekValue,0);
}

/*
 * Return the element at the front of the queue
 *
 * Inputs:
 * idx - position (from front of queue) of object to return
 *
 * Output:
 * T - the object (type matches type of queue) which was removed
 *
 * Return:
 * bool [1 if successful, 0 if not]
 */
template <class T> 
bool Queue<T>::peek(T& _peekValue, uint16_t _idx) {
   
  // cannot peek if queue is empty of requested element
  // is longer than the queue
	if (!isEmpty() && (_idx <= getSize())){
    memcpy(&_peekValue,&queue_[bottom_+_idx],sizeof(T));
		return 1 ;  // peek successful
	}
	return 0 ;  // peek unsuccessful
}

#endif // _QUEUELIST_H