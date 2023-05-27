#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CSTE 100
#define MDP 10
#define HISTORIQUE_MAX 3

// Structure des produits
typedef struct {
  char reference[CSTE];
  char nomproduit[CSTE];
  char couleur[CSTE];
  float prix;
  int quantiteenstock;
} Car;

// Structure des clients
struct Client {
  char nom[CSTE];
  char prenom[CSTE];
  char nomutilisateur[CSTE];
  char motdepasse2[CSTE];
  int cart[CSTE];
  int quantiteProduits[CSTE];
  int nbProduits;
  char historique[CSTE][CSTE]; // Historique des achats
};

// Afficher le stock des voitures disponibles
void Listevoitures() {
  FILE *f = fopen("Listevoitures.txt",
                  "r"); // ouvre le fichier "Listevoitures.txt" en mode lecture
  if (f == NULL) {      // message d'erreur si le fichier ne s'ouvre pas
    printf("Erreur lors de l'ouverture du fichier.\n");
    return;
  }
  char reference[CSTE];
  char nomproduit[CSTE];
  char couleur[CSTE];
  float prix;
  int quantiteenstock;

  printf("Voici la liste des voitures disponibles :\n");
  printf("----------------------------------------\n");
  printf("Reference  |  Nom du produit    |Couleur|   Prix    |  Quantite en "
         "stock\n");
  printf(
      "--------------------------------------------------------------------\n");

  while (fscanf(f, "%s %s %s %f %d", reference, nomproduit, couleur, &prix,
                &quantiteenstock) !=
         EOF) { // parcours le fichier jusqu'à "End Of File" (fin du fichier) en
                // lisant les données de chaque voiture dans l'ordre spécifié
    printf("%-10s | %-18s | %-5s | %.2f  | %d\n", reference, nomproduit,
           couleur, prix, quantiteenstock); // affiche ces informations
  }
  fclose(f); // ferme le fichier "Listevoiture.txt"
}

// Afficher tous les clients du shop
void Listeclients() {
  FILE *f = NULL;
  char ligne[CSTE];

  f = fopen("clients.txt",
            "rt"); // ouvre le fichier "clients.txt" en mode lecture de texte
  if (f == NULL) {
    printf(
        "Erreur lors de l'ouverture du fichier.\n"); // message d'erreur si le
                                                     // fichier ne s'ouvre pas
    return;
  }

  while (fgets(ligne, CSTE, f) != NULL) { // lit chaque ligne du fichier et la
                                          // stocke dans la variable "ligne"
    printf("%s", ligne);                  // affiche chaque ligne du fichier
  }

  fclose(f); // ferme le fichier "clients.txt"
}

// Enregistre le client dans un fichier
void SaveClient(struct Client cl) {
  char nomFichier[100];
  strcpy(nomFichier, cl.nomutilisateur);
  strcat(nomFichier, ".txt");

  FILE *fichier = fopen(nomFichier, "w");
  if (fichier == NULL) {
    printf("Erreur lors de l'ouverture du fichier.\n");
    return;
  }

  fprintf(fichier, "Nom: %s\n", cl.nom);
  fprintf(fichier, "Prenom: %s\n", cl.prenom);
  fprintf(fichier, "Nom d'utilisateur: %s\n", cl.nomutilisateur);
  fprintf(fichier, "Mot de passe: %s\n", cl.motdepasse2);

  int achatCount = 0; // Initialisation à 0
  for (int i = 0; i < cl.nbProduits; i++) {
    if (achatCount < 3 && cl.historique[i][0] != '\0') {
      achatCount++;
      fprintf(fichier, "Achat %d: %s\n", achatCount, cl.historique[i]);
    }
  }

  fclose(fichier);
  printf("Le fichier du client %s a été créé avec succès.\n",
         cl.nomutilisateur);
  printf("-----------------------------\n");

  // Affichage des achats dans le terminal
  printf("Historique des achats :\n");
  for (int i = 0; i < cl.nbProduits; i++) {
    if (achatCount <= 3 && cl.historique[i][0] != '\0') {
      printf("Achat %d: %s\n", achatCount, cl.historique[i]);
      achatCount++;
    }
  }
}

