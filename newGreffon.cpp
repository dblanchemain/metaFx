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

#include "viewContraintes.h"
#include "module.h"
#include "fx.h"
#include "newGreffon.h"

using namespace std;


NewGreffon::NewGreffon(string defIns, string defGui){
defInstall=defIns;
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
shapePluginContraintes.setPosition(sf::Vector2f(8,18));

nomPlug.setFont(font);
nomPlug.setString("Nom");
nomPlug.setFillColor(sf::Color(0,0,0));
nomPlug.setPosition(40,8);
nomPlug.setCharacterSize(14);
shapePluginBkg.setSize(sf::Vector2f(90,20));
shapePluginBkg.setFillColor(sf::Color(250,250,250));
shapePluginBkg.setPosition(30,8);

labelName.setString("Label");
labelName.setFont(font);
labelName.setCharacterSize(13);
labelName.setFillColor(sf::Color(0,0,0));
labelName.setPosition(78,45);
labelIndex.setString("Index");
labelIndex.setFont(font);
labelIndex.setCharacterSize(13);
labelIndex.setFillColor(sf::Color(0,0,0));
labelIndex.setPosition(161,45);
labelMin.setString("Min");
labelMin.setFont(font);
labelMin.setCharacterSize(13);
labelMin.setFillColor(sf::Color(0,0,0));
labelMin.setPosition(229,45);
labelMax.setString("Max");
labelMax.setFont(font);
labelMax.setCharacterSize(13);
labelMax.setFillColor(sf::Color(0,0,0));
labelMax.setPosition(293,45);

labelPlug.setFont(font);
labelPlug.setCharacterSize(13);
labelPlug.setFillColor(sf::Color(0,0,0));

labelPlugin.setSize(sf::Vector2f(90,20));
minPlugin.setSize(sf::Vector2f(60,20));
maxPlugin.setSize(sf::Vector2f(60,20));
scalePlugin.setSize(sf::Vector2f(60,20));


winContraintesFxCurseur.setSize(sf::Vector2f(2,12));
winContraintesFxCurseur.setFillColor(sf::Color(0,0,0,255));
flagContraintesFxCurseur=0;
	for(int i=0;i<7;i++){
		tabParam[i].label.setFont(font);
		tabParam[i].index.setFont(font);
		tabParam[i].min.setFont(font);
		tabParam[i].max.setFont(font);
		tabParam[i].label.setCharacterSize(13);
		tabParam[i].index.setCharacterSize(13);
		tabParam[i].min.setCharacterSize(13);
		tabParam[i].max.setCharacterSize(13);
		tabParam[i].label.setFillColor(sf::Color(0,0,0));
		tabParam[i].index.setFillColor(sf::Color(0,0,0));
		tabParam[i].min.setFillColor(sf::Color(0,0,0));
		tabParam[i].max.setFillColor(sf::Color(0,0,0));
		tabParam[i].label.setString("");
		tabParam[i].index.setString("");
		tabParam[i].min.setString("");
		tabParam[i].max.setString("");
		tabParam[i].label.setPosition(132-tabParam[i].label.getLocalBounds().width-2,80+(i*30));
   	tabParam[i].index.setPosition(201-tabParam[i].index.getLocalBounds().width-2,80+(i*30));
		tabParam[i].min.setPosition(270-tabParam[i].min.getLocalBounds().width-2,80+(i*30));
		tabParam[i].max.setPosition(339-tabParam[i].max.getLocalBounds().width-2,80+(i*30));
		
	}
}

NewGreffon::~NewGreffon(){
}

