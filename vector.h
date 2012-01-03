/*#ifndef _vector_H
#define _vector_H

const int MAX_SIZE=1024;
template <typename T>
class vector{
public:
    
    vector();
	explicit vector( int n );
    vector( int n,T init );
	~vector();
    T& operator[]( int i );
    void push_back( T key );
    void pop_back();
    T front();
    T back();
	int size();
	void clear();

private:
//	static const int MAX_SIZE = 1024;
    int top;
    T* a;
};

#endif

*/
