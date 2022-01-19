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

#include "viewContraintes.h"
#include "module.h"
#include "contrainte.h"
#include "fx.h"

using namespace std;

ViewContraintes::ViewContraintes(string defGui){
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

annuler.setTexture(bvalid);
valider.setTexture(bvalid);
defaut.setTexture(bvalid);
tAnnuler.setString("Annuler");
tAnnuler.setFont(font);
tAnnuler.setCharacterSize(13);
tAnnuler.setFillColor(sf::Color(0,0,0));
tValider.setString("Valider");
tValider.setFont(font);
tValider.setCharacterSize(13);
tValider.setFillColor(sf::Color(0,0,0));
tDefault.setString("Defaut");
tDefault.setFont(font);
tDefault.setCharacterSize(13);
tDefault.setFillColor(sf::Color(0,0,0));

shapePluginContraintes.setFillColor(sf::Color(0,0,0,0));
shapePluginContraintes.setOutlineThickness(1.f);
shapePluginContraintes.setOutlineColor(sf::Color(180,180,180));
shapePluginContraintes.setPosition(sf::Vector2f(8,12));

nomPlugin.setFont(font);


nomPlugin.setPosition(24,2);
nomPlugin.setFont(font);
nomPlugin.setCharacterSize(13);
nomPlugin.setFillColor(sf::Color(0,0,0));
shapePluginBkg.setSize(sf::Vector2f(nomPlugin.getLocalBounds().width,nomPlugin.getLocalBounds().height));
shapePluginBkg.setFillColor(sf::Color(224,224,224));
shapePluginBkg.setPosition(24,2);

labelMin.setString("Min");
labelMin.setFont(font);
labelMin.setCharacterSize(13);
labelMin.setFillColor(sf::Color(0,0,0));
labelMin.setPosition(156,25);
labelMax.setString("Max");
labelMax.setFont(font);
labelMax.setCharacterSize(13);
labelMax.setFillColor(sf::Color(0,0,0));
labelMax.setPosition(224,25);
labelScale.setString("Ratio");
labelScale.setFont(font);
labelScale.setCharacterSize(13);
labelScale.setFillColor(sf::Color(0,0,0));
labelScale.setPosition(278,25);
labelRev.setString("Inv.");
labelRev.setFont(font);
labelRev.setCharacterSize(13);
labelRev.setFillColor(sf::Color(0,0,0));
labelRev.setPosition(328,25);

labelPlug.setFont(font);
labelPlug.setCharacterSize(13);
labelPlug.setFillColor(sf::Color(0,0,0));

minPlugin.setSize(sf::Vector2f(60,20));
maxPlugin.setSize(sf::Vector2f(60,20));
scalePlugin.setSize(sf::Vector2f(40,20));


winContraintesFxCurseur.setSize(sf::Vector2f(2,12));
winContraintesFxCurseur.setFillColor(sf::Color(0,0,0,255));
flagContraintesFxCurseur=0;
}

ViewContraintes::~ViewContraintes(){
}

