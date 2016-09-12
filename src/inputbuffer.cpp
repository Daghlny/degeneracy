
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "inputbuffer.hpp"

using std::string;

class inputbuffer;

/*
 * get a line marked by @start and @end
 * returnvalue < 0 if the file is over
 * returnvalue > 0 is the characters in this line
 */
int
inputbuffer::getline(char *&start, char *&end)
{
    if( curpos >= endpos ){
        int res = FillInputBuffer();
        if( res == 0 ){
            printf("the input buffer size is too small\n");
            exit(0);
        }
        if( res < 0 )
            return res;
    }
    start = curpos;
    while( *curpos != '\n' ) curpos++;
    end = curpos-1;
    if( ++curpos >= endpos )
    {
        end = curpos;
        if( feof(file) ) return -1;
        FillInputBuffer();
    }
    return end - start;
}

/*
 * fill the input buffer by the file data
 * return value > 0 represents the bytes read from file
 * return value == 0 represents the input buffer is full
 * return value < 0 represents the file have touched EOF
 */
int
inputbuffer::FillInputBuffer()
{
    int i = endpos - curpos;
    if( i >= size ){
        return 0;
    }

    if(curpos < endpos)
    {
        memmove(buff, curpos, i);
        endpos = buff + i;
        curpos = buff;
    } else {
        curpos = buff;
        endpos = buff;
    }

    if(!feof(file))
    {
        int byteread = fread( endpos, 1, size-i, file );
        endpos += size-i;
        if( byteread < size-i ){
            if(!feof(file)){
                if(ferror(file)){
                    perror("Error reading from input file. Abort!\n");
                    exit(0);
                }
            }
        }
    }

}

size_t
inputbuffer::getsize(){
    return size;
}


