#include "header.h"

int main()
{
    CURL * myHandle = curl_easy_init();

    init();

    Buffer * buffer;
    buffer->data = NULL;
    buffer->size = 0;

    browse(myHandle, buffer);

    endwin();
    curl_easy_cleanup(myHandle);
    free(buffer->data);
    buffer = NULL;

    return(EXIT_SUCCESS);
}
