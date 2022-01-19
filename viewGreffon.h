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
#ifndef DEF_VIEWGREFFON
#define DEF_VIEWGREFFON

#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <string>

#include "module.h"
#include "contrainte.h"
#include "fx.h"
#include "mixer.h"

class ViewGreffon{

 public :
  
  ViewGreffon(std::string defGui);
  ~ViewGreffon();
  
   sf::RenderWindow winContraintesFx;
	sf::RectangleShape  winContraintesFxCurseur;
   bool flagContraintesFxCurseur;
   
   
   Mixer drawGreffon(Mixer& newPlug, int index,std::map<std::string, Fx> tableFx);
   void onClickContraintes(sf::Event e, int id);
   void onMoveContraintes(sf::Event e);
   void onParamContraintes(sf::Event e);
   void validContraintes(int id);
   void defautContraintes(int id);

 private :
  
   std::map<std::string, Fx> newTableFx;
   
	sf::Texture bNoSelect;
	sf::Texture bselect;
	sf::Texture bannul;
	sf::Texture bvalid;
	sf::Texture bdefautb;
	sf::Texture bglobal;
	
	sf::Font font;
	sf::RectangleShape shapePluginContraintes;
	sf::RectangleShape shapePluginBkg;
	sf::Text nomPlugin;
   sf::Text labelsPlugin;
	sf::RectangleShape minPlugin;
	sf::RectangleShape maxPlugin;
	sf::RectangleShape scalePlugin;
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
	sf::Text labelMin;
	sf::Text labelMax;
	sf::Text labelScale;
	sf::Text labelRev;
	
	struct usrParam{
		bool etat;
		sf::Sprite state;
		sf::Text min;
		sf::Text max;
		sf::Text scale;
		bool rev;
		sf::Sprite direction;
	};
	sf::Text labelPlug;
	std::vector<usrParam> tabParam;
	usrParam defParam;
	int textRand;
	int textDef;
	std::string apptxt;
	bool flagTxt;

	sf::Clock clockContraintesFxCurseur;
	
	Mixer newMixer;
	std::vector<Contrainte> listContrainte;
};
#endif
