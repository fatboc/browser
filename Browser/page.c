#include "header.h"

int view_page(Buffer * buffer)
//Funkcja wyszukująca znaczniki i wyświetlająca pozostałą zawartość w postaci pojedynczych znaków
{

    WINDOW *pad_ptr;
    int pad_lines;
    int pad_cols;
    int count = 0, i=0;
    char znacznik[TAG_LEN];
    int len=0;
    char * tmp = buffer->data;

    Line *first = malloc(sizeof(Line));
    first->prev=NULL;
    first->next=NULL;

    Line * curr = first;

    int curr_line =0, nr_lines=1;

    while (count<buffer->size)//(*tmp!='\0'&&tmp!=0&&tmp)
    {


        if(strcmp(znacznik,"/html")==0) break;

        if (*tmp!='<')
        {
            if(i==MAXLEN-1||*tmp=='\n')
            {
                curr->contents[i]='\0';

                Line * another = malloc(sizeof(Line));
                curr->next=another;
                another->prev = curr;
                nr_lines++;
                curr = another;
            }
            curr->contents[i]=*tmp;
            //addch(*tmp);
            //refresh();
            tmp++;
            count++;
            i++;
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

            count+=len;
            tmp++;

        }
    }

    scrolling(first, nr_lines);

    while ((curr = first) != NULL)   // set curr to head, stop if list empty.
    {
        first = first->next;
        free(curr);                // delete saved pointer.
    }

    return 0;
}

void draw_lines(Line * curr, int nr_lines, int curr_line, int term_lines)
{
    clear();
    int tmp = curr_line;
    Line * tmp_line = curr;
    while(tmp_line!=NULL&& nr_lines - tmp < term_lines + 1)
    {
        draw(curr->contents);
        tmp++;
        tmp_line++;
    }
    curr++;
    curr_line++;
}


void draw(char * line)
{

    int count = 0;

    while (*line!='\n'&&*line!='\0'&&count<MAXLEN)
    {
        addch(*line);
        refresh();
        line++;
        count++;
    }
}

int scrolling(Line * head, int nr_lines){

    int ch;
    int term_rows, term_cols, curr_line = 0;

    getmaxyx(stdscr, term_rows, term_cols);

    Line * curr=head;

    while ((ch = getch()) != EOF && ch != 'q')
    {
        if (ch == KEY_UP && curr->prev != NULL)
        {
            curr_line--;
            curr = curr->prev;
        }
        else if (ch == KEY_DOWN && curr->next != NULL
                 && curr_line + term_rows < nr_lines)
        {
            curr_line++;
            curr = curr->next;
        }
        draw_lines(curr, nr_lines, curr_line, term_rows);
    }

    return(EXIT_SUCCESS);
}


char * geturl (char * url)
{

    return (0);

}
