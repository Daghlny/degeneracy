
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

/* 
 * argv[1]: the input graph file with specified format
 * argv[2]: the output file with degeneracy ording 
 */
int main(int argc, char **argv)
{
    /* handle the input/output files */
    FILE *infile = fopen(argv[1], "r+");
    FILE *outfile = NULL;
    if( infile == NULL ){
        printf("open file error\n");
        exit(0);
    }
    char *outfilename = (char*)malloc(sizeof(char) * 100);
    strcpy(outfilename, argv[1]);
    if( argc < 3 ){
        strcat(outfilename, ".degeneracy.txt");
        outfile = fopen(outfilename, "w+");
    }
    else
        outfile = fopen(argv[2], "w+");

    /* get the total vertices number and use it to initilize arrays */
    int nodenum = 0;
    fscanf(infile, "%d\n", &nodenum);
    g = (vlist **)malloc(sizeof(vlist*) * nodenum);
    for( int i = 0; i != nodenum; ++i )
        g[i] = NULL;
    vid_t *vert = (vid_t*)malloc(sizeof(vid_t) * nodenum);
    vid_t *pos  = (vid_t*)malloc(sizeof(vid_t) * nodenum);
    vid_t *deg  = (vid_t*)malloc(sizeof(vid_t) * nodenum);
    int   *flag = (int*)malloc(sizeof(int) * nodenum);

    /* check the malloc's memory block whether is available */
    if( vert == NULL || pos == NULL || deg == NULL || flag == NULL ){
        printf("allocate memory error\n");
        exit(1);
    }

    /* initialize the @flag array */
    for( int i = 0; i != nodenum; ++i )
        flag[i] = 1;

    /* reading graph data from file */
    inputbuffer ibuff(infile);
    vid_t md = init_g(ibuff, deg, nodenum);

    int *bin = (int*)malloc(sizeof(int) * (md+1));
    init_bin(bin, deg, pos, vert, md, nodenum);

    int degeneracy = 0;
    vid_t thevertex  = 0;

    /* Main Loop */
    for( vid_t i = 0; i != nodenum; ++i )
    {
        /* the vertex @v is the chosen vertex in this iteration */
        vid_t v = vert[i];
        /* debug */
        if(v == 22){
            printf("catch it\n");
        }
        /* end */

        if(deg[v] > degeneracy)
        {
            degeneracy = deg[v];
            thevertex  = v;
        }

        /* output to file with format */
        fprintf(outfile, "%d : %d\n", v, deg[v]);

        for(vIt it = g[v]->begin(); it != g[v]->end(); ++it)
        {
            /* if(deg[*it] < deg[v]){ 
            if( pos[*it] < bin[deg[v]] ) 
            if( pos[*it] < i )*/
            if( flag[*it] == -1 )
            {
                continue;
                printf("the neighbor(id: %d)'s degree is smaller than @v(id: %d)\n", *it, v);
                exit(0);
            }

            /* for each neighbor of @v, its degree should be decreased */
            vid_t du = deg[*it];    // degree of the neighbor
            int   pu = pos[*it];    // current position of the neighbor
            if( bin[du] < i+1 ) bin[du] = i+1;
            int   pw = bin[du];     // begin position of neighbor's degree

            int   w  = vert[pw];    // first vertex in the @vert with @du degree

            if(*it != w)
            {
                /* exchange two vertices' position(@it and @w) */
                pos[*it] = pw;
                pos[w]   = pu;
                vert[pu] = w;
                vert[pw] = *it;
            }

            /* to prevent more than one degree have the same begin position in @vert */
            for( vid_t degree = du; bin[degree] == pw && degree != md; ++degree)
                ++bin[degree];

            --deg[*it];
        }
        flag[v] = -1;
    }
    printf("degeneracy: %d\n", degeneracy);
    printf("vertex: %d\n", thevertex);

    return 1;
}

/*
 * return the maximum degree of all vertices */
vid_t
init_g(inputbuffer &ibuff, vid_t* deglist, int nodenum)
{
    int count = nodenum;
    vid_t md  = 0;

    /* debug */
    vid_t last_vertex = 0;
    char* last_start  = NULL;
    /* end */

    while(--count >= 0)
    {
        char *start, *end;
        if( ibuff.getline(start, end) < 0 ) break;

        vid_t v = 0;
        while( *start == '\n' ) ++start;

        char *lstart = start;//debug

        while( *(start) != ',')
        {
            v = (10 * v) + int(*start) - 48;
            ++start;
        }
        printf("%d\n", v);//debug

        if( v > nodenum || g[v] != NULL){
            printf("the value of @v is wrong (id: %d)\n", v);
            exit(0);
        }

        last_start  = start; //debug
        last_vertex = v;     //debug

        vid_t deg = 0;
        while( *(++start) != ':' && *start != '\n'){
            deg = (10 * deg) + int(*start) - 48;
        }

        deglist[v] = deg;
        if( deg > md ) md = deg;
        g[v] = new vector<vid_t>(deg);

        for(int i = 0; i != deg; ++i)
        {
            vid_t u = 0;
            while( *(++start) != ':' && *(start) != '\n'){
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

    int start = 0;
    for( vid_t i = 0; i != md+1; ++i ){
        int num = bin[i];
        bin[i]  = start;
        start  += num;
    }

    for(vid_t i = 0; i != nodenum; ++i){
        pos[i] = bin[deglist[i]];
        vert[pos[i]] = i;
        ++bin[deglist[i]];
    }

    for(vid_t i = md; i > 0; --i)
        bin[i] = bin[i-1];   
    bin[0] = 0;
    
}


