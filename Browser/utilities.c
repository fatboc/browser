#include "header.h"


char * get_line(){

    char * line = NULL, * tmp = NULL;
    int ch = 0, count = 0;

    do{
        count++;

        tmp = realloc(line, sizeof(char)*count);

        if (tmp=NULL){
            free(line);
            line = NULL;
            break;
        }
        else{
        line = tmp;
        line[count-1] = ch;
        }
    }while ((ch = getch())!='\n');

    return (line);

}

// This is the function we pass to LC, which writes the output to a BufferStruct
static size_t WriteMemoryCallback
(void *ptr, size_t size, size_t nmemb, void *data)
{
size_t realsize = size * nmemb;

Buffer * buffer = (Buffer *) data;

buffer->data = realloc(buffer->data, buffer->size + realsize + 1);

if ( buffer->data )
{
memcpy( &( buffer->data[ buffer->size ] ), ptr, realsize );
buffer->size += realsize;
buffer->data[ buffer->size ] = '\0';
}
return realsize;
}

Buffer * get_page( CURL * myHandle, Buffer * output){

    //FILE * tmp;
    long http_code;
    double c_length;
    CURLcode result; // We’ll store the result of CURL’s webpage retrieval, for simple error checking.



    myHandle = curl_easy_init ( ) ;
    // Notice the lack of major error checking, for brevity
    curl_easy_setopt(myHandle, CURLOPT_URL, "http://www.wikipedia.com");
    curl_easy_setopt(myHandle, CURLOPT_WRITEDATA, (void *)output);

    curl_easy_setopt(myHandle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback); // Passing the function pointer to LC

    result = curl_easy_perform(myHandle);

    if(result==0) {
        printf("file downloaded\n");
    } else {
        printf("ERROR in dowloading file\n");
    }
    printf("get http return code\n");
    curl_easy_getinfo(myHandle, CURLINFO_RESPONSE_CODE, &http_code);
    printf("http code: %lu\n", http_code);

    printf("get size of download page\n");
    curl_easy_getinfo(myHandle, CURLINFO_SIZE_DOWNLOAD, &c_length);
    printf("length: %g\n", c_length);
    output->size = (int)c_length + 1;

    printf("END: close all files and sessions\n");


    //fclose(tmp);
return 0;
}
