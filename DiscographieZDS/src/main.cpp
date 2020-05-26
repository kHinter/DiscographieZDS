#include <ctype.h>
#include <algorithm>
#include <set>
#include <map>
#include <fstream>
#include <tuple>

#include "operators.hpp"

#define ERROR_MESSAGE std::cerr << "Erreur, veuillez reesayer" << std::endl;
#define PATH "disco.txt"

/*déclarations*/
void uselessBlankClear(std::string&);
bool allBlankOrEmpty(std::string&);

void ajouter(Discographie&, std::string&);

void afficher_titres(Discographie&);
void afficher_albums(Discographie&);
void afficher_artistes(Discographie&);

void enregistrer(const Discographie&, bool&);
void charger(Discographie&, bool&);

int main()
{
	Discographie disco{};
	std::string line;
	bool issaved = false;

	while (true)
	{

		int i = 0;
		std::cout << ">";
		std::getline(std::cin, line);

		//TODO vérifier que le caractère suivant le substr est "blank"

		//gestion des commandes utilisateur
		if (line.substr(0, 7) == "ajouter")
		{
			line.erase(0, 7);
			ajouter(disco, line);
		}

		else if (line.substr(0, 17) == "afficher morceaux")
		{
			if (line.length() >= 18 && !isblank(line[17]))
			{
				ERROR_MESSAGE;
				continue;
			}
			afficher_titres(disco);
		}
			
		else if (line.substr(0, 15) == "afficher albums")
		{
			if (line.length() >= 16 && !isblank(line[15]))
			{
				ERROR_MESSAGE;
				continue;
			}
			afficher_albums(disco);
		}
		else if (line.substr(0, 17) == "afficher artistes")
		{
			if (line.length() >= 18 && !isblank(line[17]))
			{
				ERROR_MESSAGE;
				continue;
			}
			afficher_artistes(disco);
		}

		else if (line.substr(0, 11) == "enregistrer")
		{
			if (line.length() >= 12 && !isblank(line[11]))
			{
				ERROR_MESSAGE;
				continue;
			}
			enregistrer(disco, issaved);
		}
		else if (line.substr(0, 7) == "charger")
		{
			if (line.length() >= 8 && !isblank(line[7]))
			{
				ERROR_MESSAGE;
				continue;
			}
			charger(disco, issaved);
		}
		else if (line.substr(0, 7) == "quitter") //QUITTER
		{
			if (line.length() >= 8 && !isblank(line[7]))
			{
				ERROR_MESSAGE;
				continue;
			}
			break;
		}
		else
		{
			ERROR_MESSAGE;
		}
	}
	return 0;
}

void uselessBlankClear(std::string& line)
{
	//CLEAR PREPHRASE
	unsigned int i = 0;
	for (; i < line.length(); i++)
	{
		if (!isblank(line[i])) break;
	}

	line.erase(0, i);
	i = line.length();

	//CLEAR APRESPHRASE

	for (; i > 0; i--)
	{
		if (!isblank(line[i])) break;
	}
	line.erase(i, line.length());

	//TODO : ESPACES DANS LA PHRASE A CLEAR
}

bool allBlankOrEmpty(std::string& line)
{
	if (line.empty()) return true;

	for (char c : line)
	{
		if (!isblank(c)) return false;
	}

	return true;
}

