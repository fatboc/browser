#include "header.h"

void * view_page(Buffer * buffer)
//Funkcja wyszukująca znaczniki i wyświetlająca pozostałą zawartość w postaci pojedynczych znaków
{

    WINDOW *pad_ptr;

    int pad_lines;
    char znacznik[TAG_LEN]="";

    int len=0, head=0, comment=0;
    char * tmp = buffer->data;
    //len - długość znacznika, head - obecna pozyzcja jest częścią el. head
    //comment - obecna pozycja jest częścią komentarza, tmp - zmienna pomocnicza

    void * result;
    static char * url;

    pad_lines = line_count(buffer, COLS);

    pad_ptr = newpad(pad_lines, COLS);

    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    wbkgd(pad_ptr,COLOR_PAIR(1));

    while ((strcmp(znacznik,"/html")!=0)&&tmp)
    {
        if (*tmp!='<')
        {
            if (len>1)
            {
                if(strncmp(znacznik, "head", 4)==0)
                {
                    head++; //ignorujemy wszystko do końca el. head
                }
                if(strncmp(znacznik, "/head", 5)==0)
                {
                    head=0;
                }
            }
            if(*tmp!='\n'&&*tmp!='\r'&&head==0&&comment==0)
            {
                waddch(pad_ptr, *tmp);
                refresh();
            }
            tmp++;
        }
        else //zaczyna się znacznik
        {
            tmp++;
            len=0;
            while(*tmp!='>'&&*tmp!='\0'&&(!comment||(comment&&strncmp(tmp-2, "-->", 3))))
            {
                if((strncmp(znacznik, "!--", 3)==0))
                {
                    comment++;//ignorujemy wszystko do końca komentarza
                }
                znacznik[len]=*tmp;
                tmp++;

                if (len<TAG_LEN-1)
                {
                    len++;
                    znacznik[len]='\0';
                }
                else //jeśli znacznik jest wyjątkowo długi
                {
                    len=0;
                    znacznik[TAG_LEN-1]='\0';
                }

            }
            result = tag_handler(znacznik, pad_ptr, buffer);
            if(result!=NULL) url=result;
            if(comment&&*(tmp-2)=='-'&&*(tmp-1)=='-'&&*tmp=='>') comment=0;
            tmp++;
        }
    }
    pad_scroll(buffer, pad_ptr, pad_lines, COLS);
    delwin(pad_ptr);
    return url;
}


int pad_scroll(Buffer * buffer, WINDOW * pad_ptr, int pad_height, int width)
{
    int key, lines=0, end=0;

    WINDOW * window;
    while (end==0)
    {
        mvprintw(LINES-1, 0, "Nacisnij 'q' by zakonczyc.");
        refresh();
        window = subpad(pad_ptr, LINES-1, COLS, lines, 0);
        keypad(window, true);
        printw("%d, ", prefresh(window, 0,0,0,0,LINES-1,width));
        refresh();
        key = wgetch(window);
        switch(key)
        {
        case KEY_UP:
        {
            if (lines <= 0) continue;
            lines--;
            break;
        }
        case KEY_DOWN:
        {
            if (lines+LINES+1 >= pad_height) continue;
            lines++;
            break;
        }
        case KEY_PPAGE: /*Page Up*/
        {
            if (lines <= 0) continue;
            lines -= LINES;
            if (lines < 0) lines = 0;
            break;
        }
        case KEY_NPAGE: /*Page Down*/
            if (lines+LINES+1 >= pad_height) continue;
            lines += LINES;
            if (lines+LINES+1 > pad_height) lines = pad_height-LINES-1;
            break;
        case KEY_HOME:
            lines = 0;
            break;
        case KEY_END:
            lines = pad_height-LINES-1;
            break;
        case 'q': /* Enter */
        {
            end = 1;
            break;
        }
        }
        delwin(window);
    }
    return(0);
}
