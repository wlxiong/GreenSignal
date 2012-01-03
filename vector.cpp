/*#include "vector.h"


template <typename T>
vector<T>::vector():a(new T[MAX_SIZE]),top(0){}

template <typename T>
vector<T>::vector( int n ):top(n),a(new T[n]){
    for( int i = 0; i < top; ++i ) a[i] = T();
}

template <typename T>
vector<T>::vector( int n,T init ):top(n),a(new T[n]){
    for( int i = 0; i < top; ++i )
		a[i] = init;
}
template <typename T>
vector<T>::~vector(){
	delete [] a;
}
template <typename T>
void vector<T>::push_back( T key ){
    assert( top!=MAX_SIZE );  
    a[top++] = key;
}
template <typename T>
void vector<T>::pop_back(){
	top--;
	assert(top>=0);
}
template <typename T>
T vector<T>::front(){
        assert(top>0);
        return a[0];
}
template <typename T>
T vector<T>::back(){
        assert(top>0);
        return a[top-1];
}
template <typename T>
int vector<T>::size(){
		return top;
}
template <typename T>
void vector<T>::clear(){
	top = 0;
}
template <typename T>
T& vector<T>::operator[]( int i ){
    //if( i >= top ) throw "Exceed the range of the array";
    assert(i<top && i>=0);
    return a[i];
}
*/
