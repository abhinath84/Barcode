#include <cstdio>
#include <iterator>
#include <climits>
#include <iostream>
#include <math.h>

using namespace std;

//this version of gcc doesn't seem to have begin/end so I add them here
namespace std
{
  template< class T, size_t N >
  T* begin( T (&array)[N] ) {return array;}
  template< class T, size_t N >
  T* end( T (&array)[N] ) {return array+N;}
}

//you shouldn't have to do this

template<class output_iterator>
void convert_number_to_array_of_digits(const unsigned number, int count,
         output_iterator first, output_iterator last)
{
    const unsigned number_bits = count;
    //extract bits one at a time
    for(unsigned i=0; i<number_bits && first!=last; ++i)
    {
        const unsigned shift_amount = number_bits-i-1;
        const unsigned this_bit = (number>>shift_amount)&1;
        *first = this_bit;
        ++first;
    }

    //pad the rest with zeros
    while(first != last)
    {
        *first = 0;
        ++first;
    }
}

int main()
{
    int number = 16;
    int count = ((number < 8) ? 1 : floor(number/8)) * sizeof(int);

    int array[32];
    convert_number_to_array_of_digits(number, count, std::begin(array), std::end(array));
    for(int i=0; i < count; ++i)
        std::cout << array[i] << ' ';
}
