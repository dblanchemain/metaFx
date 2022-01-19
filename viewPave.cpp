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
#include <iomanip>
#include <boost/filesystem.hpp>

#include "pave.h"
#include "viewPave.h"

using namespace std;

ViewPave::ViewPave(string defGui,std::string lang,sf::Color bkgColor, int fontSize,sf::Color fontColor,int pv){
stringstream adr;
adr << std::fixed << defGui<<"FreeSans.ttf";
font.loadFromFile(adr.str());
adr.clear();
adr.str("");

adr << std::fixed << defGui << "bNoSelect.png";
bNoSelect.loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui << "bselect.png";
bselect.loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui << "bvide.png";
bvalid.loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"guiDefPave.png";
selectMx2.loadFromFile(adr.str());
bkgWinPave.setTexture(selectMx2);
bkgWinPave.setPosition(0,0);
adr.clear();
adr.str("");

pvActif=pv;
adr.clear();
adr.str("");

annuler.setTexture(bvalid);
valider.setTexture(bvalid);
appBkgColor=bkgColor;

labelFile.setFont(font);
labelFile.setCharacterSize(fontSize);
labelFile.setFillColor(sf::Color(0,0,0));
labelFile.setPosition(8,18);
labelBkg.setFont(font);
labelBkg.setCharacterSize(fontSize);
labelBkg.setFillColor(sf::Color(0,0,0));
labelBkg.setPosition(8,75);
labelMin.setFont(font);
labelMin.setCharacterSize(fontSize);
labelMin.setFillColor(sf::Color(0,0,0));
labelMin.setPosition(8,119);
labelMax.setFont(font);
labelMax.setCharacterSize(fontSize);
labelMax.setFillColor(sf::Color(0,0,0));
labelMax.setPosition(150,119);
labelScale.setFont(font);
labelScale.setCharacterSize(fontSize);
labelScale.setFillColor(sf::Color(0,0,0));
labelScale.setPosition(150,75);
labelTrouNoir.setFont(font);
labelTrouNoir.setCharacterSize(fontSize);
labelTrouNoir.setFillColor(sf::Color(0,0,0));
labelTrouNoir.setPosition(8,154);
labelAnnuler.setFont(font);
labelAnnuler.setCharacterSize(fontSize);
labelAnnuler.setFillColor(sf::Color(0,0,0));
labelAnnuler.setPosition(328,25);
labelValider.setFont(font);
labelValider.setCharacterSize(fontSize);
labelValider.setFillColor(sf::Color(0,0,0));
labelValider.setPosition(328,25);
paramScale.setFont(font);
paramScale.setCharacterSize(fontSize);
paramScale.setFillColor(sf::Color(0,0,0));
paramGainMin.setFont(font);
paramGainMin.setCharacterSize(fontSize);
paramGainMin.setFillColor(sf::Color(0,0,0));
paramGainMax.setFont(font);
paramGainMax.setCharacterSize(fontSize);
paramGainMax.setFillColor(sf::Color(0,0,0));
paramFile.setFont(font);
paramFile.setCharacterSize(fontSize);
paramFile.setFillColor(sf::Color(0,0,0));

trouNoir.setTexture(bNoSelect);
trouNoir.setPosition(94,153);
adr << std::fixed << defGui<<"selectPave.png";
barOutilsPave.loadFromFile(adr.str());
selectPave.setTexture(barOutilsPave);
selectPave.setPosition(97,70);

adr.clear();
adr.str("");

flagBkgCurseur=0;
readFileLang(lang);

winBkgCurseur.setSize(sf::Vector2f(2,12));
winBkgCurseur.setFillColor(sf::Color(0,0,0,255));
winBkgCurseur.setPosition(262,80);
}

