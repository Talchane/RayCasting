#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include "check_map.hpp"

#define WIDTH 1600
#define HEIGHT 900
#define VITESSE 2
#define ROTATION 1
#define FOV M_PI/2

std::vector <std::string> lignes;									// On crée un tableau de chaîne de charactère.
std::string ligne;													// On crée un chaîne de charactère.						

std::ifstream map("../map.rc");										// On ouvre un flux permettant de lire la map.


sf::Vector2f get_x(std::vector<std::string> const& lignes)
{
	for (unsigned int i = 0; i < lignes.size(); ++i)
	{
		for (unsigned int j = 0; j  < lignes.back().size(); ++j)
		{
			if(lignes[i][j] == 'X')
				return sf::Vector2f(j, i);
		}
	}
	return (sf::Vector2f(-1, -1));
}


struct player
{
	sf::Vector2f dir = sf::Vector2f(0, -1);
	sf::Vector2f pos = get_x(lignes);

	void move(unsigned int sens, sf::Time temps)
	{
		if(sens == 1)
		{
			pos.x = pos.x + dir.x * VITESSE * temps.asSeconds();
			pos.y = pos.y + dir.y * VITESSE * temps.asSeconds();
		}
		if(sens == 2)
		{
			pos.x = pos.x - dir.x * VITESSE * temps.asSeconds();
			pos.y = pos.y - dir.y * VITESSE * temps.asSeconds();
		}
		/*if(sens == 3)
		{
			pos.x = pos.x - (dir.x - M_PI/2) * VITESSE * temps.asSeconds();
			pos.y = pos.y - (dir.y - M_PI/2) * VITESSE * temps.asSeconds();
		}
		if(sens == 4)
		{
			pos.x = pos.x + (dir.x + M_PI/2) * VITESSE * temps.asSeconds();
			pos.y = pos.y + (dir.y + M_PI/2) * VITESSE * temps.asSeconds();
		}*/

	}
	void rot(bool sens, sf::Vector2i pos_mouse, sf::Vector2i pos_old, sf::Time temps)
	{
		float	tmp = dir.x;
		float	degree = ROTATION * ((sens) ? -(pos_mouse.x - pos_old.x) / 30 * temps.asSeconds() : (pos_old.x - pos_mouse.x) / 30 * temps.asSeconds());

		dir.x = dir.x * cos(degree) - dir.y * sin(degree);
		dir.y = tmp * sin(degree) + dir.y * cos(degree);

		float norme = sqrt(dir.x*dir.x+dir.y*dir.y);
		dir = dir / norme;
	}
};



float dist_mur(sf::Vector2f const& dir, sf::Vector2f const& pos, std::vector<std::string> lignes)
{
	//which box of the map we're in
	int mapX = (int)pos.x;
	int mapY = (int)pos.y;
	//length of ray from current position to next x or y-side
	float sideDistX;
	float sideDistY;
	//length of ray from one x or y-side to next x or y-side
	float deltaDistX = fabs(1 / dir.x);
	float deltaDistY = fabs(1 / dir.y);
	//what direction to step in x or y-direction (either +1 or -1)
	int stepX;
	int stepY;
	int side; //was a NS or a EW wall hit?
	(void)side;
	//calculate step and initial sideDist
	if (dir.x < 0)
	{
		stepX = -1;
		sideDistX = (pos.x - mapX) * deltaDistX;
	}
	else
	{
		stepX = 1;
		sideDistX = (mapX + 1.0 - pos.x) * deltaDistX;
	}
	if (dir.y < 0)
	{
		stepY = -1;
		sideDistY = (pos.y - mapY) * deltaDistY;
	}
	else
	{
		stepY = 1;
		sideDistY = (mapY + 1.0 - pos.y) * deltaDistY;
	}
	while (lignes[mapY][mapX] != '1')			// L'erreur est entre ces parenthèse.
	{
		if (sideDistX < sideDistY)
		{
			sideDistX = sideDistX + deltaDistX;
			mapX = mapX + stepX;
			side = 0;
		}
		else
		{
			sideDistY = sideDistY + deltaDistY;
			mapY = mapY + stepY;
			side = 1;
		}
	}
	if (!side)
		return ((mapX - pos.x + (1 - stepX) / 2) / dir.x);
	else
		return ((mapY - pos.y + (1 - stepY) / 2) / dir.y);
}

void afficher_mur(float const& dist_mur, sf::Image &image, unsigned int colonne)
{
	unsigned short int ratio = HEIGHT * 0.5;
	unsigned short int mur_h = (unsigned short int)(ratio / dist_mur);
	unsigned short int rab_cl = (unsigned short int)((HEIGHT - mur_h) / 2);


	for (int i = 0; i < rab_cl && i < HEIGHT; ++i)
	{
		image.setPixel(colonne, i, sf::Color(80, 80, 80));  //80, 80, 80
	}

	for (int i = rab_cl; i < mur_h + rab_cl && i < HEIGHT; ++i)
	{
		image.setPixel(colonne, i, sf::Color(20, 20, 20));  // 20 20 20
	}

	for (int i = mur_h + rab_cl; i < HEIGHT; ++i)
	{
		image.setPixel(colonne, i, sf::Color(40, 40, 40));  // 40 40 40
	}


}

