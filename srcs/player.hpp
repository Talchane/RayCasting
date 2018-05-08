#ifndef PLAYER_HPP
#define PLAYER_HPP

class player
{
private:
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

public:
	player(std::vector<std::string> _lignes) : pos(get_x(_lignes))
	{
		dir = sf::Vector2f(0, -1);
	}

	sf::Vector2f dir;
	sf::Vector2f pos;

	

	void move(unsigned int sens, float const& VITESSE, sf::Time temps, std::vector<std::string> const& lignes)
	{
		if(sens == 1)
		{
			pos.x = pos.x + dir.x * VITESSE * temps.asSeconds();
			pos.y = pos.y + dir.y * VITESSE * temps.asSeconds();

			if(lignes[(int)pos.y][(int)pos.x] == '1')
				pos.x = pos.x - dir.x * VITESSE * temps.asSeconds();
		}
		if(sens == 2)
		{
			pos.x = pos.x - dir.x * (VITESSE - 1) * temps.asSeconds();
			pos.y = pos.y - dir.y * (VITESSE - 1) * temps.asSeconds();
		}
		if(sens == 3)
		{

			pos.x = pos.x - dir.y  * (VITESSE - 0.8) * temps.asSeconds();
			pos.y = pos.y + dir.x * (VITESSE - 0.8) * temps.asSeconds();
		}
		if(sens == 4)
		{
			pos.x = pos.x + dir.y  * (VITESSE - 0.8) * temps.asSeconds();
			pos.y = pos.y - dir.x * (VITESSE - 0.8) * temps.asSeconds();
		}

	}
	void rot(bool sens, sf::Vector2i pos_mouse, sf::Vector2i pos_old, sf::Time temps)
	{
		float	tmp = dir.x;
		float	degree = ROTATION * ((sens) ? -(pos_mouse.x - pos_old.x) / 18 * temps.asSeconds() : (pos_old.x - pos_mouse.x) / 18 * temps.asSeconds());

		dir.x = dir.x * cos(degree) - dir.y * sin(degree);
		dir.y = tmp * sin(degree) + dir.y * cos(degree);

		float norme = sqrt(dir.x*dir.x+dir.y*dir.y);
		dir = dir / norme;
	}
};

#endif