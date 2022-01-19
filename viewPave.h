/* MetaFx .
 *
 * this file is part of the MetaFx application
 *
 * Copyright 2022-2023 dominique Blanchemain
 *
 *
 * MetaSurface is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.
 *
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

* You should have received a copy of the GNU General Public License along with this program;
* If not, see http://www.gnu.org/licenses.
*/
#ifndef DEF_VIEWPAVE
#define DEF_VIEWPAVE

#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <string>


#include "pave.h"

class ViewPave{

 public :
  
  ViewPave(std::string defGui,std::string lang,sf::Color bkgColor, int fontSize,sf::Color fontColor,int pv);
  ~ViewPave();
  
   void drawPave();
	void onClickBkg(sf::Event e);
	void onMoveBkg(sf::Event e);
	void newBkgText(sf::Event e);
	
	void readFileLang(std::string lang);
  
   sf::RenderWindow winBkg;



 private :
  
   
	sf::Texture bNoSelect;
	sf::Texture bselect;
	sf::Texture bvalid;
	sf::Texture barOutilsPave;
	std::string nameSurface;
	std::vector<Pave> tablePave;
	int pvActif;
	int pvSelect;
	
	bool flagBkgCurseur;
	sf::RectangleShape winBkgCurseur;
	
	sf::Font font;
	sf::Color appBkgColor;
	sf::Clock clockBkgCurseur;
	bool flagTxt;
	int bkgChampIndex;
	std::string apptxt;

	sf::Text labelAnnuler;
	sf::Text labelValider;
	sf::Texture selectMx2;
   sf::Sprite bkgWinPave;
	sf::Sprite annuler;
	sf::Sprite valider;
	sf::Sprite defaut;
	sf::Sprite trouNoir;
	sf::Sprite selectPave;
	
	sf::Text minTPlugin;
	sf::Text maxTPlugin;
	sf::Text scaleTPlugin;
	sf::Text labelFile;
	sf::Text labelMin;
	sf::Text labelMax;
	sf::Text labelScale;
	sf::Text labelBkg;
	sf::Text labelTrouNoir;
	sf::Text paramFile;
	sf::Text paramScale;
	sf::Text paramGainMin;
	sf::Text paramGainMax;

};
#endif