// Rechercher un produit
void Rechercheproduit(Car car[], int numProducts) {
  char reference[CSTE];
  char couleur[CSTE];
  FILE *f;
  Car c;
  int found = 0;

  f = fopen("Listevoitures.txt", "r");
  if (f == NULL) {
    printf("Erreur lors de l'ouverture du fichier.\n");
    return;
  }

  printf("Recherche d'un produit :\n");
  printf("Numéro de référence :");
  scanf("%s", reference);
  printf("Couleur : ");
  scanf("%s", couleur);
  printf("-----------------------------\n");

  while (fscanf(f, "%s %s %s %f %d", c.reference, c.nomproduit, c.couleur,
                &c.prix, &c.quantiteenstock) != EOF) {
    if (strcmp(c.reference, reference) == 0 &&
        strcmp(c.couleur, couleur) == 0) {
      found = 1;
      printf("Détails du produit :\n");
      printf("Nom : %s\n", c.nomproduit);
      printf("Numéro de référence : %s\n", c.reference);
      printf("Quantité : %d\n", c.quantiteenstock);
      printf("Prix : %.2f\n", c.prix);
      printf("Couleur : %s\n", c.couleur);
      printf("-----------------------------\n");
      break;
    }
  }

  fclose(f);

  if (!found) {
    printf("Produit non trouvé.\n");
  }
}

//
int Supprimer(struct Client *cl, int quantite, const char *reference,
              const char *couleur) {
  FILE *f;
  FILE *temp;
  Car c;
  int found = 0;

  f = fopen("Listevoitures.txt", "r");
  if (f == NULL) {
    printf("Erreur lors de l'ouverture du fichier.\n");
    return 0;
  }

  temp = fopen("temp.txt", "w");
  if (temp == NULL) {
    printf("Erreur lors de l'ouverture du fichier temporaire.\n");
    fclose(f);
    return 0;
  }

  while (fscanf(f, "%s %s %s %f %d", c.reference, c.nomproduit, c.couleur,
                &c.prix, &c.quantiteenstock) != EOF) {
    if (strcmp(c.reference, reference) == 0) {
      if (strcmp(c.couleur, couleur) == 0) {
        found = 1;
        if (c.quantiteenstock >= quantite) {
          c.quantiteenstock -= quantite;
        } else {
          fclose(f);
          fclose(temp);
          printf("La quantité demandée n'est pas disponible en stock.\n");
          return 2;
        }
      }
    }
    fprintf(temp, "%s %s %s %.2f %d\n", c.reference, c.nomproduit, c.couleur,
            c.prix, c.quantiteenstock);
  }

  fclose(f);
  fclose(temp);

  if (!found) {
    printf("La référence de la voiture est introuvable.\n");
    remove("temp.txt");
    return 3;
  }

  remove("Listevoitures.txt");
  rename("temp.txt", "Listevoitures.txt");

  // Ajout de la référence et de la quantité dans le panier du client
  cl->cart[cl->nbProduits] = atoi(c.reference);
  cl->quantiteProduits[cl->nbProduits] = quantite;
  cl->nbProduits++;

  printf("La voiture a été supprimée.\n");
  return 1;
}

