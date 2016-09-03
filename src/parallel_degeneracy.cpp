
#include <iostream>
#include <set>
#include <fstream>
#include <cstdint>
#include <string>
#include <cstdlib>
#include <cstring>
#include <utility>
#include <algorithm>
#include "omp.h"
using std::string;

typedef uint32_t vid_t;
typedef std::vector<vid_t> vlist;
typedef std::vector<vid_t>::iterator vlistIt;

vlist **G;         // points to a array of adjacency lists
int   *degree;

/*
 *  add a edge into the @datap and update @deg array
 *  called by @readGraph() during initialization
 */
inline void
addEdge(vid_t v, vid_t u, vlist **datap, int *deg)
{
    typedef std::pair<vlistIt, bool> vlistInsert_t;
    if(datap[v] == NULL) datap[v] = new vlist();
    if(datap[u] == NULL) datap[u] = new vlist();
    vlistIt uit = std::find(datap[v]->begin(), datap[v]->end(), u);
    vlistIt vit = std::find(datap[u]->begin(), datap[u]->end(), v);

    if( uit ==  datap[v]->end() ){
        if(deg[v] < 0) ++deg[v];
        ++deg[v];
        datap[v]->push_back(u);
    }

    if( vit == datap[u]->end() ){
        if(deg[u] < 0) ++deg[u];
        ++deg[u];
        datap[u]->push_back(v);
    }
}

/*
 *  read the Graph data and construct the degree array and the adjacent list
 */
void
readGraph(vlist** datap, char *inFileName, int *degree)
{
    
    std::ifstream inFile;
    inFile.open(inFileName);

    char inbuffer[1024];
    const char *flag = " \t";

    while(!inFile.eof())
    {
        inFile.getline(inbuffer, 1024);
        if(inbuffer[0] == '#') continue;
        if(!isdigit(inbuffer[0])) break;

        string line(inbuffer);
        std::string::size_type seperator = line.find_first_of(flag, 0);
        vid_t sour = atoi(line.substr(0, seperator).c_str());
        vid_t dest = atoi(line.substr(seperator, line.size()-seperator).c_str());

        //std::cout << sour << " || " << dest << std::endl;
        addEdge(sour, dest, datap, degree);
    }
    inFile.close();
}

/*
 * return the vertex with the smallest degree
 * @len is the @memnode number
 * @mindeg will return the smallest degree
 */
vid_t
smallestDegVertex(int *deg, int len, int &mindeg)
{

    vid_t res_vertex = 0;
    mindeg = -1;
    for(int i = 0; i != len; ++i){
        //std::cout << i << " ** " << deg[i] << std::endl;
        if( deg[i] >= 0 && mindeg < 0 ){
            res_vertex = i;
            mindeg = deg[i];
            continue;
        }
        if( deg[i] < 0 ) continue;
        if( deg[i] < mindeg ){
            res_vertex = i;
            mindeg = deg[i];
        }
    }

    return res_vertex;
}

/*
 * @argv[1]: input file path
 * @argv[2]: the exact total number of the input graph
 * @argv[3]: whether output the degeneracy ordering
 * @argv[4]: the number of memory nodes should be assigned to give redundancy
 * @argv[5]: the output file with degeneracy ordering infomation
 */
int
main(int argc, char **argv)
{
   
    if( argc < 4 )
    {
        std::cout << "Program inputfile totalvertex whetheroutput [InMemNode] [outputfile]";
        std::cout << std::endl;
        exit(1);
    }

    FILE *wfile;
    int ifoutput = atoi(argv[3]);
    if( ifoutput ){
        char *outputfilename = (char*)malloc(sizeof(char) * 200);
        if( argc == 6 ){
            strcpy( outputfilename, argv[5] );
        } else {
            strcpy( outputfilename, argv[1] );
            strcat( outputfilename, ".degeneracy.txt" );
        }
        wfile = fopen(outputfilename, "w");
        if( wfile == NULL ){
            std::cout << "open output file error" << std::endl;
            exit(1);
        }
    }

    int nodenum = atoi(argv[2]);
    int memnode = nodenum+100;
    if( argc > 4 ){

        memnode = atoi(argv[4]);
    }
    G = (vlist**)malloc(sizeof(vlist*) * memnode);
    degree = (int*)malloc(sizeof(int*) * memnode);
    for( int i = 0; i != memnode; ++i )
        G[i] = NULL;
    for( int i = 0; i != memnode; ++i )
        degree[i] = -1;

    readGraph(G, argv[1], degree);
    std::cout << "finish reading graph data" << std::endl;

    int remaining_vertex = nodenum;
    vid_t currv = 0;
    int   mindeg = 0;

    std::pair<vid_t, int> result;
    result.first = 0;
    result.second = -1;

    while(remaining_vertex != 0){
     
        currv = smallestDegVertex(degree, memnode, mindeg);
        if( mindeg > result.second ){
            result.first = currv;
            result.second = mindeg;
        }
        if( ifoutput ){
            //std::cout << currv << '\t' << mindeg << std::endl;
            fprintf(wfile, "%d:%d\n", currv, mindeg);
        }
        if( G[currv] == NULL )
        {
            std::cout << "current vertex's vlist pointer is NULL" << std::endl;
            std::cout << "The Vertex: " << currv << std::endl;
            exit(0);
        }
        omp_set_num_threads(4);
        int size = G[currv]->size();
#pragma omp parallel for schedule(dynamic)
        for( int i = 0; i < size; i++) {
            vid_t v = G[currv]->at(i);
            if( G[v] == NULL ) continue;
            --degree[v];
        }


        delete G[currv];
        G[currv] = NULL;
        degree[currv] = -1;
        --remaining_vertex;
    }

    fclose(wfile);
    std::cout << "The degeneracy: " << result.second << std::endl;
    std::cout << "Vertex ID: " << result.first << std::endl;

    return 1;
}

