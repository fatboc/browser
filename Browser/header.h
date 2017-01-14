#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#define URL_START 50
#define TAG_LEN 250
#include <string.h>
#include <ncurses.h>
#define MAXLEN 128

typedef struct Line {
    char contents[MAXLEN];
    struct Line *prev, *next;
} Line;

typedef struct Buffer
{
    char * data;
    size_t size;
} Buffer;

void powitanie ();

char * get_url();

char * get_line();

int scrolling(Line * head, int nr_lines);

int get_page( CURL * myHandle, Buffer * output);

int check_url (char * url);

void errurl_msg();

int view_page (Buffer * buffer);

int * get_tag (char * tag, Buffer * result);

int handle_tag(char *tag);

void continue_msg();

void head_ignore();

static size_t save_to_buffer(void *ptr, size_t size, size_t nmemb, void *data);

void draw(char * line);

void draw_lines(Line * curr, int nr_lines, int curr_line, int term_lines);
