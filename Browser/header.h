#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#define URL_START 50
#include <string.h>
#include <ncurses.h>

typedef struct Buffer{
    char * data;
    size_t size;
}Buffer;

void powitanie ();

char * get_url();

char * get_line();

Buffer * get_page( CURL * myHandle, Buffer * output);

int check_url (char * url);

void errurl_msg();

int view_page (Buffer * buffer);

int get_tag (char * tag);

void continue_msg();

void head_ignore();


static size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data);
