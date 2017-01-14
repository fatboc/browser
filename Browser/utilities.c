#include "header.h"


char * get_line()
{

    char * line = NULL, * tmp = NULL;
    int ch = 0, count = 0;

    do
    {
        count++;

        tmp = realloc(line, sizeof(char)*count);

        if (tmp==NULL)
        {
            free(line);
            line = NULL;
            break;
        }
        else
        {
            line = tmp;
            line[count-1] = ch;
        }
    }
    while ((ch = getch())!='\n');

    return (line);

}


static size_t save_to_buffer(void *ptr, size_t size, size_t nmemb, void *data)
//właściwa funkcja zapisująca zawartość strony w zmiennej typu Buffer(struct Buffer)
{
    size_t realsize = size * nmemb;

    Buffer * buffer = (Buffer *) data;

    buffer->data = realloc(buffer->data, buffer->size + realsize + 1);

    if (buffer->data)
    {
        memcpy( &( buffer->data[ buffer->size ] ), ptr, realsize );
        buffer->size += realsize;
        buffer->data[buffer->size-1] = '\0';
    }
    return realsize;
}

int get_page(CURL * myHandle, Buffer * output)
//funkcja pobierająca plik tekstowy i zapisująca jego zawartość w strukturze Buffer
{
    double length;
    CURLcode result;

    curl_easy_setopt(myHandle, CURLOPT_URL, "http://www.example.com");
    curl_easy_setopt(myHandle, CURLOPT_WRITEDATA, (void*)output);
    curl_easy_setopt(myHandle, CURLOPT_WRITEFUNCTION, save_to_buffer);

    result = curl_easy_perform(myHandle);

    if(result==0)
    {
        printf("file downloaded\n");
    }
    else
    {
        printf("ERROR in dowloading file\n");
    }

    printf("get size of download page\n");
    curl_easy_getinfo(myHandle, CURLINFO_SIZE_DOWNLOAD, &length);
    printf("length: %g\n", length);
    output->size = (int)length;

    printf("END: close all files and sessions\n");

    return (int)result;
}

int * get_tag(char * tag, Buffer * result)
//Funkcja zapisująca treść znacznika w zmiennej Buffer
{
    char tmp[TAG_LEN];
    int len=0;

    while(*tag!='>'&&*tag!='\0'&&tag)
    {
        tmp[len]=*tag;
        tag++;

        if (len<TAG_LEN-1)
        {
            len++;
            tmp[len]='\0';
        }
        else
        {
            len =0;
            tmp[TAG_LEN-1]='\0';
        }

    }
    tag++;

    //result = malloc(sizeof(tmp)+sizeof(len));

    //strcpy(result->data, tmp);

    result->data=tmp;

    result->size=len;

    return(0);
}