ViewPave::~ViewPave(){
}
void ViewPave::readFileLang(string lang){
	ifstream fichier(lang, ios::in);
	string contenu;
	string pf[2];
	int i=0;
	char * cstr;
	if(fichier){  // si l'ouverture a réussi
		while(getline(fichier, contenu)){
	     	cstr = new char [contenu.length()+1];
	     	std::strcpy (cstr, contenu.c_str());
	     	char * p = std::strtok (cstr,"=");
			i=0;
			while (p!=0){
 				pf[i]=p;
 				i++;
				p = std::strtok(NULL," ");
  			}
  			if(pf[0]=="nameWinPave"){
	  			nameSurface=pf[1];
  			}
  			if(pf[0]=="fichier"){
		 		labelFile.setString(pf[1]);
	      }
	      if(pf[0]=="gainMin"){
		 		labelMin.setString(pf[1]);
	      }
	      if(pf[0]=="gainMax"){
		 		labelMax.setString(pf[1]);
	      }
	      if(pf[0]=="pvBkg"){
		 		labelBkg.setString(pf[1]);
	      }
	      if(pf[0]=="scale"){
		 		labelScale.setString(pf[1]);
	      }
	      if(pf[0]=="trouNoir"){
		 		labelTrouNoir.setString(pf[1]);
	      }
	      if(pf[0]=="annuler"){
		 		labelAnnuler.setString(pf[1]);
	      }
	      if(pf[0]=="valider"){
		 		labelValider.setString(pf[1]);
	      }
  		}
  		fichier.close();  // on referme le fichier
   }else{  // sinon
     cerr << "Erreur à l'ouverture du fichier Theme!" << endl;
	}
}
void ViewPave::drawPave(){
	stringstream adr;
	string s;
	
	
	
	winBkg.create(sf::VideoMode(308,200), nameSurface);
	while (winBkg.isOpen()) { 
        sf::Event eventBkg;
        while (winBkg.pollEvent(eventBkg)){
	    	   switch (eventBkg.type){
	  			case sf::Event::Closed:
						winBkg.close();
						break;
				case sf::Event::TextEntered:
		     			newBkgText(eventBkg);
						break;
				case sf::Event::MouseButtonPressed:
						onClickBkg(eventBkg);
						break;
				case sf::Event::MouseMoved:
		            onMoveBkg(eventBkg);
	          	   break;
				}           
        }
      winBkg.setActive(true);
	   winBkg.pushGLStates();
      winBkg.clear(appBkgColor);
      winBkg.draw(bkgWinPave);
	   winBkg.draw(labelFile);
	   winBkg.draw(labelBkg);
	   winBkg.draw(selectPave);
	   winBkg.draw(paramFile);
	   winBkg.draw(labelMin);
		winBkg.draw(labelMax);
   	winBkg.draw(labelScale);
   	winBkg.draw(paramGainMin);
		winBkg.draw(paramGainMax);
		winBkg.draw(paramScale);
		winBkg.draw(labelTrouNoir);
		winBkg.draw(trouNoir);  
		   
			
		if(flagBkgCurseur==1){
			if(clockBkgCurseur.getElapsedTime()>sf::seconds(0.8f)){
				winBkg.draw(winBkgCurseur);
				if(clockBkgCurseur.getElapsedTime()>sf::seconds(1.6f)){
					clockBkgCurseur.restart();
				}
			}
		}
			
		winBkg.display();
		winBkg.popGLStates();
		winBkg.setActive(false);

	}
}

