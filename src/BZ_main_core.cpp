
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

using std::string;
using std::vector;

typedef int vid_t;
typedef vector<vid_t> vlist;


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
 * read the whole graph data into memory
 */
void
readGraph(vlist** g, char *infilename)
{
    std::ifstream infile;
    infile.open(infilename);

    char inbuff[1024];
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


int main(int argc, char **argv)
{

    if( argc < 2 )
    {
        printf("the parameters number error\n");
        exit(0);
    }
    printf("The program begin running\n");

    vlist **g;      // store the graph's adjcency lists
    vid_t *pos;     // every vertex's position in @vert array
    vid_t *bin;     // every possible degree's first vertex index in @vert
    vid_t *vert;    // the sorted vertices array

    // the reading data phase
    printf("begin reading graph data in %s\n", argv[1]);
    readGraph( g, argv[1] );
    printf("finish reading graph data\n");

    

}


