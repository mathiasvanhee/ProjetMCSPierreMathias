#include "../session/session.h"
#include "../data/data.h"

int main(int argc, char const *argv[])
{
    socklen_t cltLen;
    struct sockaddr_in clt;
    int se, sd;

    addUser();
/*
    for (int i = 0; i < 5; i++){
        printf("ID: %s\n", tabUsers[i].id);
        printf("Identifiant: %s\n", tabUsers[i].identifiant);
        printf("Mot de passe: %s\n", tabUsers[i].motDePasse);
        printf("Nom: %s\n", tabUsers[i].nom);
        printf("Téléphone: %s\n", tabUsers[i].tel);
        printf("\n");
    }
*/
    se = creerSocketAddrEcoute("127.0.0.1",5000, 5);;
    while(1){
        cltLen = sizeof(struct sockaddr_in);
        sd = attenteAppel(se, &clt);
        creerProcessusServeur(se, sd, clt);
        close(sd);
    }
    close(se);
    
    return 0;
}


