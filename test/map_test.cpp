#include "../header/thin_map.h"
#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;

using namespace thinContainers;

int main( void ) {
    thin_map< string, int > str_i_map;
    str_i_map["jjhou"] = 1;
    str_i_map["jerry"] = 2;
    str_i_map["jason"] = 3;
    str_i_map["jimmy"] = 4;
    std::pair< string, int > value( "david", 5 );
    str_i_map.insert( value );
    for ( auto elem : str_i_map ) {
        cout << elem.first << " " << elem.second << endl;
    }
    int number = str_i_map["jjhou"];
    cout << number << endl;

    thin_map< string, int >::iterator iter1;
    iter1 = str_i_map.find( "mchen" );
    if ( iter1 == str_i_map.end() ) {
        cout << "not found " << endl;
    }
    iter1 = str_i_map.find( "jerry" );
    if( iter1 != str_i_map.end() ) {
        cout << "found\n" ;
    }

    iter1->second = 9;
    int number2 = str_i_map["jerry"];
    cout << number2 << endl;
    return 0;
}