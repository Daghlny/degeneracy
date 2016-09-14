
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
#include "BZ.hpp"

using std::string;
using std::vector;

void init_g(inputbuffer &, int);

int main(int argc, char **argv)
{
    FILE *infile = fopen(argv[1], "r+");
    if( infile == NULL ){
        printf("open file error\n");
        exit(0);
    }

    int nodenum = 0;
    fscanf(infile, "%d\n", &nodenum);
    g = (vlist **)malloc(sizeof(vlist*) * nodenum);
    for( int i = 0; i != nodenum; ++i )
        g[i] = NULL;

    inputbuffer ibuff(infile);
    init_g(ibuff, nodenum);

    char *start, *end;

    return 1;
}

void
init_g(inputbuffer &ibuff, int nodenum)
{
    int count = nodenum;
    while(--count >= 0)
    {
        char *start, *end;
        int result = ibuff.getline(start, end);
        if( result < 0 ) break;

        vid_t v = 0;
        do{
            v = (10 * v) + int(*start)-48;
        }while( *(++start) != ',' );

        //printf("nodenum: %d, v: %d\n", nodenum, v);
        if( v > nodenum || g[v] != NULL){
            printf("the value of @v is wrong\n");
            exit(0);
        }

        vid_t deg = 0;
        while( *(++start) != ':' ){
            deg = (10 * deg) + int(*start) - 48;
        }

        g[v] = new vector<vid_t>(deg);
        for(int i = 0; i != deg; ++i)
        {
            vid_t u = 0;
            while( *(++start) != ':' ){
                u = (10 * u) + int(*start) - 48;
            }
            g[v]->at(i) = u;
        }
    }
}