void afficherRuptures() {
  FILE *fichier;
  char ligne[CSTE];
  Car car;
  Car produitsRupture[CSTE];
  Car produitsBasStock[CSTE];
  int nbProduitsRupture = 0;
  int nbProduitsBasStock = 0;

  fichier = fopen("Listevoitures.txt", "r");
  if (fichier == NULL) {
    printf("Erreur lors de l'ouverture du fichier.\n");
    return;
  }

  printf("Produits en rupture de stock :\n");
  printf("--------------------------------\n");
  printf("Reference  |  Nom du produit    | Couleur |   Prix    |  Quantite en "
         "stock\n");
  printf("---------------------------------------------------------------------"
         "---\n");

  while (fgets(ligne, CSTE, fichier) != NULL) {
    sscanf(ligne, "%s %s %s %f %d", car.reference, car.nomproduit, car.couleur,
           &car.prix, &car.quantiteenstock);

    if (car.quantiteenstock == 0) {
      printf("%-10s | %-18s | %-7s | %.2f    | %d\n", car.reference,
             car.nomproduit, car.couleur, car.prix, car.quantiteenstock);
    } else if (car.quantiteenstock < 5) {
      if (nbProduitsBasStock < CSTE) {
        produitsBasStock[nbProduitsBasStock] = car;
        nbProduitsBasStock++;
      }
    }
  }

  fclose(fichier);

  printf("\nProduits dont les stocks sont les plus bas :\n");
  printf("---------------------------------------------\n");
  printf("Reference  |  Nom du produit    | Couleur |   Prix    |  Quantite en "
         "stock\n");
  printf("---------------------------------------------------------------------"
         "---\n");

  // Tri des produits selon les stocks les plus bas
  for (int i = 0; i < nbProduitsBasStock - 1; i++) {
    for (int j = i + 1; j < nbProduitsBasStock; j++) {
      if (produitsBasStock[i].quantiteenstock >
          produitsBasStock[j].quantiteenstock) {
        Car temp = produitsBasStock[i];
        produitsBasStock[i] = produitsBasStock[j];
        produitsBasStock[j] = temp;
      }
    }
  }

  // Affichage des 5 produits avec les stocks les plus bas
  int nbProduitsAffiches = 0;
  for (int i = 0; i < nbProduitsBasStock && nbProduitsAffiches < 5; i++) {
    printf("%-10s | %-18s | %-7s | %.2f    | %d\n",
           produitsBasStock[i].reference, produitsBasStock[i].nomproduit,
           produitsBasStock[i].couleur, produitsBasStock[i].prix,
           produitsBasStock[i].quantiteenstock);
    nbProduitsAffiches++;
  }
}
int CapaciteMax = 1000;

int Ajouter(struct Client *cl, int quantite, const char *reference,
            const char *couleur) {
  FILE *f;
  FILE *temp;
  Car c;
  int found = 0;

  f = fopen("Listevoitures.txt", "r");
  if (f == NULL) {
    printf("Erreur lors de l'ouverture du fichier.\n");
    return 0;
  }

  temp = fopen("temp.txt", "w");
  if (temp == NULL) {
    printf("Erreur lors de l'ouverture du fichier temporaire.\n");
    fclose(f);
    return 0;
  }

  while (fscanf(f, "%s %s %s %f %d", c.reference, c.nomproduit, c.couleur,
                &c.prix, &c.quantiteenstock) != EOF) {
    if (strcmp(c.reference, reference) == 0 &&
        strcmp(c.couleur, couleur) == 0) {
      found = 1;
      if (c.quantiteenstock >= quantite) {
        c.quantiteenstock += quantite;
      } else {
        fclose(f);
        fclose(temp);
        printf("La quantité demandée n'est pas disponible en stock.\n");
        return 2;
      }
    }
    fprintf(temp, "%s %s %s %.2f %d\n", c.reference, c.nomproduit, c.couleur,
            c.prix, c.quantiteenstock);
  }

  fclose(f);
  fclose(temp);

  if (!found) {
    printf("La référence de la voiture est introuvable.\n");
    remove("temp.txt");
    return 3;
  }

  remove("Listevoitures.txt");
  rename("temp.txt", "Listevoitures.txt");

  if (cl != NULL) {
    // Ajout de la référence et de la quantité dans le panier du client
    cl->cart[cl->nbProduits] = atoi(c.reference);
    cl->quantiteProduits[cl->nbProduits] = quantite;
    cl->nbProduits++;

    // Sauvegarde du client
    SaveClient(*cl);
  }

  printf("La voiture a été ajoutée au stock.\n");
  return 1;
}

int AjouterNewProducts() {
  FILE *f;
  Car c;

  f = fopen("Listevoitures.txt", "a");
  if (f == NULL) {
    printf("Erreur lors de l'ouverture du fichier.\n");
    return 0; // Indique l'échec de l'ajout
  }

  printf("Entrez la référence de la voiture que vous voulez ajouter :\n");
  scanf("%s", c.reference);
  getchar();
  printf("Entrez le nom de la voiture :\n");
  scanf("%s", c.nomproduit);
  getchar();
  printf("Entrez la couleur de la voiture (sable/nardo/mat) :\n");
  scanf("%s", c.couleur);
  getchar();
  printf("Entrez le prix de la voiture :\n");
  scanf("%f", &c.prix);
  getchar();
  printf("Entrez la quantité en stock :\n");
  scanf("%d", &c.quantiteenstock);
  getchar();

  fprintf(f, "%s %s %s %.2f %d\n", c.reference, c.nomproduit, c.couleur, c.prix,
          c.quantiteenstock);

  fclose(f);

  printf("La voiture a été ajoutée avec succès.\n");
  return 1;
}

