/* MetaFx  .
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
#include <SFML/Graphics.hpp>
#include <sstream>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <stdlib.h>
#include <ctgmath>
#include <string>
#include <cstring>
#include <boost/filesystem.hpp>

#include "info.h"

using namespace std;

Info::Info(){
  winInfoWidth=338;
  winInfoHeight=141;
  infoBarHeight=36;
  type=0;
}

Info::~Info(){
}

void Info::setInfo(string Theme, string gui){
   stringstream adr;
	adr << std::fixed <<gui;
   infoGui=adr.str();
   adr.clear();
   adr.str("");
   adr << std::fixed << infoGui<<"/"<<"FreeSans.ttf";
   font.loadFromFile(adr.str());
   adr.clear();
   adr.str("");
   adr << std::fixed <<Theme;
   fTheme=adr.str();
   adr.clear();
   adr.str("");
   ifstream fichier(fTheme, ios::in); 
   string contenu;
   float s1;
   int s2;
   string pf[2];
   int i=0;
   if(fichier){ 
     while(getline(fichier, contenu)){
       char * cstr = new char [contenu.length()+1];
       std::strcpy (cstr, contenu.c_str());
       char * p = std::strtok (cstr,"=");
       i=0;
       while (p!=0){
 	     pf[i]=p;
 	     i++;
	     p = std::strtok(NULL," ");
       }
       if(pf[0]=="infoTxtSize"){
         infoTxtSize=stoi(pf[1]);
       }
       if(pf[0]=="infoTxtFillColor"){
         infoTxtFillColor=sf::Color(std::stoul(pf[1],nullptr,16));
       }
       if(pf[0]=="infoBkgColor"){
         infoBkgColor=sf::Color(std::stoul(pf[1],nullptr,16));
       }
     }
     fichier.close();		
  }else{  // sinon
       cerr << "Erreur ?? l'ouverture Theme info!" << endl;
  }
}
void Info::setInfoTxtSize(int size){
	infoTxtSize=size;
}

void Info::setInfoTxtFillColor(sf::Color color){
	infoTxtFillColor=color;
}
void Info::setInfoBkgColor(sf::Color color){
	infoBkgColor=color;
}
void Info::setTxt(int i, sf::String txt){
	infoTxt[i].setString(txt);
}
void Info::setTxtPosX(int i, int k){
	int y=infoTxt[i].getPosition().y;
	infoTxt[i].setPosition(sf::Vector2f(k, y));
}
void Info::setTxtPosY(int i, int k){
	int x=infoTxt[i].getPosition().x;
	infoTxt[i].setPosition(sf::Vector2f(x, k));
}
bool Info::drawInfo(int ftype, string titreWInfo){
  
  type=ftype;
  titreWinInfo=titreWInfo;
  rtf=0;
  
  stringstream adr;
    
  sf::Texture selectF1;
  adr << std::fixed << infoGui<<"/fondInfo.png";
  selectF1.loadFromFile(adr.str());
  adr.clear();
  adr.str("");
  sf::Texture selectF2;
  adr << std::fixed << infoGui<<"/bvide.png";
  selectF2.loadFromFile(adr.str());
  sf::Sprite annuler(selectF2);
  annuler.setPosition(sf::Vector2f(190, 96));
  sf::Text annul;
  annul.setFont(font);
  annul.setString("Annuler"); 
  annul.setCharacterSize(infoTxtSize);
  annul.setFillColor(infoTxtFillColor);
  annul.setPosition(sf::Vector2f(195,101));
  sf::Text non;
  non.setFont(font);
  non.setString("Non"); 
  non.setCharacterSize(infoTxtSize);
  non.setFillColor(infoTxtFillColor);
  non.setPosition(sf::Vector2f(200,101));
  infoTxt[0].setFont(font);   
  infoTxt[0].setCharacterSize(infoTxtSize);
  infoTxt[0].setFillColor(infoTxtFillColor);
  infoTxt[1].setFont(font);   
  infoTxt[1].setCharacterSize(infoTxtSize);
  infoTxt[1].setFillColor(infoTxtFillColor);
  sf::Sprite alertFichier(selectF1);
  alertFichier.setPosition(sf::Vector2f(0,0));
  
  winInfo.create(sf::VideoMode(winInfoWidth, winInfoHeight), titreWinInfo);
  sf::RectangleShape menuBar(sf::Vector2f( winInfoWidth,infoBarHeight));
  
  
  while (winInfo.isOpen()) {                             // ??v??nements
	sf::Event event;
	while (winInfo.pollEvent(event)){                // Gestion des ??v??nements de la fen??tre principale
		switch (event.type){
			case sf::Event::Closed:            // Fermeture de la fen??tre
				onClose();
				break;
			case sf::Event::MouseButtonPressed:
				onClick(event);
				break;
			case sf::Event::MouseMoved:
				onMouseMove(event);
				break;
        				  					
			default:                            // on ne traite pas les autres types d'??v??nements
				break;
		}            
        
	}
        // Clear screen
	winInfo.setActive(true);
	winInfo.pushGLStates();
	winInfo.clear(infoBkgColor);
	winInfo.draw(alertFichier);

   if(type==0){
   	float lw=(228-infoTxt[0].getLocalBounds().width)/2;
   	infoTxt[0].setPosition(84+lw,infoTxt[0].getPosition().y);
   	lw=(228-infoTxt[1].getLocalBounds().width)/2;
   	infoTxt[1].setPosition(84+lw,infoTxt[1].getPosition().y);
   	winInfo.draw(annuler);
   	winInfo.draw(annul);
   }
   if(type==1){
   	float lw=(228-infoTxt[0].getLocalBounds().width)/2;
   	infoTxt[0].setPosition(84+lw,infoTxt[0].getPosition().y);
   	winInfo.draw(annuler);
   	winInfo.draw(non);
   }
   
	winInfo.draw(infoTxt[0]);
	winInfo.draw(infoTxt[1]);
       
	winInfo.display();
	winInfo.popGLStates();
	winInfo.setActive(false);
  }
  return rtf;
}
void Info::onClose(){
  rtf=0;
  winInfo.close();
}

void Info::onMouseMove(sf::Event e){
   std::cout << "mouse x: " << e.mouseMove.x << std::endl;
   std::cout << "mouse y: " << e.mouseMove.y << std::endl;
  
}
void Info::onClick(sf::Event e){
   if (e.mouseButton.button == sf::Mouse::Left){
      std::cout << "the left button was pressed" << std::endl;
      std::cout << "mouse x: " << e.mouseButton.x << std::endl;
      std::cout << "mouse y: " << e.mouseButton.y << std::endl;
   }
   if(e.mouseButton.x>190 && e.mouseButton.x<248 && e.mouseButton.y>98 && e.mouseButton.y<126){
     rtf=0;
     winInfo.close();
   }
    if(e.mouseButton.x>266 && e.mouseButton.x<316 && e.mouseButton.y>98 && e.mouseButton.y<126){
     rtf=1;
     winInfo.close();
   }
   
}