void diviser_ray(const player &joueur, sf::Image &image, std::vector<std::string> lignes)
{
	float diff_ray = FOV / WIDTH;

	sf::Vector2f vec;

	vec.x = joueur.dir.x * cos(FOV / 2) - joueur.dir.y * sin(FOV / 2);
	vec.y = joueur.dir.x * sin(FOV / 2) + joueur.dir.y * cos(FOV / 2);

	unsigned int w = 1;

	float test_x;
	float test_y;

	test_x = vec.x;
	test_y = vec.y;

	//	afficher_mur(dist_mur(vec, joueur.pos, lignes), image, w, vec);

	for (w = 0; w < WIDTH; ++w)
	{
		vec.x = test_x * cos(-diff_ray) - test_y * sin(-diff_ray);
		vec.y = test_x * sin(-diff_ray) + test_y * cos(-diff_ray);


		test_x = vec.x;
		test_y = vec.y;
		afficher_mur(dist_mur(vec, joueur.pos, lignes), image, w);
	}
}


int main()
{
	sf::Clock chrono;													// On crée un chrono.
	sf::Time temps;														// On crée un temps.

	while(getline(map, ligne))											// Tant qu'on a pas parcouru toutes les lignes de la map
	{
		lignes.push_back(ligne);										// Alors on ajoute dans notre tableau de chaîne de charactère la "ligne" ème ligne.													
	}

	if(!check_map(lignes))												// Puis on effectue une vérification de la map.
		return EXIT_FAILURE;												// Si une erreur a été rencontrée, alors on quitte le programme.

	player joueur;														// On crée un joueur.

	sf::RenderWindow fenetre(sf::VideoMode(1600, 900), "RayCasting", sf::Style::Fullscreen);	// Création d'une fenêtre de taille 1600 x 900 pixels nommée "RayCasting".
	fenetre.setVerticalSyncEnabled(true);
	fenetre.setMouseCursorVisible(false);

	sf::Event evenement;												// Création d'un évènement.

	sf::Image image;													// Déclaration d'une image.
	image.create(WIDTH, HEIGHT, sf::Color::Black);						// Création de l'image.				

	sf::Texture texture;												// Création d'une texture.
	sf::Sprite spa;														// Création d'un sprite.

	sf::Vector2i pos_old = sf::Mouse::getPosition(fenetre);

	while(fenetre.isOpen())												// Tant que la fenètre est ouverte
	{
		//for (double long i = 0; i < 50000000; ++i)	{}				// (Simulation de pc pas puissant)

		temps = chrono.restart();										// Réinisialisation le chrono et assignement de temps à sa valeur précédente.

		diviser_ray(joueur, image, lignes);								// Euh...					

		texture.loadFromImage(image);									// Chargement de l'image
		spa.setTexture(texture);										// Changement de texture

		while(fenetre.pollEvent(evenement))								// Tant qu'un évènement est détecté
		{
			if(evenement.type == sf::Event::Closed)						// Si l'évènement détecté est la fermeture de la fenêtre
				fenetre.close();										// Alors on ferme de la fenêtre.

			if (evenement.type == sf::Event::MouseMoved)
			{
			    if (evenement.mouseMove.x > pos_old.x)
			    	joueur.rot(true, sf::Mouse::getPosition(fenetre), pos_old, temps);

			    if (evenement.mouseMove.x < pos_old.x)
			    	joueur.rot(false, sf::Mouse::getPosition(fenetre), pos_old, temps);

			    pos_old = sf::Mouse::getPosition(fenetre);
			}
			if(pos_old.x > 800 || pos_old.x < 800)
			{
				pos_old = sf::Vector2i(800, sf::Mouse::getPosition().y);
				sf::Mouse::setPosition(pos_old);
			}
			
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))			// Si on appuie sur échape
			fenetre.close();											// Alors on ferme la fenêtre

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z))				// Si on appuie sur la flèche du haut
			joueur.move(1, temps);									// Alors on bouge le joueur

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			joueur.move(2, temps);

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			joueur.move(3, temps);

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			joueur.move(4, temps);

		fenetre.clear(sf::Color::Black);								// Effacement de tout ce qui apparait sur la fenêtre avec la couleur noir.
		fenetre.draw(spa);
		fenetre.display();												// Affichage de tout ce qui a été dessiné.
	}

	return 0;															// On ferme le programme à la fin.
}
