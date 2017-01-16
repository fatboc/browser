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
    int ch=0,i;

    char * url;

    url = get_url();

    get_page(myHandle, buffer, url);

    while(ch==0)
    {
        int c;
        while(c=getch()!='q');

        Link * tmp = view_page(buffer);

        if (tmp==NULL)
        {
            ch=1;
            break;
        }

        char * text = choose_link(tmp);

        if(text==NULL)
        {
            ch=1;
            break;
        }

        clear();
        free(buffer->data);
        buffer->data=NULL;
        buffer->size=0;

        get_page(myHandle, buffer, text);
        for(i=0; i<URL_START; i++)
            free(tmp[i].url);
    }

    return(EXIT_SUCCESS);
}

char * get_line()
{

    char * line = NULL, * tmp = NULL;
    int ch = 0, count = 0;

    while ((ch = getch())!='\n')
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

    count++;
    tmp = realloc(line, sizeof(char)*count);

    if (tmp==NULL)
    {
        free(line);
        return(NULL);
    }
    else
    {
        line = tmp;
        line[count-1] = '\0';
    }
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

    printw("Pobieranie \"%s\"...\n", url);
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

    printw("\nNacisnij 'q' by wyswietlic strone.\n");
    refresh();

    return (int)result;
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

char * get_url()
{
    static char * url;
    echo();
    keypad(stdscr, false);
    printw("Wprowadz adres:\n");
    refresh();
    url = get_line();

    noecho();
    keypad(stdscr, true);
    return(url);
}

char * choose_link(Link * links)
{

    WINDOW *pad_ptr;
    int pad_lines = 155;
    int count=0;
    pad_ptr = newpad(pad_lines, COLS);

    wprintw(pad_ptr, "Znalezione linki:\n\n");

    while(links[count].index!=0)
    {
        wprintw(pad_ptr, "(%d) %s\n\n", links[count].index, links[count].url);
        count++;
    }
    int i = pad_scroll(pad_ptr, pad_lines, COLS);
    delwin(pad_ptr);
    if (i==-1) return NULL;
    if(links[0].url==NULL) return NULL;

    clear();
    printw("Wpisz numer linku, ktory chcialbys wyswietlic:\n");
    echo();
    char * choice;
    choice = get_line();
    int x = atoi(choice);
    if(x>count)
    {printw("NIE. NIE %d.\n", x);
    refresh();
    int c;
    while(c=getch()!='q');
    return NULL;
    }
    static char * result;
    result = links[x-1].url;

    return(result);
}
