#include "header.h"

int main()
{

    CURL * myHandle = curl_easy_init();
    curl_global_init( CURL_GLOBAL_ALL );

    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    scrollok(stdscr,TRUE);
    idlok(stdscr,TRUE);
    nonl();

    int ch;

    Buffer * output;
    output->data = NULL;
    output->size = 0;

    get_page(myHandle, output);
    refresh();
    //do{printw("ELO"); refresh();}
    while((ch = getch())!='q');

    view_page(output);
    printw("\nLibCurl rules!\n");
    refresh();

    curl_easy_cleanup( myHandle );
    endwin();			/* End curses mode		  */
    return 0;
}
