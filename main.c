#include <ESDL.h>
#include <math.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define TILE_H 48
#define TILE_W 96
#define N 11

/**
 * Convertis les coordonnées cartésiennes en coordonnées isométriques
 * @param x Position horizontale cartésienne
 * @param y Position verticale cartésienne
 */
void toIso(int * x, int * y){
	* x = (* x - * y * 2) / 2; // Diamond map
	* y = (* x + * y * 2) / 2; // Diamond map

	/** x = (* x + * y); // Slide map
	* y = (* y / 2); // Slide map*/

	/** x = * x + ((* y / TILE_H % 2 == 0) ? 0 : 1 ) * (TILE_W/2); // Staggered map
	* y = * y / 2; // Staggered map*/
}

/**
 * Centre la carte au milieu de la fenêtre
 * @param x Position horizontale isométrique
 * @param y Position horizontale isométrique
 */
void centerMap(int * x, int * y){
	
	* x += (SCREEN_WIDTH - TILE_W) / 2; // Diamond map
	
	//*x += (SCREEN_WIDTH - (N + 1)  * TILE_W) / 2; // Slide AND Staggered map
	
	* y += (SCREEN_HEIGHT - N * TILE_H) /2;

}

/**
 * Dessine une tile aux coordonnées posX et posY
 * @param posX Coordonnées X de la tile à dessiner
 * @param posY Coordonnées Y de la tile à dessiner
 */
void drawTile(int posX, int posY){

}

/**
 * Récupère les indices x et y de la tile en posX / posY
 * @param mouseX Coordonnées X de la souris
 * @param mouseY Coordonnées Y de la souris
 */
void getIndexMap(int mouseX, int mouseY){

}

/**
 * Affiche un curseur à la position pointée
 * @param mouseX Coordonnées X de la souris
 * @param mouseY Coordonnées Y de la souris
 */
void showCursor(int mouseX, int mouseY){
	posX = SDL_getmousex();
	posY = SDL_getmousey();

}

int main(){
	int x, y, tmpX, tmpY;
	int posX = 0, posY = 0;

	SDL_initWindow(SCREEN_WIDTH, SCREEN_HEIGHT, 0, "Tactics Arena", "M_ICON.png", 1, "global.ttf", 20, 0);

	/* Start a new context named ingame */
	t_context *ingame = SDL_newContext("Tactics Arena", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	for (x = 0; x < N; x++){
			for (y = 0; y < N; y++){

				posX =  x * TILE_W; // Diamond
				posY =  y * TILE_H; //Diamond

				toIso(&posX, &posY);

				centerMap(&posX, &posY);

				if(x == N-1 || y == N-1){
					SDL_newImage(ingame, NULL, "base_Cube.png", posX, posY);
				}else{
					SDL_newImage(ingame, NULL, "base_Tile.png", posX, posY);
				} // Diamond map

				/*if(x == 0 || y == N-1){
					SDL_newImage(ingame, NULL, "base_Cube.png", posX, posY);
				}else{
					SDL_newImage(ingame, NULL, "base_Tile.png", posX, posY);
				}*/ // Slide map

				/*if(x == 0 || y == N-1 || (x == N - 1 && y % 2 != 0) ){
					SDL_newImage(ingame, NULL, "base_Cube.png", posX, posY);
				}else{
					SDL_newImage(ingame, NULL, "base_Tile.png", posX, posY);
				}*/ // Staggered map

			}
	}
	
	SDL_generate(ingame);

	while(1){
		showCursor();


		//SDL_newImage(ingame, NULL, "cursor.png", posX, posY);
		
		SDL_generate(ingame);

		if (SDL_isKeyPressed(SDLK_UP)) {

		}
		/* If user request exit, we need to quit while() */
		if (SDL_requestExit()) break;
	}

	/* Cleanup ingame context */
	SDL_freeContext(ingame);

	return 0;
}