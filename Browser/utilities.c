#include "header.h"

int init()
{

    //powitanie();

    //char * url = malloc(sizeof(char)*URL_START);

    curl_global_init( CURL_GLOBAL_ALL );

    initscr();
    raw();
    //noecho();
    keypad(stdscr, TRUE);
    scrollok(stdscr,TRUE);
    idlok(stdscr,TRUE);
    nonl();

    return(0);

}

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
    curl_easy_setopt(myHandle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(myHandle, CURLOPT_WRITEDATA, (void*)output);
    curl_easy_setopt(myHandle, CURLOPT_WRITEFUNCTION, save_to_buffer);

    result = curl_easy_perform(myHandle);

    if(result==0)
    {
        printw("file downloaded\n");
    }
    else
    {
        printw("ERROR in dowloading file\n");
    }

    printw("get size of download page\n");
    curl_easy_getinfo(myHandle, CURLINFO_SIZE_DOWNLOAD, &length);
    printw("length: %g\n", length);
    output->size = (int)length;

    printw("END: close all files and sessions\n");

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

int line_count(Buffer * buffer, int pad_cols)
{

    int fts=1, x = 0, count=0, len=0;
    char * tmp = buffer->data;
    char znacznik[TAG_LEN];
    while (count<buffer->size)
    {
        if(strcmp(znacznik,"/html")==0) break;

        if (*tmp!='<')
        {
            if (x==pad_cols||*tmp=='\n')
            {
                fts++;
                x=0;
            }
            tmp++;
            count++;
            x++;
        }
        else
        {
            tmp++;
            len=0;
            while(*tmp!='>'&&*tmp!='\0'&&tmp)
            {
                znacznik[len]=*tmp;
                tmp++;

                if (len<TAG_LEN-1)
                {
                    len++;
                    znacznik[len]='\0';
                }
                else
                {
                    len=0;
                    znacznik[TAG_LEN-1]='\0';
                }

            }
            count+=len;
            tmp++;

        }
    }

    return(fts);
}

void str_to_lower(char * str)
{
    int i;
    for (i=0; i<strlen(str); i++)
        str[i]=tolower(str[i]);
}
