#include "application.h"

infosDiffusion_t listeDesDiffusions[];

int main(int argc, char const *argv[])
{
    struct sockaddr_in clt;
    int se, sd;

    se = creerSockAddrEcoute("127.0.0.1",5250, 5);
    while(1){
        sd = attenteAppel(se, &clt);
        creerProcessusServeur(se, sd, &clt);
        close(sd);
    }
    close(se);
    
    return 0;
}

void creerProcessusServeur(int se, int sd, struct sockaddr_in *clt)
{
    int pid;
    CHECK(pid = fork(), "fork + Problème de création de processus !");
    if (pid == 0)
    {
        close(se);
        printf("Processus fils : %d pour gerer un nouveau client\n", getpid());
        dialogueAvecClient(sd, clt);
        close(sd);
        exit(0);
    }
    printf("HERE");
    close(sd);
    traiterSIGCHLD(SIGCHLD);
}


void dialogueAvecClient(int sd, struct sockaddr_in *clt)
{
    while(1){

    }
}

