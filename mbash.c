 #include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_HISTORY 100

int main(int argc, char *argv[], char envp[]) {
    char input[1024];
    char cwd[1024];
    char hostname[1024];
    char *username;
    char history[MAX_HISTORY][1024];
    int history_count = 0;

    while (1) {
        // afficher le prompt
        username = getlogin();
        gethostname(hostname, sizeof(hostname));
        getcwd(cwd, sizeof(cwd));
        printf("%s@%s:%s$ ", username, hostname, cwd);
        
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\r\n")] = 0; // enlever les caractères de fin de ligne

// Ajouter la commande à l'historique
        strcpy(history[history_count++ % MAX_HISTORY], input);

        // Parser la commande et les arguments
        char *command = strtok(input, " ");
        char *args[64];
        int i = 0;
        while (command != NULL) {
            args[i++] = command;
            command = strtok(NULL, " ");
        }
        args[i] = NULL; // execve attend un tableau d'arguments terminé par NULL

        if (strcmp(args[0], "cd") == 0) {
            // Gérer la commande cd
            if (args[1] == NULL || strcmp(args[1], "~") == 0)
            {
                chdir(getenv("HOME"));
            }
            else
            {
                chdir(args[1]);
            }   
        } else if (strcmp(args[0], "pwd") == 0) {
            // Gérer la commande pwd
            char cwd[1024];
            getcwd(cwd, sizeof(cwd));
            printf("%s\n", cwd);
        }
        else if (strcmp(args[0], "history") == 0) {
            // Gérer la commande history
            int start = history_count - 10;
            if (start < 0) {
                start = 0;
            }
            for (int i = start; i < history_count; i++) {
                printf("%d %s\n", i + 1, history[i % MAX_HISTORY]);
            }
        }
//        else if (strcmp(args[0], "ls") == 0) {
//            // Gérer la commande ls
//            args[0] = "ls";
//            int pid = fork();
//            if (pid == 0)
//            {
//                execve("/bin/ls", args, NULL);
//            }
//            else
//            {
//                waitpid(pid, NULL, 0);
//            }
//
//        }
        else {
            printf("%s", "else");
            char inter[50] ="/usr/bin/" ;
            strcat(inter,args[0]);
            int pid;
            pid= fork();
            // Lancer la commande en utilisant execve
            if (pid == 0)
            {
                printf("%s","fils");

                execve(inter, args, envp);
            }
            else
            {
                printf("%s","pere1");
                waitpid(pid, NULL, 0);
                printf("%s","pere2");
            }
            
        }
    }

    return 0;
}

/*
#define MAXLI 2048

char cmd[MAXLI];
char path[MAXLI];
char* args[MAXLI];
int pathidx;

void mbash();

 int main(int argc, char *argv[], char *envp[]) {
    while (1) {
        printf("Commande: ");
        fgets(cmd, MAXLI, stdin);
        mbash(cmd,envp);
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
/*
void mbash(char* cmd,char *envp[]) {
    // Supprimer le saut de ligne final
    cmd[strlen(cmd) - 1] = '\0';

    // Séparer la commande en utilisant " " comme délimiteur
    char* token = strtok(cmd, " ");
    int i = 0;
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;



    // Vérifier si la commande est "cd"
    printf(args[0]);
    if (strcmp(args[0], "cd") == 0) {
        printf("argument %s", args[1]);
        if (args[1] == NULL || strcmp(args[1], "~") == 0) {
            chdir(getenv("HOME"));

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
    //char path[1024];
    char* pathToken = strtok_r(pathEnv, ":", &pathEnv);
    while (pathToken != NULL) {
    sprintf(path, "%s/%s", pathToken, args[0]);
    if (access(path, X_OK) == 0) {
    found = 1;
    break;
    }
    pathToken = strtok_r(NULL, ":", &pathEnv);
    }
    /*int found = 0;
    char* pathEnv = getenv("PATH");
    char* pathToken = strtok_r(pathEnv, ":", getenv("PATH"));
    while (pathToken != NULL) {
        sprintf(path, "%s/%s", pathToken, args[0]);
        if (access(path, X_OK) == 0) {
            found = 1;
            break;
        }
        pathToken = strtok_r(NULL, ":",getenv("PATH"));
    } */
    /*

        printf("\ncommande %s",path);
        printf("\nargument %s", args[1]);

    if (!found) {
        printf("Erreur: Commande introuvable.\n");
    } else {
        pid_t pid = fork();
        if (pid == 0) {
            execve(path, args, envp);
            printf("Erreur: Impossible d'exécuter la commande.\n");
        }else {
            // le "&" n'est pas presents
            if (!strcmp(args[i - 1], "&") == 0){
                waitpid(pid,NULL,0);
            }
        }
    }
}
} */