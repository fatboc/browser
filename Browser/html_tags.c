#include "header.h"

int tag_handler(CURL * myHandle, char * tag, WINDOW * window, Buffer * buffer, char * url)
{
    str_to_lower(tag);

//    {}

    if(strcmp(tag, "b")==0)
    {
        wattron(window, A_BOLD);
    }

    if(strcmp(tag, "/b")==0)
    {
        wattroff(window, A_BOLD);
    }
    if(strcmp(tag, "u")==0)
    {
        wattron(window, A_UNDERLINE);
    }
    if(strcmp(tag, "/u")==0)
    {
        wattroff(window, A_UNDERLINE);
    }
    if(strcmp(tag, "i")==0)
    {
        wattroff(window, A_ITALIC);
    }

    if(strcmp(tag, "/i")==0)
    {
        wattroff(window, A_ITALIC);
    }
    if(strcmp(tag, "strong")==0)
    {
        wattron(window, A_BOLD);
    }
    if(strcmp(tag, "/strong")==0)
    {
        wattroff(window, A_BOLD);
    }
    if(strcmp(tag, "h1")==0)
    {
        wattron(window, A_STANDOUT);
    }
    if(strcmp(tag, "/h1")==0)
    {
        wattroff(window, A_STANDOUT);
    }
    if(strcmp(tag, "em")==0)
    {
        wattron(window, A_BOLD);
        wattron(window, A_ITALIC);
    }
    if(strcmp(tag, "/em")==0)
    {
        wattroff(window, A_BOLD);
        wattron(window, A_ITALIC);
    }
    if(strcmp(tag, "br")==0)
    {
        waddch(window, '\n');
    }
    if(strcmp(tag, "/p")==0)
    {
        waddch(window, '\n');
    }
    if(strcmp(tag, "/div")==0)
    {
        waddch(window, '\n');
    }
    if(strncmp(tag, "a href", 6)==0)
    {
        int len=0;
        init_pair(2, COLOR_BLUE, COLOR_WHITE);
        wattron(window, COLOR_PAIR(2));
        wattron(window, A_UNDERLINE);
        while(*tag!='\"')
            tag++;
            tag++;
        while(*tag!='\"'){len++;
            tag++;}
        tag-=len;
        //url = realloc(url, sizeof(char)*(len+1));

        strncpy(url, tag, len);

        url[len]='\0';
        printw("%s", url); refresh();
        int c;
        while(c=getch()!='q');

        //follow_link(myHandle, window, url, buffer);

    }
    if(strcmp(tag, "/a")==0)
    {
        wattroff(window, COLOR_PAIR(2));
        wattroff(window, A_UNDERLINE);
    }


    return 0;
}
