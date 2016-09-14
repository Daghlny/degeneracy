
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

int main(int argc, char **argv)
{
    FILE *infile = fopen(argv[1], "r+");
    if( infile == NULL ){
        printf("open file error\n");
        exit(0);
    }

    /* get the total vertices number and use it to initilize arrays */
    int nodenum = 0;
    fscanf(infile, "%d\n", &nodenum);
    g = (vlist **)malloc(sizeof(vlist*) * nodenum);
    for( int i = 0; i != nodenum; ++i )
        g[i] = NULL;
    vid_t *vert = (vid_t*)malloc(sizeof(vid_t) * nodenum);
    vid_t *pos  = (vid_t*)malloc(sizeof(vid_t) * nodenum);
    vid_t *deg  = (vid_t*)malloc(sizeof(vid_t) * nodenum);

    /* reading graph data from file */
    inputbuffer ibuff(infile);
    vid_t md = init_g(ibuff, deg, nodenum);

    int *bin = (int*)malloc(sizeof(int) * (md+1));
    init_bin(bin, md, nodenum, )

    

    return 1;
}

/*
 * return the maximum degree of all vertices */
vid_t
init_g(inputbuffer &ibuff, vid_t* deglist, int nodenum)
{
    int count = nodenum;
    vid_t md  = 0;
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

        deglist[v] = deg;
        if( deg > md ) md = deg;
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

    return md;
}

void
init_bin(int *bin, vid_t *deglist, vid_t *pos, vid_t *vert, vid_t md, vid_t nodenum)
{
    for( int i = 0; i != md+1; ++i)
        bin[i] = 0;
    for( vid_t i = 0; i != nodenum; ++i )
        ++bin[ deglist[i] ];

    int start = 1;
    for( vid_t i = 0; i != md+1; ++i ){
        int num = bin[i];
        bin[i] = start;
        start += num;
    }

    for(vid_t i = 0; i != nodenum; ++i){
        pos[i] = bin[deg[i]];
        vert[pos[i]] = i;
        ++bin[deg[i]];
    }

    for(vid_t i = nodenum; i >= 0; --i)
        bin[i] = bin[i-1];   
    
}