void AfficherHistorique(struct Client cl) {
  printf("Historique des achats de %s :\n", cl.nomutilisateur);
  printf("-----------------------------\n");

  if (cl.nbProduits == 0) {
    printf("Aucun achat effectué.\n");
  } else {
    int numAchat = 1;
    for (int i = 0; i < cl.nbProduits && numAchat <= 3; i++) {
      if (cl.historique[i][0] != '\0') {
        printf("Achat %d: %s\n", numAchat, cl.historique[i]);
        numAchat++;
      }
    }
  }
}

float CalculerTotal(struct Client *cl) {
  float total = 0.0;

  FILE *f = fopen("Listevoitures.txt", "r");
  if (f == NULL) {
    printf("Erreur lors de l'ouverture du fichier.\n");
    return 0;
  }

  for (int i = 0; i < cl->nbProduits; i++) {
    FILE *fichier =
        fopen("Listevoitures.txt",
              "r"); // Nouveau pointeur de fichier pour chaque itération
    if (fichier == NULL) {
      printf("Erreur lors de l'ouverture du fichier.\n");
      fclose(f);
      return 0;
    }

    Car c;
    while (fscanf(fichier, "%s %s %s %f %d", c.reference, c.nomproduit,
                  c.couleur, &c.prix, &c.quantiteenstock) != EOF) {
      if (atoi(c.reference) == cl->cart[i]) {
        total += c.prix * cl->quantiteProduits[i];
        total -= 5000 * cl->quantiteProduits[i]; // Soustraire 5000 pour chaque
                                                 // produit acheté
        break;
      }
    }

    fclose(fichier); // Fermer le pointeur de fichier à chaque itération
  }

  fclose(f);

  return total;
}

int Acheter(struct Client *cl, int quantite, const char *reference,
            const char *couleur) {
  FILE *f;
  FILE *temp;
  Car c;
  int found = 0;

  f = fopen("Listevoitures.txt", "r");
  if (f == NULL) {
    printf("Erreur lors de l'ouverture du fichier.\n");
    return 0;
  }

  temp = fopen("temp.txt", "w");
  if (temp == NULL) {
    printf("Erreur lors de l'ouverture du fichier temporaire.\n");
    fclose(f);
    return 0;
  }

  while (fscanf(f, "%s %s %s %f %d", c.reference, c.nomproduit, c.couleur,
                &c.prix, &c.quantiteenstock) != EOF) {
    if (strcmp(c.reference, reference) == 0 &&
        strcmp(c.couleur, couleur) == 0) {
      found = 1;
      if (c.quantiteenstock >= quantite) {
        c.quantiteenstock -= quantite;
      } else {
        fclose(f);
        fclose(temp);
        printf("La quantité demandée n'est pas disponible en stock.\n");
        return 2;
      }
    }
    fprintf(temp, "%s %s %s %.2f %d\n", c.reference, c.nomproduit, c.couleur,
            c.prix, c.quantiteenstock);
  }

  fclose(f);
  fclose(temp);

  if (!found) {
    printf("La référence de la voiture est introuvable.\n");
    remove("temp.txt");
    return 3;
  }

  remove("Listevoitures.txt");
  rename("temp.txt", "Listevoitures.txt");

  // Ajout de la référence et de la quantité dans le panier du client
  cl->cart[cl->nbProduits] = atoi(c.reference);
  cl->quantiteProduits[cl->nbProduits] = quantite;
  cl->nbProduits++;

  printf("La voiture a été ajoutée à votre panier.\n");
  printf("-----------------------------\n");

  // Enregistrement de l'achat dans l'historique du client
  char achat[CSTE];
  snprintf(achat, sizeof(achat), "Reference: %s, Couleur: %s, Quantite: %d",
           reference, couleur, quantite);
  if (cl->nbProduits < HISTORIQUE_MAX) {
    // Décalage des éléments existants vers la droite
    for (int i = cl->nbProduits; i > 0; i--) {
      strcpy(cl->historique[i], cl->historique[i - 1]);
    }
    strcpy(cl->historique[0], achat);
    cl->nbProduits++;
  } else {
    // Décalage des éléments existants vers la droite et écrasement du dernier
    // élément
    for (int i = HISTORIQUE_MAX - 1; i > 0; i--) {
      strcpy(cl->historique[i], cl->historique[i - 1]);
    }
    strcpy(cl->historique[0], achat);
  }

  SaveClient(*cl);

  return 1;
}

