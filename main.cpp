#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <cstring>
#include <cmath>
#include <vector>
#include <iomanip>

typedef struct{
	std::string bez;
	float wert;
	std::string dim;
}bezWertPaar;

void FDFAuslesen(char* DateiName);
void UmfangAuslesen(std::ifstream &file, std::vector<bezWertPaar> &liste);
void FlaecheAuslesen(std::ifstream &file, std::vector<bezWertPaar> &liste);
void LaengeAuslesen(std::ifstream &file, std::vector<bezWertPaar> &liste);

size_t SchlussklammerFinden(std::string Zeile, size_t start);

void BildschirmAusgabe(std::vector<bezWertPaar> &liste);
bool DateiAusgabe(char* DateiName, std::vector<bezWertPaar> &liste);

bezWertPaar BezeichnungWertTrennen(std::string eingabe);

int main(int argc, char** argv)
{
	std::ifstream file;
	if(argc == 1)
	{
		std::cout<<"Keine Datei zum Auswerten angegeben\n";
		system("PAUSE");
		return 1;
	}

	FDFAuslesen(argv[1]);
	
	system("PAUSE");
	return 0;
}

void FDFAuslesen(char* DateiName)
{
	std::ifstream file;
	
	std::cout<<"Versuche "<<DateiName<<" zu oeffnen...";
	file.open(DateiName, std::ios::in);
	
	if(!file.good())
	{
		std::cout<<"Klappt nicht\n";
		system("PAUSE");
		return ;
	}
	std::cout<<"geoeffnet\n";
	
	std::vector<bezWertPaar> liste;
	
	UmfangAuslesen(file, liste);
	
	file.clear();
	file.seekg(0L, file.beg); //Zeiger an Anfang von file setzen
	
	FlaecheAuslesen(file, liste);
	
	file.clear();
	file.seekg(0L, file.beg); //Zeiger an Anfang von file setzen
	
	LaengeAuslesen(file, liste);

	BildschirmAusgabe(liste);
	DateiAusgabe(DateiName, liste);
	
	file.close();

	return;
}

void UmfangAuslesen(std::ifstream &file, std::vector<bezWertPaar> &liste)
{
	char leseZeile[4001];
	std::string Zeile;
	std::string subZeile;
	std::string suchBegriff;
	std::size_t found, foundEnde;
	
	bezWertPaar paareContainer;

	while(!file.eof())
	{
		file.getline(leseZeile, 4001);
		Zeile = leseZeile;
		suchBegriff = "<</C";
		foundEnde = Zeile.find(suchBegriff);
		if(foundEnde == 0)
		{
			found = Zeile.find("/PolyLineDimension");
			if(found != std::string::npos)
			{
				suchBegriff = "/Contents(";
				found = Zeile.find(suchBegriff);
				if(found != std::string::npos)
				{
					found += suchBegriff.length() - 1;
					foundEnde = SchlussklammerFinden(Zeile, found);
					if(foundEnde != std::string::npos)
					{
						subZeile = Zeile.substr(found+1, foundEnde - found - 1);
						paareContainer = BezeichnungWertTrennen(subZeile);
						liste.push_back(paareContainer);
					}
				}
				
			}
		}
	}
}

void FlaecheAuslesen(std::ifstream &file, std::vector<bezWertPaar> &liste)
{
	char leseZeile[4001];
	std::string Zeile;
	std::string subZeile;
	std::string suchBegriff;
	std::size_t found, foundEnde;
	
	bezWertPaar paareContainer;

	while(!file.eof())
	{
		file.getline(leseZeile, 4001);
		Zeile = leseZeile;
		suchBegriff = "<</C";
		foundEnde = Zeile.find(suchBegriff);
		if(foundEnde == 0)
		{
			found = Zeile.find("/PolygonDimension");
			if(found != std::string::npos)
			{
				suchBegriff = "/Contents(";
				found = Zeile.find(suchBegriff);
				if(found != std::string::npos)
				{
					found += suchBegriff.length() - 1;
					foundEnde = SchlussklammerFinden(Zeile, found);
					if(foundEnde != std::string::npos)
					{
						subZeile = Zeile.substr(found+1, foundEnde - found - 1);
						paareContainer = BezeichnungWertTrennen(subZeile);
						liste.push_back(paareContainer);
					}
				}
				
			}
		}
	}
}

void LaengeAuslesen(std::ifstream &file, std::vector<bezWertPaar> &liste)
{
	char leseZeile[4001];
	std::string Zeile;
	std::string subZeile;
	std::string suchBegriff;
	std::size_t found, foundEnde;
	
	bezWertPaar paareContainer;

	while(!file.eof())
	{
		file.getline(leseZeile, 4001);
		Zeile = leseZeile;
		suchBegriff = "<</LL";
		foundEnde = Zeile.find(suchBegriff);
		if(foundEnde == 0)
		{
			found = Zeile.find("/LineDimension");
			if(found != std::string::npos)
			{
				suchBegriff = "/Contents(";
				found = Zeile.find(suchBegriff);
				if(found != std::string::npos)
				{
					found += suchBegriff.length() - 1;
					foundEnde = SchlussklammerFinden(Zeile, found);
					if(foundEnde != std::string::npos)
					{
						subZeile = Zeile.substr(found+1, foundEnde - found - 1);
						paareContainer = BezeichnungWertTrennen(subZeile);
						liste.push_back(paareContainer);
					}
				}
				
			}
		}
	}
}

