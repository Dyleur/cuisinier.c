#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>
#include "ipc_config.h"

int main() {
    key_t msg_key_prod;
    int msg_id_prod;
    struct msg_buffer message;

    FILE *f = fopen(MSG_KEY_TAB_TO_PROD, "a"); 
    if(f) fclose(f);

    msg_key_prod = ftok(MSG_KEY_TAB_TO_PROD, PROJ_ID_MSG_TAB_TO_PROD);
    msg_id_prod = msgget(msg_key_prod, 0666 | IPC_CREAT);

    printf("=== CUISINIER ===\n");

    int commande_traitee = 0;

    while(1) {
        printf("En attente de commandes validées...\n");

        if (msgrcv(msg_id_prod, &message, sizeof(message)-sizeof(long), MSG_TYPE_VALIDEE, 0) == -1) {
            perror("Erreur réception message");
            continue;
        }

        commande_traitee++;
        printf("\nCOMMANDE VALIDÉE #%d \n", commande_traitee);
        printf("Plat: %s\n", message.mtext);
        
        printf("\nPROCESSUS DE PRÉPARATION:\n");
        sleep(1);
        printf(" Préparation... OK\n"); 
        sleep(1);
        
        printf("\nCOMMANDE FINALISÉE:\n");
        printf("S_%d => [TABLEAU:VALIDÉ] => [CUISINIER:PRÊT]\n", message.serveur_id);
        printf("C_%d : %s - Livraison au serveur S_%d\n", 
               message.cmd_id, message.mtext, message.serveur_id);

        printf("\nAppuyez sur Entrée pour continuer...");
        getchar();
    }

    return 0;
}