int Supprimerclient(struct Client *cl, const char *nomUtilisateur) {
  int reponse;
  struct Client client;
  char nomutilisateur[100];
  char motdepasse[100];

  printf("Souhaitez-vous supprimer un compte client ? :\n1.oui\n2.non\n");
  scanf("%d", &reponse);

  if (reponse == 1) {
    printf("Suppression d'un compte client\n");
    printf("Nom d'utilisateur : ");
    scanf(" %s", nomutilisateur);
    printf("Mot de passe : ");
    scanf(" %s", motdepasse);

    // Recherche du fichier client contenant les informations de l'utilisateur
    char nomFichier[100];
    strcpy(nomFichier, nomutilisateur);
    strcat(nomFichier, ".txt");

    FILE *fichierUtilisateur = fopen(nomFichier, "r");
    if (fichierUtilisateur == NULL) {
      printf("Erreur lors de l'ouverture du fichier de l'utilisateur.\n");
      return 0;
    }

    fscanf(fichierUtilisateur, "Nom: %s\n", client.nom);
    fscanf(fichierUtilisateur, "Prenom: %s\n", client.prenom);
    fscanf(fichierUtilisateur, "Nom d'utilisateur: %s\n",
           client.nomutilisateur);
    fscanf(fichierUtilisateur, "Mot de passe: %s\n", client.motdepasse2);

    fclose(fichierUtilisateur);

    if (strcmp(motdepasse, client.motdepasse2) != 0) {
      printf("Le mot de passe fourni est incorrect.\n");
      return 0;
    }

    // Suppression de la ligne correspondante dans le fichier clients.txt
    FILE *fichierClients = fopen("clients.txt", "r");
    FILE *fichierTemp = fopen("temp.txt", "w");

    if (fichierClients == NULL || fichierTemp == NULL) {
      printf("Erreur lors de l'ouverture des fichiers.\n");
      return 0;
    }

    char ligne[100];
    char utilisateur[100];
    int found = 0;

    while (fgets(ligne, sizeof(ligne), fichierClients)) {
      sscanf(ligne, "%*s %*s %s", utilisateur);

      if (strcmp(utilisateur, nomutilisateur) == 0) {
        found = 1;
        continue;
      }

      fprintf(fichierTemp, "%s", ligne);
    }

    fclose(fichierClients);
    fclose(fichierTemp);

    if (!found) {
      printf("Le client avec le nom d'utilisateur '%s' n'a pas été trouvé.\n",
             nomutilisateur);
      remove("temp.txt");
      return 0;
    }

    remove("clients.txt");
    rename("temp.txt", "clients.txt");

    // Suppression du fichier de l'utilisateur
    remove(nomFichier);

    printf("Le client avec le nom d'utilisateur '%s' a été supprimé avec "
           "succès.\n",
           nomutilisateur);
    return 1;
  }

  return 0;
}

