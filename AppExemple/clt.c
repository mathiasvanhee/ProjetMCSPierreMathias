#include "../session/session.h"
#include "../data/data.h"

void dialogueAvecServeur(int sd);

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
    char buffer[MAX_BUFF];
    int nbOctets;
    PAUSE("PAUSE");
    ecrireStream(sd, "unpseudo:motedepassesecret");
    printf("Envoi de \"unpseudo:motedepassesecret\"\n");
    lireStream(sd, buffer);
    printf("Réponse reçue du serveur : \n\"%s\"\n", buffer);

    PAUSE("PAUSE");
    ecrireStream(sd, "autreuser:motedepassesecret");
    printf("Envoi de \"autreuser:motedepassesecret\"\n");
    lireStream(sd, buffer);
    printf("Réponse reçue du serveur : \n\"%s\"\n", buffer);

    PAUSE("PAUSE");
    ecrireStream(sd, "unpseudo:fauxmdp");
    printf("Envoi de \"unpseudo:fauxmdp\"\n");
    lireStream(sd, buffer);
    printf("Réponse reçue du serveur : \n\"%s\"\n", buffer);

    PAUSE("PAUSE");
    ecrireStream(sd, "azenroiza rezoiarej zaoerh zaerh aizgeru_ièzae r");
    printf("Envoi de \"azenroiza rezoiarej zaoerh zaerh aizgeru_ièzae r\"\n");
    lireStream(sd, buffer);
    printf("Réponse reçue du serveur : \n\"%s\"\n", buffer);

    PAUSE("PAUSE");
    ecrireStream(sd, "mvan:1234moi4321");
    printf("Envoi de \"mvan:1234moi4321\"\n");
    lireStream(sd, buffer);
    printf("Réponse reçue du serveur : \n\"%s\"\n", buffer);
}