#include "header.h"

int main()
{
    //powitanie();

    //char * url = malloc(sizeof(char)*URL_START);

    curl_global_init( CURL_GLOBAL_ALL );
    CURL * myHandle = curl_easy_init();

//    initscr();			/* Start curses mode 		*/
    //raw();				/* Line buffering disabled	*/
    //keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/

    Buffer * output;
    output->data = NULL;
    output->size = 0;

    get_page(myHandle, output);
    view_page(output);
    printf("LibCurl rules!\n");
    free(output->data);
    output->data =0;
    output->size = 0;
    curl_easy_cleanup( myHandle );
    return 0;
}
