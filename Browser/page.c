#include "header.h"

int view_page(CURL * myHandle, Buffer * buffer, char * url)
//Funkcja wyszukująca znaczniki i wyświetlająca pozostałą zawartość w postaci pojedynczych znaków
{
clear();

    WINDOW *pad_ptr;
    int pad_lines, pad_cols;
    char znacznik[TAG_LEN];
    int len=0, count=0;
    char * tmp = buffer->data;
    int StartX, StartY;

    if(COLS>MAXLEN)
        pad_cols = MAXLEN;
    else
        pad_cols = COLS;

    pad_lines = line_count(buffer, pad_cols);

    pad_ptr = newpad(pad_lines, pad_cols);

    int Key, cols = 0, Choice =0;
    int head = 0;

    int Height = LINES;

    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_RED, COLOR_WHITE);

    wbkgd(pad_ptr,COLOR_PAIR(1));


    WINDOW * SubWin;

    StartX = (COLS - pad_cols)/2;
    StartY = (LINES - pad_lines)/2;



    while (count<buffer->size)//(*tmp!='\0'&&tmp!=0&&tmp)

    {
        str_to_lower(znacznik);
        if(strcmp(znacznik,"/html")==0) break;


        if (*tmp!='<')
        {
        if (len>3)
                {
                    if(strncmp(znacznik, "head", 4)==0)
                    {
                        head++;
                    }
                    if(strncmp(znacznik, "/head", 5)==0)
                    {
                        head=0;
                    }
                }
            if(tmp!='\n'&&tmp!='\r'&&head==0){
            waddch(pad_ptr, *tmp);
            refresh();}
            tmp++;
            count++;
        }
        else
        {
            tmp++;
            len=0;
            while(*tmp!='>'&&*tmp!='\0'&&tmp)
            {
                znacznik[len]=*tmp;
                tmp++;

                if (len<TAG_LEN-1)
                {
                    len++;
                    znacznik[len]='\0';
                }
                else
                {
                    len=0;
                    znacznik[TAG_LEN-1]='\0';
                }

            }
            tag_handler(myHandle, znacznik, pad_ptr, buffer, url);
            count+=len;
            tmp++;

        }
    }

    int ch;
    while(ch=getch()!='q');


    while (Choice == 0)
    {
        SubWin = subpad(pad_ptr, LINES, COLS, cols, 0);
        keypad(SubWin, true);

        prefresh(SubWin, 0,0,0,0,LINES,COLS);
        Key = wgetch(SubWin);
        switch(Key)
        {
        case KEY_UP:
        {
            if (cols <= 0) continue;
            cols--;
            break;
        }
        case KEY_DOWN:
        {
            if (cols+Height+1 >= pad_lines) continue;
            cols++;
            break;
        }
        case KEY_PPAGE: /*Page Up*/
        {
            if (cols <= 0) continue;
            cols -= Height;
            if (cols < 0) cols = 0;
            break;
        }
        case KEY_NPAGE: /*Page Down*/
            if (cols+Height+1 >= pad_lines) continue;
            cols += Height;
            if (cols+Height+1 > pad_lines) cols = pad_lines-Height-1;
            break;
        case KEY_HOME:
            cols = 0;
            break;
        case KEY_END:
            cols = pad_lines-cols-1;
            break;
        case 'q': /* Enter */
        {
            Choice = 1;
            break;
        }
        }
        wclear(SubWin);
        delwin(SubWin);
    }
    delwin(pad_ptr);
    printw("%s", url); refresh();
        int c;
        while(c=getch()!='q');

   // free(url);
    return 0;
}

char * geturl (char * url)
{

    return (0);

}


