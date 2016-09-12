
/*
 * @Algorithm: This algorithm is designed by Batagelj & Zaversnik
 * the algorithm first published in the paper named "An O(m) Algorithm
 * for Cores Decomposition of Networks"
 */

#include <algorithm>
#include <iostream>
#include <set>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <fstream>

#include "inputbuffer.hpp"

using std::string;
using std::vector;

typedef int vid_t;
typedef vector<vid_t> vlist;

int main(int argc, char **argv)
{
    FILE *infile = fopen(argv[1], "r+");
    if( infile == NULL ){
        printf("open file error\n");
        exit(0);
    }

    inputbuffer ibuff(200, infile);
    std::cout << "size: " << ibuff.getsize() << std::endl;
    char *start, *end;
    for( int i = 0; i != 10; ++i ){
        int value = ibuff.getline(start, end);
        if( value < 0 ){
            printf("there is wrong of line\n");
            exit(0);
        }
        string s(start, end);
        std::cout << s << std::endl;
    }

    return 1;
}


