#include "application.h"

listeDiffusions_t listeDiffusions;
sem_t mutexConnexion, mutexListeDiffusions;
int nextIdGlobal = 1;

int main(int argc, char const *argv[])
{
    //struct sockaddr_in clt;
    infoConnexion_t infoConnexion;
    int se, nbThread = 0;
    //pthread_t tids[NBMAX_THREADS];
    pthread_t tid;
    initListeDiffusions(&listeDiffusions);
    
    CHECK(sem_init(&mutexConnexion, 0, 0), "init mutexConnexion");
    CHECK(sem_init(&mutexListeDiffusions, 0, 1), "init mutexListeDiffusions");

    CHECK(se = creerSockAddrEcoute("127.0.0.1",5250, 5), "PB creerSockAddrEcoute : busy");
    
    
    while(1){
        infoConnexion.sd = attenteAppel(se, &infoConnexion.addrClt);
        //pthread_create(&tids[nbThread++], NULL, (pf_t)&dialogueAvecClient, &infoConnexion);
        pthread_create(&tid, NULL, (pf_t)&dialogueAvecClient, &infoConnexion);
        sem_wait(&mutexConnexion);
    }

    CHECK(sem_destroy(&mutexConnexion), "destroy mutexConnexion");
    CHECK(sem_destroy(&mutexListeDiffusions), "destroy mutexListeDiffusions");
    close(se);
    return 0;
}

void * dialogueAvecClient(infoConnexion_t * pInfoConnexion)
{
    req_t reqClient;
    req_t repServ;
    int isSockClosed = 0;

    char addrIP[30];

    int sd = pInfoConnexion->sd;
    struct sockaddr_in addrClt;
    memcpy(&addrClt, &pInfoConnexion->addrClt, sizeof(pInfoConnexion->addrClt));
    strcpy(addrIP, inet_ntoa(addrClt.sin_addr));

    sem_post(&mutexConnexion);
    while (!isSockClosed)
    {
        lireRepStream(sd, &reqClient, (fct_Serial *) &str_to_rep);
        switch (reqClient.idReq)
        {
        /*case LISTE_INFOS:

            break;

        case INFOS_DIFFUSION:

            break;
        */
        case DEMANDE_RETIRER_LISTE:

            break;

        case DEMANDE_LISTE:
            // Il n'y a que l'id de la requête, aucune information n'est demandée
            break;

        case DEMANDE_AJOUTER_LISTE:
            
            insererListeDiffusions(&listeDiffusions, &reqClient.r.reqAjouterListe, addrIP);
            break;

        case SOCKET_CLOSED:
            //on stop
            isSockClosed = 1;
            close(sd);
            break;
        default:
            break;
        }
    }
    return NULL;
}

void initListeDiffusions(listeDiffusions_t * pListe){
    pListe->taille = 0;
    pListe->tabPInfos = malloc(sizeof(infosDiffusion_t * ) * BLOC_MEM_LISTE);
    pListe->alloc_len = BLOC_MEM_LISTE;
}

void insererListeDiffusions(listeDiffusions_t * pListe, demandeAjouterListe_t * pDemandeDiffusion, char addrIP[MAX_DESC]){
    infosDiffusion_t * pNewDiff;
    pNewDiff = malloc(sizeof(infosDiffusion_t));
    
    strcpy(pNewDiff->addrIP, addrIP);
    strcpy(pNewDiff->description, pDemandeDiffusion->description);
    pNewDiff->id = nextIdGlobal++;
    pNewDiff->port = pDemandeDiffusion->port;

    //on attend la mutex
    sem_wait(&mutexListeDiffusions);

    //on réalloue si on a pas assez allloué de cases
    if(pListe->taille == pListe->alloc_len){
        pListe->alloc_len += BLOC_MEM_LISTE;
        pListe->tabPInfos = realloc(pListe->tabPInfos, sizeof(pNewDiff) * pListe->alloc_len);
    }

    //insertion
    pListe->tabPInfos[pListe->taille] = pNewDiff;
    pListe->taille++;

    //on libère la mutex
    sem_post(&mutexListeDiffusions);
}

infosDiffusion_t * trouverDiffusion(listeDiffusions_t * pListe, long id){
    for (int i = 0; i < pListe->taille; i++)
    {
        if(pListe->tabPInfos[i]->id == id){

            return pListe->tabPInfos[i];
        }
    }
    return NULL;
}


int supprimerDiffusion(listeDiffusions_t * pListe, long id){
    //on attend la mutex
    sem_wait(&mutexListeDiffusions);
    int foundI = -1;

    for (int i = 0; i < pListe->taille; i++)
    {
        if(pListe->tabPInfos[i]->id == id){

            foundI = i;
            break;
        }
    }

    if(foundI == -1){
        sem_post(&mutexListeDiffusions);
        return 0;
    }

    free(pListe->tabPInfos[foundI]);
    for(int i = foundI; i < pListe->taille - 1; i++){
        pListe->tabPInfos[i] = pListe->tabPInfos[i + 1];
    }
    pListe->taille--;

    //on réalloue si on a pas réduit assez
    if(pListe->taille <= pListe->alloc_len - BLOC_MEM_LISTE){
        pListe->alloc_len -= BLOC_MEM_LISTE;
        pListe->tabPInfos = realloc(pListe->tabPInfos, sizeof(infosDiffusion_t *) * pListe->alloc_len);
    }

    //on libère la mutex
    sem_post(&mutexListeDiffusions);
    return 1;
}