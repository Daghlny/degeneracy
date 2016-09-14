

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

class inputbuffer;

vlist **g;

vid_t init_g(inputbuffer &, int);
void  init_bin(int *bin, vid_t *deg, vid_t *pos, vid_t *vert, vid_t md, vid_t nodenum);


