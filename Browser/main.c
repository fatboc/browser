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
    char url[MAXLEN]="www.example.com";

    Buffer * output, *robert;
    printw("%x", output);
    refresh();

    get_page(myHandle, output, url);
    refresh();
    //do{printw("ELO"); refresh();}
    while((ch = getch())!='q');

    view_page(myHandle, output, url);

    free(output->data);
    free(output);
    output=NULL;
    printw("%x", robert);
    refresh();
    follow_link(myHandle, url, robert);

    free(robert->data);
    free(robert);
    robert=NULL;
    curl_easy_cleanup( myHandle );
    endwin();			/* End curses mode		  */
    return 0;
}
