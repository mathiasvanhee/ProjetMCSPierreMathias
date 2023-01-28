#include "application.h"
int diffusionEnCours = 0;

int main(int argc, char const *argv[])
{
    int sock = creerSocket(SOCK_STREAM);
    connectSrv(sock, "127.0.0.1", 5250);
    dialogueAvecServeur(sock);
    close(sock);
    return 0;
}

void dialogueAvecServeur(int sd)
{
    PAUSE("PAUSE");
    Menu();
    // serveurClient();
}

/**
 * @brief création d'un serveur d'écoute pour la diffuion
 *
 */
void serveurClient()
{
    int se;
    char IP[16];
    char desc[MAX_BUFF];
    int port;

    pthread_t tid;

    printf("Veuillez entrer l'adresse IP du serveur : \n");
    scanf("%s", IP);
    printf("Veuillez entrer le port d'écoute du serveur : \n");
    scanf("%d", &port);
    printf("Veuillez entrer une description sur la diffusion :\n");
    viderBuffer();
    fgets(desc, sizeof(desc), stdin); // fgets permet de récupérer les espaces

    printf("%s\n", IP);
    printf("%d\n", port);
    printf("%s\n", desc);

    // demarrerVideo(); //lance la capture vidéo

    se = creerSockAddrEcoute(IP, port, 5);
    diffusionEnCours = 1;
    printf("passage\n");
    pthread_create(&tid, NULL, (pf_t)threadEcoute, &se);
    while (diffusionEnCours)
        ;

    // arreterVideo();
    close(se);
}

void *threadEcoute(int *se)
{

    /* printf("hello xterm\nappuyer sur enter");
    getchar();
    exit(0);
    system("xterm"); */

    int nbThread = 0;
    struct sockaddr_in clt;
    int sd;
    pthread_t tids[NBMAX_THREADS];

    while (diffusionEnCours)
    {
        sd = attenteAppel(*se, &clt);
        pthread_create(&tids[nbThread++], NULL, (pf_t)diffusion, &sd);
    }

    for (int i = 0; i < nbThread; i++)
        pthread_join(tids[i], NULL);

    pthread_exit(NULL);
}

void *diffusion()
{
    pthread_exit(NULL);
}

void Menu()
{
    initscr();
    noecho();
    cbreak();
    curs_set(0);

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    WINDOW *menu = newwin(10, 40, yMax / 2, xMax / 2);
    // box(menu, 0, 0);
    refresh();
    wrefresh(menu);

    // autorise les fleches du clavier
    keypad(menu, TRUE);

    char *choix[3] = {"Voir la liste des diffusions", "Lancer une diffusion", "Quitter"};
    int action;
    int selection = 0;

    while (1)
    {
        for (int i = 0; i < 3; i++)
        {
            if (i == selection)
                wattron(menu, A_REVERSE); // met en surbrillance le choix actuel
            mvwprintw(menu, i + 1, 1, choix[i]);
            wattroff(menu, A_REVERSE);
        }
        action = wgetch(menu);

        switch (action)
        {
        case KEY_UP:
            selection--;
            selection = (selection < 0) ? 2 : selection;
            break;
        case KEY_DOWN:
            selection++;
            selection = (selection > 2) ? 0 : selection;
            break;
        default:
            break;
        }
        if (action == 10)
        {
            printw("Vous avez choisi : %s", choix[selection]);
            refresh();
            switch (selection)
            {
            case 0:
                // afficherListeDiffusion();
                break;
            case 1:
                // serveurClient();
                break;
            case 2:
                endwin();
                exit(0);
                break;
            default:
                break;
            }
        }
    }
    // fermeture du menu

    /*WINDOW *w;
    char list[2][28] = {"Voir la liste des diffusions", "Lancer une diffusion"};
    char item[28];
    int ch, i = 0;

    int termwidth = getmaxy(stdscr);
    int termheight = getmaxx(stdscr);

    initscr();                // initialize Ncurses
    w = newwin(10, 40, termheight/2, termwidth/2); // create a new window
    //box(w, 0, 0);             // sets default borders for the window
    // now print all the menu items and highlight the first one

    for (i = 0; i < 5; i++)
    {
        if (i == 0)
            wattron(w, A_STANDOUT); // highlights the first item.
        else
            wattroff(w, A_STANDOUT);
        sprintf(item, "%s", list[i]);
        mvwprintw(w, i + 1, 2, "%s", item);
    }
    wrefresh(w); // update the terminal screen
    i = 0;
    noecho();        // disable echoing of characters on the screen
    keypad(w, TRUE); // enable keyboard input for the window.
    curs_set(0);     // hide the default screen cursor.
    init_pair(1, COLOR_MAGENTA, COLOR_RED);
    // get the input
    while ((ch = wgetch(w)) != 'q')
    {
        // right pad with spaces to make the items appear with even width.

        sprintf(item, "%s", list[i]);
        mvwprintw(w, i + 1, 2, "%s", item);

        // use a variable to increment or decrement the value based on the input.
        switch (ch)
        {
        case KEY_UP:
            i--;
            i = (i < 0) ? 4 : i;
            addstr("up");
            break;
        case KEY_DOWN:
            i++;
            i = (i > 4) ? 0 : i;
            break;
            addstr("down");
            refresh();
        }
        // now highlight the next item in the list.
        wattron(w, A_STANDOUT);
        sprintf(item, "%s", list[i]);
        mvwprintw(w, i + 1, 2, "%s", item);
        wattroff(w, A_STANDOUT);
    }
    delwin(w);
    endwin();*/
}

void viderBuffer()
{
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}