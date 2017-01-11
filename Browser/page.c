#include "header.h"

int view_page(Buffer * buffer){

    int count = 0;
    char * tmp = buffer->data;
    //printf("%s\n\n\n", tmp);
    while (count<buffer->size){
        putchar(*tmp);
        //refresh();
        tmp++;
        count ++;
    }

    return 0;
}


char * geturl (char * url){

return (EOF);

}
