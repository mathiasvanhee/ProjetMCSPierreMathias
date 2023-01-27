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
    serveurClient();
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
    scanf("%s",IP);
    printf("Veuillez entrer le port d'écoute du serveur : \n");
    scanf("%d",&port);
    printf("Veuillez entrer une description sur la diffusion :\n");
    scanf("%s",desc);//marche pas, pour mettre phrase

    printf("%s:%d:%s",IP,port,desc);

    //demarrerVideo(); //lance la capture vidéo

    se = creerSockAddrEcoute(IP,port, 5);
    printf("%s;%d;%s",IP,port,desc);
    diffusionEnCours = 1;
    pthread_create(&tid,NULL,threadEcoute,&se);
    while(diffusionEnCours);
    
    //arreterVideo();
    close(se);
}

void threadEcoute(int* se){
    int nbThread = 0;
    struct sockaddr_in clt;
    int sd;
    pthread_t tids[NBMAX_THREADS];

    while(diffusionEnCours){
        sd = attenteAppel(se, &clt);
        pthread_create(&tids[nbThread++],NULL,diffusion,&sd);
    }

    for(int i = 0; i < nbThread; i++)
        pthread_join(tids[i],NULL);
}

void diffusion(){

}