bool NewGreffon::drawGreffon(std::string hm){
	stringstream adr;
	home=hm;
	shapePluginContraintes.setSize(sf::Vector2f(374,76+(28*7)));
	float winHeight=88+(28*7)+60;
	bheight=80+(30*7)+16;
   annuler.setPosition(248,bheight);
   tAnnuler.setPosition(256,bheight+6);
   valider.setPosition(312,bheight);
   tValider.setPosition(320,bheight+6);
   
	winContraintesFx.create(sf::VideoMode(390, winHeight), "Nouveau Greffon");
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
			winContraintesFx.draw(nomPlug);
			winContraintesFx.draw(labelName);
			winContraintesFx.draw(labelIndex);
			winContraintesFx.draw(labelMin);
			winContraintesFx.draw(labelMax);

			winContraintesFx.draw(annuler);
			winContraintesFx.draw(tAnnuler);
			winContraintesFx.draw(valider);
			winContraintesFx.draw(tValider);
			
		   for(int i=0;i<7;i++){
		   	labelPlugin.setPosition(45,78+(i*30));
		   	winContraintesFx.draw(labelPlugin);
		   	minPlugin.setPosition(145,78+(i*30));
		   	winContraintesFx.draw(minPlugin);
		   	maxPlugin.setPosition(213,78+(i*30));
		   	winContraintesFx.draw(maxPlugin);
		   	scalePlugin.setPosition(281,78+(i*30));
		   	winContraintesFx.draw(scalePlugin);
		   }
		   
		   for(int i=0;i<7;i++){
		   	tabParam[i].label.setPosition(132-tabParam[i].label.getLocalBounds().width-2,80+(i*30));
	   		winContraintesFx.draw(tabParam[i].label);
		   	tabParam[i].index.setPosition(201-tabParam[i].index.getLocalBounds().width-2,80+(i*30));
	   		winContraintesFx.draw(tabParam[i].index);
	   		tabParam[i].min.setPosition(270-tabParam[i].min.getLocalBounds().width-2,80+(i*30));
	   		winContraintesFx.draw(tabParam[i].min);
	   		tabParam[i].max.setPosition(339-tabParam[i].max.getLocalBounds().width-2,80+(i*30));
				winContraintesFx.draw(tabParam[i].max);
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

	return rt;

}

void NewGreffon::onClickContraintes(sf::Event e){
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

   if(e.mouseButton.x>31 && e.mouseButton.x<120 &&e.mouseButton.y>8 && e.mouseButton.y<30){
   	textDef=0;
   	textRand=0;
   	nomPlug.setString("");
   	apptxt="";
   	flagTxt=1;
   	flagContraintesFxCurseur=1;
   	winContraintesFxCurseur.setPosition(118,10);
   }
   if(e.mouseButton.x>76 && e.mouseButton.x<342 &&e.mouseButton.y>80 && e.mouseButton.y<80+(30*7)){
   	id=(e.mouseButton.y-80)/30;
   	jd=0;
   	std::cout << "id: " << id <<"jd"<<jd<< std::endl;
   	textDef=id;
   	textRand=1;
   	tabParam[id].max.setString("");
   	apptxt="";
   	flagTxt=1;
   	flagContraintesFxCurseur=1;
   	winContraintesFxCurseur.setPosition(134,82+(id*30));
   }
   if(e.mouseButton.x>144 && e.mouseButton.x<342 &&e.mouseButton.y>80 && e.mouseButton.y<80+(30*7)){
   	id=(e.mouseButton.y-80)/30;
   	jd=((e.mouseButton.x-144)/70)+1;
   	std::cout << "id: " << id <<"jd"<<jd<< std::endl;
   	textDef=id;
   	textRand=1;
   	switch (jd){
  				case 0:
  					tabParam[id].label.setString("");
  					break;
  				case 1:
  					tabParam[id].index.setString("");
  					break;
  				case 2:
  					tabParam[id].min.setString("");
  					break;
  				case 3:
  					tabParam[id].max.setString("");
  					break;
  			}
   	apptxt="";
   	flagTxt=1;
   	flagContraintesFxCurseur=1;
   	winContraintesFxCurseur.setPosition(134+(jd*69),82+(id*30));
   }
   
}
void NewGreffon::onMoveContraintes(sf::Event e){
	std::cout << "mouse x: " << e.mouseMove.x << std::endl;
   std::cout << "mouse y: " << e.mouseMove.y << std::endl;
	
}

void NewGreffon::onParamContraintes(sf::Event e){
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
				 nomPlug.setString(apptxt);
     			 break;
     		case 1:
     			switch (jd){
     				case 0:
     					tabParam[id].label.setString(apptxt);
     					break;
     				case 1:
     					tabParam[id].index.setString(apptxt);
     					break;
     				case 2:
     					tabParam[id].min.setString(apptxt);
     					break;
     				case 3:
     					tabParam[id].max.setString(apptxt);
     					break;
     			}

     			 break;
     		default:
    			break;
	   }
      
   }
}

