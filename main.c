#include <ESDL.h>
#include <math.h>

#define SCREEN_WIDTH 1280 /**< Largeur de la fenêtre */
#define SCREEN_HEIGHT 720 /**< Hauteur de la fenêtre */
#define HEIGHT_DECOR 82 /**< Hauteur d'un décor */
#define TILE_H 48 /**< Hauteur d'une tile */
#define TILE_W 96 /**< Largeur d'une tile */
#define N 11 /**< Taille de la map */

typedef enum{diamond, staggered, slide}type_Map;

int idCursor = -1; /**< Identifiant du curseur */
int mX = -1; /**< Position X à t-1 de la souris*/
int mY = -1; /**< Position Y à t-1 de la souris*/


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

	toIso(tMap, &posX, &posY); // Convertis les coordonnées en coordonnées isométriques

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
 * Dessine un décor
 * @param context Contexte dans lequel dessiner
 * @param tMap    Type de la map
 * @param posX Coordonnées X de la tile à dessiner
 * @param posY Coordonnées Y de la tile à dessiner
 */
void drawDecor(t_context * context , type_Map tMap, int posX, int posY){
	
	toIso(tMap, &posX, &posY); // Convertis les coordonnées en coordonnées isométriques

	posX += offsetX(tMap);
	posY += offsetY() - HEIGHT_DECOR / 2;

	SDL_newImage(context, NULL, "rock.png", posX, posY);
}

/**
 * Affiche un curseur à la position indiquée
 * @param context Contexte dans lequel dessiner
 * @param tMap Type de la map
 * @param x Indice X de la map
 * @param y Indice Y de la map
 */
void showCursor(t_context * context, type_Map tMap, int x, int y){
	int posX, posY;

	posX = x * TILE_W;
	posY = y * TILE_H;

	toIso(tMap, &posX, &posY);

	posX += offsetX(tMap);
	posY += offsetY();

	if(idCursor == - 1){
		SDL_newImage(context, NULL, "cursor.png", posX, posY); // Initialise le curseur si pas encore dessiné
		idCursor = context->nbImg - 1;
	}else{
		SDL_editImage(context, idCursor, posX, posY); // Met à jour la position du curseur
	}

	SDL_generate(context);
}

/**
 * Affiche un curseur à la position pointée
 * @param context Contexte dans lequel dessiner
 * @param tMap    Type de la map
 */
void showMouseCursor(t_context * context, type_Map tMap){
	int mouseX, mouseY;
	int x = - 1, y = -1;

	mouseX = SDL_getmousex(); // Récupère la position actuelle X de la souris 
	mouseY = SDL_getmousey(); // Récupère la position actuelle Y de la souris 

	getIndexMap(tMap, mouseX, mouseY, &x, &y);

	if(x >= 0 && x < N && y >= 0 && y < N && (mX != x || mY != y)){
			showCursor(context, tMap, x, y); // Affiche en surbillance la zone pointée
			
			mX = x; // Stocke la position actuelle X de la souris
			mY = y; // Stocke la position actuelle Y de la souris
	}else{
		SDL_Delay(50);
	}
}

/**
 * Dessine une carte
 * @param context Contexte dans lequel dessiner
 * @param tMap    Type de la map
 */
void drawMap(t_context * context, type_Map tMap){
	int x, y;
	int posX = 0, posY = 0;

	for (x = 0; x < N; x++){
			for (y = 0; y < N; y++){

				posX =  x * TILE_W;
				posY =  y * TILE_H;

				drawTile(context, tMap, posX, posY);

				if(x % 2 == 0 && y % 2 == 0){
					drawDecor(context, tMap, posX, posY);
				}

			}
	}

}

/**
 * Glisse un objet
 * @param  context Contexte dans lequel glisser
 * @param  typeObj Type de l'objet à glisser
 * @param  idObj   Identifiant de l'objet
 * @return         Retourne 1 en cas de succés, -1 le cas échéant
 */
