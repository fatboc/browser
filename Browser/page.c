#include "header.h"

int view_page(Buffer * buffer)
//Funkcja wyszukująca znaczniki i wyświetlająca pozostałą zawartość w postaci pojedynczych znaków
{
    int count = 0;
    char znacznik[TAG_LEN];
    int len=0;
    char * tmp = buffer->data;

    while (count<buffer->size)//(*tmp!='\0'&&tmp!=0&&tmp)
    {
        if(strcmp(znacznik,"/html")==0) break;

        if (*tmp!='<')
        {
            putchar(*tmp);
            //refresh();
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
            //printf("\n----------\ntag:%s; len:%d;\n----------\n", znacznik, len);
            count+=len;
            tmp++;

        }
    }
    return 0;
}


char * geturl (char * url)
{

    return (0);

}
