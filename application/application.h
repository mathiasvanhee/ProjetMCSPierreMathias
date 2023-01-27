#include "../session/session.h"
#include "../data/data.h"
#include "../protocole/protocole.h"
#include <pthread.h>

#define NBMAX_THREADS 10

void dialogueAvecServeur(int sd);
void serveurClient();
void threadEcoute(int* se);
void diffusion();