int drag(t_context * context, t_typeData typeObj, int idObj){
	int posX = - 1, posY = - 1, mouseX = SDL_getmousex(), mouseY = SDL_getmousey();
	int generate = -1, zoneM = 10; // Zone d'erreur de la souris pour plus de précision

	switch (typeObj) {
	
		case IMG:
		
			if (!(context->contextImg) || !(context->nbImg)) return -1;

			if(idObj <= context->nbImg - 1){
				posX = context->contextImg[idObj].x + context->contextImg[idObj].buffer->w / 2;;
				posY = context->contextImg[idObj].y + context->contextImg[idObj].buffer->h / 2;;

				if(posX < mouseX - zoneM || posX > mouseX + zoneM || posY < mouseY - zoneM || posY > mouseY + zoneM){
					SDL_editImage(context, idObj, mouseX - context->contextImg[idObj].buffer->w / 2, mouseY - context->contextImg[idObj].buffer->h / 2);
					generate = 1;
				}

			}else{
				return -1;
			}
			
			break;
		
		case TEXT:
			
			if (!(context->contextText) || !(context->nbText)) return -1;
			
			if(idObj <= context->nbText - 1){
				posX = context->contextText[idObj].x + context->contextText[idObj].buffer->w / 2;
				posY = context->contextText[idObj].y + context->contextText[idObj].buffer->h / 2;;

				if(posX < mouseX - zoneM || posX > mouseX + zoneM || posY < mouseY - zoneM || posY > mouseY + zoneM){
					SDL_editText(context, idObj, context->contextText[idObj].content, context->contextText[idObj].couleur, mouseX - context->contextText[idObj].buffer->w / 2, mouseY - context->contextText[idObj].buffer->h / 2);
					generate = 1;
				}
			}else{
				return -1;
			}
			
			break;

		case SPRITE:
			
			if (!(context->contextSprite) || !(context->nbSprite)) return -1;
			
			if(idObj <= context->nbSprite - 1){
				posX = context->contextSprite[idObj].x + context->contextSprite[idObj].buffer->w / 2;
				posY = context->contextSprite[idObj].y + context->contextSprite[idObj].buffer->h / 2;

				if(posX < mouseX - zoneM || posX > mouseX + zoneM || posY < mouseY - zoneM || posY > mouseY + zoneM){
					
					SDL_editSprite(context, idObj, mouseX - context->contextSprite[idObj].buffer->w / 2, mouseY - context->contextSprite[idObj].buffer->h / 2, 
									context->contextSprite[idObj].position, context->contextSprite[idObj].animation, context->contextSprite[idObj].hide);
					generate = 1;
				}
			}else{
				return -1;
			}
			
			break;
			
		case RECTANGLE:
			
			if (!(context->contextRect) || !(context->nbRect)) return -1;
			
			if(idObj <= context->nbRect - 1){
				posX = context->contextRect[idObj].def.x + context->contextRect[idObj].def.w / 2;;
				posY = context->contextRect[idObj].def.y + context->contextRect[idObj].def.h / 2;;

				if(posX < mouseX - zoneM || posX > mouseX + zoneM || posY < mouseY - zoneM || posY > mouseY + zoneM){
					SDL_editRect(context, idObj,  context->contextRect[idObj].color, context->contextRect[idObj].def.h, context->contextRect[idObj].def.w, mouseX - context->contextRect[idObj].def.w / 2, mouseY - context->contextRect[idObj].def.h / 2);
					generate = 1;
				}
			}else{
				return -1;
			}
			
			break;
		
		default:
			return -1;
			break;
	}
	
	if(generate == 1){
		SDL_generate(context);
	}

	return 1;
}

/**
 * Dépose un objet
 * @param context Contexte dans lequel déposer
 * @param typeObj Type de l'objet
 * @param idObj   Identifiant de l'objet
 * @param posX    Position X où déposer l'objet
 * @param posY    Position Y où déposer l'objet
 * @return 		  Retourne 1 en cas de succés, -1 le cas échéant
 */
int drop(t_context * context, t_typeData typeObj, int idObj, int posX, int posY){

	switch (typeObj) {
	
		case IMG:
		
			if (!(context->contextImg) || !(context->nbImg)) return -1;
			
			if(idObj <= context->nbImg - 1){
				SDL_editImage(context, idObj, posX, posY);
			}else{
				return -1;
			}
			
			break;
		
		case TEXT:
			
			if (!(context->contextText) || !(context->nbText)) return -1;
			
			if(idObj <= context->nbText - 1){
				SDL_editText(context, idObj, context->contextText[idObj].content, context->contextText[idObj].couleur, posX, posY);
			}else{
				return -1;
			}
			
			break;

		case SPRITE:
			
			if (!(context->contextSprite) || !(context->nbSprite)) return -1;
			
			if(idObj <= context->nbSprite - 1){
				SDL_editSprite(context, idObj, posX, posY, context->contextSprite[idObj].position, context->contextSprite[idObj].animation, context->contextSprite[idObj].hide);
			}else{
				return -1;
			}
			
			break;
			
		case RECTANGLE:
			
			if (!(context->contextRect) || !(context->nbRect)) return -1;
			
			if(idObj <= context->nbRect - 1){
				SDL_editRect(context, idObj,  context->contextRect[idObj].color, context->contextRect[idObj].def.h, context->contextRect[idObj].def.w, posX, posY);
			}else{
				return -1;
			}
			
			break;
		
		default:
			return -1;
			break;
	}
	

	return 1;
}

int main(){
	type_Map tMap = diamond;
	int x, y, posX, posY;
	int overObj = -1;

	SDL_initWindow(SCREEN_WIDTH, SCREEN_HEIGHT, 0, "Tactics Arena", "M_ICON.png", 1, "global.ttf", 20, 0);

	// Start a new context named ingame
	t_context *ingame = SDL_newContext("Tactics Arena", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	drawMap(ingame, tMap);

	SDL_newSprite(ingame, "rock.png", colorGreenLight, HEIGHT_DECOR, TILE_W, 288, 128, 1, 1, 0);

	SDL_generate(ingame);

	while(1){

		if(SDL_isMousePressed(SDL_BUTTON_LEFT)){
			overObj = SDL_ismouseover(ingame, SPRITE);
			
			if(overObj >= 0){
				while(SDL_isMousePressed(SDL_BUTTON_LEFT)){
					drag(ingame, SPRITE, overObj);
					SDL_Delay(40);
				}
			}

		}else{

			if(overObj >= 0){
				getIndexMap(tMap, SDL_getmousex(), SDL_getmousey(), &x, &y);

				posX = x * TILE_W;
				posY = y * TILE_H;

				toIso(tMap, &posX, &posY); // Convertis les coordonnées en coordonnées isométriques

				posX += offsetX(tMap);
				posY += offsetY() - HEIGHT_DECOR / 2;

				drop(ingame, SPRITE, overObj, posX, posY);
				SDL_generate(ingame);
			}

			SDL_Delay(50);
			overObj = -1;
		}

		if (SDL_isKeyPressed(SDLK_UP)) {

		}
		// If user request exit, we need to quit while()
		if (SDL_requestExit()) break;

	}

	// Cleanup ingame context
	SDL_freeContext(ingame);

	return 0;
}
