#include "header.h"

int main()
{
    //powitanie();

    //char * url = malloc(sizeof(char)*URL_START);

    curl_global_init( CURL_GLOBAL_ALL );
    CURL * myHandle = curl_easy_init();

    initscr();			/* Start curses mode 		*/
    raw();
    noecho();			/* Line buffering disabled	*/
    keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
    //scrollok(stdscr,TRUE);
    idlok(stdscr,TRUE);

    nonl();


    Buffer * output;
    output->data = NULL;
    output->size = 0;
char c;

    get_page(myHandle, output);
    view_page(output);
    printf("LibCurl rules!\n");

    curl_easy_cleanup( myHandle );
    endwin();			/* End curses mode		  */
    return 0;
}
