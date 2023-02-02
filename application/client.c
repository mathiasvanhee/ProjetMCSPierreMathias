#include "application.h"
int diffusionEnCours = 0;
int se;
int sockDialogueServPrincipal;

int main(int argc, char const *argv[])
{
    sockDialogueServPrincipal = creerSocket(SOCK_STREAM);
    connectSrv(sockDialogueServPrincipal, "127.0.0.1", 5250);
    dialogueAvecServeur(sockDialogueServPrincipal);
    close(sockDialogueServPrincipal);
    return 0;
}

void dialogueAvecServeur(int sd)
{
    PAUSE("PAUSE");
    while (Menu() != 0)
        ;
}

/**
 * @brief Menu de l'application
 *
 */
int Menu()
{
    printf("entrer menu");
    initscr();
    noecho();
    cbreak();
    curs_set(0);

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    WINDOW *menu = newwin(10, 40, yMax / 2, xMax / 2);
    // box(menu, 0, 0);
    printw("Dans menu");

    refresh();
    wrefresh(menu);

    // autorise les fleches du clavier
    keypad(menu, TRUE);

    char choix[3][30] = {"Voir la liste des diffusions", "Lancer une diffusion", "Quitter"};

    if (diffusionEnCours == 1)
    {
        strcpy(choix[1], "Arreter la diffusion");
    }

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
                afficherListeDiffusion();
                break;
            case 1:
                if (diffusionEnCours == 1)
                {
                    diffusionEnCours = 0;
                    strcpy(choix[1], "Lancer une diffusion");
                }
                else
                {
                    endwin();
                    serveurClient();
                    return 1;
                }
                break;

            case 2:
                endwin();
                return 0;
                break;
            default:
                break;
            }
        }
    }
}

void afficherListeDiffusion()
{
}

/**
 * @brief création d'un serveur d'écoute pour la diffuion
 *
 */
void serveurClient()
{
    char IP[16];
    char desc[MAX_DESC];
    int port;

    pthread_t tid;

    /*printf("Veuillez entrer l'adresse IP du serveur : \n");
    scanf("%s", IP);*/
    printf("Veuillez entrer le port d'écoute du serveur : \n");
    scanf("%d", &port);
    printf("Veuillez entrer une description sur la diffusion :\n");
    viderBuffer();
    fgets(desc, sizeof(desc), stdin); // fgets permet de récupérer les espaces

    /*printf("%s\n", IP);*/
    printf("%d\n", port);
    printf("%s\n", desc);
    
    se = creerSockAddrEcoute("0.0.0.0", port, 5);

    if(se == -1){
        //le bind n'a pas marché : 
        fprintf(stderr, "le port %d est indisponible", port);
        PAUSE("");
        return ;
    }

    // demarrerVideo(); //lance la capture vidéo
    //On envoie requête au serveur principal, afin de l'informer de la création d'une diffusion.
    req_t demandeAjouterListe, repServeur;
    initReqAjouterListe(&demandeAjouterListe, port, desc);
    envoyerReqStream(sockDialogueServPrincipal, &demandeAjouterListe, (fct_Serial *) &req_to_str);
    lireRepStream(sockDialogueServPrincipal, &repServeur, (fct_Serial *) &str_to_rep);
    if(repServeur.idReq != SUCCESS){
        close(se);
        fprintf(stderr, "Le serveur central n'a pas accepté la diffusion.");
        PAUSE("");
        return;
    }

    printf("se = %d\n", se);
    
    diffusionEnCours = 1;
    pthread_create(&tid, NULL, (pf_t)threadEcoute, &se);
}

/**
 * @brief Thread d'écoute pour la diffusion
 *
 * @param se
 * @return void*
 */
void *threadEcoute(int *se)
{
    pthread_t tid;

    
    pthread_create(&tid, NULL, (pf_t)connectThread, NULL);
    while (diffusionEnCours)
    ;

    pthread_cancel(tid);
    // arreterVideo(); //arrete la capture vidéo
    printf("Fin de la diffusion\n");
    
    //on informe le serveur central de la fin de diffusion :
    req_t demandeFinDiffusion, repServeur;
    initReqRetirerListe(&demandeFinDiffusion, REASON_DEFAULT);
    envoyerReqStream(sockDialogueServPrincipal, &demandeFinDiffusion, (fct_Serial *) &req_to_str);
    lireRepStream(sockDialogueServPrincipal, &repServeur, (fct_Serial *) &str_to_rep);
    if(repServeur.idReq != SUCCESS){
        //TODO : à gérer (normalement impossible)
        fprintf(stderr, "Le serveur central n'a pas accepté la fin de diffusion.");
        PAUSE("");
    }

    close(*se);
    pthread_exit(NULL);
}

void *connectThread()
{
    int sd;
    int nbThread = 0;
    struct sockaddr_in clt;
    pthread_t tids[NBMAX_THREADS];
    while (diffusionEnCours)
    {
        sd = attenteAppel(se, &clt);
        pthread_create(&tids[nbThread++], NULL, (pf_t)diffusion, &sd);
    }
    pthread_exit(NULL);
}

void *diffusion()
{

    pthread_exit(NULL);
}

void viderBuffer()
{
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}