void NewGreffon::validContraintes(){
	std::cout << "valider "  << std::endl;
	Contrainte nc;
	string s;
	for(int i=0;i<7;i++){
		if(tabParam[i].label.getString()!="" && tabParam[i].index.getString()!="" && tabParam[i].min.getString()!="" && tabParam[i].max.getString()!=""){
			nc.etat=0;
			nc.label=tabParam[i].label.getString();
			s=tabParam[i].index.getString();
			nc.index=stoi(s);
			s=tabParam[i].min.getString();
			nc.min=stof(s);
	  		s=tabParam[i].max.getString();
			nc.max=stof(s);
			nc.scale=1;
			nc.rev=0;
			nc.path="";
			newMixer.push_back(nc);
		}
	}
	savePlugin();
	winContraintesFx.close();
}
void NewGreffon::defautContraintes(){
	for(int i=0;i<7;i++){
		tabParam[i].label.setString("");
		tabParam[i].index.setString("");
		tabParam[i].min.setString("");
		tabParam[i].max.setString("");
	} 
}
bool NewGreffon::modifContraintes(int id, string hm){
	home=hm;
	stringstream adr;
	readListExterne();
	string ref=tableLabelExterne[id].getString();
	string s;
	std::cout << "greffon : " << ref <<" size :"<<tableExterne[ref].tableParam.size()<< std::endl;

	for(int i=0;i<tableExterne[ref].tableParam.size();i++){
		tabParam[i].label.setString(tableExterne[ref].tableParam[i].label);
		s=to_string(tableExterne[ref].tableParam[i].index);
		tabParam[i].index.setString(s);
		adr.clear();
		adr.str("");
  		adr << std::fixed<< std::setprecision(2)<<tableExterne[ref].tableParam[i].min;
 		tabParam[i].min.setString(adr.str());
 		adr.clear();
		adr.str("");
		adr << std::fixed<< std::setprecision(2)<<tableExterne[ref].tableParam[i].max;
		tabParam[i].max.setString(adr.str());
	}
	adr.clear();
	adr.str("");
	bool nrt=drawGreffon(hm);
	if(nrt==0){
		for(int i=0;i<tableExterne[ref].tableParam.size();i++){
		tableExterne[ref].tableParam[i].label=tabParam[i].label.getString();
		s=tabParam[i].index.getString();
 	   tableExterne[ref].tableParam[i].index=stoi(s);
 	   s=tabParam[i].min.getString();
 	   tableExterne[ref].tableParam[i].min=stof(s);
 	   s=tabParam[i].max.getString();
 	   tableExterne[ref].tableParam[i].max=stof(s);
 	   }
 	   saveListExterne();
	}
}
bool NewGreffon::supContraintes(int id, string hm){
	home=hm;
	std::map<std::string, Fx>::iterator it;
	readListExterne();
	string ref=tableLabelExterne[id].getString();
	it=tableExterne.find(ref);
	tableExterne.erase(it);
	tableLabelExterne.erase(tableLabelExterne.begin()+id);
	saveListExterne();
}
void NewGreffon::savePlugin(){
	rt=1;
	string s=home+"/.config/metaFx";
	string nameFile="/listeExterne.txt";
	string wfile=s+nameFile;
	ofstream fichier(wfile, ios::app);
	string s2;	
	int mx=newMixer.size()-1;
   if(fichier){
   	s2=nomPlug.getString();
   	fichier<<"Plugin="<<s2<<","<<s2<<",4,1,1"<< endl;
   	fichier<<"Param=";
   	for(int i=0;i<mx;i++){
   		fichier<<newMixer[i].etat<<","<<newMixer[i].label<<","<<newMixer[i].index<<","<<newMixer[i].min<<","<<newMixer[i].max<<","<<newMixer[i].scale<<","<<newMixer[i].rev<<";";
   	}
   	fichier<<newMixer[mx].etat<<","<<newMixer[mx].label<<","<<newMixer[mx].index<<","<<newMixer[mx].min<<","<<newMixer[mx].max<<","<<newMixer[mx].scale<<","<<newMixer[mx].rev;
   	fichier<< endl;
   	
   	fichier.close();
   	rt=0;
   }else{
   	cerr << "Erreur à l'ouverture du fichier!" << endl;
   }
}
void NewGreffon::readListExterne(){
	string path=home+"/.config/metaFx";
	std::cout << "base : " << path << std::endl;
	string file=path+"/listeExterne.txt";
	Contrainte pr;
	int id=0;
	int size=0;
	string nc;
	sf::Text ns;
	ifstream fichier(file, ios::in); 
	string contenu;
	float s1;
	int s2;
	string pf[2];
	vector<string> pf2;
	vector<string> pf3;
	vector<string> pf4;
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
       if(pf[0]=="Plugin"){
      	pf2=explode(pf[1], ',');
      	std::cout << "pf1 ok "<<pf[1]<<" size=  "<<pf2.size()<< std::endl;
      	tableExterne[pf2[0]].label=pf2[0];
      	tableExterne[pf2[0]].name=pf2[1];
      	tableExterne[pf2[0]].type=stoi(pf2[2]);
      	tableExterne[pf2[0]].input=stoi(pf2[3]);
      	tableExterne[pf2[0]].output=stoi(pf2[4]);

      	ns.setFont(font);
			ns.setCharacterSize(12);
			ns.setFillColor(sf::Color(0,0,0));
    		ns.setString(pf2[0]);
         tableLabelExterne.push_back(ns);
         id++;
       }
       if(pf[0]=="Param"){
      	pf3=explode(pf[1], ';');
      	for(int m=0;m<pf3.size();m++){
      		std::cout << "size "<<pf3.size()<<"param pf3: "<<pf3[m]  << std::endl;
      	}
      	for(int m=0;m<pf3.size();m++){
      		pf4=explode(pf3[m], ',');
 	     		tableExterne[pf2[0]].tableParam.push_back(pr);
 	     		tableExterne[pf2[0]].tableParam.back().etat=stoi(pf4[0]);
 	     		tableExterne[pf2[0]].tableParam.back().label=pf4[1];
 	     		tableExterne[pf2[0]].tableParam.back().index=stoi(pf4[2]);
 	     		tableExterne[pf2[0]].tableParam.back().min=stof(pf4[3]);
 	     		tableExterne[pf2[0]].tableParam.back().max=stof(pf4[4]);
 	     		tableExterne[pf2[0]].tableParam.back().scale=stoi(pf4[5]);
 	     		tableExterne[pf2[0]].tableParam.back().rev=stoi(pf4[6]);
      	}
      	
			pf3.clear();
			pf4.clear();
		
       }		 
     	pf2.clear();
     }
     fichier.close();
	}else{  // sinon
      cerr << "Erreur à l'ouverture du fichier!" << endl;
	}
	for(int q=0;q<tableLabelExterne.size();q++){
		nc=tableLabelExterne[q].getString();
   	std::cout<< "name ouverture " << nc << std::endl;
   	std::cout<< "name  " <<tableExterne[nc].name<< std::endl;
   	std::cout<< "name  " <<tableExterne[nc].tableParam.size()<< std::endl;
   	for(int q=0;q<tableExterne[nc].tableParam.size();q++){
   	std::cout<< "name plug " <<tableExterne[nc].tableParam[q].label<< std::endl;
   	}
   }
}
const vector<string>  NewGreffon::explode(const string& s, const char& c){
	string buff{""};
	vector<string> v;
	
	for(auto n:s)
	{
		if(n != c) buff+=n; else
		if(n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if(buff != "") v.push_back(buff);
	
	return v;
}
void NewGreffon::saveListExterne(){
	string s=home+"/.config/metaFx";
	string nameFile="/listeExterne.txt";
	string wfile=s+nameFile;
	ofstream fichier(wfile, ios::out | ios::trunc);	
	string ref;
	if(fichier){
		for(int i=0;i<tableLabelExterne.size();i++){
			ref=tableLabelExterne[i].getString();
			s=tableExterne[ref].label;
			s=tableExterne[ref].name;
			fichier<<"Plugin="<<s<<","<<s<<",4,1,1"<< endl;
			fichier<<"Param=";
			int mx=tableExterne[ref].tableParam.size()-1;
			for(int j=0;j<mx;j++){
   			fichier<<tableExterne[ref].tableParam[j].etat<<","<<tableExterne[ref].tableParam[j].label<<","<<tableExterne[ref].tableParam[j].index<<","<<tableExterne[ref].tableParam[j].min<<","<<tableExterne[ref].tableParam[j].max<<","<<tableExterne[ref].tableParam[j].scale<<","<<tableExterne[ref].tableParam[j].rev<<";";
	   	}
	   	fichier<<tableExterne[ref].tableParam[mx].etat<<","<<tableExterne[ref].tableParam[mx].label<<","<<tableExterne[ref].tableParam[mx].index<<","<<tableExterne[ref].tableParam[mx].min<<","<<tableExterne[ref].tableParam[mx].max<<","<<tableExterne[ref].tableParam[mx].scale<<","<<tableExterne[ref].tableParam[mx].rev;
	   	fichier<< endl;
		}
	fichier.close();
	}else{  // sinon
      cerr << "Erreur à l'ouverture du fichier!" << endl;
	}
}