int Connexionreussie(struct Client *client) {
  Car c;
  int choixclientco;
  int quantite, numProducts;
  char reference[CSTE];
  char couleur[CSTE];
  printf("\n\nQue voulez-vous faire :\n");
  printf("\n1.Afficher le stock de voitures\n2.Afficher "
         "l'historique\n3.Rechercher une voiture\n4.Acheter une "
         "voiture\n5.Quitter\n");
  scanf("%d", &choixclientco);

  do {
    switch (choixclientco) {
    case 1:
      Listevoitures();
      Connexionreussie(client);
      break;
    case 2:
      AfficherHistorique(*client);
      Connexionreussie(client);
      break;
    case 3:
      Rechercheproduit(&c, numProducts);
      Connexionreussie(client);
      break;
    case 4:
      client->nbProduits = 0;
      do {
        printf(
            "Entrez la référence de la voiture que vous souhaitez acheter : ");
        scanf("%s", reference);
        getchar();
        printf("Entrez la couleur de la voiture que vous souhaitez acheter : ");
        scanf("%s", couleur);
        getchar();
        printf("Entrez la quantité souhaitée : ");
        scanf("%d", &quantite);
        getchar();
        int resultat = Acheter(client, quantite, reference, couleur);

        printf("Voulez-vous continuer vos achats ? (oui/non) : \n");
        printf("-----------------------------\n");
        char choix[10];
        scanf("%s", choix);
        getchar();

        if (strcmp(choix, "oui") != 0) {
          break; // Sortir de la boucle si la réponse n'est pas "oui"
        }
      } while (1); // Boucle infinie, sort de la boucle uniquement si la réponse
                   // n'est pas "oui"

      float total = CalculerTotal(client);
      printf("Le total des achats est de : %.2f\n", total);
      Connexionreussie(client);
      break;

    case 5:
      Menu();
    default:
      printf("Entrez un des choix possibles.\n");
      break;
    }
  } while (choixclientco > 4 || choixclientco <= 0);
  return 0;
}

int Connexion() {
  int tentative, reponse;
  char nomutilisateur[50];
  char motdepasse[50];
  struct Client client;
  tentative = 3;
  while (tentative > 0) {
    printf("Nom d'utilisateur : ");
    scanf("%s", nomutilisateur);
    printf("Mot de passe : ");
    scanf("%s", motdepasse);

    char nomFichier[100];
    strcpy(nomFichier, nomutilisateur);
    strcat(nomFichier, ".txt");

    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
      printf("Erreur lors de l'ouverture du fichier.\n");
      return 0;
    }

    fscanf(fichier, "Nom: %s\n", client.nom);
    fscanf(fichier, "Prenom: %s\n", client.prenom);
    fscanf(fichier, "Nom d'utilisateur: %s\n", client.nomutilisateur);
    fscanf(fichier, "Mot de passe: %s\n", client.motdepasse2);

    fclose(fichier);

    if (strcmp(motdepasse, client.motdepasse2) == 0) {
      printf("\nConnexion réussie.\n");
      Connexionreussie(&client);
      return 1;
    } else {
      printf("Nom d'utilisateur ou mot de passe incorrect.\n");
      tentative--;
    }
  }

  printf("Tentatives épuisées. Voulez-vous créer un compte ?\n1.oui\n2.non\n");
  scanf("%d", &reponse);
  if (reponse == 1) {
    Newclient();
  } else {
    Menu();
  }
  return 0;
}

// Crée un nouveau compte client
int Newclient() {
  int reponse;
  struct Client client;
  printf("Souhaitez-vous créer un nouveau compte ? :\n1.oui\n2.non\n");
  scanf("%d", &reponse);
  if (reponse == 1) {
    printf("Création d'un nouveau compte client\n");
    printf("Nom : ");
    scanf(" %s", client.nom);
    printf("Prénom : ");
    scanf(" %s", client.prenom);
    printf("Nom d'utilisateur : ");
    scanf(" %s", client.nomutilisateur);
    printf("Mot de passe : ");
    scanf(" %s", client.motdepasse2);

    FILE *fichierClients = fopen("clients.txt", "a");
    if (fichierClients == NULL) {
      printf("Erreur lors de l'ouverture du fichier.\n");
      return 0;
    }

    fprintf(fichierClients, "%s %s %s\n", client.nom, client.prenom,
            client.nomutilisateur);

    fclose(fichierClients);

    SaveClient(client);
  }
  return 0;
}

int Client() {
  int choixclient;
  struct Client client;
  char nomUtilisateur;
  printf("Vous êtes en mode client.\n");
  printf("Que voulez-vous faire :\n");
  printf(
      "\n1.Se connecter\n2.Nouveau compte\n3.Supprimer un compte\n4.Quitter\n");
  scanf("%d", &choixclient);
  do {
    switch (choixclient) {
    case (1):
      Connexion();
      break;
    case (2):
      Newclient();
      Client();
      break;
    case (3):
      Supprimerclient(&client, &nomUtilisateur);
      Client();
      break;
    case (4):
      Menu();
    default:
      printf("Entrez un des choix possible\n");
      break;
    }
  } while (choixclient > 4 || choixclient <= 0);
  return 0;
}

