#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <SDL/SDL.h>

#define TAILLEGRAPH 100		/* 1022 max */
#define TAILLECASE 9

#define PROBABILITE 50			/* Compris entre 0 et 100 inclus, probabilité d'avoir une case blanche ou noire */
/* Plus PROBABILITE est proche de 100, plus il y aura de cases blanches et proche de 0 pour les cases noires */
/* 50 permet d'avoir un choix équitable entre blanc et noir */



void Innitialiser(int *cases, int *copyCases, SDL_Surface *ecran){

	int i,j;

	SDL_Surface *carre = NULL;
	carre = SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLECASE, TAILLECASE, 32, 0, 0, 0, 0);
	SDL_Rect position;

	for(i = 0; i < (TAILLEGRAPH * TAILLECASE); i += TAILLECASE){

		for(j = 0; j < (TAILLEGRAPH * TAILLECASE); j += TAILLECASE){

			if(cases[ ((i / TAILLECASE) + (j / TAILLECASE) + ((i / TAILLECASE) * (TAILLEGRAPH - 1))) ] == 0){

				position.x = j;
				position.y = i;

				SDL_FillRect(carre, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
				SDL_BlitSurface(carre, NULL, ecran, &position);
			}

			if(cases[ ((i / TAILLECASE) + (j / TAILLECASE) + ((i / TAILLECASE) * (TAILLEGRAPH - 1))) ] == 1){

				copyCases[ ((i / TAILLECASE) + (j / TAILLECASE) + ((i / TAILLECASE) * (TAILLEGRAPH - 1))) ] = 1;

				position.x = j;
				position.y = i;

				SDL_FillRect(carre, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
				SDL_BlitSurface(carre, NULL, ecran, &position);
			}
		}
	}

	SDL_Flip(ecran);
	SDL_FreeSurface(carre);
}

int compteVoisines(int *cases, int numc){

	int nbCell = 0;

	if((numc - TAILLEGRAPH - 1) >= 0 && cases[numc - TAILLEGRAPH - 1] == 1 && (numc % TAILLEGRAPH) != 0){		/* Voisine haut gauche */

		nbCell++;
	}

	if((numc - TAILLEGRAPH) >= 0 && cases[numc - TAILLEGRAPH] == 1){		/* Voisine haut */

		nbCell++;
	}

	if((numc - TAILLEGRAPH + 1) >= 0 && cases[numc - TAILLEGRAPH + 1] == 1 && ((numc + 1) % TAILLEGRAPH) != 0){		/* Voisine haut droite */

		nbCell++;
	}

	if((numc - 1) >= 0 && cases[numc - 1] == 1 && (numc % TAILLEGRAPH) != 0){		/* Voisine gauche */

		nbCell++;
	}

	if((numc + 1) < (TAILLEGRAPH * TAILLEGRAPH) && cases[numc + 1] == 1 && ((numc + 1) % TAILLEGRAPH) != 0){		/* Voisine droite */

		nbCell++;
	}

	if((numc + TAILLEGRAPH - 1) < (TAILLEGRAPH * TAILLEGRAPH) && cases[numc + TAILLEGRAPH - 1] == 1 && (numc % TAILLEGRAPH) != 0){		/* Voisine bas gauche */

		nbCell++;
	}

	if((numc + TAILLEGRAPH) < (TAILLEGRAPH * TAILLEGRAPH) && cases[numc + TAILLEGRAPH] == 1){		/* Voisine bas */

		nbCell++;
	}

	if((numc + TAILLEGRAPH + 1) < (TAILLEGRAPH * TAILLEGRAPH) && cases[numc + TAILLEGRAPH + 1] == 1 && ((numc + 1) % TAILLEGRAPH) != 0){		/* Voisine bas droite */

		nbCell++;
	}

	return nbCell;
}


void evolution(int *cases, int *copyCases){

	int i,j,retour;

	for(i = 0; i < (TAILLEGRAPH * TAILLECASE); i += TAILLECASE){

		for(j = 0; j < (TAILLEGRAPH * TAILLECASE); j += TAILLECASE){

			retour = compteVoisines(cases, ((i / TAILLECASE) + (j / TAILLECASE) + ((i / TAILLECASE) * (TAILLEGRAPH - 1))) );


			if(cases[ ((i / TAILLECASE) + (j / TAILLECASE) + ((i / TAILLECASE) * (TAILLEGRAPH - 1))) ] == 0){

				if(retour == 3){		/* Si 3 voisines = nouvelle cellule */

					copyCases[ ((i / TAILLECASE) + (j / TAILLECASE) + ((i / TAILLECASE) * (TAILLEGRAPH - 1))) ] = 1;
				}
			}



			if(cases[ ((i / TAILLECASE) + (j / TAILLECASE) + ((i / TAILLECASE) * (TAILLEGRAPH - 1))) ] == 1){

				if(retour < 2 || retour > 3){		/* Si moins de 2 ou plus de 3 voisines = cellule morte */

					copyCases[ ((i / TAILLECASE) + (j / TAILLECASE) + ((i / TAILLECASE) * (TAILLEGRAPH - 1))) ] = 0;
				}
			}
		}
	}
}

void redessiner(int *cases, int *copyCases, SDL_Surface *ecran){


	int i,j;

	SDL_Surface *carre = NULL;
	carre = SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLECASE, TAILLECASE, 32, 0, 0, 0, 0);
	SDL_Rect position;

	for(i = 0; i < (TAILLEGRAPH * TAILLECASE); i += TAILLECASE){

		for(j = 0; j < (TAILLEGRAPH * TAILLECASE); j += TAILLECASE){


			if(copyCases[ ((i / TAILLECASE) + (j / TAILLECASE) + ((i / TAILLECASE) * (TAILLEGRAPH - 1))) ] == 0){

				cases[ ((i / TAILLECASE) + (j / TAILLECASE) + ((i / TAILLECASE) * (TAILLEGRAPH - 1))) ] = 0;

				position.x = j;
				position.y = i;

				SDL_FillRect(carre, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
				SDL_BlitSurface(carre, NULL, ecran, &position);
			}



			if(copyCases[ ((i / TAILLECASE) + (j / TAILLECASE) + ((i / TAILLECASE) * (TAILLEGRAPH - 1))) ] == 1){

				cases[ ((i / TAILLECASE) + (j / TAILLECASE) + ((i / TAILLECASE) * (TAILLEGRAPH - 1))) ] = 1;

				position.x = j;
				position.y = i;

				SDL_FillRect(carre, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
				SDL_BlitSurface(carre, NULL, ecran, &position);
			}
		}
	}

	SDL_Flip(ecran);
	SDL_FreeSurface(carre);
}

int random_fill(){
	int a;

	a = rand() % 101;

	if(a < PROBABILITE){

		a = 0;
	}

	if(a > PROBABILITE){

		a = 1;
	}

	if(a == PROBABILITE){

		a = random_fill();
	}

	return a;
}

int main(int argc, char *argv[]){

	srand(time(NULL));

	int cases[TAILLEGRAPH * TAILLEGRAPH];
	int copyCases[TAILLEGRAPH * TAILLEGRAPH];

	int generation = 0;

	int i,j;

	SDL_Surface *ecran = NULL;

	SDL_Init(SDL_INIT_VIDEO);
	ecran = SDL_SetVideoMode(TAILLEGRAPH * TAILLECASE,TAILLEGRAPH * TAILLECASE,32,SDL_HWSURFACE | SDL_DOUBLEBUF);
	SDL_WM_SetCaption("Jeu De La Vie", NULL);

	SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));

	SDL_Flip(ecran);

	for(i = 0; i < (TAILLEGRAPH * TAILLECASE); i += TAILLECASE){

		for(j = 0; j < (TAILLEGRAPH * TAILLECASE); j += TAILLECASE){

			cases[ ((i / TAILLECASE) + (j / TAILLECASE) + ((i / TAILLECASE) * (TAILLEGRAPH - 1))) ] = random_fill();
		}
	}

	Innitialiser(cases,copyCases,ecran);

	SDL_Event event;

	int continu = 1;

	while(continu){

		SDL_PollEvent(&event);

		switch(event.type){

            case SDL_QUIT: continu = 0;
        }

		usleep(9999);

		SDL_PollEvent(&event);

		generation++;
		printf("Generation numero : %d\n", generation);

		evolution(cases,copyCases);

		redessiner(cases,copyCases,ecran);

		SDL_PollEvent(&event);
	}

	SDL_FreeSurface(ecran);
	SDL_Quit();
}
