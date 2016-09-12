
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
        int res = FillInputBuffer(curpos);
        if( res == 0 )
        {
            printf("the input buffer size is too small\n");
            exit(0);
        }
        if( res < 0 )
            return res;
    }

    start = curpos;
    while( *curpos != '\n'){
        if( curpos > endpos ){
            /* 
             * if the @curpos > @endpos, it means we have already read all data in 
             * inputbuffer, but we still attach the line's end
             * so we need to get more data from file
             */
            int byteread = FillInputBuffer(start);
            //printf("byteread: %d, curpos: %d\n", byteread, curpos);
            start = curpos;
            curpos += size-byteread;
        }
        curpos++;
    }
    end = curpos-1;

    if( ++curpos >= endpos )
    {
        end = curpos;
        if( feof(file) ) return -1;
        FillInputBuffer(curpos);
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
inputbuffer::FillInputBuffer(char *pos)
{
    //printf("endpos: %d, pos: %d\n", endpos, pos);
    int i = endpos - pos;
    if( i >= size ){
        return 0;
    }

    if(pos < endpos)
    {
        memmove(buff, pos, i);
        endpos = buff + i;
        curpos = buff;
    } else if(pos == endpos){
        curpos = buff;
        endpos = buff;
    } else {
        printf(" the @pos > @endpos in @FillInputBuffer()\n");
        exit(0);
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


