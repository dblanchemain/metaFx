/* MetaSurface  .
 *
 * this file is part of the MetaSurface application
 *
 * Copyright 2020-2021 dominique Blanchemain
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
#ifndef DEF_NEWGREFFON
#define DEF_NEWGREFFON

#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <string>

#include "module.h"
#include "contrainte.h"
#include "fx.h"
#include "mixer.h"

class NewGreffon{

 public :
  
  NewGreffon(std::string defIns, std::string defGui);
  ~NewGreffon();
  
   sf::RenderWindow winContraintesFx;
	sf::RectangleShape  winContraintesFxCurseur;
   bool flagContraintesFxCurseur;
	bool rt;
   bool drawGreffon(std::string hm);
   void readListExterne();
   void onClickContraintes(sf::Event e);
   void onMoveContraintes(sf::Event e);
   void onParamContraintes(sf::Event e);
   void validContraintes();
   void defautContraintes();
   bool modifContraintes(int id, std::string hm);
   bool supContraintes(int id, std::string hm);
   void savePlugin();
   void saveListExterne();
   const std::vector<std::string>  explode(const std::string& s, const char& c);
   
   struct usrParam{
		sf::Text label;
		sf::Text index;
		sf::Text min;
		sf::Text max;
	};
	std::string defInstall;

 private :
  
   std::map<std::string, Fx> newTableFx;
   std::string home;
   std::map<std::string, Fx> tableExterne;
	std::vector<sf::Text> tableLabelExterne;
   
	sf::Texture bNoSelect;
	sf::Texture bselect;
	sf::Texture bannul;
	sf::Texture bvalid;
	sf::Texture bdefautb;
	sf::Texture bglobal;
	
	sf::Font font;
	sf::RectangleShape shapePluginContraintes;
	sf::RectangleShape shapePluginBkg;
	sf::Text nomPlug;
   sf::Text labelsPlugin;
	sf::RectangleShape minPlugin;
	sf::RectangleShape maxPlugin;
	sf::RectangleShape scalePlugin;
	sf::RectangleShape labelPlugin;
	
	float bheight;
	sf::Text tAnnuler;
	sf::Text tValider;
	sf::Text tDefault;
	sf::Sprite annuler;
	sf::Sprite valider;
	sf::Sprite defaut;
	sf::Sprite state;
	sf::Sprite rev;
	
	sf::Text minTPlugin;
	sf::Text maxTPlugin;
	sf::Text scaleTPlugin;
	sf::Text labelName;
	sf::Text labelMin;
	sf::Text labelMax;
	sf::Text labelIndex;
	sf::Text labelRev;
	
	
	sf::Text labelPlug;
	usrParam tabParam[7];
	int textRand;
	int textDef;
	std::string apptxt;
	bool flagTxt;
	int id;
	int jd;

	sf::Clock clockContraintesFxCurseur;
	
	std::vector<Contrainte>  newMixer;
	std::vector<Contrainte> listContrainte;
};
#endif
