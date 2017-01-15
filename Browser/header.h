#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#define URL_START 50
#define TAG_LEN 250
#include <string.h>
#include <ncurses.h>
#define MAXLEN 128


typedef struct Buffer
{
    char * data;
    size_t size;
} Buffer;

int init();

void powitanie ();

int line_count(Buffer * buffer, int pad_cols);

void str_to_lower(char * str);

char * get_url();

char * get_line();

int get_page( CURL * myHandle, Buffer * output);

int check_url (char * url);

void errurl_msg();

int view_page (Buffer * buffer);

int * get_tag (char * tag, Buffer * result);

int handle_tag(char *tag);

void continue_msg();

void head_ignore();

static size_t save_to_buffer(void *ptr, size_t size, size_t nmemb, void *data);

int pad_scroll(Buffer * buffer, WINDOW * subwin, int StartY, int StartX, int Height, int Width);

int tag_handler(char * tag, WINDOW * window, Buffer * buffer);
