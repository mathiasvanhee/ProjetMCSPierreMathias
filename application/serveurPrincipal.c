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

    CHECK(se = creerSockAddrEcoute((char *)"127.0.0.1",5250, 5), "PB creerSockAddrEcoute : busy");
    
    
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
    int isSockClosed = 0, isClientEnDiffusion = 0;
    long idDiffusionClient = 0;

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
        case DEMANDE_RETIRER_LISTE:
            supprimerDiffusion(&listeDiffusions, idDiffusionClient);
            isClientEnDiffusion = 0;
            repServ.idReq = SUCCESS;//pas encore traitée
            break;

        case DEMANDE_LISTE:
            // Il n'y a que l'id de la requête, aucune information n'est demandée
            if(listeDiffusions_to_listeInfos(&listeDiffusions, &repServ.r.repListeInfos)){
                repServ.idReq = LISTE_INFOS;
            }
            else{
                repServ.idReq = ERROR;
            }
            break;

        case DEMANDE_AJOUTER_LISTE:
            idDiffusionClient = insererListeDiffusions(&listeDiffusions, &reqClient.r.reqAjouterListe, addrIP);
            isClientEnDiffusion = 1;
            repServ.idReq = SUCCESS;
            break;

        case SOCKET_CLOSED:
            //on stoppe
            isSockClosed = 1;
            close(sd);

            //on supprime la diffusion de la liste si toujours présente :
            if(isClientEnDiffusion)
                supprimerDiffusion(&listeDiffusions, idDiffusionClient);
            
            break;
        case BAD_REQUEST :
            repServ.idReq = BAD_REQUEST;
            break;
        case DEMANDE_INFOS_DIFFUSION : 
            if(getDiffusion(&listeDiffusions, &repServ.r.repInfosDiffusion, reqClient.r.reqInfosDiffusion.id) == 0){
                repServ.idReq = INFOS_DIFFUSION;
            }
            else{
                repServ.idReq = ERROR;
            }
            break;
        case LISTE_INFOS:
            free(reqClient.r.repListeInfos.tabInfos);
            repServ.idReq = BAD_REQUEST;
            break;
        default:
            repServ.idReq = BAD_REQUEST;
            break;
        }

        if(!isSockClosed)
            envoyerReqStream(sd, &repServ, (fct_Serial *) &req_to_str);
    }
    return NULL;
}

void initListeDiffusions(listeDiffusions_t * pListe){
    pListe->taille = 0;
    pListe->tabPInfos = (infosDiffusion_t **) malloc(sizeof(infosDiffusion_t * ) * BLOC_MEM_LISTE);
    pListe->alloc_len = BLOC_MEM_LISTE;
}

long insererListeDiffusions(listeDiffusions_t * pListe, demandeAjouterListe_t * pDemandeDiffusion, char addrIP[MAX_DESC]){
    long returnId;
    infosDiffusion_t * pNewDiff;
    pNewDiff = (infosDiffusion_t*) malloc(sizeof(infosDiffusion_t));
    
    strcpy(pNewDiff->addrIP, addrIP);
    strcpy(pNewDiff->description, pDemandeDiffusion->description);
    pNewDiff->id = nextIdGlobal++;
    pNewDiff->port = pDemandeDiffusion->port;

    //on attend la mutex
    sem_wait(&mutexListeDiffusions);

    //on réalloue si on a pas assez allloué de cases
    if(pListe->taille == pListe->alloc_len){
        pListe->alloc_len += BLOC_MEM_LISTE;
        pListe->tabPInfos = (infosDiffusion_t**) realloc(pListe->tabPInfos, sizeof(pNewDiff) * pListe->alloc_len);
    }

    //insertion
    pListe->tabPInfos[pListe->taille] = pNewDiff;
    pListe->taille++;

    returnId = pNewDiff->id;

    //on libère la mutex
    sem_post(&mutexListeDiffusions);

    return returnId;
}

int getDiffusion(listeDiffusions_t * pListe, infosDiffusion_t * pInfos,  long id){
    //on attend la mutex
    sem_wait(&mutexListeDiffusions);
    for (int i = 0; i < pListe->taille; i++)
    {
        if(pListe->tabPInfos[i]->id == id){
            strcpy(pInfos->addrIP, pListe->tabPInfos[i]->addrIP);
            strcpy(pInfos->description, pListe->tabPInfos[i]->description);
            pInfos->id = pListe->tabPInfos[i]->id;
            pInfos->port = pListe->tabPInfos[i]->port;
            
            //on libère la mutex
            sem_post(&mutexListeDiffusions);
            return 1;
        }
    }
    //on libère la mutex
    sem_post(&mutexListeDiffusions);
    return 0;
    
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
        pListe->tabPInfos = (infosDiffusion_t **) realloc(pListe->tabPInfos, sizeof(infosDiffusion_t *) * pListe->alloc_len);
    }

    //on libère la mutex
    sem_post(&mutexListeDiffusions);
    return 1;
}

int listeDiffusions_to_listeInfos(listeDiffusions_t * pListeSrc, listeInfos_t * pListeDest){
    sem_wait(&mutexListeDiffusions);
    pListeDest->taille = pListeSrc->taille;
    pListeDest->tabInfos = (infoListe_t *) malloc(sizeof(infoListe_t) * pListeDest->taille);
    if(pListeDest->tabInfos == NULL){
        sem_post(&mutexListeDiffusions);
        return 0;
    }

    for(int i = 0; i < pListeDest->taille; i++){
        strcpy(pListeDest->tabInfos[i].description, pListeSrc->tabPInfos[i]->description);
        pListeDest->tabInfos[i].id =  pListeSrc->tabPInfos[i]->id;
    }

    sem_post(&mutexListeDiffusions);
    return 1;
}