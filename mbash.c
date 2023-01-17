 #include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLI 2048

char cmd[MAXLI];
char path[MAXLI];
char* args[MAXLI];
int pathidx;

void mbash();

int main(int argc, char** argv) {
    while (1) {
        printf("Commande: ");
        fgets(cmd, MAXLI, stdin);
        mbash(cmd);
    }
    return 0;
}
/* L’objectif consiste à écrire une version miniature de bash, nommée mbash
qui permet de lancer interactivement des commandes en utilisant exactement la
même syntaxe que bash. Étant donné le nombre d’heures que vous allez pouvoir
consacrer à ce projet, mbash est vraiment une version extrêmement limitée par
rapport à bash. Le minimum demandé consiste à pouvoir se déplacer avec la
commande cd, à pouvoir afficher le répertoire courant avec pwd et à lancer
une commande, comme le fait bash, c’est-à-dire en respectant le contenu de la
variable PATH.*/
// Fonction qui permet de lancer une commande en utilisant la variable PATH
void mbash(char* cmd) {
    // Supprimer le saut de ligne final
    cmd[strlen(cmd) - 1] = '\0';

    // Séparer la commande en utilisant " " comme délimiteur
    char* token = strtok(cmd, " ");
    int i = 0;
    while (token != NULL) {
        args[i] = token;
        i++;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;



    // Vérifier si la commande est "cd"
    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL || args[1] == "~") {
            chdir(getenv("$HOME"));

        } else if (chdir(args[1]) != 0){
                printf("Erreur: Impossible de changer de répertoire.\n");
        }
    }
    // Vérifier si la commande est "pwd"
    else if (strcmp(args[0], "pwd") == 0) {
        char cwd[MAXLI];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            printf("Erreur: Impossible de récupérer le répertoire courant.\n");
        }
    }
    else {
    // Vérifier si la commande existe dans les répertoires de la variable PATH
    int found = 0;
    char* pathEnv = getenv("PATH");
    char* pathToken = strtok(pathEnv, ":");
    while (pathToken != NULL) {
        sprintf(path, "%s/%s", pathToken, args[0]);
        if (access(path, X_OK) == 0) {
            found = 1;
            break;
        }
        pathToken = strtok(NULL, ":");
    }

        printf("commande %s",path);
        printf("argument %s", args[0]);

//    if (!found) {
//        printf("Erreur: Commande introuvable.\n");
//    } else {
//        pid_t pid = fork();
//        if (pid == 0) {
//            execve(path, args, NULL);
//            printf("Erreur: Impossible d'exécuter la commande.\n");
//        }else {
//            // le "&" est presents
//            if (!strcmp(args[i - 1], "&") == 0){
//                waitpid(pid,NULL,0);
//            }
//        }
//    }
}
}


/*
void mbash() {
    // Supprimer le saut de ligne final
    cmd[strlen(cmd) - 1] = '\0';

    // Séparer la commande en utilisant " " comme délimiteur
    char* token = strtok(cmd, " ");
    int i = 0;
    while (token != NULL) {
        args[i] = token;
        i++;
        printf("i : " + i);
        token = strtok(NULL, " ");
    }

    args[strlen(args)-1] = NULL;

    // Vérifier si la commande est "cd"
    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            chdir(getenv("$HOME"));

        } else if (chdir(args[1]) != 0){
                printf("Erreur: Impossible de changer de répertoire.\n");
        }
    }
    // Vérifier si la commande est "pwd"
    else if (strcmp(args[0], "pwd") == 0) {
        char cwd[MAXLI];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            printf("Erreur: Impossible de récupérer le répertoire courant.\n");
        }
    }
    else {
    // Vérifier si la commande existe dans les répertoires de la variable PATH
    int found = 0;
    char* pathEnv = getenv("PATH");
    char* pathToken = strtok(pathEnv, ":");
    while (pathToken != NULL) {
        sprintf(path, "%s/%s", pathToken, args[0]);
        if (access(path, X_OK) == 0) {
            found = 1;
            break;
        }
        pathToken = strtok(NULL, ":");
    }

    if (!found) {
        printf("Erreur: Commande introuvable.\n");
    } else {
        pid_t pid = fork();
        if (pid == 0) { 
            execve(path, args, NULL);
            printf("Erreur: Impossible d'exécuter la commande.\n");
        }else {
            // le "&" est presents
            if (!strcmp(args[i - 1], "&") == 0){
                waitpid(pid,NULL,0);
            } 
        }
    }
}
}*/