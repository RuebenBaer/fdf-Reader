#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <cstring>
#include <cmath>
#include <vector>
#include <iomanip>

void FDFAuslesen(char* DateiName);
void UmfangAuslesen(std::ifstream &file);
void FlaecheAuslesen(std::ifstream &file);
void LaengeAuslesen(std::ifstream &file);

size_t SchlussklammerFinden(std::string Zeile, size_t start);

void BildschirmAusgabe(void);
bool DateiAusgabe(void);

typedef struct{
	std::string bez;
	float wert;
	std::string dim;
}bezWertPaar;

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

	std::string antwort;
	bool speichern;
	std::cout<<"Ergebnis(se) speichern? [j/n]: ";
	std::cin>>antwort;
	if((antwort.compare("j") == 0)||(antwort.compare("J") == 0))
	{
		std::cout<<"Ergebnis wird gespeichert\n";
		speichern = true;
	}
	else
	{
		std::cout<<"Keine Speicherung der Ergebnisse\n";
		speichern = false;
	}
	
	if(speichern)std::cout<<"jajaja\n";
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
	
	UmfangAuslesen(file);
	
	file.clear();
	file.seekg(0L, file.beg); //Zeiger an Anfang von file setzen
	
	FlaecheAuslesen(file);
	
	file.clear();
	file.seekg(0L, file.beg); //Zeiger an Anfang von file setzen

	BildschirmAusgabe();
	DateiAusgabe();
	
	file.close();

	return;
}

void UmfangAuslesen(std::ifstream &file)
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
						std::cout<<"Ausgabe Polylinienmessung:\n";
						paareContainer = BezeichnungWertTrennen(subZeile);
						std::cout<<"\t"<<paareContainer.bez<<"\t"<<paareContainer.wert<<"\t"<<paareContainer.dim<<"\n\n";
					}
				}
				
			}
		}
	}
}

void FlaecheAuslesen(std::ifstream &file)
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
						std::cout<<"Ausgabe Flaechennmessung:\n";
						paareContainer = BezeichnungWertTrennen(subZeile);
						std::cout<<"\t"<<paareContainer.bez<<"\t"<<paareContainer.wert<<"\t"<<paareContainer.dim<<"\n\n";
					}
				}
				
			}
		}
	}
}

void LaengeAuslesen(std::ifstream &file)
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
						std::cout<<"Ausgabe Laengennmessung:\n";
						paareContainer = BezeichnungWertTrennen(subZeile);
						std::cout<<"\t"<<paareContainer.bez<<"\t"<<paareContainer.wert<<"\t"<<paareContainer.dim<<"\n\n";
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
			std::cout<<"Zeile[found - 1] = "<<Zeile[found - 1]<<"\n";
			if(Zeile[found - 1] == '\\')
			{
				std::cout<<"Inkrementiere start ("<<start<<") => ";
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
			std::cout<<"eingabe.substr = "<<eingabe<<"\n";
	}
	
	foundEnde = eingabe.find(' ');
	str_wert = eingabe.substr(0, foundEnde);
		std::cout<<"str_wert = "<<str_wert<<"\n";
	std::replace(str_wert.begin(), str_wert.end(), ',', '.');
	ausgabe.wert = atof(str_wert.c_str()); 
	
	ausgabe.dim = eingabe.substr(foundEnde+1);
	
	return ausgabe;
}