void ajouter(Discographie& disco, std::string& line)
{
	Morceaux nouveau_morceau{ "titre inconnu", "album inconnu", "artiste inconnu" };

	if (allBlankOrEmpty(line))
	{
		disco.emplace_back(nouveau_morceau); //VERIFIE cas : ajouter <None>
		return;
	}
	if (!isblank(line[0])) //Vérifie si c'est une autre lettre collée a la commande
	{
		ERROR_MESSAGE;
		return;
	}

	if (std::count(line.begin(), line.end(), '|') > 2) //Pas plus de 2 séparateurs
	{
		ERROR_MESSAGE;
		return;
	}

	auto debut{ line.begin() };
	auto separateur{ std::find(debut, line.end(), '|')};

	if (separateur != line.end())
	{
		std::string temp{ debut, separateur };
		uselessBlankClear(temp);

		if (!allBlankOrEmpty(temp)) nouveau_morceau.titre = temp;
	}
	else
	{
		std::string temp{ debut, line.end()};
		uselessBlankClear(temp);

		if (!allBlankOrEmpty(temp)) nouveau_morceau.titre = temp;

		disco.emplace_back(nouveau_morceau);
		return;
	}
	
	debut = separateur + 1;
	separateur = std::find(debut, line.end(), '|');

	if (separateur != line.end())
	{
		std::string temp{ debut, separateur };
		uselessBlankClear(temp);

		if (!allBlankOrEmpty(temp)) nouveau_morceau.album = temp;
	}
	else
	{
		std::string temp{ debut, line.end() };
		uselessBlankClear(temp);

		if (!allBlankOrEmpty(temp)) nouveau_morceau.album = temp;

		disco.emplace_back(nouveau_morceau);
		return;
	}

	debut = separateur + 1;
	separateur = std::find(debut, line.end(), '|');

	if (separateur != line.end())
	{
		std::string temp{ debut, separateur };
		uselessBlankClear(temp);

		if (!allBlankOrEmpty(temp)) nouveau_morceau.artiste = temp;
	}
	else
	{
		std::string temp{ debut, line.end() };
		uselessBlankClear(temp);

		if (!allBlankOrEmpty(temp)) nouveau_morceau.artiste = temp;

		disco.emplace_back(nouveau_morceau);
		return;
	}
}

void afficher_titres(Discographie& disco)
{
	std::vector<std::string> morceaux{};
	//remplir le vec morceaux pour le trier après

	for (auto[titre, album, artiste] : disco)
	{
		morceaux.emplace_back(titre + " | " + album + " | " + artiste);
	}
	std::sort(morceaux.begin(), morceaux.end());

	for (auto& morceau : morceaux)
	{
		std::cout << morceau << std::endl;
	}
}

void afficher_albums(Discographie& disco)
{
	std::map<std::string, std::vector<std::string>> albums{};

	for (auto [titre, album, artiste] : disco)
	{
		albums[album + " | " + artiste].emplace_back(titre);
	}
	for (auto[first, second] : albums)
	{
		std::sort(second.begin(), second.end()); //trier les titres de chaques albums

		/* affichage */
		std::cout << "--> " << first << std::endl;
		for (auto& titre : second)
		{
			std::cout << "\t/--> " << titre << std::endl;
		}
	}
}

void afficher_artistes(Discographie& disco)
{
	std::map<std::tuple<std::string, std::string>, std::vector<std::string>> Table{};
	std::string artiste_precedant;

	for (const auto[titre, album, artiste] : disco)
	{
		Table[make_tuple(artiste, album)].emplace_back(titre);
	}
	for (auto& elem : Table)
	{
		if (artiste_precedant != std::get<0>(elem.first))
		{
			std::cout << "--> " << std::get<0>(elem.first) << std::endl;
		}
		std::cout << "\t/--> " << std::get<1>(elem.first) << std::endl;

		for (auto& artiste : elem.second)
		{
			std::cout << "\t\t/--> " << artiste << std::endl;
		}

		artiste_precedant = std::get<0>(elem.first);
	}
}

void enregistrer(const Discographie& disco, bool& issaved)
{
	std::ofstream file(PATH, std::ios::ate);
	if (file)
	{
		file.seekp(0, std::ios::beg); //Réecriture à chaque enregistrement
		for (const auto& elem : disco)
		{
			file << elem << std::endl;
		}
		std::cout << "Donnee enregistrees avec succes !" << std::endl;
		file.close();
	}
	else
	{
		throw std::runtime_error("Le fichier n'a pas reussi a s'ouvrir en ecriture");
	}
	issaved = true;
}

void charger(Discographie& disco, bool& issaved)
{
	if (!issaved)
	{
		std::ifstream file(PATH);
		if (file)
		{
			Morceaux morceau{};

			while (!file.eof())
			{
				std::getline(file, morceau.titre, ';');
				std::getline(file, morceau.album, ';');
				std::getline(file, morceau.artiste);

				if (!allBlankOrEmpty(morceau.titre))
				{
					disco.emplace_back(morceau);
				}
			}
			std::cout << "Donnee importees avec succes !" << std::endl;
			issaved = true;
			file.close();
		}
		else
		{
			throw std::runtime_error("Le fichier n'a pas reussi a s'ouvrir en lecture");
		}
	}
}