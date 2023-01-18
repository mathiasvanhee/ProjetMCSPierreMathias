#include "../session/session.h"
#include "../data/data.h"

struct user {
    char id[3];
    char login[30];
    char password[30];
    char name[30];
    char tel[20];
};


void dialogueAvecClient(int sd, struct sockaddr_in * clt);
void creerProcessusServeur(int se, int sd, struct sockaddr_in * clt);

struct user tabUsers[] = {
    {"01", "unpseudo", "motedepassesecret", "monnom", "0123456789"},
    {"02", "pglp", "jambon1234", "pierre", "0708091011"},
    {"03", "mvan", "1234moi4321", "mathias", "0706050403"}};

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

void dialogueAvecClient(int sd, struct sockaddr_in *clt)
{
    char buffer[MAX_BUFF];
    char password[30];
    char login[30];
    int nbOctets;
    int resultScan;
    int i;
    for (int j = 0; j < 5; j++)
    {
        lireStream(sd, buffer);
        printf("Message reçu de [%s:%d] : \"%s\"\n", inet_ntoa(clt->sin_addr), ntohs(clt->sin_port), buffer);

        // resultScan = sscanf(buffer, "%[^:]s:%[^:]s", login, password);
        char *token = strtok(buffer, ":");
        strcpy(login, token);
        token = strtok(NULL, ":");
        if (token == NULL){
            ecrireStream(sd, "Erreur de format des données");
        }
        else
        {
            strcpy(password, token);
            printf("pseudo : %s ; pass : %s\n", login, password);

            for (i = 0; i < 4; i++)
            {
                if (strcmp(tabUsers[i].login, login) == 0 && strcmp(tabUsers[i].password, password) == 0)
                {
                    sprintf(buffer, "Connecté en tant que %s, nom=%s, tel=%s", tabUsers[i].login, tabUsers[i].name, tabUsers[i].tel);
                    break;
                }
            }
            if(i==4) strcpy(buffer, "Utilisateur introuvable");
            ecrireStream(sd, buffer);
        }
    }
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
