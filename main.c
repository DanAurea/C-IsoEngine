#include <ESDL.h>
#include <math.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define TILE_H 48
#define TILE_W 96
#define N 11

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
	posY -= offsetY();

	* x = ( posX/ (TILE_W/2) + posY/(TILE_H/2)) / 2;
	* y = ( posY/ (TILE_H/2) - posX/(TILE_W/2)) / 2;
	
	printf("x : %i y : %i \n", * x, * y);

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
 * @param mouseX Coordonnées X de la souris
 * @param mouseY Coordonnées Y de la souris
 */
void showCursor(int mouseX, int mouseY){
	
}

int main(){
	int x, y, mouseX, mouseY;
	int posX = 0, posY = 0;

	SDL_initWindow(SCREEN_WIDTH, SCREEN_HEIGHT, 0, "Tactics Arena", "M_ICON.png", 1, "global.ttf", 20, 0);

	/* Start a new context named ingame */
	t_context *ingame = SDL_newContext("Tactics Arena", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	for (x = 0; x < N; x++){
			for (y = 0; y < N; y++){

				posX =  x * TILE_W;
				posY =  y * TILE_H;

				drawTile(ingame, diamond, posX, posY);

			}
	}
	
	SDL_generate(ingame);

	while(1){
		mouseX = SDL_getmousex();
		mouseY = SDL_getmousey();
		
		getIndexMap(diamond, mouseX, mouseY, &x, &y);

		showCursor(mouseX, mouseY); // Affiche en surbillance la zone pointée
		
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