Module ViewContraintes::drawContraintesFx(Module& newPlug, std::map<std::string, Fx> tableFx){
	stringstream adr;
	newModule=newPlug;
	newTableFx=tableFx;
	//newPlug.tableContraintes.size();
	string s;
	
	std::cout << "newPlug: "<<newPlug.label<< " newPlug size: "<<newPlug.tableContraintes.size()<< std::endl;
	shapePluginContraintes.setSize(sf::Vector2f(374,54+(28*newPlug.tableContraintes.size())));
	s=" "+newPlug.label+" ";
	nomPlugin.setString(s);
	float winHeight=58+(28*newPlug.tableContraintes.size())+60;
	bheight=60+(28*newPlug.tableContraintes.size())+16;
   annuler.setPosition(248,bheight);
   tAnnuler.setPosition(256,bheight+6);
   valider.setPosition(312,bheight);
   tValider.setPosition(320,bheight+6);
   defaut.setPosition(18,bheight);
   tDefault.setPosition(26,bheight+6);
   
	for(int i=0;i<newPlug.tableContraintes.size();i++){
		defParam.etat=newPlug.tableContraintes[i].etat;
 		defParam.min.setFont(font);
 		defParam.min.setCharacterSize(13);
 		defParam.min.setFillColor(sf::Color(0,0,0));
		adr << std::fixed<< std::setprecision(2)<<newPlug.tableContraintes[i].min;
		defParam.min.setString(adr.str());
		adr.clear();
		adr.str("");
 		defParam.max.setFont(font);
 		defParam.max.setCharacterSize(13);
 		defParam.max.setFillColor(sf::Color(0,0,0));
 		adr << std::fixed<< std::setprecision(2)<<newPlug.tableContraintes[i].max;
		defParam.max.setString(adr.str());
		adr.clear();
		adr.str("");
 		defParam.scale.setFont(font);
 		defParam.scale.setCharacterSize(13);
 		defParam.scale.setFillColor(sf::Color(0,0,0));
 		adr << std::fixed<< std::setprecision(2)<<newPlug.tableContraintes[i].scale;
		defParam.scale.setString(adr.str());
		adr.clear();
		adr.str("");
		defParam.rev=newPlug.tableContraintes[i].rev;
 		tabParam.push_back(defParam);
	}   
   
   for(int i=0;i<newPlug.tableContraintes.size();i++){
   	labelPlug.setPosition(20,50+(i*30));
   	s=newPlug.tableContraintes[i].label;
   	labelPlug.setString(s);
   }
	
	winContraintesFx.create(sf::VideoMode(390, winHeight), newPlug.label);
	while (winContraintesFx.isOpen()) { 
        sf::Event eventContraintesFx;
        while (winContraintesFx.pollEvent(eventContraintesFx)){
    	     switch (eventContraintesFx.type){
		         case sf::Event::Closed:
			         winContraintesFx.close();
						break;
            	case sf::Event::MouseButtonPressed:
			      	onClickContraintes(eventContraintesFx);
                	break;
            	case sf::Event::MouseMoved:
			         onMoveContraintes(eventContraintesFx);
                	break;
               case sf::Event::TextEntered:
           			onParamContraintes(eventContraintesFx);
						break;
        				  					
		         default: 
                	break;
  	       	}            
        }
			winContraintesFx.setActive(true);
			winContraintesFx.pushGLStates();
			winContraintesFx.clear(sf::Color(224,224,224));
			
			winContraintesFx.draw(shapePluginContraintes);
			winContraintesFx.draw(shapePluginBkg);
			//winContraintesFx.draw(nomPlugin);
		
			winContraintesFx.draw(labelMin);
			winContraintesFx.draw(labelMax);
			winContraintesFx.draw(labelScale);
			winContraintesFx.draw(labelRev);
			
			winContraintesFx.draw(defaut);
			winContraintesFx.draw(tDefault);
			winContraintesFx.draw(annuler);
			winContraintesFx.draw(tAnnuler);
			winContraintesFx.draw(valider);
			winContraintesFx.draw(tValider);
			
			for(int i=0;i<newPlug.tableContraintes.size();i++){
		   	labelPlug.setPosition(20,50+(i*30));
		   	s=newPlug.tableContraintes[i].label;
		   	labelPlug.setString(s);
		   	winContraintesFx.draw(labelPlug);
		   }
		   for(int i=0;i<newPlug.tableContraintes.size();i++){
		   	minPlugin.setPosition(140,48+(i*30));
		   	winContraintesFx.draw(minPlugin);
		   	maxPlugin.setPosition(208,48+(i*30));
		   	winContraintesFx.draw(maxPlugin);
		   	scalePlugin.setPosition(276,48+(i*30));
		   	winContraintesFx.draw(scalePlugin);
		   }
		   for(int i=0;i<newPlug.tableContraintes.size();i++){
		   	state.setPosition(110,50+(i*30));
		   	if(tabParam[i].etat==0){
		   		state.setTexture(bNoSelect);
	   		}else{
	   			state.setTexture(bselect);
	   		}
	   		winContraintesFx.draw(state);
	   		tabParam[i].min.setPosition(196-tabParam[i].min.getLocalBounds().width-2,50+(i*30));
	   		winContraintesFx.draw(tabParam[i].min);
	   		tabParam[i].max.setPosition(196+68-tabParam[i].max.getLocalBounds().width-2,50+(i*30));
				winContraintesFx.draw(tabParam[i].max);
				tabParam[i].scale.setPosition(312-tabParam[i].scale.getLocalBounds().width-2,50+(i*30));
				winContraintesFx.draw(tabParam[i].scale);
	   		if(tabParam[i].rev==0){
		   		rev.setTexture(bNoSelect);
	   		}else{
	   			rev.setTexture(bselect);
	   		}
	   		rev.setPosition(330,50+(i*30));
	   		winContraintesFx.draw(rev);
		   }
		   
			
			if(flagContraintesFxCurseur==1){
				if(clockContraintesFxCurseur.getElapsedTime()>sf::seconds(0.8f)){
					winContraintesFx.draw(winContraintesFxCurseur);
					if(clockContraintesFxCurseur.getElapsedTime()>sf::seconds(1.6f)){
						clockContraintesFxCurseur.restart();
					}
				}
			}
			
			winContraintesFx.display();
			winContraintesFx.popGLStates();
			winContraintesFx.setActive(false);
			
	}
	return newModule;
}
void ViewContraintes::onClickContraintes(sf::Event e){
	std::cout << "mouse x: " << e.mouseButton.x << std::endl;
	std::cout << "mouse y: " << e.mouseButton.y << std::endl;
	if( e.mouseButton.x>defaut.getPosition().x && e.mouseButton.x<defaut.getPosition().x+57 && e.mouseButton.y>defaut.getPosition().y && e.mouseButton.y<defaut.getPosition().y+30){
		 defautContraintes();
	}
	if( e.mouseButton.x>annuler.getPosition().x && e.mouseButton.x<annuler.getPosition().x+57 && e.mouseButton.y>annuler.getPosition().y && e.mouseButton.y<annuler.getPosition().y+30){
		 winContraintesFx.close();	
	}
	if( e.mouseButton.x>valider.getPosition().x && e.mouseButton.x<valider.getPosition().x+57 && e.mouseButton.y>valider.getPosition().y && e.mouseButton.y<valider.getPosition().y+30){
		 validContraintes();
	}
	
	int id;
   if(e.mouseButton.x>112 && e.mouseButton.x<126 &&e.mouseButton.y>54 && e.mouseButton.y<54+(28*tabParam.size())){
   	id=(e.mouseButton.y-54)/28;
   	if(tabParam[id].etat==0){
   		tabParam[id].etat=1;
   	}else{
   		tabParam[id].etat=0;
   	}
   	std::cout << "id: " << id <<"tabParam[id].etat"<<tabParam[id].etat<< std::endl;
   }
   if(e.mouseButton.x>142 && e.mouseButton.x<200 &&e.mouseButton.y>50 && e.mouseButton.y<50+(30*tabParam.size())){
   	id=(e.mouseButton.y-50)/30;
   	textDef=id;
   	textRand=0;
   	tabParam[id].min.setString("");
   	apptxt="";
   	flagTxt=1;
   	flagContraintesFxCurseur=1;
   	winContraintesFxCurseur.setPosition(196,52+(id*30));
   }
   if(e.mouseButton.x>208 && e.mouseButton.x<268 &&e.mouseButton.y>50 && e.mouseButton.y<50+(30*tabParam.size())){
   	id=(e.mouseButton.y-50)/30;
   	textDef=id;
   	textRand=1;
   	tabParam[id].max.setString("");
   	apptxt="";
   	flagTxt=1;
   	flagContraintesFxCurseur=1;
   	winContraintesFxCurseur.setPosition(264,52+(id*30));
   }
   if(e.mouseButton.x>276 && e.mouseButton.x<316 &&e.mouseButton.y>50 && e.mouseButton.y<50+(30*tabParam.size())){
   	id=(e.mouseButton.y-50)/30;
   	textDef=id;
   	textRand=2;
   	tabParam[id].scale.setString("");
   	apptxt="";
   	flagTxt=1;
   	flagContraintesFxCurseur=1;
   	winContraintesFxCurseur.setPosition(312,52+(id*30));
   }
   if(e.mouseButton.x>332 && e.mouseButton.x<346 &&e.mouseButton.y>54 && e.mouseButton.y<54+(28*tabParam.size())){
   	id=(e.mouseButton.y-54)/28;
   	if(tabParam[id].rev==0){
   		tabParam[id].rev=1;
   	}else{
   		tabParam[id].rev=0;
   	}
   	std::cout << "id: " << id <<"tabParam[id].etat"<<tabParam[id].etat<< std::endl;
   }
}
void ViewContraintes::onMoveContraintes(sf::Event e){
	std::cout << "mouse x: " << e.mouseMove.x << std::endl;
   std::cout << "mouse y: " << e.mouseMove.y << std::endl;
	
}

