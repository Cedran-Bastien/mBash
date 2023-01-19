#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>

// Taille maximale de l'historique
#define MAX_HISTORY 100

int main(int argc, char *argv[], char *envp[]) {
    char input[1024];

    // variables pour le prompt
    char cwd[1024];
    char hostname[1024];
    char *username;

    // variables pour l'historique des commandes
    char history[MAX_HISTORY][1024];
    int history_count = 0;

    while (1) {
        // récupérer le nom d'utilisateur, le nom de l'hôte et le répertoire courant
        username = getlogin();
        gethostname(hostname, sizeof(hostname));
        getcwd(cwd, sizeof(cwd));

        // afficher le prompt
        printf("%s@%s:%s$ ", username, hostname, cwd);
        
        // Lire la commande
        fgets(input, sizeof(input), stdin);

        // enlever les caractères de fin de ligne
        input[strcspn(input, "\r\n")] = 0; 

        // Ajouter la commande à l'historique
        strcpy(history[history_count++ % MAX_HISTORY], input);

        // Parser la commande et les arguments
        char *command = strtok(input, " ");
        char *args[64];
        int i = 0;

        // Si la commande est vide, on recommence
        if (command == NULL) {
            continue;
        }

        // tant qu'il y a des arguments, on les ajoute au tableau args
        while (command != NULL) {
            args[i++] = command;
            command = strtok(NULL, " ");
        }
        // execve attend un tableau d'arguments terminé par NULL
        args[i] = NULL; 

        // Gérer les commande internes cd, pwd et history
        if (strcmp(args[0], "cd") == 0) {
            // si le premier argument est cd et que le second argument est vide (NULL) ou ~, on change de répertoire vers $HOME
            if (args[1] == NULL || strcmp(args[1], "~") == 0)
            {
                chdir(getenv("HOME"));
            }
            // sinon, on change de répertoire vers le second argument
            else
            {
                chdir(args[1]);
            }   
        } else if (strcmp(args[0], "pwd") == 0) {
            // Créer un tableau de caractères de taille 1024 et utiliser la fonction getcwd pour récupérer le répertoire courant
            char cwd[1024];
            getcwd(cwd, sizeof(cwd));
            // Afficher le répertoire courant
            printf("%s\n", cwd);
        }
        else if (strcmp(args[0], "history") == 0) {
            // Créer une variable start pour stocker le numéro de la première commande de l'historique à afficher
            int start = history_count - 10;
            // Si start est négatif, mettre start à 0
            if (start < 0) {
                start = 0;
            }
            // Créer une boucle pour afficher les 10 dernières commandes de l'historique
            for (int i = start; i < history_count; i++) {
                printf("%d %s\n", i + 1, history[i % MAX_HISTORY]);
            }
        }
        else {
            // Gérer les commandes externes

            // Trouver le chemin absolu de la commande
            int found = 0;

            // Copier la variable d'environnement PATH dans un tableau de caractères
            char *path = strdup(getenv("PATH")) ;

            // Créer un pointeur pour strtok_r
            char *saveptr;

            // Utiliser strtok_r pour récupérer le premier répertoire du PATH
            char *inter = strtok_r(path, ":", &saveptr);

            // Créer un tableau de caractères pour stocker le chemin absolu de la commande
            char *Complet=malloc(sizeof(char)*50);

            // Créer une boucle pour parcourir les répertoires du PATH
            while (inter != NULL)
            {
                // Créer un chemin absolu en concaténant le répertoire courant avec le nom de la commande
                sprintf(Complet,"%s/%s",inter,args[0]);

                // Vérifier si la commande est exécutable
                if (access(Complet, X_OK) == 0) {
                    found = 1;
                    break;
                }
                // Mettre à jour inter avec le prochain répertoire du PATH
                inter = strtok_r(NULL, ":", &saveptr);
            }

            // Si la commande n'est pas trouvée, afficher un message d'erreur
            if  (!found){
                printf("%s : commande introuvable\n" , args[0]);
            }
            // Sinon, lancer la commande 
            else {
                // Créer un processus fils
                int pid;
                pid= fork();
                if (pid == 0)
                {
                    // Lancer la commande en utilisant execve
                    execve(Complet, args, envp);
                }
                else
                {
                    // Attendre la fin du processus fils
                    waitpid(pid, NULL, 0);
                }
            }
            
        }
    }

    return 0;
}
