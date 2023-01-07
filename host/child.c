#include "../session/session.h"

creerProcessusService(.....)
{
    int pid;
    // Création du processus enfant
    CHECK(pid=fork(),"creation des fils");
    if (pid == 0)
    {
        // Code du processus enfant
        traiterSigChild(...);
        exit(EXIT_SUCCESS);
    }
    // Code du processus parent
};

traiterSigChild(...){
    int status;
    int pid;
    // Attente de la fin d'un processus fils
    pid = waitpid(-1, &status, WNOHANG);
    if (pid == -1)
    {
        perror("waitpid");
        exit(EXIT_FAILURE);
    }
    if (pid == 0)
    {
        // Aucun processus fils n'a terminé
        return;
    }
    // Un processus fils a terminé
    // ...
};