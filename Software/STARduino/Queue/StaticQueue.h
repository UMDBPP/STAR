// header defining the interface of the source.
#ifndef _QUEUELIST_H
#define _QUEUELIST_H

// define a default size of 10, but allow user to override
#ifndef QUEUE_LEN
#define QUEUE_LEN 10
#endif

template <class T>
class Queue
{
public:
	Queue(); 
	bool push(const T); 
	bool pop(T&);  // pop an element off the stack
  bool peek(T&);
  bool peek(T&,uint16_t);
	bool isEmpty();
	bool isFull();
  uint16_t getSize();
private:
	uint16_t bottom ;  // Number of elements on Stack
	uint16_t top ;  
	T queue[QUEUE_LEN];  
} ;

//constructor with the default size 10
template <class T>
Queue<T>::Queue(){
  top = 0;
  bottom = 0;
}

// push an element onto the Stack 
template <class T>
bool Queue<T>::isEmpty(){
  
	return top == bottom;
}

// push an element onto the Stack 
template <class T>
bool Queue<T>::isFull(){
  
	return top - bottom == QUEUE_LEN;
}

// pop an element off the Stack
template <class T> 
uint16_t Queue<T>::getSize(){

		return top - bottom;
}

 // push an element onto the Stack 
template <class T>
bool Queue<T>::push(const T item){
	if (!isFull()){
    //printf("push: %d\n",item);
    memcpy(&queue[top++],&item,sizeof(T));
    //printf("queue: %d at %d\n",queue[top-1], top-1);
		return 1 ;  // push successful
	}
	return 0 ;  // push unsuccessful
}

// pop an element off the Stack
template <class T> 
bool Queue<T>::pop(T& popValue) {
	if (!isEmpty()){
    //printf("%d\n",bottom);
    memcpy(&popValue,&queue[bottom++],sizeof(T));
    //printf("%d\n",bottom);
		return 1 ;  // pop successful
	}
	return 0 ;  // pop unsuccessful
}

// pop an element off the Stack
template <class T> 
bool Queue<T>::peek(T& peekValue) {
  
	return peek(peekValue,0);
}

// pop an element off the Stack
template <class T> 
bool Queue<T>::peek(T& peekValue, uint16_t idx) {
  
  //printf("idx: %d, bottom: %d, top: %d, isEmpty(): %d\n",idx,bottom,top,isEmpty());
  
	if (!isEmpty() && (idx <= getSize())){
    //printf("peek: %d at %d\n",queue[bottom+idx],bottom+idx);
    memcpy(&peekValue,&queue[bottom+idx],sizeof(T));
		return 1 ;  // peek successful
	}
	return 0 ;  // peek unsuccessful
}
#endif