void BildschirmAusgabe(void)
{
	std::cout<<std::setw(56)<<char(218)<<std::setfill(char(196))<<std::setw(20)<<char(194);
	std::cout<<std::setw(16)<<char(191)<<"\n";
	std::cout.fill(' ');
	std::cout<<std::setw(56)<<char(179)<<std::setw(19)<<"Kills       "<<char(179)<<std::setw(15)<<"Deaths    "<<char(179)<<"\n";
	std::cout.fill(char(196));
	std::cout<<char(218)<<std::setw(34)<<char(194)<<std::setw(14)<<char(194)<<std::setw(7)<<char(197);
	std::cout<<std::setw(10)<<char(194)<<std::setw(10)<<char(197);
	std::cout<<std::setw(8)<<char(194)<<std::setw(8)<<char(197);
	std::cout<<std::setw(10)<<char(194)<<std::setw(13)<<char(191)<<"\n";
	
	std::cout.fill(' ');
	
	std::cout<<char(179)<<std::setw(32)<<"Name"<<" "<<char(179);
	std::cout<<std::setw(12)<<"ID"<<" "<<char(179);
	std::cout<<std::setw(5)<<"MMR"<<" "<<char(179);
	std::cout<<std::setw(8)<<"by me "<<" "<<char(179);
	std::cout<<std::setw(8)<<"by mate"<<" "<<char(179);
	std::cout<<std::setw(6)<<"me "<<" "<<char(179);
	std::cout<<std::setw(6)<<"mate"<<" "<<char(179);
	std::cout<<std::setw(8)<<"Bounty"<<" "<<char(179);
	std::cout<<std::setw(11)<<"Extracted"<<" "<<char(179)<<"\n";
	
	std::cout.fill(char(196));
	std::cout<<char(192)<<std::setw(34)<<char(193);
	std::cout<<std::setw(14)<<char(193);
	std::cout<<std::setw(7)<<char(193);
	std::cout<<std::setw(10)<<char(193);
	std::cout<<std::setw(10)<<char(193);
	std::cout<<std::setw(8)<<char(193);
	std::cout<<std::setw(8)<<char(193);
	std::cout<<std::setw(10)<<char(193);
	std::cout<<std::setw(13)<<char(217)<<"\n\n";
	std::cout.fill(' ');
}

bool DateiAusgabe(void)
{
	std::ofstream ausgabe;
	ausgabe.open("./BildschirmSpiegel.txt", std::ios::out|std::ios::app|std::ios::ate);
	if(!ausgabe.good())
	{
		std::cerr<<"Bildschirmspiegel konnte nicht geoeffnet werden\n";
		return false;
	}
	
	ausgabe<<std::setw(56)<< char(43) <<std::setfill(char(45))<<std::setw(20)<<char(43);
	ausgabe<<std::setw(16)<<char(43)<<"\n";
	ausgabe.fill(' ');
	ausgabe<<std::setw(56)<<char(124)<<std::setw(19)<<"Kills       "<<char(124)<<std::setw(15)<<"Deaths    "<<char(124)<<"\n";
	ausgabe.fill(char(45));
	ausgabe<<char(43)<<std::setw(34)<<char(43)<<std::setw(14)<<char(43)<<std::setw(7)<<char(43);
	ausgabe<<std::setw(10)<<char(43)<<std::setw(10)<<char(43);
	ausgabe<<std::setw(8)<<char(43)<<std::setw(8)<<char(43);
	ausgabe<<std::setw(10)<<char(43)<<std::setw(13)<<char(43)<<"\n";
	
	ausgabe.fill(' ');
	
	ausgabe<<char(124)<<std::setw(32)<<"Name"<<" "<<char(124);
	ausgabe<<std::setw(12)<<"ID"<<" "<<char(124);
	ausgabe<<std::setw(5)<<"MMR"<<" "<<char(124);
	ausgabe<<std::setw(8)<<"by me "<<" "<<char(124);
	ausgabe<<std::setw(8)<<"by mate"<<" "<<char(124);
	ausgabe<<std::setw(6)<<"me "<<" "<<char(124);
	ausgabe<<std::setw(6)<<"mate"<<" "<<char(124);
	ausgabe<<std::setw(8)<<"Bounty"<<" "<<char(124);
	ausgabe<<std::setw(11)<<"Extracted"<<" "<<char(124)<<"\n";
	
	ausgabe.fill(char(45));
	ausgabe<<char(43)<<std::setw(34)<<char(43);
	ausgabe<<std::setw(14)<<char(43);
	ausgabe<<std::setw(7)<<char(43);
	ausgabe<<std::setw(10)<<char(43);
	ausgabe<<std::setw(10)<<char(43);
	ausgabe<<std::setw(8)<<char(43);
	ausgabe<<std::setw(8)<<char(43);
	ausgabe<<std::setw(10)<<char(43);
	ausgabe<<std::setw(13)<<char(43)<<"\n\n";
	ausgabe.fill(' ');
	
	ausgabe<<"Assists: "<<"\n";
	ausgabe<<"Geschaetzte neue MMR: "<<"\n\n";
	
	return true;
}