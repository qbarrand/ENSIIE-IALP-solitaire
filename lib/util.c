/** 
 * \file util.c 
 * \brief Fonctions utilitaires de l'application.
 *
 * \author Quentin Barrand <quentin.barrand@ensiie.fr>
 */
#include "util.h"
#include "consts.h"

/* Prototypes statiques */
static int getOrdByNumber(char);
static int getOrdByTwoNumbers(char, char);
static int getAbsByLetter(char);

/** Affiche l'usage du programme.
 *
 * \param executable le nom du programme (correspond à <tt>argv[0]</tt>).
 */
void help(char* executable)
{
    printf("Usage : %s [options]"
        "-h    Affiche l'aide et quitte le programme."
        "-n    Autorise les coups horizontaux et verticaux (par défaut)."
        "-d    Autorise les coups en diagonale.\n", executable);
}


/** Récupère les options de la ligne de commande
 *
 * \param config la configuration actuelle de l'application.
 * \param argc le nombre d'arguments depuis la ligne de commande.
 * \param argv la matrice représentant les arguments de la ligne de commande.
 *
 * \return un code de statut: 
 *    - 0 : tous les paramètres ont été lus correctement
 *    - 1 : a affiché l'aide
 *    - 2 : une erreur de lecture des paramètres est survenue
 */
int getOptions(options* config, int argc, char** argv)
{
    const int FUNC_SUCCESS         = 0;
    const int FUNC_PRINTED_HELP    = 1;
    const int FUNC_INCORRECT_PARAM = 2;

    int i;

    config->n = 0;
    config->d = 0;
    config->confExists = 0;

    for(i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-')
        {
            switch(argv[i][1])
            {
                case 'h':
                    help(argv[0]);
                    return FUNC_PRINTED_HELP;

                case 'n':
                    config->n = 1;
                    continue;

                case 'd':
                    config->d = 1;
                    continue;

                default:
                    return FUNC_INCORRECT_PARAM;
            }
        }
        else
            /* Il s'agit du chemin vers le fichier de configuration
             * du damier. Il ne doit pas pouvoir être spécifié plusieurs fois.
             */
            if(config->confExists == 0)
            {
                config->confPath = argv[i];
                config->confExists = 1;
            }
    }

    /* Par défaut on active l'option -n */
    if(config->n == 0 && config->d == 0)
    {
        config->n = 1;

        #ifdef DEBUG
        printf("Utilisation du déplacement vertical et horizontal "
            "par défaut.\n");
        #endif
    }

    return FUNC_SUCCESS;
}


/**
 *
 *
 */
int toCoord(char* userinput, char* coord, options config)
{
    const int FUNC_SUCCESS        = 0;
    const int FUNC_INCORRECT_CHAR = 1;
    const int FUNC_OUT_OF_BOUNDS  = 2;

    int i, j = 0, current;

    for(i = 0; i < strlen(userinput); i++)
    {
        #ifdef DEBUG
        printf("Caractère %c\n", userinput[i]);
        #endif

        if((current = getAbsByLetter(userinput[i])) != -1)
        {
            coord[j] = current;
            j++;
            continue;
        }

        if((current = 
            getOrdByTwoNumbers(userinput[i], userinput[i + 1])) != -1)
        {
            coord[j] = current;
            i++;
            j++;
            continue;
        }

        if((current = getOrdByNumber(userinput[i])) != -1)
        {
            coord[j] = current;
            j++;
            continue;
        }

        return FUNC_INCORRECT_CHAR;
    }

    #ifdef DEBUG
        for(i = 0; i < 4; i++)
            printf("Nombre %d\n", coord[i]);
    #endif

    if(coord[0] + 1 > config.confWidth ||
        coord[1] + 1 > config.confLength ||
        coord[2] + 1 > config.confWidth ||
        coord[3] + 1 > config.confLength)
        return FUNC_OUT_OF_BOUNDS;

    return FUNC_SUCCESS;
}


/**
 *
 *
 */
static int getOrdByNumber(char c)
{
    const int FUNC_NOT_A_NUMBER = -1;

    return (c >= '0' && c <= '9') ? c - '0' : FUNC_NOT_A_NUMBER;
}


/**
 *
 *
 */
static int getOrdByTwoNumbers(char c1, char c2)
{
    const int FUNC_NOT_A_NUMBER = -1;

    char p1 = getOrdByNumber(c1);
    char p2 = getOrdByNumber(c2);

    if(p1 == -1 || p2 == -1) return FUNC_NOT_A_NUMBER;

    int n1 = p1 - '0';
    int n2 = p2 - '0';

    return 10 * p1 + p2;
}


/**
 *
 *
 */
static int getAbsByLetter(char c)
{
    const int FUNC_NOT_A_LETTER = -1;

    if(c >= 'A' && c <= 'Z') return c - 'A';
    if(c >= 'a' && c <= 'z') return c - 'a';

    return FUNC_NOT_A_LETTER;
}


