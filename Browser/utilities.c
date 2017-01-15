#include "header.h"

int init()
//funkcja rozpoczynająca działanie programu, ładuje podstawowe moduły oraz zmienia
//ustawienia początkowe
{
    curl_global_init( CURL_GLOBAL_ALL );  //inicjalizacja CURL

    initscr();                            //inicjalizacja ncurses
    raw();                                //znaki przekazywane bezpośrednio do programu
    noecho();                             //wprowadzone znaki nie pojawiają się w konsoli
    keypad(stdscr, TRUE);                 //dostęp do całej klawiatury

    return(EXIT_SUCCESS);

}

int browse(CURL * myHandle, Buffer * buffer)
//funckja łącząca pobranie i wyświetlenie strony
{
    int ch;

    char url[TAG_LEN]="http://www.example.com";

    get_page(myHandle, buffer, url);

    while((ch = getch())!='w')
    {
    int sh;
    while((sh = getch())!='q');

    char * tmp = view_page(buffer);

    clear();
    free(buffer->data);
    buffer->data=NULL;
    buffer->size=0;

    printw("%s\n", tmp);

    get_page(myHandle, buffer, tmp);
    }



    return(EXIT_SUCCESS);
}

char * get_line()
{

    char * line = NULL, * tmp = NULL;
    int ch = 0, count = 0;

    do
    {
        count++;

        tmp = realloc(line, sizeof(char)*count);

        if (tmp==NULL)
        {
            free(line);
            line = NULL;
            break;
        }
        else
        {
            line = tmp;
            line[count-1] = ch;
        }
    }
    while ((ch = getch())!='\n');

    return (line);

}


static size_t save_to_buffer(void *ptr, size_t size, size_t nmemb, void *data)
//właściwa funkcja zapisująca zawartość strony w zmiennej typu Buffer(struct Buffer)
{
    size_t tmp = size * nmemb;

    Buffer * buffer = (Buffer *) data;

    buffer->data = realloc(buffer->data, buffer->size + tmp + 1);

    if (buffer->data)
    {
        memcpy(&(buffer->data[buffer->size]), ptr, tmp);
        buffer->size += tmp;
        buffer->data[buffer->size-1] = '\0';
    }
    return tmp;
}

int get_page(CURL * myHandle, Buffer * buffer, char * url)
//funkcja pobierająca informacje o stronie oraz jej zawartość i ją w strukturze Buffer
{
    double length;
    long code;
    CURLcode result;

    printw("Pobieranie...\n");
    refresh();
    curl_easy_setopt(myHandle, CURLOPT_URL, url);

    //wybór adresu url żądanej strony
    curl_easy_setopt(myHandle, CURLOPT_FOLLOWLOCATION, 1L);
    //podążanie za adresem w razie przekierowań
    curl_easy_setopt(myHandle, CURLOPT_WRITEDATA, (void*)buffer);

    //wybór miejsca zapisu danych (struktura Buffer)
    curl_easy_setopt(myHandle, CURLOPT_WRITEFUNCTION, save_to_buffer);
    //wybór funcji zapisującej dane

    result = curl_easy_perform(myHandle);

    if(result==0)
    {
        printw("Plik pobrany.\n");
    }
    else
    {
        printw("Blad pobierania. Kod: %d\n", result);
    }

    curl_easy_getinfo(myHandle, CURLINFO_SIZE_DOWNLOAD, &length);
    //pobieranie informacji o długości pliku
    printw("Dlugosc: %.0f\n", length);

    curl_easy_getinfo(myHandle, CURLINFO_RESPONSE_CODE, &code);
    //pobieranie kodu http
    printw("Kod HTTP: %d\n", code);

    buffer->size = (int)length;

    printw("\nWsicnij 'q' by wyswietlic strone.\n");
    refresh();

    return (int)result;
}

int * get_tag(char * tag, Buffer * result)
//Funkcja zapisująca treść znacznika w zmiennej Buffer
{
    char tmp[TAG_LEN];
    int len=0;

    while(*tag!='>'&&*tag!='\0'&&tag)
    {
        tmp[len]=*tag;
        tag++;

        if (len<TAG_LEN-1)
        {
            len++;
            tmp[len]='\0';
        }
        else
        {
            len =0;
            tmp[TAG_LEN-1]='\0';
        }

    }
    tag++;

    //result = malloc(sizeof(tmp)+sizeof(len));

    //strcpy(result->data, tmp);

    result->data=tmp;

    result->size=len;

    return(0);
}

int line_count(Buffer * buffer, int pad_cols)
//funcja zliczajaca ilosc wierszy wyswietlanej strony
{
    char znacznik[TAG_LEN]="";

    int len=0, head=0, comment=0;
    char * tmp = buffer->data;
    //zmienne podobnie jak w view_page()

    int lines=1;

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
                lines++;
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
            if (!strcmp(znacznik, "br")||!strcmp(znacznik, "/p")||!strcmp(znacznik, "/div")||
                !strcmp(znacznik, "/h1")||!strcmp(znacznik, "/tr")) lines++; //formatowanie wymusza nową linię

            if(comment&&*(tmp-2)=='-'&&*(tmp-1)=='-'&&*tmp=='>') comment=0;
            tmp++;
        }
    }
    return lines;
}

void str_to_lower(char * str)
{
    int i;
    for (i=0; i<strlen(str); i++)
        str[i]=tolower(str[i]);
}