void ViewPave::onClickBkg(sf::Event e){
	std::cout << "mouse x: " << e.mouseButton.x << std::endl;
   std::cout << "mouse y: " << e.mouseButton.y << std::endl;
   stringstream adr;
   /*
	if(e.mouseButton.x>64 && e.mouseButton.x<214 && e.mouseButton.y>11 && e.mouseButton.y<41){
		string path=getcwd(NULL,0);
   	string s=path;
   	SF_INFO sfinfo;
		fileSelector.initSelector(s);
		string rtf2=fileSelector.selector();
		path=getcwd(NULL,0);
		tablePave[pvActif].path=path;
		tablePave[pvActif].file=rtf2;
		paramFileBkg.setString(rtf2);
		paramFileBkg.setPosition(262-paramFileBkg.getLocalBounds().width-4,18);
		std::cout << "path: " << tablePave[pvActif].path<< "file :"<<tablePave[pvActif].file<< std::endl;
		string sFileName=tablePave[pvActif].path+"/"+tablePave[pvActif].file;
	}
	*/
	
	if(e.mouseButton.x>278 && e.mouseButton.x<296 && e.mouseButton.y>32 && e.mouseButton.y<84){
   	tablePave[pvActif].file="";
   	paramFile.setString("");
   }
   if(e.mouseButton.x>100 && e.mouseButton.x<144 && e.mouseButton.y>62 && e.mouseButton.y<118){
		//winSelectBkg.create(sf::VideoMode(236,160), "Bkg");
   }
   if(e.mouseButton.x>184 && e.mouseButton.x<240 && e.mouseButton.y>166 && e.mouseButton.y<194){
   	winBkg.close();
   	pvActif=65535;
   }
   if(e.mouseButton.x>246 && e.mouseButton.x<292 && e.mouseButton.y>166 && e.mouseButton.y<194){
   	
   	string s=paramScale.getString();
   	tablePave[pvActif].scale=stof(s);
   	s=paramGainMin.getString();
   	tablePave[pvActif].gainMin=stof(s);
   	s=paramGainMax.getString();
   	tablePave[pvActif].gainMax=stof(s);
   	std::cout <<" rayon save "<<tablePave[pvActif].rayon<<" gainMin "<<tablePave[pvActif].gainMin<<" gainMax "<<tablePave[pvActif].gainMax<< endl;
		pvActif=65535;   	
   	winBkg.close();
   }
   if(e.mouseButton.x>95 && e.mouseButton.x<110 && e.mouseButton.y>156 && e.mouseButton.y<170){
	 	if(tablePave[pvActif].trouNoir==1){
	 		trouNoir.setTexture(bNoSelect);
	 		//tablePave[pvActif].trouNoir=0;
	 	}else{
	 		trouNoir.setTexture(bselect);
	 		//tablePave[pvActif].trouNoir=1;
	 	}
 	}
 	if(e.mouseButton.x>207 && e.mouseButton.x<265 && e.mouseButton.y>72 && e.mouseButton.y<94){
 		bkgChampIndex=1;
   	apptxt="";
   	winBkgCurseur.setPosition(264,76);
	 	flagBkgCurseur=1;
	 	clockBkgCurseur.restart();
 	}
 	if(e.mouseButton.x>72 && e.mouseButton.x<130 && e.mouseButton.y>116 && e.mouseButton.y<138){
 		bkgChampIndex=2;
   	apptxt="";
   	winBkgCurseur.setPosition(128,120);
	 	flagBkgCurseur=1;
	 	clockBkgCurseur.restart();
 	}
 	if(e.mouseButton.x>208 && e.mouseButton.x<266 && e.mouseButton.y>116 && e.mouseButton.y<138){
 		bkgChampIndex=3;
   	apptxt="";
   	winBkgCurseur.setPosition(264,120);
	 	flagBkgCurseur=1;
	 	clockBkgCurseur.restart();
 	}
 	if(paramScale.getString()!="" && e.mouseButton.x>268 && e.mouseButton.x<280 && e.mouseButton.y>72 && e.mouseButton.y<82){
   	string s=paramScale.getString();
   	float tk=stof(s);
   	tk=tk+0.1;
   	tablePave[pvActif].scale=tk;
   	adr << std::fixed<< std::setprecision(2)<<tk;
      paramScale.setString(adr.str());
   	paramScale.setPosition(264-paramScale.getLocalBounds().width-2,74);
   }
   if(paramScale.getString()!="" && e.mouseButton.x>268 && e.mouseButton.x<280 && e.mouseButton.y>82 && e.mouseButton.y<92){
   	string s=paramScale.getString();
   	float tk=stof(s);
   	tk=tk-0.1;
   	if(tk<0){
   		tk=0;
   	}
   	tablePave[pvActif].scale=tk;
   	adr << std::fixed<< std::setprecision(2)<<tk;
      paramScale.setString(adr.str());
   	paramScale.setPosition(264-paramScale.getLocalBounds().width-2,74);
   }
   if(paramGainMin.getString()!="" && e.mouseButton.x>130 && e.mouseButton.x<144 && e.mouseButton.y>116 && e.mouseButton.y<126){
   	string s= paramGainMin.getString();
   	float tk=stof(s);
   	tk=tk+0.01;
   	adr << std::fixed<< std::setprecision(2)<<tk;
   	tablePave[pvActif].gainMin=tk;
      paramGainMin.setString(adr.str());
   	paramGainMin.setPosition(128-paramGainMin.getLocalBounds().width-2,118);
   }
   if(paramGainMin.getString()!="" && e.mouseButton.x>130 && e.mouseButton.x<144 && e.mouseButton.y>126 && e.mouseButton.y<136){
   	string s= paramGainMin.getString();
   	float tk=stof(s);
   	tk=tk-0.01;
   	if(tk<0){
   		tk=0;
   	}
   	adr << std::fixed<< std::setprecision(2)<<tk;
   	tablePave[pvActif].gainMin=tk;
      paramGainMin.setString(adr.str());
   	paramGainMin.setPosition(128-paramGainMin.getLocalBounds().width-2,118);
   }
   if(paramGainMax.getString()!="" && e.mouseButton.x>266 && e.mouseButton.x<282 && e.mouseButton.y>116 && e.mouseButton.y<126){
   	string s= paramGainMax.getString();
   	float tk=stof(s);
   	tk=tk+0.01;
   	adr << std::fixed<< std::setprecision(2)<<tk;
   	tablePave[pvActif].gainMax=tk;
      paramGainMax.setString(adr.str());
   	paramGainMax.setPosition(264-paramGainMax.getLocalBounds().width-2,118);
   }
   if(paramGainMax.getString()!="" && e.mouseButton.x>266 && e.mouseButton.x<282 && e.mouseButton.y>126 && e.mouseButton.y<136){
   	string s= paramGainMax.getString();
   	float tk=stof(s);
   	tk=tk-0.01;
   	if(tk<0){
   		tk=0;
   	}
   	adr << std::fixed<< std::setprecision(2)<<tk;
   	tablePave[pvActif].gainMax=tk;
      paramGainMax.setString(adr.str());
   	paramGainMax.setPosition(264-paramGainMax.getLocalBounds().width-2,118);
   }
 	
}
void ViewPave::onMoveBkg(sf::Event e){
	std::cout << "mouse x: " << e.mouseMove.x << std::endl;
   std::cout << "mouse y: " << e.mouseMove.y << std::endl;
}
void ViewPave::newBkgText(sf::Event e){
   int key=e.key.code;
   std::cout << "key code: " << e.key.code << std::endl;
   stringstream adr;
   if (e.text.unicode < 128){
		if(key==8){
	      apptxt=apptxt.substr(0,apptxt.length()-1);
	 	}else{
			apptxt=apptxt+static_cast<char>(e.text.unicode);
			flagTxt=1;
		}
		switch (bkgChampIndex){
       		case 1:
       			adr << std::fixed<< std::setprecision(2)<<apptxt;
       			paramScale.setString(adr.str());
       			paramScale.setPosition(264-paramScale.getLocalBounds().width-2,74);
       			break;
       		case 2:
       			adr << std::fixed<< std::setprecision(2)<<apptxt;
       			paramGainMin.setString(adr.str());
       			paramGainMin.setPosition(128-paramGainMin.getLocalBounds().width-2,118);
       			break;
       		case 3:
       			adr << std::fixed<< std::setprecision(2)<<apptxt;
       			paramGainMax.setString(adr.str());
       			paramGainMax.setPosition(264-paramGainMax.getLocalBounds().width-2,118);
       			break;
       		default:
       			break;
	    }
	    flagTxt=0;
   }
   adr.clear();
  	adr.str(""); 
}