int coGerant() {
  struct Client client;
  Car car;
  int choixgerant, numProducts, nbProduits;
  int quantite;
  char reference[CSTE];
  char couleur[CSTE];
  char nomUtilisateur;
  char **produitsRupture;
  int nbProduitsRupture;
  int CapaciteMax;
  int placeRestante;
  int ajoutResult;
  printf("\nVous êtes en mode gérant.\n");
  printf("Que voulez-vous faire :\n");
  printf("\n1.Afficher le stock de voitures\n2.Afficher les produits en "
         "rupture de stock\n3.Rechercher une voiture\n4.Ajouter une "
         "voiture\n5.Ajouter une nouvelle voiture\n6.Supprimer une "
         "voiture\n7.Afficher les clients\n8.Supprimer un client\n9.Quitter\n");
  scanf("%d", &choixgerant);
  do {
    switch (choixgerant) {
    case (1):
      Listevoitures();
      coGerant();
      break;
    case (2):
      afficherRuptures();
      coGerant();
    case (3):
      Rechercheproduit(&car, numProducts);
      coGerant();
      break;
    case (4):
      printf("Entrez la référence de la voiture que vous souhaitez ajouter : ");
      scanf("%s", reference);
      getchar();
      printf("Entrez la couleur de la voiture que vous souhaitez ajouter : ");
      scanf("%s", couleur);
      getchar();
      printf("Entrez la quantité souhaitée : ");
      scanf("%d", &quantite);
      getchar();
      int resultat = Ajouter(&client, quantite, reference, couleur);
      coGerant();
    case (5):
      ajoutResult = AjouterNewProducts();
      if (ajoutResult != 1) {
        printf("Échec de l'ajout de nouveaux produits.\n");
      }
      coGerant();
    case (6):
      printf(
          "Entrez la référence de la voiture que vous souhaitez supprimer : ");
      scanf("%s", reference);
      getchar();
      printf("Entrez la couleur de la voiture que vous souhaitez supprimer : ");
      scanf("%s", couleur);
      getchar();
      printf("Entrez la quantité souhaitée : ");
      scanf("%d", &quantite);
      getchar();
      int resultat1 = Supprimer(&client, quantite, reference, couleur);
      coGerant();
    case (7):
      Listeclients();
      coGerant();
    case (8):
      Supprimerclient(&client, &nomUtilisateur);
      coGerant();
    case (9):
      Menu();
    default:
      printf("Entrez un des choix possible\n");
      break;
    }
  } while (choixgerant > 8 || choixgerant <= 0);
}

int Gerant() {
  int tentatives;
  char motdepasse[MDP];
  char checkmotdepasse[MDP] = {'1', '2', '3', '4'}; // demander si lettres
  tentatives = 3;

  while (tentatives > 0) {
    printf("Entrez le mot de passe du gérant :\n");
    scanf("%s", motdepasse);

    if (strcmp(motdepasse, checkmotdepasse) == 0) {
      coGerant();
      return 0;
    } else {
      printf("Mot de passe incorrect.\n");
      tentatives--;
    }
  }

  printf("Tentatives épuisées. Accès refusé.\n");
  Menu();
  return 0;
}

// jsp ou mettre aussi :
int Menu() {
  int choixmenu;
  printf("Bienvenue chez le meilleur fournisseur de voitures !\n\n");
  printf("*** Menu ***\n");
  printf("Que souhaitez-vous faire ?\n");
  printf("1. Gérant\n");
  printf("2. Client\n");
  printf("3. Quitter\n");
  scanf("%d", &choixmenu);

  switch (choixmenu) {
  case 1:
    Gerant(); // Interface du gérant
    break;
  case 2:
    Client(); // Interface du client
    break;
  case 3:
    printf("Au revoir !\n");
    break;
  default:
    printf("Veuillez entrer un chiffre entre 1 et 3.\n");
    Menu(); // Redémarrage du menu en cas de saisie invalide
    break;
  }

  return choixmenu;
}

int main() {
  printf("Bienvenue chez Carshop !\n");
  printf("=========================================\n");
  printf("              Carshop              \n");
  printf("=========================================\n");
  Menu();

  return 0;
}