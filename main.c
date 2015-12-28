#include <ESDL.h>
#include <math.h>

#define SCREEN_WIDTH 1280 /**< Largeur de la fenêtre */
#define SCREEN_HEIGHT 720 /**< Hauteur de la fenêtre */
#define TILE_H 48 /**< Hauteur d'une tile */
#define TILE_W 96 /**< Largeur d'une tile */
#define N 11 /**< Taille de la map */

typedef enum{diamond, staggered, slide}type_Map;

/**
 * Convertis les coordonnées cartésiennes en coordonnées isométriques
 * @param tMap Type de la carte
 * @param x Position horizontale cartésienne
 * @param y Position verticale cartésienne
 */
void toIso(type_Map tMap, int * x, int * y){

	if(tMap == diamond){
		* x = (* x - * y * 2) / 2;
		* y = (* x + * y * 2) / 2;
	}else if(tMap == slide || tMap == staggered){

		if(tMap == slide){
			* x = (* x + * y);
		}else{
			* x = * x + ((* y / TILE_H % 2 == 0) ? 0 : 1 ) * (TILE_W/2);
		}

		* y = (* y / 2);
	}

}

/**
 * Décalage en X
 * @param tMap Type de la carte
 */
int offsetX(type_Map tMap){

	if(tMap == diamond)
		return (SCREEN_WIDTH - TILE_W) / 2; // Diamond map
	else
		return (SCREEN_WIDTH - (N + 1)  * TILE_W) / 2; // Slide AND Staggered map
}

/**
 * Décalage en Y
 */
int offsetY(){
	return (SCREEN_HEIGHT - N * TILE_H) /2;
}

/**
 * Récupère les indices x et y de la tile en posX / posY
 * @param tMap Type de la carte
 * @param posX Coordonnée X pointée
 * @param posY Coordonnée Y pointée
 * @param x Pointeur sur l'indice x
 * @param x Pointeur sur l'indice y
 */
void getIndexMap(type_Map tMap, int posX, int posY, int * x, int * y){
	posX -= offsetX(tMap);

	if(tMap == diamond){
		posX -= TILE_W / 2; // Diamond map
		posY -= offsetY();

		* x = floor( ( (float)posX/ (TILE_W/2) + (float)posY/(TILE_H/2)) / 2 );;
		* y = floor( ( (float)posY/ (TILE_H/2) - (float)posX/(TILE_W/2)) / 2 );;

	}

}

/**
 * Dessine une tile aux coordonnées posX et posY
 * @param context Contexte dans lequel dessiner
 * @param tMap Type de la carte
 * @param posX Coordonnées X de la tile à dessiner
 * @param posY Coordonnées Y de la tile à dessiner
 */
void drawTile(t_context * context , type_Map tMap, int posX, int posY){
	int x = posX / TILE_W, y = posY / TILE_H;

	toIso(tMap, &posX, &posY);

	posX += offsetX(tMap);
	posY += offsetY();

	if(y == N-1 || ( x == 0 && (tMap == slide || tMap == staggered) )){
		SDL_newImage(context, NULL, "base_Cube.png", posX, posY);
	}

	if(tMap == diamond){
		if(x == N-1){
			SDL_newImage(context, NULL, "base_Cube.png", posX, posY);
		}else{
			SDL_newImage(context, NULL, "base_Tile.png", posX, posY);
		}
	}else if(tMap == slide){
		if(x != 0){
			SDL_newImage(context, NULL, "base_Tile.png", posX, posY);
		}
	}else if(tMap == staggered){
		if((x == N - 1 && y % 2 != 0)){
			SDL_newImage(context, NULL, "base_Cube.png", posX, posY);
		}else{
			SDL_newImage(context, NULL, "base_Tile.png", posX, posY);
		}
	}

}

/**
 * Affiche un curseur à la position pointée
 * @param context Contexte dans lequel dessiner
 * @param idCursor Identifiant du curseur
 * @param tMap Type de la map
 * @param x Indice X de la map
 * @param y Indice Y de la map
 */
void showCursor(t_context * context, int * idCursor, type_Map tMap, int x, int y){
	int posX, posY;

	posX = x * TILE_W;
	posY = y * TILE_H;

	toIso(tMap, &posX, &posY);

	posX += offsetX(tMap);
	posY += offsetY();

	if(* idCursor == - 1){
		SDL_newImage(context, NULL, "cursor.png", posX, posY); // Initialise le curseur si pas encore dessiné
		* idCursor = context->nbImg - 1;
	}else{
		SDL_editImage(context, * idCursor, posX, posY); // Met à jour la position du curseur
	}
}



int main(){
	int x, y, mouseX, mouseY, idCursor = -1;
	int posX = 0, posY = 0;
	int tmpX = -1, tmpY = -1;
	type_Map tMap = diamond;

	SDL_initWindow(SCREEN_WIDTH, SCREEN_HEIGHT, 0, "Tactics Arena", "M_ICON.png", 1, "global.ttf", 20, 0);

	// Start a new context named ingame
	t_context *ingame = SDL_newContext("Tactics Arena", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	for (x = 0; x < N; x++){
			for (y = 0; y < N; y++){

				posX =  x * TILE_W;
				posY =  y * TILE_H;

				drawTile(ingame, tMap, posX, posY);

			}
	}

	SDL_generate(ingame);

	x = -1;
	y = -1;

	while(1){
		mouseX = SDL_getmousex();
		mouseY = SDL_getmousey();

		getIndexMap(tMap, mouseX, mouseY, &x, &y);

		if(tmpX != x || tmpY != y){
			if(x >= 0 && x < N && y >= 0 && y < N){
				showCursor(ingame, &idCursor, tMap, x, y); // Affiche en surbillance la zone pointée

				tmpX = x;
				tmpY = y;
			}
		}

		SDL_generate(ingame);

		if (SDL_isKeyPressed(SDLK_UP)) {

		}
		// If user request exit, we need to quit while()
		if (SDL_requestExit()) break;
	}

	// Cleanup ingame context
	SDL_freeContext(ingame);
	return 0;
}
