#define WIDTH 1600
#define HEIGHT 900
#define ROTATION 1
#define FOV M_PI/2

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <fstream>
#include <cmath>
#include <iostream>
#include "check_map.hpp"
#include "player.hpp"
#include "affichage.hpp"



int main()
{
	std::vector <std::string> lignes;									// On crée un tableau de chaîne de charactère.
	std::string ligne;													// On crée un chaîne de charactère.						

	std::ifstream map("../map.rc");										// On ouvre un flux permettant de lire la map.

	sf::SoundBuffer buff_fire;
	sf::SoundBuffer buff_cant_fire;

    buff_fire.loadFromFile("../images_sons/bruit_tir.wav");
    buff_cant_fire.loadFromFile("../images_sons/bruit_tir_impossible.wav");

    sf::Sound tir(buff_fire);
    sf::Sound cant_fire(buff_cant_fire);
	

	sf::Font font;
	font.loadFromFile("arial.ttf");
	
	sf::Texture tex_gun_is_shooting;
	tex_gun_is_shooting.loadFromFile("../images_sons/pistolet_en_tir.png");

	sf::Texture tex_gun;
	tex_gun.loadFromFile("../images_sons/pistolet_normal.png");
	sf::Time temps_arme;

	sf::Texture tex_balle;
	tex_balle.loadFromFile("../images_sons/balle.png");

	sf::Sprite balle(tex_balle);
	balle.setColor(sf::Color(180, 180, 180));
	balle.setPosition(1475, 890);
	balle.setScale(0.07, 0.15);
	balle.setRotation(-90);

	sf::Sprite arme(tex_gun);
	arme.setPosition(680, 654);
	arme.setScale(0.8, 0.8);

	unsigned int nombre_balles = 1000;

	sf::Text affichage_nb_balles(std::to_string(nombre_balles), font, 60); 
	affichage_nb_balles.setPosition(1500, 830);

	bool accroupi = false;												// On crée un bool qui indiquera si le personnage est accroupi ou non.
	float VITESSE = 1.7;												// On crée un float qui indiquera la vitesse de déplacement du personnage.
	sf::Clock chrono;													// On crée un chrono.
	sf::Time temps;														// On crée un temps.

	while(getline(map, ligne))											// Tant qu'on a pas parcouru toutes les lignes de la map
	{
		lignes.push_back(ligne);										// Alors on ajoute dans notre tableau de chaîne de charactère la "ligne" ème ligne.													
	}

	if(!check_map(lignes))												// Puis on effectue une vérification de la map.
		return EXIT_FAILURE;											// Si une erreur a été rencontrée, alors on quitte le programme.

	player joueur(lignes);												// On crée un joueur.

	sf::RenderWindow fenetre(sf::VideoMode(1600, 900), "RayCasting", sf::Style::Fullscreen);	// Création d'une fenêtre de taille 1600 x 900 en plein écran nommée "RayCasting".
	fenetre.setVerticalSyncEnabled(true);								// On synchronise le temps de rafraichissement de pixel de l'image avec celui du CPU.
	fenetre.setMouseCursorVisible(false);								// On rend la souris invisible.

	bool can_shoot = true;												// Création d'un bool qui indiquera si on peut tirer.

	sf::Event evenement;												// Création d'un évènement.

	sf::Image image;													// Déclaration d'une image.
	image.create(WIDTH, HEIGHT, sf::Color::Black);						// Création de l'image.				

	sf::Texture texture;												// Création d'une texture.
	sf::Sprite spa;														// Création d'un sprite.

	sf::Vector2i pos_old = sf::Mouse::getPosition(fenetre);				// On change la valeur de l'ancienne position de la souris.

	while(fenetre.isOpen())												// Tant que la fenètre est ouverte
	{
		// for (double long i = 0; i < 50000000; ++i)	{}				// (Simulation de pc pas puissant)
		affichage_nb_balles.setString(std::to_string(nombre_balles));

		temps = chrono.restart();										// Réinisialisation le chrono et assignement de temps à sa valeur précédente.
		temps_arme = temps_arme + temps;								// Changement du temps de tir.

		diviser_ray(joueur, image, lignes, accroupi);					// Euh...					

		texture.loadFromImage(image);									// Chargement de l'image
		spa.setTexture(texture);										// Changement de texture

		while(fenetre.pollEvent(evenement))								// Tant qu'un évènement est détecté
		{
			if(evenement.type == sf::Event::Closed)						// Si l'évènement détecté est la fermeture de la fenêtre
				fenetre.close();										// Alors on ferme de la fenêtre.

			if(pos_old.x != 800)
			{
				pos_old = sf::Vector2i(800, sf::Mouse::getPosition().y);
				sf::Mouse::setPosition(pos_old);
			}

			if(nombre_balles > 999)
				nombre_balles = 999;

			if (evenement.type == sf::Event::MouseMoved)
			{
			    if (evenement.mouseMove.x > pos_old.x)
			    	joueur.rot(true, sf::Mouse::getPosition(fenetre), pos_old, temps);

			    if (evenement.mouseMove.x < pos_old.x)
			    	joueur.rot(false, sf::Mouse::getPosition(fenetre), pos_old, temps);

			    pos_old = sf::Mouse::getPosition(fenetre);
			}

			if (evenement.type == sf::Event::MouseButtonPressed)
			{
				if (nombre_balles > 0 && can_shoot == true)
				{
				    if (evenement.mouseButton.button == sf::Mouse::Left)
				    {
				    	tir.play();
				    	arme.setTexture(tex_gun_is_shooting);
				    	temps_arme = sf::Time::Zero;
				    	nombre_balles--;
				  		can_shoot = false;
				    }
				}
				else
				{
					if(nombre_balles == 0)
					{

					}
				}
			}
			
		}

		if(temps_arme.asSeconds() > 0.25)
		{
			arme.setTexture(tex_gun);
		}

		if(temps_arme.asSeconds() > 0.5)
		{
			can_shoot = true;
			temps_arme = sf::Time::Zero;
		}

		if(VITESSE > 0)
			goto deplacements;

		std::cout << "ERREUR VITESSE" << std::endl;

		#include "deplacements.hpp"

		fenetre.clear(sf::Color::Black);								// Effacement de tout ce qui apparait sur la fenêtre avec la couleur noir.
		fenetre.draw(spa);												// Dessinage du lieu.
		fenetre.draw(arme);												// Dessinage de l'arme.
		fenetre.draw(affichage_nb_balles);								// Dessinage du nombre de balles.
		fenetre.draw(balle);											// Dessinage de la balle.
		fenetre.display();												// Affichage de tout ce qui a été dessiné.
	}

	return 0;															// On ferme le programme à la fin.
}
