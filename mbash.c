#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#define MAXLI 2048

char cmd[MAXLI];
char path[MAXLI];
int pathidx;
void mbash();
int main(int argc, char** argv) {
    static char etat_automate[] =
    while (1) {
        printf("Commande: ");

        //recuperation
        fgets(cmd, MAXLI, stdin);
        //automate
        switch (cmd) {
            case " ":
                break;

        }


        mbash(cmd);
    }
    return 0;
}

void mbash() {
    printf("Execute: %s", cmd);
    system(cmd);
}
