/*2025.01.21
**操作符 ++ -- * 
 */

#include <iostream>

using namespace std;

class INT {
    friend ostream& operator<<( ostream& os, const INT& i );
public:
    INT( int i ) : m_i( i ) {}
    //a++
    INT& operator++()
    {
        ++m_i;
        return *this;
    }
    //++b;
    const INT operator++( int ) 
    {
        INT temp = *this;
        ++(*this);
        return temp;
    }
    //--a
    INT& operator--() 
    {
        --m_i;
        return *this;
    }
    //a--
    const INT operator--( int )
    {
        INT temp = *this;
        --(*this);
        return temp;
    }
    //* 
    int& operator*() const{
        return (int&)m_i;
    }
private:
    int m_i;
};

ostream& operator<<( ostream& os, const INT& i )
{
    os << '[' << i.m_i << ']' << endl;
    return os;
}

int main( void )
{
    INT I(5);
    cout << I++; //返回的是I之前的值，I现在= 6
    cout << ++I; //返回的是I之后的值，I现在= 7
    cout << I--; //返回的是I之前的值，I现在= 6
    cout << --I; //返回的是I之后的值，I现在= 5
    cout << *I;
}