void ViewContraintes::onParamContraintes(sf::Event e){
	int key=e.key.code;
   stringstream adr;
   if (e.text.unicode < 128){
		if(key==8){
	       	apptxt=apptxt.substr(0,apptxt.length()-1);
	       	
	 	}else{
		apptxt=apptxt+static_cast<char>(e.text.unicode);
		flagTxt=1;
      }
      switch (textRand){
    		case 0:
				 tabParam[textDef].min.setString(apptxt);
     			 break;
     		case 1:
     			 tabParam[textDef].max.setString(apptxt);
     			 break;
     		case 2:
     			 tabParam[textDef].scale.setString(apptxt);
     			 break;
     		default:
    			break;
	   }
      
   }
	
}

void ViewContraintes::validContraintes(){
	std::cout << "valider "  << std::endl;
	string s;
	for(int i;i<newModule.tableContraintes.size();i++){
		newModule.tableContraintes[i].etat=tabParam[i].etat;
		s=tabParam[i].min.getString();
		newModule.tableContraintes[i].min=stof(s);
		s=tabParam[i].max.getString();
		newModule.tableContraintes[i].max=stof(s);
		s=tabParam[i].scale.getString();
		newModule.tableContraintes[i].scale=stof(s);
		newModule.tableContraintes[i].rev=tabParam[i].rev;
		std::cout << "contrainte: " << i <<"etat"<<tabParam[i].etat<<"new etat"<<newModule.tableContraintes[i].etat<< std::endl;
	}
	winContraintesFx.close();
	
}
void ViewContraintes::defautContraintes(){
	stringstream adr;
	for(int q=0;q<newTableFx[newModule.label].tableParam.size();q++){
		newModule.tableContraintes[q].etat=newTableFx[newModule.label].tableParam[q].etat;
 	   newModule.tableContraintes[q].min=newTableFx[newModule.label].tableParam[q].min;
 	   newModule.tableContraintes[q].max=newTableFx[newModule.label].tableParam[q].max;
 	   newModule.tableContraintes[q].scale=newTableFx[newModule.label].tableParam[q].scale;
 	   newModule.tableContraintes[q].rev=newTableFx[newModule.label].tableParam[q].rev;
	}
	for(int i=0;i<newModule.tableContraintes.size();i++){
		tabParam[i].etat=newModule.tableContraintes[i].etat;
		adr << std::fixed<< std::setprecision(2)<< newModule.tableContraintes[i].min;
		tabParam[i].min.setString(adr.str());
		adr.clear();
		adr.str("");
 		adr << std::fixed<< std::setprecision(2)<< newModule.tableContraintes[i].max;
		tabParam[i].max.setString(adr.str());
		adr.clear();
		adr.str("");
 		adr << std::fixed<< std::setprecision(2)<< newModule.tableContraintes[i].scale;
		tabParam[i].scale.setString(adr.str());
		adr.clear();
		adr.str("");
		tabParam[i].rev=newModule.tableContraintes[i].rev;
	}  
}