size_t SchlussklammerFinden(std::string Zeile, size_t strt)
{
	size_t found;
	size_t start = strt;
	do
	{
		found = Zeile.find(')', start);
		if(found == std::string::npos)
			return std::string::npos;
		if(found > 0)
		{
			if(Zeile[found - 1] == '\\')
			{
				start = found + 1;
				std::cout<<start<<"\n";
			}
			else
			{
				return found;
			}
		}
	}while(start < Zeile.length());
	
	return found;
}

bezWertPaar BezeichnungWertTrennen(std::string eingabe)
{
	bezWertPaar ausgabe;
	ausgabe.bez = "ohne Bezeichnung";
	ausgabe.wert = std::numeric_limits<float>::min();
	std::size_t found, foundEnde;
	std::string trenner = "\\n";
	std::string str_wert;
	
	found = eingabe.find(trenner);
	if(found != std::string::npos)
	{
		ausgabe.bez = eingabe.substr(0, found);
		eingabe = eingabe.substr(found + trenner.length());
	}
	
	foundEnde = eingabe.find(' ');
	str_wert = eingabe.substr(0, foundEnde);
	std::replace(str_wert.begin(), str_wert.end(), ',', '.');
	ausgabe.wert = atof(str_wert.c_str()); 
	
	ausgabe.dim = eingabe.substr(foundEnde+1);
	
	return ausgabe;
}

void BildschirmAusgabe(std::vector<bezWertPaar> &liste)
{
	std::cout.fill(char(196));
	std::cout<<char(218);
	std::cout<<std::setw(34)<<char(194);
	std::cout<<std::setw(14)<<char(194);
	std::cout<<std::setw(9)<<char(191)<<"\n";
	
	std::cout.fill(' ');
	std::cout<<char(179)<<std::setw(32)<<"Bezeichnung"<<" "<<char(179);
	std::cout<<std::setw(12)<<"Menge"<<" "<<char(179);
	std::cout<<std::setw(7)<<"Dim"<<" "<<char(179)<<"\n";
	
	std::cout.fill(char(196));
	std::cout<<char(195);
	std::cout<<std::setw(34)<<char(197);
	std::cout<<std::setw(14)<<char(197);
	std::cout<<std::setw(9)<<char(180)<<"\n";
	
	std::cout.fill(' ');
	for(std::vector<bezWertPaar>::iterator it = liste.begin(); it != liste.end(); it++)
	{
		std::cout<<char(179)<<std::setw(32)<<(it)->bez<<" "<<char(179);
		std::cout<<std::setw(12)<<(it)->wert<<" "<<char(179);
		std::cout<<std::setw(7)<<(it)->dim<<" "<<char(179)<<"\n";
	}
	
	std::cout.fill(char(196));
	std::cout<<char(192);
	std::cout<<std::setw(34)<<char(193);
	std::cout<<std::setw(14)<<char(193);
	std::cout<<std::setw(9)<<char(217)<<"\n";
	
	return;
}

bool DateiAusgabe(char* DateiName, std::vector<bezWertPaar> &liste)
{
	std::ofstream ausgabe, csvAusgabe;
	ausgabe.open("./BildschirmSpiegel.txt", std::ios::out|std::ios::app|std::ios::ate);
	if(!ausgabe.good())
	{
		std::cerr<<"Bildschirmspiegel konnte nicht geoeffnet werden\n";
		return false;
	}
	csvAusgabe.open("./Ausgabe.csv", std::ios::out|std::ios::app|std::ios::ate);
	if(!csvAusgabe.good())
	{
		std::cerr<<"Ausgabe.csv konnte nicht geoeffnet werden\n";
		return false;
	}	
	csvAusgabe<<DateiName<<";\nBezeichnung;Menge;Dim;\n";
	
	ausgabe.fill(char(45));
	ausgabe<<char(43);
	ausgabe<<std::setw(34)<<char(43);
	ausgabe<<std::setw(14)<<char(43);
	ausgabe<<std::setw(9)<<char(43)<<"\n";
	
	ausgabe.fill(' ');
	ausgabe<<char(124)<<std::setw(32)<<"Bezeichnung"<<" "<<char(124);
	ausgabe<<std::setw(12)<<"Menge"<<" "<<char(124);
	ausgabe<<std::setw(7)<<"Dim"<<" "<<char(124)<<"\n";
	
	ausgabe.fill(char(45));
	ausgabe<<char(43);
	ausgabe<<std::setw(34)<<char(43);
	ausgabe<<std::setw(14)<<char(43);
	ausgabe<<std::setw(9)<<char(43)<<"\n";
	
	char buffer[50];
	std::string str_wert;
	
	ausgabe.fill(' ');
	for(std::vector<bezWertPaar>::iterator it = liste.begin(); it != liste.end(); it++)
	{
		ausgabe<<char(124)<<std::setw(32)<<(it)->bez<<" "<<char(124);
		ausgabe<<std::setw(12)<<(it)->wert<<" "<<char(124);
		ausgabe<<std::setw(7)<<(it)->dim<<" "<<char(124)<<"\n";
		
		sprintf(buffer, "%0.2f", it->wert);
		str_wert = buffer;
		std::replace(str_wert.begin(), str_wert.end(), '.', ',');
		csvAusgabe<<it->bez<<";"<<str_wert<<";"<<it->dim<<";\n";
	}
	
	ausgabe.fill(char(45));
	ausgabe<<char(43);
	ausgabe<<std::setw(34)<<char(43);
	ausgabe<<std::setw(14)<<char(43);
	ausgabe<<std::setw(9)<<char(43)<<"\n\n";
	
	ausgabe.close();
	
	csvAusgabe<<"\n";
	csvAusgabe.close();
	
	return true;
}