#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#define URL_START 50
#define TAG_LEN 250
#include <string.h>
#include <ncurses.h>
#define MAXLEN 128
#include <ctype.h>


typedef struct Buffer
{
    char * data;
    size_t size;
} Buffer;

int init();

void powitanie ();

void str_to_lower(char * str);

int line_count(Buffer * buffer, int pad_cols);

char * get_url();

char * get_line();

int get_page(CURL * myHandle, Buffer * output, char * url);

int check_url (char * url);

void errurl_msg();

int view_page (CURL * myHandle, Buffer * buffer, char * url);

int * get_tag (char * tag, Buffer * result);

int tag_handler(CURL * myHandle, char *tag, WINDOW * window, Buffer * buffer, char * url);

void continue_msg();

void head_ignore();

static size_t save_to_buffer(void *ptr, size_t size, size_t nmemb, void *data);

int pad_scroll(Buffer * buffer, WINDOW * subwin, int StartY, int StartX, int Height, int Width);

int follow_link(CURL * myHandle, char * url, Buffer * buffer);
