/* MetaFx  .
 *
 * This is the main function for the MetaFx.
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
#include <SFML/System.hpp>
#include <libgen.h>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

#include <ctgmath>
#include <string>
#include <cstring>
#include <boost/filesystem.hpp>
#include <vector>
#include <thread>

#include <lo/lo.h>
#include <lo/lo_cpp.h>

#include "faust/audio/jack-dsp.h"
#include "faust/dsp/llvm-dsp.h"
#include "faust/dsp/interpreter-dsp.h"
#include "faust/dsp/dsp-adapter.h"
#include "faust/dsp/proxy-dsp.h"
#include "faust/dsp/poly-dsp.h"
#include "faust/gui/meta.h"
#include "faust/gui/FUI.h"
#include "faust/gui/GTKUI.h"
#include "faust/gui/MidiUI.h"
#include "faust/gui/OSCUI.h"
#include "faust/gui/OSCControler.h"
#include "faust/gui/SoundUI.h"
#include "faust/misc.h"

#include "metaFx.h"
#include "mixer.h"
#include "fx.h"
#include "module.h"
#include "graph.h"
#include "connector.h"
#include "viewContraintes.h"
#include "pave.h"
#include "viewGreffon.h"
#include "newGreffon.h"
#include "apropos.h"

using namespace std;

list<GUI*> GUI::fGuiList;
ztimedmap GUI::gTimedZoneMap;

int main(int argc, char* argv[]){


//*********************************************************************************************************************
//													Configuration de l'application
//*********************************************************************************************************************
std::stringstream adr;
adr << std::fixed << argv[1]<<"/gui/";
defGui=adr.str();
defInstall=argv[1];

home=argv[2];
pref=home+"/.config/metaFx";
adr.clear();
adr.str("");


	char * cstr = new char [pref.length()+1]; //   Lecture du fichier preference.spc
	std::strcpy (cstr, pref.c_str());
	int ncwd=chdir(cstr);
	string nameFile="preferences.spc";
	ifstream fichier(nameFile, ios::in);
	string contenu;
	string pf[2];
	int i=0;
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
  			if(pf[0]=="Lang"){
	  			prefLang=pf[1];
  			}
  			if(pf[0]=="Theme"){
	  			prefTheme=pf[1];
  			}
  			if(pf[0]=="Daw"){
	  			prefDAW=stoi(pf[1]);
  			}
  			if(pf[0]=="destination"){
	  			prefServeur=pf[1];
	  			defServeur.setString(pf[1]);
  			}
  			if(pf[0]=="portD"){
	  			prefPortD=stoi(pf[1]);
	  			defPort.setString(pf[1]);
  			}
  		}
  		fichier.close();  // on referme le fichier
   }else{  // sinon
     cerr << "Erreur à l'ouverture Préférences!" << endl;
	}
std::cout << "Lang: " << prefLang<< std::endl;
std::cout << "Theme : " << prefTheme << std::endl;
string defBaseTheme=defInstall+"/Theme/"+prefTheme+"/"+prefTheme;
readFileTheme(defBaseTheme);
defGui=defInstall+"/Theme/"+prefTheme+"/gui/";
std::cout << "gui: " << defGui<< std::endl;
adr << std::fixed << defGui<<"FreeSans.ttf";
font.loadFromFile(adr.str());
adr.clear();
adr.str("");
defBaseLang=defInstall+"/Lang/"+prefLang;
std::cout << "Lang: " << defBaseLang<< std::endl;

defServeur.setFont(font);
defServeur.setPosition(378-defServeur.getLocalBounds().width-2,93);
defPort.setFont(font);
defPort.setPosition(458-defPort.getLocalBounds().width-2,93);

newApropos.setApropos(defInstall,prefTheme,defGui,prefLang);
fileSelector.setSelectorGui(defInstall+"/Theme/"+prefTheme+"/gui");
fileSelector.setSelectorTheme(defInstall+"/Theme/"+prefTheme+"/Defaut");
fileSelector.setTitreSelector(L"Sélecteur de fichier");
fileSelector.setSelectorLang(prefLang);
//std::cout << "selector theme " <<fileSelector.getSelectorTheme()<< std::endl;

newInfo.setInfo(defBaseTheme, defGui);
//*********************************************************************************************************************
//													Configuration de l'interface
//*********************************************************************************************************************
adr << std::fixed << defGui<<"g7002.png";
groupe[0].loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"g4813.png";
groupe[1].loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"g7012.png";
groupe[2].loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"g7022.png";
groupe[3].loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"g7010.png";
groupe[4].loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"g7004.png";
groupe[5].loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"g7017.png";
groupe[6].loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"g7016.png";
groupe[7].loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"g4794.png";
groupe[8].loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"g7013.png";
groupe[9].loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"g4894c.png";
groupe[10].loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"g7009.png";
groupe[11].loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"g7006.png";
groupe[12].loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"g7019.png";
groupe[13].loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"g7018.png";
groupe[14].loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"g7014.png";
groupe[15].loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"g6928.png";
groupe[16].loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"g7003.png";
groupe[17].loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"g5074.png";
groupe[18].loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"g7011.png";
groupe[19].loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"g7007.png";
groupe[20].loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"g7023.png";
groupe[21].loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"g7021.png";
groupe[22].loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"g5074.png";
groupe[23].loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"g7005.png";
groupe[24].loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"g5100.png";
groupe[25].loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"g4906.png";
groupe[26].loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"g7001.png";
groupe[27].loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"g7000.png";
groupe[28].loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"g7020.png";
groupe[29].loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"g4971.png";
groupe[30].loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"g7024.png";
groupe[31].loadFromFile(adr.str());
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"guiDefMixer.png";
selectMx.loadFromFile(adr.str());
bkgWinMixer.setTexture(selectMx);
bkgWinMixer.setPosition(0,0);
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"guiDefPave.png";
selectMx2.loadFromFile(adr.str());
bkgWinPave.setTexture(selectMx2);
bkgWinPave.setPosition(0,0);
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"fondFx.png";
selectMx3.loadFromFile(adr.str());
bkgWinExterne.setTexture(selectMx3);
bkgWinExterne.setPosition(0,0);
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"param.png";
selectMx4.loadFromFile(adr.str());
fondPref.setTexture(selectMx4);
fondPref.setPosition(0,0);
adr.clear();
adr.str("");

adr << std::fixed << defGui<<"selectGroupe.png";
barOutilsDef1.loadFromFile(adr.str());
mixerIntNew.setTexture(barOutilsDef1);
mixerIntNew.setPosition(4,31);
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"selectPave.png";
barOutilsPave.loadFromFile(adr.str());
selectPave.setTexture(barOutilsPave);
selectPave.setPosition(226,31);
selectPave2.setTexture(barOutilsPave);
selectPave2.setPosition(sf::Vector2f(97,70));
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"fond1.png";
npv.loadFromFile(adr.str());
tableTexturePv.push_back(npv);
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"fond2.png";
npv.loadFromFile(adr.str());
tableTexturePv.push_back(npv);
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"fond3.png";
npv.loadFromFile(adr.str());
tableTexturePv.push_back(npv);
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"fond4.png";
npv.loadFromFile(adr.str());
tableTexturePv.push_back(npv);
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"fond5.png";
npv.loadFromFile(adr.str());
tableTexturePv.push_back(npv);
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"fond6.png";
npv.loadFromFile(adr.str());
tableTexturePv.push_back(npv);
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"fond7.png";
npv.loadFromFile(adr.str());
tableTexturePv.push_back(npv);
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"fond8.png";
npv.loadFromFile(adr.str());
tableTexturePv.push_back(npv);
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"fond9.png";
npv.loadFromFile(adr.str());
tableTexturePv.push_back(npv);
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"fond10.png";
npv.loadFromFile(adr.str());
tableTexturePv.push_back(npv);
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"fond11.png";
npv.loadFromFile(adr.str());
tableTexturePv.push_back(npv);
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"fond12.png";
npv.loadFromFile(adr.str());
tableTexturePv.push_back(npv);
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"fond15.png";
npv.loadFromFile(adr.str());
tableTexturePv.push_back(npv);
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"fond13.png";
npv.loadFromFile(adr.str());
tableTexturePv.push_back(npv);
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"fond16.png";
npv.loadFromFile(adr.str());
tableTexturePv.push_back(npv);
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"fond14.png";
npv.loadFromFile(adr.str());
tableTexturePv.push_back(npv);
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"fond17.png";
npv.loadFromFile(adr.str());
tableTexturePv.push_back(npv);
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"fond18.png";
npv.loadFromFile(adr.str());
tableTexturePv.push_back(npv);
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"fond19.png";
npv.loadFromFile(adr.str());
tableTexturePv.push_back(npv);
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"fond20.png";
npv.loadFromFile(adr.str());
tableTexturePv.push_back(npv);
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"reInit.png";
nTextureInit.loadFromFile(adr.str());
reGraph.setTexture(nTextureInit);
reGraph.setPosition(150,3);
adr.clear();
adr.str("");

adr << std::fixed << defGui<<"fondColor.png";
npv.loadFromFile(adr.str());
selectBkg.setTexture(npv);
selectBkg.setPosition(0,0);

adr.clear();
adr.str("");
adr << std::fixed << defGui<<"trash.png";
barOutilsDef2.loadFromFile(adr.str());
barTrash1.setTexture(barOutilsDef2);
barTrash1.setPosition(60,31);
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"trash2.png";
barOutilsDef3.loadFromFile(adr.str());
barTrash2.setTexture(barOutilsDef3);
barTrash2.setPosition(82,31);
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"trash3.png";
barOutilsDef4.loadFromFile(adr.str());
barTrash3.setTexture(barOutilsDef4);
barTrash3.setPosition(106,31);
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"mv1.png";
barOutilsMv1.loadFromFile(adr.str());
barMv1.setTexture(barOutilsMv1);
barMv1.setPosition(138,31);
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"mv3.png";
barOutilsMv2.loadFromFile(adr.str());
barMv2.setTexture(barOutilsMv2);
barMv2.setPosition(166,31);
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"mvx.png";
barOutilsMv3.loadFromFile(adr.str());
barMv3.setTexture(barOutilsMv3);
barMv3.setPosition(194,31);
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"read.png";
actRun.loadFromFile(adr.str());
appRun.setTexture(actRun);
appRun.setPosition(290,31);
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"stop.png";
actStop.loadFromFile(adr.str());
adr.clear();
adr.str("");

adr << std::fixed << defGui<<"objetColor.png";												// Image de la sphère de rotation
if (!selectC.loadFromFile(adr.str()))
   return EXIT_FAILURE;
adr.clear();
adr.str("");

adr << std::fixed << defGui<<"bNoSelect.png";
bNoSelect.loadFromFile(adr.str());
fxSelect.setTexture(bNoSelect);
fxSelect.setPosition(252,203);
defSelectDaw.setTexture(bNoSelect);
defSelectDaw.setPosition(104,66);
adr.clear();
adr.str("");
adr << std::fixed << defGui<<"bselect.png";
bSelect.loadFromFile(adr.str());
adr.clear();
adr.str("");
defSelectGain.setTexture(bSelect);
defSelectGain.setPosition(104,132);  
defSpeed.setTexture(bNoSelect);
defSpeed.setPosition(252,66);
defTrouNoir.setTexture(bNoSelect);
defTrouNoir.setPosition(94,153);
defRevGain.setTexture(bNoSelect);
defRevGain.setPosition(76,240);
defRelatif.setTexture(bSelect);
defRelatif.setPosition(196,240);
defRevSpeed.setTexture(bNoSelect);
defRevSpeed.setPosition(252,168);

selectGrp.setTexture(selectC);
selectGrp.setPosition(sf::Vector2f(0,0));

adr << std::fixed << defGui<<"winCloser.png";
ldCloser.loadFromFile(adr.str());
winCloser.setTexture(ldCloser);
adr.clear();
adr.str("");



labelFichierSon.setFont(font);
labelFichierSon.setPosition(8,18);
labelDaw.setFont(font);
labelDaw.setPosition(8,65);
labelSpeedMax.setFont(font);
labelSpeedMax.setPosition(162,135);
labelMixerGain.setFont(font);
labelMixerGain.setPosition(8,135);
labelGainMin.setFont(font);
labelGainMin.setPosition(8,170);
labelGainMin2.setFont(font);
labelGainMin2.setPosition(8,119);
labelGainMax.setFont(font);

labelGainMax.setPosition(8,205);
labelGainMax2.setFont(font);
labelGainMax2.setPosition(150,119);
labelSpeed.setFont(font);
labelSpeed.setPosition(162,65);
labelSpeedMin.setFont(font);
labelSpeedMin.setPosition(162,100);
labelRevSpeed.setFont(font);
labelRevSpeed.setPosition(162,170);
labelPiste.setFont(font);
labelPiste.setPosition(8,100);
labelFx.setFont(font);
labelFx.setPosition(162,205);
labelGroupe.setFont(font);
labelGroupe.setPosition(8,277);
labelRevGain.setFont(font);
labelRevGain.setPosition(8,242);
labelRelatif.setFont(font);
labelRelatif.setPosition(120,242);
labelToolTip.setFont(font);
labelToolTip.setPosition(54,142);

paramTrack.setFont(font);
paramGainMin.setFont(font);
paramGainMax.setFont(font);
paramFile.setFont(font);
paramSpeedMin.setFont(font);
paramSpeedMax.setFont(font);
labelRayon.setFont(font);
labelRayon.setPosition(150,75);
labelTrouNoir.setFont(font);
labelTrouNoir.setPosition(8,154);


labelPlugin.setFont(font);
labelPlugin.setCharacterSize(12);
labelPlugin.setFillColor(sf::Color(0,0,0));

paramGainMin2.setFont(font);
paramGainMax2.setFont(font);
paramRayon.setFont(font);

labelImg.setFont(font);
labelImg.setPosition(8,75);


paramFileBkg.setFont(font);
paramFileBkg.setCharacterSize(12);
paramFileBkg.setFillColor(sf::Color(0,0,0,255));

toolTip.setFont(font);
toolTip.setCharacterSize(12);
toolTip.setFillColor(sf::Color(0,0,0,255));

winPaveCurseur.setSize(sf::Vector2f(2,12));
winPaveCurseur.setFillColor(sf::Color(0,0,0,255));
winPaveCurseur.setPosition(132,98);
winBkgCurseur.setSize(sf::Vector2f(2,12));
winBkgCurseur.setFillColor(sf::Color(0,0,0,255));
winBkgCurseur.setPosition(262,80);


menu1.setSize(sf::Vector2f(122,130));
menu1.setFillColor(menu1BkgColor);
menu1.setPosition(sf::Vector2f(0, 26));
menu2.setSize(sf::Vector2f(112,144));
menu2.setFillColor(menu1BkgColor);
menu2.setPosition(sf::Vector2f(90, 26));
for(int i=0;i<4;i++){
listMenu2[i].setFont(font);
listMenu2[i].setFillColor(sf::Color(0,0,0));
listMenu2[i].setCharacterSize(12);
}
menuSelectBox.setSize(sf::Vector2f(110,20));
menuSelectBox.setFillColor(sf::Color(120,120,120));
menuSelectBox.setPosition(sf::Vector2f(4,34));

menu1Flag=0;
menuFichier.setFont(font);
menuAPropos.setFont(font);
menuAide.setFont(font);
for(int i=0;i<6;i++){
menuLab[i].setFont(font);
}


selectFxName.setFont(font);
selectFxName.setCharacterSize(12);
selectFxName.setPosition(132,98);

//*************************************************************************************************** 
//													configuration view graph
//********************************************************************************************************************* 
menuFx1.setSize(sf::Vector2f(112,216));
menuFx1.setFillColor(sf::Color(203,213,217));
menuFx1.setPosition(sf::Vector2f(0, 28));
menuFx1.setOutlineThickness(1);
menuFx1.setOutlineColor(sf::Color(0,0,0));
menuFx2.setSize(sf::Vector2f(112,91));
menuFx2.setFillColor(sf::Color(203,213,217));
menuFx2.setPosition(sf::Vector2f(90, 28));
menuFx2.setOutlineThickness(1);
menuFx2.setOutlineColor(sf::Color(0,0,0));
menuFx1Flag=0;
menuFx2Flag=0;
menuLabelFx.setFont(font);
menuLabelFx.setCharacterSize(12);
menuLabelFx.setFillColor(sf::Color(0,0,0));
menuLabelFx.setPosition(sf::Vector2f(10,6));
menuLabelFx.setString("          ");
menuLabel2Fx.setFont(font);
menuLabel2Fx.setCharacterSize(12);
menuLabel2Fx.setFillColor(sf::Color(0,0,0));
menuLabel2Fx.setPosition(sf::Vector2f(90,6));
menuLabel2Fx.setString("          ");
menuFxBkgSlider.setSize(sf::Vector2f(18,214));
menuFxBkgSlider.setPosition(sf::Vector2f(92,28));
menuFxBkgSlider.setFillColor(sf::Color(203,213,217));
menuFxSlider.setPosition(sf::Vector2f(96,28));
menuFxSlider.setFillColor(sf::Color(255,255,255));
menuFxSlider.setOutlineThickness(1);
menuFxSlider.setOutlineColor(sf::Color(0,0,0));
menuFxBkgSliderLineLeft.setSize(sf::Vector2f(2,214));
menuFxBkgSliderLineLeft.setPosition(sf::Vector2f(94,28));
menuFxBkgSliderLineLeft.setFillColor(sf::Color(70,70,70));
menuFxBkgSliderLineBottom.setSize(sf::Vector2f(18,2));
menuFxBkgSliderLineBottom.setPosition(sf::Vector2f(94,244));
menuFxBkgSliderLineBottom.setFillColor(sf::Color(70,70,70));
menuFxSelectBox.setSize(sf::Vector2f(94,14));
menuFxSelectBox.setFillColor(sf::Color(120,120,120));
menuFxSelectBox.setPosition(sf::Vector2f(1,30));
menuFx2SelectBox.setSize(sf::Vector2f(109,14));
menuFx2SelectBox.setFillColor(sf::Color(120,120,120));
menuFx2SelectBox.setPosition(sf::Vector2f(91,30));
menuFx2SelectSep.setSize(sf::Vector2f(105,2));
menuFx2SelectSep.setFillColor(sf::Color(120,120,120));
menuFx2SelectSep.setPosition(sf::Vector2f(91,60));

extBkgSlider.setSize(sf::Vector2f(18,318));
extBkgSlider.setPosition(sf::Vector2f(160,1));
extBkgSlider.setFillColor(sf::Color(203,213,217));
extSlider.setPosition(sf::Vector2f(161,2));
extSlider.setFillColor(sf::Color(255,255,255));
extSlider.setOutlineThickness(1);
extSlider.setOutlineColor(sf::Color(0,0,0));
extBkgSliderLineLeft.setSize(sf::Vector2f(2,318));
extBkgSliderLineLeft.setPosition(sf::Vector2f(159,1));
extBkgSliderLineLeft.setFillColor(sf::Color(70,70,70));
extBkgSliderLineBottom.setSize(sf::Vector2f(20,2));
extBkgSliderLineBottom.setPosition(sf::Vector2f(160,318));
extBkgSliderLineBottom.setFillColor(sf::Color(70,70,70));
extSelectBox.setSize(sf::Vector2f(156,14));
extSelectBox.setFillColor(sf::Color(70,70,70));
extSelectBox.setPosition(sf::Vector2f(1,1));
baseFxIndex=0;
menuFxSliderFlag=0;
fxIndex=0;
circleDaw.setFillColor(sf::Color(0,0,0));
circleDaw.setRadius(4);
if(prefDAW==0){
	circleDaw.setPosition(sf::Vector2f(56,60));
}else{
  	circleDaw.setPosition(sf::Vector2f(56,88));
}
//*************************************************************************************************** 
//												init
//********************************************************************************************************************* 
baseIdCompte=1;
defautGroupe=0;
typeMove=0;
flagPaveCurseur=0;
flagBkgCurseur=0;
flagTxt=0;
baseOSC="5510";
appFlag=0;
for(int i=0; i<64;i++){
vertices[i].setPrimitiveType(sf::LinesStrip);
vertices[i].resize(0);
}
vertices1.setPrimitiveType(sf::LinesStrip);
vertices1.resize(0);
cubicFlag=0;


defGroupe.setTexture(groupe[defautGroupe]);  
defGroupe.setPosition(100,273);

readFileLang(defBaseLang);
readListModules();
readListExterne();

for(int i=0;i<64;i++){
	moduleActif[i]=65535;
}

winPrincipale.create(sf::VideoMode(810,846), nameSurface);
sf::FloatRect visibleArea(0, 0, 810,846);
winPrincipale.setView(sf::View(visibleArea));
createMixer();
std::cout << "baseIdCompte : " << baseIdCompte<< std::endl;
std::cout << "baseposX: " << tableMixer[tableMixer.size()-1].getPosX()<< std::endl;

mixerActif=65535;
mixerSelect=65535;
pvActif=65535;
pvSelect=65535;
rtIndex=0;
sessionPath="";
sessionFile="";
flagToolTip=0;
flagToolTip2=0;
//*************************************************************************************************** 
//													gestion des événements
//********************************************************************************************************************* 
 while (winPrincipale.isOpen()) {
   sf::Event eventPrincipal;
   sf::Event eventSelectGrp;
	sf::Event eventWinPave;
	sf::Event eventBkg;
	sf::Event eventSelectBkg;
	sf::Event eventExterne;
	sf::Event eventListExterne;
	sf::Event eventPref;
	
	while (winPrincipale.pollEvent(eventPrincipal)){        
 			switch (eventPrincipal.type){
  			case sf::Event::Closed:
				onWineClose();
				break;
		case sf::Event::Resized:
				onEventResized(eventPrincipal);
				break;
  		case sf::Event::KeyPressed:
  					onKeyPressed(eventPrincipal);
				break;
		case sf::Event::MouseButtonPressed:
				onClick(eventPrincipal);
				break;
		case sf::Event::MouseWheelScrolled:
				mouseWheel(eventPrincipal);
				break;
		case sf::Event::MouseButtonReleased:
				onMouseUp(eventPrincipal);
				break;
		case sf::Event::MouseMoved:
				onMouseMove(eventPrincipal);
				break;
		case sf::Event::TextEntered:
     			onTextEvent(eventPrincipal);
				break;
	  	default:
				break;
  		  	} 
  		  	
  		         
	}
	while (winSelectGrp.pollEvent(eventSelectGrp)){        
 			switch (eventSelectGrp.type){
  			case sf::Event::Closed:
					winSelectGrp.close();
					break;
			case sf::Event::MouseButtonPressed:
					onClickSelectGrp(eventSelectGrp);
					break;
			}     
	}
	while (winSelectBkg.pollEvent(eventSelectBkg)){        
 			switch (eventSelectBkg.type){
  			case sf::Event::Closed:
					winSelectBkg.close();
					break;
			case sf::Event::MouseButtonPressed:
					onClickSelectBkg(eventSelectBkg);
					break;
			}     
	}
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
	while (winExterne.pollEvent(eventExterne)){        
 			switch (eventExterne.type){
  			case sf::Event::Closed:
					winExterne.close();
					break;
			case sf::Event::MouseButtonPressed:
					onWinExterneClick(eventExterne);
					break;
			case sf::Event::MouseMoved:
	            onWinExterneMove(eventExterne);
          	   break;
			}     
	}
	while (winListExterne.pollEvent(eventListExterne)){        
 			switch (eventListExterne.type){
  			case sf::Event::Closed:
					winListExterne.close();
					break;
			case sf::Event::MouseButtonPressed:
					onListExterneClick(eventListExterne);
					break;
			case sf::Event::MouseMoved:
	            onListExterneMove(eventListExterne);
          	   break;
			}     
	}
	while (winPref.pollEvent(eventPref)){        
 			switch (eventPref.type){
  			case sf::Event::Closed:
					winPref.close();
					break;
			case sf::Event::TextEntered:
	     			onTextPref(eventPref);
					break;
			case sf::Event::MouseButtonPressed:
					onPrefClick(eventPref);
					break;
			case sf::Event::MouseMoved:
	            onPrefMove(eventPref);
          	   break;
			}     
	}
	while (winPave.pollEvent(eventWinPave)){ 
    	      switch (eventWinPave.type){
		        case sf::Event::Closed:
			       	winPave.close();
			       	pvActif=65535;
			     		break;
	       	  case sf::Event::TextEntered:
	     			   newPaveText(eventWinPave);
					   break;
	        	  case sf::Event::MouseButtonPressed:
		  			   onClickPave(eventWinPave);
	          	   break;
	        	  case sf::Event::MouseButtonReleased:
	     			   //onMouseUpWinPave(eventWinPave);
	          	   break;
	           case sf::Event::MouseMoved:
		            onPaveMove(eventWinPave);
	          	   break;
        				  					
		        default: 
                	break;
  	      	 }            
   }
   
//*********************************************************************************************************************   
//													Contenu de la fenêtre principale
//*********************************************************************************************************************  
  
  	winPrincipale.setActive(true);
	winPrincipale.pushGLStates();
	winPrincipale.clear(AppBackGround);

	
   winPrincipale.draw(menuBar);
   winPrincipale.draw(barOutils);

	winPrincipale.draw(mixerIntNew);
	winPrincipale.draw(selectPave);
	winPrincipale.draw(barTrash1);
	winPrincipale.draw(barTrash2);
	winPrincipale.draw(barTrash3);
	winPrincipale.draw(barMv1);
	winPrincipale.draw(barMv2);
	winPrincipale.draw(barMv3);
	winPrincipale.draw(appRun);
   
   winPrincipale.draw(menuFichier);
   if(menu1Flag==1){
   	winPrincipale.draw(menu1);
   	winPrincipale.draw(menuSelectBox);
   	for(int i=0;i<6;i++){
   		winPrincipale.draw(menuLab[i]);
   	}
   }
   winPrincipale.draw(menuAPropos);
   winPrincipale.draw(menuAide);
   
	for(int i=0;i<tableMixer.size();i++){
		mixerSprite.setTexture(groupe[tableMixer[i].getGroupe()]);
		mixerSprite.setPosition(sf::Vector2f(tableMixer[i].getPosX(),tableMixer[i].getPosY()));
		winPrincipale.draw(mixerSprite);
	}   
	for(int i=0;i<tablePave.size();i++){
		paveSprite.setTexture(tableTexturePv[tablePave[i].img]);
		paveSprite.setPosition(sf::Vector2f(tablePave[i].pavX,tablePave[i].pavY));
		paveSprite.setScale(sf::Vector2f(tablePave[i].scale,tablePave[i].scale));
		// std::cout << "tablePave[i].pavX: " << tablePave[i].pavX<< std::endl;
		winPrincipale.draw(paveSprite);
		/*
		sf::CircleShape shape(tablePave[i].rayon*tablePave[i].scale);
		shape.setPosition(sf::Vector2f(tablePave[i].pavX,tablePave[i].pavY));
		shape.setFillColor(sf::Color(0,0,0,0));
		shape.setOutlineThickness(2);
		shape.setOutlineColor(sf::Color(250, 0, 0));
		winPrincipale.draw(shape);
		*/
	} 
	if(flagToolTip==1){
		winPrincipale.draw(winToolTip);
		winPrincipale.draw(toolTip);
	}
   
	if(winSelectGrp.isOpen()){
		winSelectGrp.setActive(true);
    	winSelectGrp.pushGLStates();
    	winSelectGrp.clear(sf::Color(203,213,217));

    	winSelectGrp.draw(selectGrp);

    	winSelectGrp.display();
    	winSelectGrp.popGLStates();
    	winSelectGrp.setActive(false);

	} 
	if(winSelectBkg.isOpen()){
		winSelectBkg.setActive(true);
    	winSelectBkg.pushGLStates();
    	winSelectBkg.clear(sf::Color(203,213,217));

    	winSelectBkg.draw(selectBkg);

    	winSelectBkg.display();
    	winSelectBkg.popGLStates();
    	winSelectBkg.setActive(false);

	} 
	if(winPave.isOpen()) {
			winPave.setActive(true);
			winPave.pushGLStates();
	   	winPave.clear(AppBackGround);
		   	
		   winPave.draw(bkgWinMixer);
		   winPave.draw(labelFichierSon);
			winPave.draw(labelDaw);
			winPave.draw(labelSpeed);
			//winPave.draw(labelDimMixer);
			winPave.draw(labelMixerGain);
			winPave.draw(labelGainMin);
			winPave.draw(labelGainMax);
			winPave.draw(labelRevGain);
			winPave.draw(labelRelatif);
			winPave.draw(labelSpeedMin);
			winPave.draw(labelSpeedMax);
			winPave.draw(labelRevSpeed);
			winPave.draw(labelRevSpeed);
			winPave.draw(labelPiste);
			winPave.draw(labelFx);
			winPave.draw(labelGroupe);
			
			winPave.draw(fxSelect);
			winPave.draw(defSelectDaw);
			winPave.draw(defSpeed);
			winPave.draw(defSelectGain);
			winPave.draw(defRevGain);
			winPave.draw(defRelatif);
			winPave.draw(defRevSpeed);
			winPave.draw(paramTrack);
			winPave.draw(defGroupe);
			winPave.draw(paramGainMin);
			winPave.draw(paramGainMax);
			winPave.draw(paramFile);
			winPave.draw(paramSpeedMin);
			winPave.draw(paramSpeedMax);
			winPave.draw(paramTrack);
		   	
			
	   	if(flagPaveCurseur==1){
				if(clockCurseur.getElapsedTime()>sf::seconds(0.8f)){
					winPave.draw(winPaveCurseur);
					if(clockCurseur.getElapsedTime()>sf::seconds(1.6f)){
						clockCurseur.restart();
					}
				}
			}
	   	
	   	winPave.display();
			winPave.popGLStates();
			winPave.setActive(false);
	  } 
	  if(winBkg.isOpen()) {
			winBkg.setActive(true);
			winBkg.pushGLStates();
	   	winBkg.clear(AppBackGround);
		   winBkg.draw(bkgWinPave);
		   winBkg.draw(labelFichierSon);
		   winBkg.draw(labelImg);
		   winBkg.draw(selectPave2);
		   winBkg.draw(paramFileBkg);
		   winBkg.draw(labelGainMin2);
			winBkg.draw(labelGainMax2);
	   	winBkg.draw(labelRayon);
	   	winBkg.draw(paramGainMin2);
			winBkg.draw(paramGainMax2);
			winBkg.draw(paramRayon);
			winBkg.draw(labelTrouNoir);
			winBkg.draw(defTrouNoir);
			
			if(flagBkgCurseur==1){
				if(clockCurseur.getElapsedTime()>sf::seconds(0.8f)){
					winBkg.draw(winBkgCurseur);
					if(clockCurseur.getElapsedTime()>sf::seconds(1.6f)){
						clockCurseur.restart();
					}
				}
			}
	   	
	   	winBkg.display();
			winBkg.popGLStates();
			winBkg.setActive(false);
	  }
	  if(winExterne.isOpen()) {
			winExterne.setActive(true);
			winExterne.pushGLStates();
	   	winExterne.clear(AppBackGround);
		   
	   	winExterne.draw(bkgWinExterne);
	   	string s;
	   	float vpos=14;
	   	for(int i=0;i<tableMixer[idAjout].listExterne.size();i++){
				labelPlugin.setPosition(70,vpos);
				s=tableMixer[idAjout].listExterne[i].label;
				labelPlugin.setString(s);
   			winExterne.draw(labelPlugin);
	   		vpos+=33.5;
	   	}
	   	winExterne.display();
			winExterne.popGLStates();
			winExterne.setActive(false);
	  }
	  if(winListExterne.isOpen()) {
			winListExterne.setActive(true);
			winListExterne.pushGLStates();
	   	winListExterne.clear(menu1BkgColor);
		   
		   winListExterne.draw(extBkgSlider);
	   	winListExterne.draw(extSlider);
	   	winListExterne.draw(extBkgSliderLineLeft);
	   	winListExterne.draw(extBkgSliderLineBottom);
	   	winListExterne.draw(extSelectBox);
	   	
	   	vpos=-1;
	   	int j=extIndex;
	   	while(j<18 && j<tableLabelExterne.size()){
	   		tableLabelExterne[j].setPosition(sf::Vector2f(4,vpos)); 
     			winListExterne.draw(tableLabelExterne[j]); 
     			vpos+=18;
     			j++;
			}
		   	
	   	winListExterne.display();
			winListExterne.popGLStates();
			winListExterne.setActive(false);
	  }
	  if(winPref.isOpen()) {
			winPref.setActive(true);
			winPref.pushGLStates();
	   	winPref.clear(menu1BkgColor);
		   
		   winPref.draw(fondPref);
		   winPref.draw(circleDaw);
		   winPref.draw(defServeur);
		   winPref.draw(defPort);
		   if(flagBkgCurseur==1){
				if(clockCurseur.getElapsedTime()>sf::seconds(0.8f)){
					winPref.draw(winBkgCurseur);
					if(clockCurseur.getElapsedTime()>sf::seconds(1.6f)){
						clockCurseur.restart();
					}
				}
			}
	   	
	   	winPref.display();
			winPref.popGLStates();
			winPref.setActive(false);
	  }
	  
   
   winPrincipale.display();
	winPrincipale.popGLStates();
	winPrincipale.setActive(false);
   
   
   for(int i=0;i<tableMixer.size();i++){
   	if(winSecond[i].isOpen()) {
   		
			sf::Event eventSecond[i];	   		
   		while (winSecond[i].pollEvent(eventSecond[i])){        
 					switch (eventSecond[i].type){
  						case sf::Event::Closed:
						winSecond[i].close();
						break;
					case sf::Event::Resized:
						onWinSecondResized(eventSecond[i], i);
						break;
					case sf::Event::MouseButtonReleased:
						onWinSecondUp(eventSecond[i], i);
						break;
					case sf::Event::MouseButtonPressed:
						onWinSecondClick(eventSecond[i], i);
						break;
					case sf::Event::MouseMoved:
						onWinSecondMove(eventSecond[i], i);
						break;
					default:
						break;
   			}
   		}
			winSecond[i].setActive(true);
			winSecond[i].pushGLStates();
	   	winSecond[i].clear(AppBackGround);
	   	
	   	menuBar.setSize(sf::Vector2f(winSecond[i].getSize().x,menuBarHeight));
	   	winSecond[i].draw(menuBar);
	   	winSecond[i].draw(menuLabelFx);
	   	winSecond[i].draw(menuLabel2Fx);
	   	winSecond[i].draw(reGraph);
   		if(menuFx1Flag==1 && idFx==i){
		   	winSecond[i].draw(menuFx1);
		   	winSecond[i].draw(menuFxBkgSlider);
		   	winSecond[i].draw(menuFxSlider);
		   	winSecond[i].draw(menuFxBkgSliderLineLeft);
		   	winSecond[i].draw(menuFxBkgSliderLineBottom);
		   	winSecond[i].draw(menuFxSelectBox);
		   	vpos=30;
		   	int q=0;
		   	for(int j=baseFxIndex+2;j<tableLabelModule.size();j++){
		   		if(q<12){
			   		tableLabelModule[j].setPosition(sf::Vector2f(4,vpos)); 
	        			winSecond[i].draw(tableLabelModule[j]); 
	        			vpos+=18;
	        			q++;
	        		}
				}
	   	}
	   	if(menuFx2Flag==1 && idFx==i){
		   	winSecond[i].draw(menuFx2);
		   	winSecond[i].draw(menuFx2SelectBox);
		   	vpos=30;
		   	listMenu2[0].setPosition(sf::Vector2f(94,vpos));
		   	winSecond[i].draw(listMenu2[0]);
		   	menuFx2SelectSep.setPosition(sf::Vector2f(94,vpos+25));
		   	winSecond[i].draw(menuFx2SelectSep);
		   	vpos=66;
		   	for(int j=1;j<4;j++){
		   		listMenu2[j].setPosition(sf::Vector2f(94,vpos));
		   		winSecond[i].draw(listMenu2[j]);
		   		vpos+=18;
		   	}
	   	}
	   	if(flagToolTip2==1){
				winSecond[i].draw(winToolTip);
				winSecond[i].draw(toolTip);
			}
			for (int j=0; j<tableMixer[i].listModules.size();j++){
				if(tableMixer[i].listModules[j].type==0){
   			viewModule(j,i);
   			}
   		}	
   		  	
	   	if(cubicFlag==1){
  				winSecond[i].draw(vertices[i]);
   		}
		   for(int j=0;j<tableMixer[i].tableConnectors.size();j++){
		   	if(tableMixer[i].tableConnectors[j].type==0){
			   	points1.clear();
					vertices1.clear();
			   	drawConnector(tableMixer[i].tableConnectors[j],i);
			   	winSecond[i].draw(vertices1);
		   	}
		   }
		   
	   	winSecond[i].display();
			winSecond[i].popGLStates();
			winSecond[i].setActive(false);
	 	}
	 }
 }
}
//*********************************************************************************************************************   
//													Actions fichiers
//********************************************************************************************************************* 

void readFileTheme(string theme){
	int mX;
	ifstream fichier(theme, ios::in);
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
  			if(pf[0]=="AppBackGround"){
	  			AppBackGround=sf::Color(std::stoul(pf[1],nullptr,16));
  			}
  			
  			if(pf[0]=="windowAppWidth"){
		 		windowAppWidth=stoi(pf[1]);
	      }
	      if(pf[0]=="windowAppHeight"){
		 		windowAppHeight=stoi(pf[1]);
	      }
	      if(pf[0]=="backGroundMenuBar"){
	      	sf::Color c=sf::Color(std::stoul(pf[1],nullptr,16));
	  			menuBar.setFillColor(c);
	  			AppBackGroundMenuBar=c;
	  			winToolTip.setFillColor(c);
  			}
  			if(pf[0]=="menuBarHeight"){
         	menuBarHeight=stoi(pf[1]);
         	menuBar.setSize(sf::Vector2f(windowAppWidth,menuBarHeight));
         	menuBar.setPosition(sf::Vector2f(0,0));
         }
         if(pf[0]=="barOutilsBackGround"){
	      	sf::Color c=sf::Color(std::stoul(pf[1],nullptr,16));
	  			barOutils.setFillColor(c);
  			}
  			if(pf[0]=="barOutilsHeight"){
         	barOutilsHeight=stoi(pf[1]);
         	barOutils.setSize(sf::Vector2f(windowAppWidth,barOutilsHeight));
         	barOutils.setPosition(sf::Vector2f(0,menuBarHeight+1));
         }
         if(pf[0]=="menu1BkgColor"){
				menu1BkgColor=sf::Color(std::stoul(pf[1],nullptr,16));
         }
         if(pf[0]=="menuFichierFontSize"){
				menuFichier.setCharacterSize(stoi(pf[1]));
				menuAPropos.setCharacterSize(stoi(pf[1]));
				menuAide.setCharacterSize(stoi(pf[1]));
				menuLab[0].setCharacterSize(stoi(pf[1]));
				menuLab[1].setCharacterSize(stoi(pf[1]));
				menuLab[2].setCharacterSize(stoi(pf[1]));
				menuLab[3].setCharacterSize(stoi(pf[1]));
				menuLab[4].setCharacterSize(stoi(pf[1]));
				menuLab[5].setCharacterSize(stoi(pf[1]));
         }
         if(pf[0]=="menuFichierFontColor"){
				menuFichier.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				menuAPropos.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				menuAide.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				menuLab[0].setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				menuLab[1].setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				menuLab[2].setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				menuLab[3].setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				menuLab[4].setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				menuLab[5].setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
         }
         if(pf[0]=="menuFichierPositionX"){
				mX=stoi(pf[1]);
         }
         if(pf[0]=="menuFichierPositionY"){
				menuFichier.setPosition(sf::Vector2f(mX,stoi(pf[1])));
				menuLab[0].setPosition(sf::Vector2f(mX,stoi(pf[1])+30));
				menuLab[1].setPosition(sf::Vector2f(mX,stoi(pf[1])+50));
				menuLab[2].setPosition(sf::Vector2f(mX,stoi(pf[1])+70));
				menuLab[3].setPosition(sf::Vector2f(mX,stoi(pf[1])+90));
				menuLab[4].setPosition(sf::Vector2f(mX,stoi(pf[1])+110));
				menuLab[5].setPosition(sf::Vector2f(mX,stoi(pf[1])+130));
         }
         if(pf[0]=="menuAproposPositionX"){
				mX=stoi(pf[1]);
         }
         if(pf[0]=="menuAproposPositionY"){
				menuAPropos.setPosition(sf::Vector2f(mX,stoi(pf[1])));
         }
         if(pf[0]=="menuAidePositionX"){
				mX=stoi(pf[1]);
         }
         if(pf[0]=="menuAidePositionY"){
				menuAide.setPosition(sf::Vector2f(mX,stoi(pf[1])));
         }
         if(pf[0]=="mixerPanelFontSize"){
				labelFichierSon.setCharacterSize(stoi(pf[1]));
				labelDaw.setCharacterSize(stoi(pf[1]));
				labelSpeed.setCharacterSize(stoi(pf[1]));
				labelMixerGain.setCharacterSize(stoi(pf[1]));
				labelGainMin.setCharacterSize(stoi(pf[1]));
				labelGainMax.setCharacterSize(stoi(pf[1]));
				labelRevGain.setCharacterSize(stoi(pf[1]));
				labelRelatif.setCharacterSize(stoi(pf[1]));
				labelGainMin2.setCharacterSize(stoi(pf[1]));
				labelGainMax2.setCharacterSize(stoi(pf[1]));
				labelSpeedMin.setCharacterSize(stoi(pf[1]));
				labelSpeedMax.setCharacterSize(stoi(pf[1]));
				labelRevSpeed.setCharacterSize(stoi(pf[1]));
				labelPiste.setCharacterSize(stoi(pf[1]));
				labelFx.setCharacterSize(stoi(pf[1]));
				labelGroupe.setCharacterSize(stoi(pf[1]));
				paramTrack.setCharacterSize(stoi(pf[1]));
				paramGainMin.setCharacterSize(stoi(pf[1]));
				paramGainMax.setCharacterSize(stoi(pf[1]));
				paramFile.setCharacterSize(stoi(pf[1]));
				paramSpeedMin.setCharacterSize(stoi(pf[1]));
				paramSpeedMax.setCharacterSize(stoi(pf[1]));
				labelImg.setCharacterSize(stoi(pf[1]));
				labelRayon.setCharacterSize(stoi(pf[1]));
				paramGainMin2.setCharacterSize(stoi(pf[1]));
				paramGainMax2.setCharacterSize(stoi(pf[1]));
				paramRayon.setCharacterSize(stoi(pf[1]));
				labelTrouNoir.setCharacterSize(stoi(pf[1]));
				defServeur.setCharacterSize(stoi(pf[1]));
				defPort.setCharacterSize(stoi(pf[1]));
				labelToolTip.setCharacterSize(stoi(pf[1]));
				toolTip.setCharacterSize(stoi(pf[1]));
         }
         if(pf[0]=="mixerPanelFontColor"){
				labelFichierSon.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				labelDaw.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				labelSpeed.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				labelMixerGain.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				labelGainMin.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				labelGainMax.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				labelRevGain.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				labelRelatif.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				labelGainMin2.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				labelGainMax2.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				labelSpeedMin.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				labelSpeedMax.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				labelRevSpeed.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				labelPiste.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				labelFx.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				labelGroupe.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				paramTrack.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				paramGainMin.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				paramGainMax.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				paramFile.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				paramSpeedMin.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				paramSpeedMax.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				labelImg.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				labelRayon.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				paramGainMin2.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				paramGainMax2.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				paramRayon.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				labelTrouNoir.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				defServeur.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				defPort.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				toolTip.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
				labelToolTip.setFillColor(sf::Color(std::stoul(pf[1],nullptr,16)));
         }
         
  		if(pf[0]=="fileSelectorWidth"){
  			fileSelector.setSelectorWidth(stoi(pf[1]));
  			std::cout << "fileSelectorWidth: " << fileSelector.getSelectorWidth()<< std::endl;
  		}
  		if(pf[0]=="fileSelectorHeight"){
  			fileSelector.setSelectorHeight(stoi(pf[1]));
  		}
  		if(pf[0]=="fileSelectorBkgColor"){
  			fileSelector.setSelectorBkgColor(sf::Color(std::stoul(pf[1],nullptr,16)));
  		}
  		if(pf[0]=="fileSelectorBarColor"){
  			fileSelector.setSelectorBarColor(sf::Color(std::stoul(pf[1],nullptr,16)));
  		}
  		if(pf[0]=="fileSelectorPathColor"){
  			fileSelector.setSelectorPathColor(sf::Color(std::stoul(pf[1],nullptr,16)));
  		}
  		if(pf[0]=="fileSelectorFileColor"){
  			fileSelector.setSelectorFileColor(sf::Color(std::stoul(pf[1],nullptr,16)));
  		}
  		if(pf[0]=="fileSelectorLineColor"){
  			fileSelector.setSelectorLineColor(sf::Color(std::stoul(pf[1],nullptr,16)));
  		}
  		if(pf[0]=="fileSelectorSlider1Color"){
  			fileSelector.setSelectorSlider1Color(sf::Color(std::stoul(pf[1],nullptr,16)));
  		}
  		if(pf[0]=="fileSelectorSlider2Color"){
  			fileSelector.setSelectorSlider2Color(sf::Color(std::stoul(pf[1],nullptr,16)));
  		}
  		if(pf[0]=="fileSelectorSlider1thColor"){
  			fileSelector.setSelectorSlider1thColor(sf::Color(std::stoul(pf[1],nullptr,16)));
  		}
  		if(pf[0]=="fileSelectorSlider2thColor"){
  			fileSelector.setSelectorSlider2thColor(sf::Color(std::stoul(pf[1],nullptr,16)));
  		}
  		if(pf[0]=="fileSelectorFontColor"){
  			fileSelector.setSelectorFontColor(sf::Color(std::stoul(pf[1],nullptr,16)));
  		}
  		if(pf[0]=="fileSelectorFontSize"){
  			fileSelector.setSelectorFontSize(stoi(pf[1]));
  			fileSelector.setSelectFontSize(stoi(pf[1]));
  		}
	}
  	fichier.close();  // on referme le fichier
   }else{  // sinon
     cerr << "Erreur à l'ouverture du fichier Theme!" << endl;
	}

}
void readFileLang(string lang){
	ifstream fichier(lang, ios::in);
	string contenu;
	sf::String s;
	wstring ws;
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
				p = std::strtok(NULL,"=");
  			}
  			if(pf[0]=="nameSurface"){
	  			nameSurface=pf[1];
  			}
  			
  			if(pf[0]=="fichier"){
		 		menuFichier.setString(pf[1]);
	      }
	      if(pf[0]=="aPropos"){
		 		s=sf::String::fromUtf8(pf[1].begin(),pf[1].end());
				menuAPropos.setString(s);
	      }
	      if(pf[0]=="aide"){
		 		menuAide.setString(pf[1]);
	      }
	      if(pf[0]=="menu1"){
		 		menuLab[0].setString(pf[1]);
	      }
	      if(pf[0]=="menu2"){
		 		menuLab[1].setString(pf[1]);
	      }
	      if(pf[0]=="menu3"){
		 		menuLab[2].setString(pf[1]);
	      }
	      if(pf[0]=="menu4"){
		 		menuLab[3].setString(pf[1]);
	      }
	      if(pf[0]=="menu5"){
		 		s=sf::String::fromUtf8(pf[1].begin(),pf[1].end());
				menuLab[4].setString(s);
	      }
	      if(pf[0]=="menu6"){
		 		menuLab[5].setString(pf[1]);
	      }
	      if(pf[0]=="fichierSon"){
		 		labelFichierSon.setString(pf[1]);
	      }
	      if(pf[0]=="daw"){
		 		labelDaw.setString(pf[1]);
	      }
	      if(pf[0]=="speed"){
		 		labelSpeed.setString(pf[1]);
	      }
	      if(pf[0]=="mixerGain"){
		 		labelMixerGain.setString(pf[1]);
	      }
	      if(pf[0]=="gainMin"){
		 		labelGainMin.setString(pf[1]);
		 		labelGainMin2.setString(pf[1]);
	      }
	      if(pf[0]=="gainMax"){
		 		labelGainMax.setString(pf[1]);
		 		labelGainMax2.setString(pf[1]);
	      }
	      if(pf[0]=="revGain"){
		 		labelRevGain.setString(pf[1]);
	      }
	      if(pf[0]=="relatif"){
		 		labelRelatif.setString(pf[1]);
	      }
	      if(pf[0]=="speedMin"){
		 		labelSpeedMin.setString(pf[1]);
	      }
	      if(pf[0]=="speedMax"){
		 		labelSpeedMax.setString(pf[1]);
	      }
	      if(pf[0]=="revSpeed"){
		 		labelRevSpeed.setString(pf[1]);
	      }
	      if(pf[0]=="piste"){
		 		labelPiste.setString(pf[1]);
	      }
	      if(pf[0]=="fx"){
		 		labelFx.setString(pf[1]);
	      }
	      if(pf[0]=="groupe"){
		 		labelGroupe.setString(pf[1]);
	      }
	      if(pf[0]=="modulesFx"){
		 		menuLabelFx.setString(pf[1]);
	      }
	      if(pf[0]=="externe"){
		 		menuLabel2Fx.setString(pf[1]);
	      }
	      if(pf[0]=="ajouter"){
		 		listMenu2[0].setString(pf[1]);
	      }
	      if(pf[0]=="nouveau"){
		 		listMenu2[1].setString(pf[1]);
	      }
	      if(pf[0]=="modifier"){
		 		listMenu2[2].setString(pf[1]);
	      }
	      if(pf[0]=="supprimer"){
		 		listMenu2[3].setString(pf[1]);
	      }
	      if(pf[0]=="scale"){
		 		labelRayon.setString(pf[1]);
	      }
	      if(pf[0]=="pvBkg"){
		 		labelImg.setString(pf[1]);
	      }
	      if(pf[0]=="trouNoir"){
		 		labelTrouNoir.setString(pf[1]);
	      }
	      if(pf[0]=="greffon"){
		 		labelGreffon=pf[1];
	      }
	      if(pf[0]=="supExterne1"){
		 		supExterne1=pf[1];
	      }
	      if(pf[0]=="supExterne2"){
		 		supExterne2=pf[1];
	      }
	      if(pf[0]=="supNouv1"){
		 		supNouv1=pf[1];
	      }
	      if(pf[0]=="supNouv2"){
				supNouv2=pf[1];
	      }
	      if(pf[0]=="supQuit1"){
	      	s=sf::String::fromUtf8(pf[1].begin(),pf[1].end());
		 		supQuit1=s;
	      }
	      if(pf[0]=="toolTip1"){
	      	ws=sf::String::fromUtf8(pf[1].begin(),pf[1].end());
		 		sToolTip[0]=ws;
	      }
	      if(pf[0]=="toolTip2"){
	      	ws=sf::String::fromUtf8(pf[1].begin(),pf[1].end());
		 		sToolTip[1]=ws;
	      }
	      if(pf[0]=="toolTip3"){
	      	ws=sf::String::fromUtf8(pf[1].begin(),pf[1].end());
		 		sToolTip[2]=ws;
	      }
	      if(pf[0]=="toolTip4"){
	      	ws=sf::String::fromUtf8(pf[1].begin(),pf[1].end());
		 		sToolTip[3]=ws;
	      }
	      if(pf[0]=="toolTip5"){
	      	ws=sf::String::fromUtf8(pf[1].begin(),pf[1].end());
		 		sToolTip[4]=ws;
	      }
	      if(pf[0]=="toolTip6"){
	      	ws=sf::String::fromUtf8(pf[1].begin(),pf[1].end());
		 		sToolTip[5]=ws;
	      }
	      if(pf[0]=="toolTip7"){
	      	ws=sf::String::fromUtf8(pf[1].begin(),pf[1].end());
		 		sToolTip[6]=ws;
	      }
	      if(pf[0]=="toolTip8"){
	      	ws=sf::String::fromUtf8(pf[1].begin(),pf[1].end());
		 		sToolTip[7]=ws;
	      }
	      if(pf[0]=="toolTip9"){
	      	ws=sf::String::fromUtf8(pf[1].begin(),pf[1].end());
		 		sToolTip[8]=ws;
	      }
	      if(pf[0]=="toolTip10"){
	      	ws=sf::String::fromUtf8(pf[1].begin(),pf[1].end());
		 		sToolTip[9]=ws;
	      }
  		}
  		fichier.close();  // on referme le fichier
   }else{  // sinon
     cerr << "Erreur à l'ouverture du fichier Theme!" << endl;
	}
}
//************************************************************************
//   windows preferences
//************************************************************************
void onPrefClick(sf::Event e){
	if(e.mouseButton.x>52 && e.mouseButton.x<68 && e.mouseButton.y>60 && e.mouseButton.y<72){
		circleDaw.setPosition(sf::Vector2f(56,60));
		prefDAW=0;
	}
	if(e.mouseButton.x>52 && e.mouseButton.x<68 && e.mouseButton.y>86 && e.mouseButton.y<102){
		circleDaw.setPosition(sf::Vector2f(56,88));
		prefDAW=1;
	}
	if(e.mouseButton.x>296 && e.mouseButton.x<380 && e.mouseButton.y>90 && e.mouseButton.y<112){
 		bkgChampIndex=1;
   	apptxt="";
   	winBkgCurseur.setPosition(378,94);
	 	flagBkgCurseur=1;
	 	defServeur.setString("");
	 	clockCurseur.restart();
 	}
 	if(e.mouseButton.x>414 && e.mouseButton.x<462 && e.mouseButton.y>90 && e.mouseButton.y<112){
 		bkgChampIndex=2;
   	apptxt="";
   	winBkgCurseur.setPosition(460,94);
	 	flagBkgCurseur=1;
	 	defPort.setString("");
	 	clockCurseur.restart();
 	}
 	if(e.mouseButton.x>358 && e.mouseButton.x<414 && e.mouseButton.y>134 && e.mouseButton.y<164){
 		winPref.close();
 	}
 	if(e.mouseButton.x>420 && e.mouseButton.x<470 && e.mouseButton.y>134 && e.mouseButton.y<164){
 		savePref();
 		winPref.close();
 	}
}
void savePref(){
	char * cstr = new char [pref.length()+1]; //   Lecture du fichier preference.spc
	std::strcpy (cstr, pref.c_str());
	int ncwd=chdir(cstr);
	string nameFile="preferences.spc";
	ofstream fichier(nameFile, ios::out | ios::trunc);	
	if(fichier){
		fichier<<"Lang=Fr"<<std::endl;
		fichier<<"Theme=Defaut"<<std::endl;
		fichier<<"Daw="<<prefDAW<<std::endl;
		fichier<<"destination="<<prefServeur<<std::endl;
		fichier<<"portD="<<prefPortD<<std::endl;
		fichier.close();
	}else{  // sinon
      cerr << "Erreur à l'ouverture du fichier!" << endl;
	}
}

void onTextPref(sf::Event e){
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
		float tk;
   	float tk2;
   	string s2;
		switch (bkgChampIndex){
       		case 1:
       			defServeur.setString(apptxt);
       			defServeur.setPosition(378-defServeur.getLocalBounds().width-2,93);
       			break;
       		case 2:
       			prefPortD=stoi(apptxt);
	  				defPort.setString(apptxt);
       			defPort.setPosition(458-defPort.getLocalBounds().width-2,93);
       			break;
	    }
	    flagTxt=0;
   }
   adr.clear();
  	adr.str(""); 
}

void onPrefMove(sf::Event e){
	std::cout << "mouse x: " << e.mouseMove.x << std::endl;
   std::cout << "mouse y: " << e.mouseMove.y << std::endl;
}
//************************************************************************
//   windows bkg
//************************************************************************

void onClickBkg(sf::Event e){
	std::cout << "mouse x: " << e.mouseButton.x << std::endl;
   std::cout << "mouse y: " << e.mouseButton.y << std::endl;
   stringstream adr;
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
	if(e.mouseButton.x>278 && e.mouseButton.x<296 && e.mouseButton.y>8 && e.mouseButton.y<28){
	 	std::cout <<"testFileDSP()"<< endl;
	 	readFileDSP("",tablePave[pvActif].path,tablePave[pvActif].file);
	}
	if(e.mouseButton.x>278 && e.mouseButton.x<296 && e.mouseButton.y>32 && e.mouseButton.y<84){
   	tablePave[pvActif].file="";
   	paramFileBkg.setString("");
   }
   if(e.mouseButton.x>100 && e.mouseButton.x<144 && e.mouseButton.y>62 && e.mouseButton.y<118){
		winSelectBkg.create(sf::VideoMode(236,160), "Bkg");
   }
   if(e.mouseButton.x>184 && e.mouseButton.x<240 && e.mouseButton.y>166 && e.mouseButton.y<194){
   	winBkg.close();
   	pvActif=65535;
   }
   if(e.mouseButton.x>246 && e.mouseButton.x<292 && e.mouseButton.y>166 && e.mouseButton.y<194){
   	
   	string s=paramRayon.getString();
   	tablePave[pvActif].scale=stof(s);
   	s=paramGainMin2.getString();
   	tablePave[pvActif].gainMin=stof(s);
   	s=paramGainMax2.getString();
   	tablePave[pvActif].gainMax=stof(s);
   	std::cout <<" rayon save "<<tablePave[pvActif].rayon<<" gainMin "<<tablePave[pvActif].gainMin<<" gainMax "<<tablePave[pvActif].gainMax<< endl;
		pvActif=65535;   	
   	winBkg.close();
   }
   if(e.mouseButton.x>95 && e.mouseButton.x<110 && e.mouseButton.y>156 && e.mouseButton.y<170){
	 	if(tablePave[pvActif].trouNoir==1){
	 		defTrouNoir.setTexture(bNoSelect);
	 		tablePave[pvActif].trouNoir=0;
	 	}else{
	 		defTrouNoir.setTexture(bSelect);
	 		tablePave[pvActif].trouNoir=1;
	 	}
 	}
 	if(e.mouseButton.x>207 && e.mouseButton.x<265 && e.mouseButton.y>72 && e.mouseButton.y<94){
 		bkgChampIndex=1;
   	apptxt="";
   	winBkgCurseur.setPosition(264,76);
	 	flagBkgCurseur=1;
	 	paramRayon.setString("");
	 	clockCurseur.restart();
 	}
 	if(e.mouseButton.x>72 && e.mouseButton.x<130 && e.mouseButton.y>116 && e.mouseButton.y<138){
 		bkgChampIndex=2;
   	apptxt="";
   	winBkgCurseur.setPosition(128,120);
	 	flagBkgCurseur=1;
	 	paramGainMin2.setString("");
	 	clockCurseur.restart();
 	}
 	if(e.mouseButton.x>208 && e.mouseButton.x<266 && e.mouseButton.y>116 && e.mouseButton.y<138){
 		bkgChampIndex=3;
   	apptxt="";
   	winBkgCurseur.setPosition(264,120);
	 	flagBkgCurseur=1;
	 	paramGainMax2.setString("");
	 	clockCurseur.restart();
 	}
 	if(paramRayon.getString()!="" && e.mouseButton.x>268 && e.mouseButton.x<280 && e.mouseButton.y>72 && e.mouseButton.y<82){
   	string s=paramRayon.getString();
   	float tk=stof(s);
   	tk=tk+0.1;
   	if(tk>4){
   		tk=4;
   	}
   	tablePave[pvActif].scale=tk;
   	adr << std::fixed<< std::setprecision(2)<<tk;
      paramRayon.setString(adr.str());
   	paramRayon.setPosition(264-paramRayon.getLocalBounds().width-2,74);
   }
   if(paramRayon.getString()!="" && e.mouseButton.x>268 && e.mouseButton.x<280 && e.mouseButton.y>82 && e.mouseButton.y<92){
   	string s=paramRayon.getString();
   	float tk=stof(s);
   	tk=tk-0.1;
   	if(tk<0){
   		tk=0;
   	}
   	tablePave[pvActif].scale=tk;
   	adr << std::fixed<< std::setprecision(2)<<tk;
      paramRayon.setString(adr.str());
   	paramRayon.setPosition(264-paramRayon.getLocalBounds().width-2,74);
   }
   if(paramGainMin2.getString()!="" && e.mouseButton.x>130 && e.mouseButton.x<145 && e.mouseButton.y>117 && e.mouseButton.y<127){
   	string s= paramGainMin2.getString();
   	string s2= paramGainMax2.getString();
   	float tk=stof(s);
   	float tk2=stof(s2);
   	tk=tk+0.01;
   	if(tk>tk2){
   		tk=tk2-0.1;
   	}
   	if(tk>1){
   		tk=1;
   	}
   	adr << std::fixed<< std::setprecision(2)<<tk;
   	tablePave[pvActif].gainMin=tk;
      paramGainMin2.setString(adr.str());
   	paramGainMin2.setPosition(128-paramGainMin2.getLocalBounds().width-2,118);
   }
   if(paramGainMin2.getString()!="" && e.mouseButton.x>130 && e.mouseButton.x<145 && e.mouseButton.y>127 && e.mouseButton.y<137){
   	string s= paramGainMin2.getString();
   	float tk=stof(s);
   	tk=tk-0.01;
   	if(tk<0){
   		tk=0;
   	}
   	adr << std::fixed<< std::setprecision(2)<<tk;
   	tablePave[pvActif].gainMin=tk;
      paramGainMin2.setString(adr.str());
   	paramGainMin2.setPosition(128-paramGainMin2.getLocalBounds().width-2,118);
   }
   if(paramGainMax2.getString()!="" && e.mouseButton.x>266 && e.mouseButton.x<282 && e.mouseButton.y>116 && e.mouseButton.y<126){
   	string s= paramGainMax2.getString();
   	float tk=stof(s);
   	tk=tk+0.01;
   	if(tk>2){
   		tk=2;
   	}
   	adr << std::fixed<< std::setprecision(2)<<tk;
   	tablePave[pvActif].gainMax=tk;
      paramGainMax2.setString(adr.str());
   	paramGainMax2.setPosition(264-paramGainMax2.getLocalBounds().width-2,118);
   }
   if(paramGainMax2.getString()!="" && e.mouseButton.x>266 && e.mouseButton.x<282 && e.mouseButton.y>126 && e.mouseButton.y<136){
   	string s= paramGainMax2.getString();
   	string s2= paramGainMin2.getString();
   	float tk=stof(s);
   	float tk2=stof(s2);
   	tk=tk-0.01;
   	if(tk<tk2){
   		tk=tk2+0.01;
   	}
   	if(tk<0){
   		tk=0;
   	}
   	adr << std::fixed<< std::setprecision(2)<<tk;
   	tablePave[pvActif].gainMax=tk;
      paramGainMax2.setString(adr.str());
   	paramGainMax2.setPosition(264-paramGainMax2.getLocalBounds().width-2,118);
   }
 	
}
void onMoveBkg(sf::Event e){
	std::cout << "mouse x: " << e.mouseMove.x << std::endl;
   std::cout << "mouse y: " << e.mouseMove.y << std::endl;
}
void newBkgText(sf::Event e){
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
		float tk;
   	float tk2;
   	string s2;
		switch (bkgChampIndex){
       		case 1:
       			adr << std::fixed<< std::setprecision(2)<<apptxt;
       			paramRayon.setString(adr.str());
       			if(stof(adr.str())>4.0){
       				paramRayon.setString("4.00");
       			}
       			if(stof(adr.str())<0.1){
       				paramRayon.setString("0.1");
       			}
       			paramRayon.setPosition(264-paramRayon.getLocalBounds().width-2,74);
       			break;
       		case 2:
       			adr << std::fixed<< std::setprecision(2)<<apptxt;
       			paramGainMin2.setString(adr.str());
       			s2= paramGainMax2.getString();
       			tk=stof(adr.str());
   				tk2=stof(s2);
   				if(tk>tk2){
   					adr.clear();
  						adr.str("");
   					tk=tk2-0.01;
   					adr << std::fixed<< std::setprecision(2)<<tk;
   					paramGainMin2.setString(adr.str());
   				}
       			if(tk<0.1){
       				paramGainMin2.setString("0.01");
       			}
       			
       			paramGainMin2.setPosition(128-paramGainMin2.getLocalBounds().width-2,118);
       			break;
       		case 3:
       			adr << std::fixed<< std::setprecision(2)<<apptxt;
       			paramGainMax2.setString(adr.str());
  					s2= paramGainMin2.getString();
       			tk=stof(adr.str());
   				tk2=stof(s2);
   				if(tk<tk2){
   					tk=tk2+0.01;
   					adr.clear();
  						adr.str("");
   					adr << std::fixed<< std::setprecision(2)<<tk;
   					paramGainMax2.setString(adr.str());
   				}
   				if(tk>2.0){
       				paramGainMax2.setString("2.00");
       			}
       			paramGainMax2.setPosition(264-paramGainMax2.getLocalBounds().width-2,118);
       			break;
       		default:
       			break;
	    }
	    flagTxt=0;
   }
   adr.clear();
  	adr.str(""); 
}
//*********************************************************************************************************************   
//													Actions événements
//********************************************************************************************************************* 

void onWineClose(){
	char commandeSf[255];
   snprintf(commandeSf, 255, "%s", "killall -9 dynamic-jack-gtk");
   int pdSf=system(commandeSf);
  winPrincipale.close();
}
void onEventResized(sf::Event e){
   sf::FloatRect visibleArea(0, 0, e.size.width, e.size.height);	//fenêtre
   winPrincipale.setView(sf::View(visibleArea));
   windowAppWidth=e.size.width;
   windowAppHeight=e.size.height;
   workSpaceWidth=windowAppWidth;											//espace de travail
   workSpaceHeight=windowAppHeight-menuBarHeight;
   menuBar.setSize(sf::Vector2f(windowAppWidth,menuBarHeight));
   std::cout << "resize x: " << e.size.width<< std::endl;
   std::cout << "resize y: " << e.size.height << std::endl;
}
void onKeyPressed(sf::Event e){
	
}
void onTextEvent(sf::Event e){
	
}
void mouseWheel(sf::Event e){
	if (e.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel && pvActif<65535){
		
		if(e.mouseWheelScroll.delta==1){
			tablePave[pvActif].scale=tablePave[pvActif].scale-0.01;
			if(tablePave[pvActif].scale<-2){
			 	tablePave[pvActif].scale=2;
			} 	
		}else{
			tablePave[pvActif].scale=tablePave[pvActif].scale+0.01;
			if(tablePave[pvActif].scale>4){
			 	tablePave[pvActif].scale=4;
			} 	
		}
	}
}
void onClick(sf::Event e){
	std::cout << "the left button was pressed" << std::endl;
   std::cout << "mouse x: " << e.mouseButton.x << std::endl;
   std::cout << "mouse y: " << e.mouseButton.y << std::endl;
   stringstream adr;
   flagToolTip=0;
	if(e.mouseButton.x>10 && e.mouseButton.x<56 && e.mouseButton.y<28){
		menu1Flag=1;
	}
	if(menu1Flag==1 && e.mouseButton.x>4 && e.mouseButton.x<122 && e.mouseButton.y>30 && e.mouseButton.y<154){
		int index=(e.mouseButton.y-30)/20;
		switch (index) {
			case 0:
				nouveau();
				break;
			case 1:
				ouvrir();
				break;
			case 2:
				sauvegarder();
				break;
			case 3:
				sauvegarderSous();
				break;
			case 4:
				preferences();
				break;
			case 5:
				quitter();
				break;
		}
	}
	if(e.mouseButton.x>686 && e.mouseButton.x<744 && e.mouseButton.y<28){
			apropos();
	}
	if(e.mouseButton.x>758 && e.mouseButton.x<790 && e.mouseButton.y<28){
	  string cmd="firefox http://blanchemain.info/Documents/Programmation/index.php?page=metaFx &";
  	  char* dest = new char[cmd.length() + 1];
     std::copy(cmd.begin(), cmd.end(), dest);
     system(dest);
   }
	//*********************************************************************************************************************   
	//													Actions Événements barre d'outils
	//********************************************************************************************************************* 
	if(menu1Flag==0 && e.mouseButton.y>29 && e.mouseButton.y<59 ){
		if(e.mouseButton.x>4 && e.mouseButton.x<50 ){
			createMixer();
		}
		if(e.mouseButton.x>60 && e.mouseButton.x<78 ){
			deleteMixer();
		}
		if(e.mouseButton.x>82 && e.mouseButton.x<100 ){
			deleteGrpMixer();
		}
		if(e.mouseButton.x>106 && e.mouseButton.x<124 ){
			deleteAllMixer();
		}
		if(e.mouseButton.x>140 && e.mouseButton.x<160 ){
			typeMove=0;
		}
		if(e.mouseButton.x>167 && e.mouseButton.x<187 ){
			typeMove=1;
		}
		if(e.mouseButton.x>196 && e.mouseButton.x<216 ){
			typeMove=2;
		}
		if(e.mouseButton.x>228 && e.mouseButton.x<272 ){
			createPave();
		}
		if(e.mouseButton.x>290 && e.mouseButton.x<314 ){
			if(appFlag==0){
				appRun.setTexture(actStop);
				appFlag=1;
				defStringDSP();
			}else{
				appRun.setTexture(actRun);
				appFlag=0;
				char commandeSf[255];
		      snprintf(commandeSf, 255, "%s", "killall -9 dynamic-jack-gtk");
		      int pdSf=system(commandeSf);
			}
		}
	}
	
	mixerSelect=65535;
	for(int i=0;i<tableMixer.size();i++){
		if(e.mouseButton.x>tableMixer[i].getPosX() && e.mouseButton.x<tableMixer[i].getPosX()+24 && e.mouseButton.y>tableMixer[i].getPosY() && e.mouseButton.y<tableMixer[i].getPosY()+24){
			mixerSelect=i;
			mixerActif=mixerSelect;
			pvActif=65535;
			pvSelect=65535;
			if(e.mouseButton.button == sf::Mouse::Right){
				if(appFlag==0){
					if(winBkg.isOpen()){
						winBkg.close();
					}
			  		winPave.create(sf::VideoMode(308,336), "Mixer");
			  		readParamMixer();
				}
			}
		oldMoveX=e.mouseButton.x;
		oldMoveY=e.mouseButton.y;
		}
	}
	if(mixerSelect==65535 && mixerActif!=65535 && e.mouseButton.x>0 && e.mouseButton.y> menuBarHeight+barOutilsHeight && appFlag==1){
		tableMixer[mixerActif].setPosX(e.mouseButton.x-12);
		tableMixer[mixerActif].setPosY(e.mouseButton.y-12);
	}
	if(appFlag==0){
		float x;
		float y;
		float dt;
		for(int i=0;i<tablePave.size();i++){
			x=tablePave[i].pavX+(tablePave[i].rayon*tablePave[i].scale)-e.mouseButton.x;
			y=tablePave[i].pavY+(tablePave[i].rayon*tablePave[i].scale)-e.mouseButton.y;
			dt=sqrt((x*x)+(y*y));
			if(dt<tablePave[i].rayon*tablePave[i].scale){
				pvActif=i;
				pvSelect=i;
				mixerActif=65535;
				mixerSelect=65535;
				if(e.mouseButton.button == sf::Mouse::Right && appFlag==0){
					if(winPave.isOpen()){
						winPave.close();
					}
			  		winBkg.create(sf::VideoMode(308,200), "Pave");
			  		paramRayon.setString("");
			  		paramGainMin2.setString("");
			  		paramGainMax2.setString("");
			  		
			  		adr.clear();
					adr.str("");
			  		adr << std::fixed<< std::setprecision(2)<<tablePave[i].gainMin;
	    			paramGainMin2.setString(adr.str());
	    			paramGainMin2.setPosition(128-paramGainMin2.getLocalBounds().width-2,118);
	    			adr.clear();
					adr.str("");
	    			adr << std::fixed<< std::setprecision(2)<<tablePave[i].scale;
	    			paramRayon.setString(adr.str());
	    			paramRayon.setPosition(264-paramRayon.getLocalBounds().width-2,74);
	    			adr.clear();
					adr.str("");
					adr << std::fixed<< std::setprecision(2)<<tablePave[i].gainMax;
	    			paramGainMax2.setString(adr.str());
	    			paramGainMax2.setPosition(264-paramGainMax2.getLocalBounds().width-2,118);
			  		if(tablePave[i].trouNoir==0){
			  			defTrouNoir.setTexture(bNoSelect);
		 			}else{
		 				defTrouNoir.setTexture(bSelect);
		 			}
		 			std::cout <<" rayon get "<<tablePave[pvActif].rayon<<" gainMin "<<tablePave[pvActif].gainMin<<" gainMax "<<tablePave[pvActif].gainMax<< endl;
			  		paramFileBkg.setString(tablePave[i].file);
			  		//readParamMixer();

				}
			}
		}
	}
	std::cout << "mixerActif " << mixerActif << std::endl;
	std::cout << "pvActif button" << pvActif << std::endl;
	std::cout << "mixerSelect " << mixerSelect << std::endl;
}
void onMouseUp(sf::Event e){
	mixerSelect=65535;
	pvSelect=65535;
	std::cout << "mixerSelect " << mixerSelect << std::endl;
}

void onMouseMove(sf::Event e){
   std::cout << "mouse x: " << e.mouseMove.x << std::endl;
   std::cout << "mouse y: " << e.mouseMove.y << std::endl;
   int dx;
   int dy;
   int grp;
   float dt;
   float dt2;
   float pos;
   float cx;
   float cy;
   string s;
   if(menu1Flag==1 && e.mouseMove.x>122 || e.mouseMove.y>172){
		menu1Flag=0;
	}
	if(menu1Flag==1 && e.mouseMove.x>4 && e.mouseMove.x<122 && e.mouseMove.y>30 && e.mouseMove.y<160){
		flagToolTip=0;
		int index=(e.mouseMove.y-30)/20;
		if(index<6){
			menuSelectBox.setPosition(sf::Vector2f(4,34+(index*20)));
		}
	}
	if(e.mouseMove.y>30 && e.mouseMove.y<60){
		if(e.mouseMove.x>4 && e.mouseMove.x<50){
			flagToolTip=1;
			toolTip.setString(sToolTip[0]);
			float wx=(204-toolTip.getLocalBounds().width)/2;
			toolTip.setPosition(sf::Vector2f(wx,62));
			winToolTip.setSize(sf::Vector2f(200,20));
			winToolTip.setPosition(sf::Vector2f(10,60));
		}
		if(e.mouseMove.x>60 && e.mouseMove.x<80){
			flagToolTip=1;
			toolTip.setString(sToolTip[1]);
			float wx=(208-toolTip.getLocalBounds().width)/2;
			toolTip.setPosition(sf::Vector2f(wx,62));
			winToolTip.setSize(sf::Vector2f(150,20));
			winToolTip.setPosition(sf::Vector2f(30,60));
		}
		if(e.mouseMove.x>82 && e.mouseMove.x<102){
			flagToolTip=1;
			toolTip.setString(sToolTip[2]);
			float wx=(296-toolTip.getLocalBounds().width)/2;
			toolTip.setPosition(sf::Vector2f(wx,62));
			winToolTip.setSize(sf::Vector2f(200,20));
			winToolTip.setPosition(sf::Vector2f(50,60));
		}
		if(e.mouseMove.x>104 && e.mouseMove.x<124){
			flagToolTip=1;
			toolTip.setString(sToolTip[3]);
			float wx=(320-toolTip.getLocalBounds().width)/2;
			toolTip.setPosition(sf::Vector2f(wx,62));
			winToolTip.setSize(sf::Vector2f(180,20));
			winToolTip.setPosition(sf::Vector2f(70,60));
		}
		if(e.mouseMove.x>138 && e.mouseMove.x<162){
			flagToolTip=1;
			toolTip.setString(sToolTip[4]);
			float wx=(380-toolTip.getLocalBounds().width)/2;
			toolTip.setPosition(sf::Vector2f(wx,62));
			winToolTip.setSize(sf::Vector2f(180,20));
			winToolTip.setPosition(sf::Vector2f(100,60));
		}
		if(e.mouseMove.x>168 && e.mouseMove.x<190){
			flagToolTip=1;
			toolTip.setString(sToolTip[5]);
			float wx=(414-toolTip.getLocalBounds().width)/2;
			toolTip.setPosition(sf::Vector2f(wx,62));
			winToolTip.setSize(sf::Vector2f(180,20));
			winToolTip.setPosition(sf::Vector2f(120,60));
		}
		if(e.mouseMove.x>196 && e.mouseMove.x<218){
			flagToolTip=1;
			toolTip.setString(sToolTip[6]);
			float wx=(450-toolTip.getLocalBounds().width)/2;
			toolTip.setPosition(sf::Vector2f(wx,62));
			winToolTip.setSize(sf::Vector2f(180,20));
			winToolTip.setPosition(sf::Vector2f(140,60));
		}
		if(e.mouseMove.x>226 && e.mouseMove.x<272){
			flagToolTip=1;
			toolTip.setString(sToolTip[7]);
			float wx=(430-toolTip.getLocalBounds().width)/2;
			toolTip.setPosition(sf::Vector2f(wx,62));
			winToolTip.setSize(sf::Vector2f(120,20));
			winToolTip.setPosition(sf::Vector2f(160,60));
		}
		if(e.mouseMove.x>292 && e.mouseMove.x<314){
			flagToolTip=1;
			toolTip.setString(sToolTip[8]);
			float wx=(480-toolTip.getLocalBounds().width)/2;
			toolTip.setPosition(sf::Vector2f(wx,62));
			winToolTip.setSize(sf::Vector2f(120,20));
			winToolTip.setPosition(sf::Vector2f(180,60));
		}
	}
	if(e.mouseMove.y>60){
		flagToolTip=0;
	}
	if(mixerSelect!=65535){
		
		switch (typeMove){
	        case 0:
		       	tableMixer[mixerActif].setPosX(e.mouseMove.x-12);
					tableMixer[mixerActif].setPosY(e.mouseMove.y-12);
					if(tableMixer[mixerActif].getRelatif()==0){
						controlGainMixer(e,mixerActif);
					}else{
						controlGainRelatif(e,mixerActif);
					}
		     		break;
       	  case 1:
       	  		dx=e.mouseMove.x-oldMoveX;
       	  		dy=e.mouseMove.y-oldMoveY;
       	  		grp=tableMixer[mixerActif].getGroupe();
       	  		for(int i=0;i<tableMixer.size();i++){
       	  			if(tableMixer[i].getGroupe()==grp){
	       	  			tableMixer[i].setPosX(tableMixer[i].getPosX()+dx);
	       	  			tableMixer[i].setPosY(tableMixer[i].getPosY()+dy);
	       	  			if(tableMixer[mixerActif].getRelatif()==0){
								controlGainMixer(e,i);
							}else{
								controlGainRelatif(e,i);
							}
       	  			}
       	  		}
       	  		oldMoveX=e.mouseMove.x;
					oldMoveY=e.mouseMove.y;
       	  		std::cout << "dx: " << dx << std::endl;
				   break;
        	  case 2:
	  			   dx=e.mouseMove.x-oldMoveX;
       	  		dy=e.mouseMove.y-oldMoveY;
       	  		for(int i=0;i<tableMixer.size();i++){
       	  			tableMixer[i].setPosX(tableMixer[i].getPosX()+dx);
	       	  		tableMixer[i].setPosY(tableMixer[i].getPosY()+dy);
	       	  		if(tableMixer[mixerActif].getRelatif()==0){
							controlGainMixer(e,i);
						}else{
							controlGainRelatif(e,i);
						}
       	  		}
       	  		oldMoveX=e.mouseMove.x;
					oldMoveY=e.mouseMove.y;
          	   break;			
	        default: 
             	break;
  	      	 }       
	}
	if(pvSelect!=65535){ 
		tablePave[pvActif].pavX=e.mouseMove.x-((tableTexturePv[tablePave[pvActif].img].getSize().x/2)*tablePave[pvActif].scale);
		tablePave[pvActif].pavY=e.mouseMove.y-((tableTexturePv[tablePave[pvActif].img].getSize().y/2)*tablePave[pvActif].scale);
	}
}

void nouveau(){
	newInfo.setTxtPosX(0, 100);
	newInfo.setTxtPosY(0, 30);
	newInfo.setTxtPosX(1, 100);
	newInfo.setTxtPosY(1, 50);
	newInfo.setInfoTxtSize(14);
	newInfo.setTxt(0,supNouv1);
	sf::String s;
	s=sf::String::fromUtf8(supNouv2.begin(),supNouv2.end());
	newInfo.setTxt(1,s);
	bool rt=newInfo.drawInfo(0, "Alerte");
	if(rt==1){	   	
		for(int i=0;i<tableMixer.size();i++){
			if(winSecond[i].isOpen()){
				winSecond[i].close();
			}
			tableMixer[i].listExterne.clear();
			tableMixer[i].listModules.clear();
			tableMixer[i].tableConnectors.clear();
		}
		if(winPave.isOpen()){
			winPave.close();
		}
		if(winBkg.isOpen()){
			winPave.close();
		}
		if(winListExterne.isOpen()){
			winPave.close();
		}
		if(winExterne.isOpen()){
			winPave.close();
		}
		tablePave.clear();
		tableMixer.clear();
	}
}
void sauvegarder(){
	if(sessionPath=="" || sessionFile==""){
		sauvegarderSous();
	}else{
		saveMetaFx();
	}
}
void ouvrir(){
	nouveau();
	string s=home;
	fileSelector.initSelector(s);
	string rtf2=fileSelector.selector();
	string path=getcwd(NULL,0);
	sessionPath=path;
	sessionFile=rtf2;
	string nameFile=sessionPath+"/"+sessionFile;
	ifstream fichier(nameFile, ios::in);
	string contenu;
	Mixer rt;
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
				p = std::strtok(NULL,"=");
  			}
  			if(pf[0]=="Mixer"){
	  			pf2=explode(pf[1], ',');
	  			rt.setId(stoi(pf2[0]));
	  			rt.setGroupe(stoi(pf2[1]));
	  			rt.setPosX(stoi(pf2[2]));
	  			rt.setPosY(stoi(pf2[3]));
	  			rt.setDaw(stoi(pf2[4]));
	  			rt.setFx(stoi(pf2[5]));
	  			rt.setGain(stof(pf2[6]));
	  			rt.setRevGain(stoi(pf2[7]));
	  			rt.setRelatif(stoi(pf2[8]));
	  			rt.setSpeed(stof(pf2[9]));
	  			rt.setRevSpeed(stoi(pf2[10]));
	  			rt.setTrack(stoi(pf2[11]));
	  			rt.setGainMin(stof(pf2[12]));
	  			rt.setGainMax(stof(pf2[13]));
	  			rt.setSpeedMin(stof(pf2[14]));
	  			rt.setSpeedMax(stof(pf2[15]));
	  			if(pf2[16]!="/"){
	  				rt.setPath(pf2[16]);
	  			}else{
	  				rt.setPath("");
	  			}
	  			if(pf2[17]!="/"){
	  				rt.setPath(pf2[17]);
	  			}else{
	  				rt.setPath("");
	  			}
	  			tableMixer.push_back(rt);
	  			
  			}
  			if(pf[0]=="Externe"){
  				if(pf[1]!=""){
  					pf2.clear();
  					pf2=explode(pf[1], ';');
  					Fx nfx;
  					for(int j=0;j<pf2.size();j++){
  						pf3=explode(pf2[j], ',');
  						nfx.label=pf3[0];
  						nfx.name=pf3[1];
  						nfx.type=stoi(pf3[2]);
  						nfx.input=stoi(pf3[3]);
  						nfx.output=stoi(pf3[4]);
  						tableMixer.back().listExterne.push_back(nfx);
  					}
  				}
  			}
  			if(pf[0]=="Param"){
  				if(pf[1]!=""){
  					pf2.clear();
  					pf3.clear();
  					pf4.clear();
  					Contrainte nc;
  					pf2=explode(pf[1], '|');					// niveau mixer
  					for(int j=0;j<pf2.size();j++){
  						pf3.clear();
  						pf3=explode(pf2[j], ';');				// niveau greffon
  						for(int k=0;k<pf3.size();k++){
  							pf4.clear();
  							pf4=explode(pf3[k], ',');			// niveau contrainte
  							if(pf4.size()==8){
	  							nc.etat=stoi(pf4[0]);
								nc.label=pf4[1];
								nc.index=stoi(pf4[2]);
								nc.min=stof(pf4[3]);
								nc.max=stof(pf4[4]);
								nc.scale=stof(pf4[5]);
								nc.rev=stoi(pf4[6]);
								if(pf4[7]=="/"){
									nc.path="";
								}else{
									nc.path=pf4[7];
								}
								tableMixer.back().listExterne[j].tableParam.push_back(nc);
							}
  						}
  						
  					}
  				}
  			}
  			
  			if(pf[0]=="Interne"){
  				string sm=tableLabelModule[0].getString();
	  			Module nModule;
				nModule.initModule(sm, 0, 10, 200, 0, 36, 146, 36);
				tableMixer.back().listModules.push_back(nModule);
	   		sm=tableLabelModule[1].getString();
				nModule.initModule(sm,0, 800, 200, 0, 36, 0, 36);
				tableMixer.back().listModules.push_back(nModule);
  				if(pf[1]!=""){
  					pf2.clear();
  					pf2=explode(pf[1], ';');
  					Module md;
  					for(int j=0;j<pf2.size();j++){
  						pf3.clear();
  						pf3=explode(pf2[j],',');
  						md.label=pf3[0];
  						md.type=stoi(pf3[1]);
  						if(pf3[2]=="/"){
							md.path="";
						}else{
							md.path=pf3[2];
						}
  						md.x=stoi(pf3[3]);
  						md.y=stoi(pf3[4]);
  						md.inX=stoi(pf3[5]);
  						md.inY=stoi(pf3[6]);
  						md.outX=stoi(pf3[7]);
  						md.outY=stoi(pf3[8]);
  						tableMixer.back().listModules.push_back(md);
  					}
  				}
  			}
  			if(pf[0]=="intParam"){
  				if(pf[1]!=""){
  					pf2.clear();
  					pf3.clear();
  					pf4.clear();
  					Contrainte nc;
  					pf2=explode(pf[1], '|');
  					for(int j=0;j<pf2.size();j++){
  						pf3.clear();
  						pf3=explode(pf2[j], ';');				// niveau greffon
  						for(int k=0;k<pf3.size();k++){
  							pf4.clear();
  							pf4=explode(pf3[k], ',');			// niveau contrainte
  							nc.etat=stoi(pf4[0]);
							nc.label=pf4[1];
							nc.index=stoi(pf4[2]);
							nc.min=stof(pf4[3]);
							nc.max=stof(pf4[4]);
							nc.scale=stof(pf4[5]);
							nc.rev=stoi(pf4[6]);
							nc.path=pf4[7];
							tableMixer.back().listModules[j+2].tableContraintes.push_back(nc);
  						}
  					}
  				}
  			}
  			if(pf[0]=="intConnect"){
  				if(pf[1]!=""){
  				pf2.clear();
  				pf3.clear();
  				pf2=explode(pf[1], ';');
  				Connector connect;
	  				for(int j=0;j<pf2.size();j++){
	  					pf3.clear();
  						pf3=explode(pf2[j], ',');
	           		connect.entree=stoi(pf3[0]);
						connect.sortie=stoi(pf3[1]);
						connect.type=stoi(pf3[2]);
						connect.startX=stoi(pf3[3]);
						connect.startY=stoi(pf3[4]);
						if(stoi(pf3[0])==0){
							connect.startX=156;
							connect.startY=236;
						}
						connect.endX=stoi(pf3[5]);
						connect.endY=stoi(pf3[6]);
						if(stoi(pf3[1])==1){
							connect.endX=800;
							connect.endY=236;
						}
						connect.ctStartX=stoi(pf3[7]);
						connect.ctStartY=stoi(pf3[8]);
						connect.ctEndX=stoi(pf3[9]);
						connect.ctEndY=stoi(pf3[10]);				
						tableMixer.back().tableConnectors.push_back(connect);
						tableMixer.back().listModules[stoi(pf3[0])].out.push_back(stoi(pf3[1]));
						tableMixer.back().listModules[stoi(pf3[1])].in.push_back(stoi(pf3[0]));
						std::cout << "pf3[0] out"<<pf3[0]<<"  " <<pf3[1] << std::endl;
						std::cout << "pf3[0] in"<<pf3[1]<<"  "<<pf3[0] << std::endl;
					}
				}	
  			}
  		}
  		fichier.close();  // on referme le fichier
   }else{  // sinon
     cerr << "Erreur à l'ouverture Préférences!" << endl;
	}	
	
}
void sauvegarderSous(){
	string s=home;
	fileSelector.initSelector(s);
	string rtf2=fileSelector.selector();
	string path=getcwd(NULL,0);
	sessionPath=path;
	sessionFile=rtf2;
	saveMetaFx();
}
void preferences(){
	winPref.create(sf::VideoMode(500,180), labelGreffon);
}
void quitter(){
	newInfo.setTxtPosX(0, 100);
	newInfo.setTxtPosY(0, 30);
	newInfo.setInfoTxtSize(14);
	newInfo.setTxt(0,supQuit1);
	newInfo.setTxt(1,"");
	bool rt=newInfo.drawInfo(1, "Alerte");
	if(rt==0){
		winPrincipale.close();
	}else{
		sauvegarder();
	}
}
//*********************************************************************************************************************
//													gestion des fichiers
//*********************************************************************************************************************
void saveMetaFx(){
	string wfile=sessionPath+"/"+sessionFile;
	ofstream fichier(wfile, ios::out | ios::trunc);	
	string ref;
	int mx;
	if(fichier){
		
		for(int m=0;m<tableMixer.size();m++){
			fichier<<"Mixer=";
			fichier<<tableMixer[m].getId()<<","<<tableMixer[m].getGroupe()<<","<<tableMixer[m].getPosX()<<","<<tableMixer[m].getPosY()<<","<<tableMixer[m].getDaw()<<","<<tableMixer[m].getFx()<<","<<tableMixer[m].getGain()<<","<<tableMixer[m].getRevGain()<<","<<tableMixer[m].getRelatif()<<","<<tableMixer[m].getSpeed()<<","<<tableMixer[m].getRevSpeed()<<","<<tableMixer[m].getTrack()<<","<<tableMixer[m].getGainMin()<<","<<tableMixer[m].getGainMax()<<","<<tableMixer[m].getSpeedMin()<<","<<tableMixer[m].getSpeedMax()<<",";
			if(tableMixer[m].getPath()==""){
				fichier<<"/";
			}else{
				fichier<<tableMixer[m].getPath();
			}
			fichier<<",";
			if(tableMixer[m].getFile()==""){
				fichier<<"/";
			}else{
				fichier<<tableMixer[m].getFile();
			}
			fichier<< std::endl;
			fichier<<"Externe=";
			for(int j=0;j<tableMixer[m].listExterne.size();j++){
				fichier<<tableMixer[m].listExterne[j].label<<","<<tableMixer[m].listExterne[j].name<<","<<tableMixer[m].listExterne[j].type<<","<<tableMixer[m].listExterne[j].input<<","<<tableMixer[m].listExterne[j].output;
				if(j<tableMixer[m].listExterne.size()-1){
					fichier<<";";
				}
			}
			fichier<<std::endl;
			fichier<<"Param=";
			for(int j=0;j<tableMixer[m].listExterne.size();j++){
				mx=tableMixer[m].listExterne[j].tableParam.size();
				for(int k=0;k<mx;k++){
					fichier<<tableMixer[m].listExterne[j].tableParam[k].etat<<","<<tableMixer[m].listExterne[j].tableParam[k].label<<","<<tableMixer[m].listExterne[j].tableParam[k].index<<","<<tableMixer[m].listExterne[j].tableParam[k].min<<","<<tableMixer[m].listExterne[j].tableParam[k].max<<","<<tableMixer[m].listExterne[j].tableParam[k].scale<<","<<tableMixer[m].listExterne[j].tableParam[k].rev<<",";
					if(tableMixer[m].listExterne[j].tableParam[k].path==""){
						fichier<<"/";
					}else{
						fichier<<tableMixer[m].listExterne[j].tableParam[k].path;
					}
					if(k<tableMixer[m].listExterne[j].tableParam.size()-1){
					fichier<<";";
					}
				}
				
				if(j<tableMixer[m].listExterne.size()-1){
					fichier<<"|";
				}
			}
			fichier<<std::endl;
			
			fichier<<"Interne=";
			
			for(int j=2;j<tableMixer[m].listModules.size();j++){
				fichier<<tableMixer[m].listModules[j].label<<","<<tableMixer[m].listModules[j].type<<",";
				if(tableMixer[m].listModules[j].path==""){
					fichier<<"/";
				}else{
					fichier<<tableMixer[m].listModules[j].path;
				}
				fichier<<","<<tableMixer[m].listModules[j].x<<","<<tableMixer[m].listModules[j].y<<","<<tableMixer[m].listModules[j].inX<<","<<tableMixer[m].listModules[j].inY<<","<<tableMixer[m].listModules[j].outX<<","<<tableMixer[m].listModules[j].outY<<","<<tableMixer[m].listModules[j].in.size()<<",";
				if(j<tableMixer[m].listModules.size()-1){
					fichier<<";";
				}
			}
			fichier<<std::endl;
			fichier<<"intParam=";
			for(int j=2;j<tableMixer[m].listModules.size();j++){
				mx=tableMixer[m].listModules[j].tableContraintes.size();
				for(int k=0;k<mx;k++){
					fichier<<tableMixer[m].listModules[j].tableContraintes[k].etat<<","<<tableMixer[m].listModules[j].tableContraintes[k].label<<","<<tableMixer[m].listModules[j].tableContraintes[k].index<<","<<tableMixer[m].listModules[j].tableContraintes[k].min<<","<<tableMixer[m].listModules[j].tableContraintes[k].max<<","<<tableMixer[m].listModules[j].tableContraintes[k].scale<<","<<tableMixer[m].listModules[j].tableContraintes[k].rev<<",";
					if(tableMixer[m].listModules[j].tableContraintes[k].path==""){
						fichier<<"/";
					}else{
						fichier<<tableMixer[m].listModules[j].tableContraintes[k].path;
					}
					
					if(k<tableMixer[m].listModules[j].tableContraintes.size()-1){
					fichier<<";";
					}
				}
				if(j<tableMixer[m].listModules.size()-1){
					fichier<<"|";
				}
			}
			fichier<<std::endl;
			fichier<<"intConnect=";
			for(int j=0;j<tableMixer[m].tableConnectors.size();j++){
				fichier<<tableMixer[m].tableConnectors[j].entree<<","<<tableMixer[m].tableConnectors[j].sortie<<","<<tableMixer[m].tableConnectors[j].type<<","<<tableMixer[m].tableConnectors[j].startX<<","<<tableMixer[m].tableConnectors[j].startY<<","<<tableMixer[m].tableConnectors[j].endX<<","<<tableMixer[m].tableConnectors[j].endY<<","<<tableMixer[m].tableConnectors[j].ctStartX<<","<<tableMixer[m].tableConnectors[j].ctStartY<<","<<tableMixer[m].tableConnectors[j].ctEndX<<","<<tableMixer[m].tableConnectors[j].ctEndY;
				if(j<tableMixer[m].tableConnectors.size()-1){
					fichier<<";";
				}
			}
			
		}
		fichier<<endl;
		fichier.close();
	}else{  // sinon
      cerr << "Erreur à l'ouverture du fichier!" << endl;
	}

}

//*********************************************************************************************************************
//													gestion de la surface
//*********************************************************************************************************************
void createMixer(){
	newMixer.setId(baseIdCompte);
	baseIdCompte+=1;
	newMixer.setPosX(200);
	newMixer.setPosY(300);
	newMixer.setGainMin(0.1);
	newMixer.setGainMax(1.0);
	newMixer.setRevGain(1);
	newMixer.setDaw(1);
	newMixer.setSpeed(1);
	newMixer.setSpeedMin(1);
	newMixer.setSpeedMax(1);
	newMixer.setRevSpeed(1);
	newMixer.setRelatif(0);
	newMixer.setGain(0);
	newMixer.setFx(1);
	newMixer.setTrack(0);
	newMixer.setGroupe(defautGroupe);
	tableMixer.push_back(newMixer);
	mixerSelect=tableMixer.size()-1;
	mixerActif=mixerSelect;
	addEntree(mixerActif);
	addSortie(mixerActif);
}

void createPave(){
	std::cout << "create Pave" << std::endl;
	Pave pv;
	pv.pavX=200;
	pv.pavY=200;
	pv.rayon=100;
	pv.scale=1.0;
	pv.img=0;
	pv.gainMin=0.10;
	pv.gainMax=0.99;
	pv.trouNoir=0;
	tablePave.push_back(pv);
}

void readParamMixer(){
	stringstream adr;
	string s=tableMixer[mixerActif].getFile();
	paramFile.setString(s);
	int tk=tableMixer[mixerActif].getTrack();
	paramTrack.setString(to_string(tk));
   paramTrack.setPosition(130-paramTrack.getLocalBounds().width-2,102);
   tk=tableMixer[mixerActif].getSpeedMin();
   paramSpeedMin.setString(to_string(tk));
   paramSpeedMin.setPosition(278-paramSpeedMin.getLocalBounds().width-4,102);
   tk=tableMixer[mixerActif].getSpeedMax();
   paramSpeedMax.setString(to_string(tk));
   paramSpeedMax.setPosition(278-paramSpeedMax.getLocalBounds().width-4,138);
   float dp=tableMixer[mixerActif].getGainMin();
	adr << std::fixed<< std::setprecision(2)<<dp;
	paramGainMin.setString(adr.str());
	paramGainMin.setPosition(132-paramGainMin.getLocalBounds().width-2,170);
	adr.clear();
	adr.str("");
	dp=tableMixer[mixerActif].getGainMax();
	adr << std::fixed<< std::setprecision(2)<<dp;
	paramGainMax.setString(adr.str());
	paramGainMax.setPosition(132-paramGainMax.getLocalBounds().width-2,206);
	adr.clear();
	adr.str("");
 	bool p=tableMixer[mixerActif].getDaw();
   if(p==1){
   	defSelectDaw.setTexture(bNoSelect);
   }else{
   	defSelectDaw.setTexture(bSelect);
   }
 	p=tableMixer[mixerActif].getFx();
   if(p==1){
   	fxSelect.setTexture(bNoSelect);
   }else{
   	fxSelect.setTexture(bSelect);
   }
	p=tableMixer[mixerActif].getSpeed(); 
 	if(p==1){
 		defSpeed.setTexture(bNoSelect);
 	}else{
 		defSpeed.setTexture(bSelect);
 	}
 	p=tableMixer[mixerActif].getGain();
 	if(p==1){
 		defSelectGain.setTexture(bNoSelect);
 	}else{
 		defSelectGain.setTexture(bSelect);
 	}
 	p=tableMixer[mixerActif].getRelatif();
 	if(p==1){
 		defRelatif.setTexture(bNoSelect);
 	}else{
 		defRelatif.setTexture(bSelect);
 	}
}
void deleteMixer(){
	for(int i=0;i<tableMixer.size();i++){
		if(winSecond[i].isOpen()){
			winSecond[i].close();
		}
	}
	if(mixerActif!=65535){
	tableMixer.erase (tableMixer.begin()+mixerActif);
	mixerSelect=65535;
	mixerActif=mixerSelect;
	}
	if(pvActif!=65535){
		tablePave.erase(tablePave.begin()+pvActif);
		pvActif=65535;
	}
}

void deleteGrpMixer(){
	for(int i=0;i<tableMixer.size();i++){
		if(winSecond[i].isOpen()){
			winSecond[i].close();
		}
	}
	int grp=tableMixer[mixerActif].getGroupe();
	std::cout << "mixerActif " << mixerActif << "mixerActif grp " << grp << std::endl;
	for(int i=0;i<tableMixer.size();i++){
		std::cout << "mixer " << i << "mixer read grp " << tableMixer[i].getGroupe() << std::endl;
		if(tableMixer[i].getGroupe()==grp){
			tableMixer.erase (tableMixer.begin()+i);
			i=i-1;
		}
	}
	mixerSelect=65535;
	mixerActif=mixerSelect;
}
void deleteAllMixer(){
	for(int i=0;i<tableMixer.size();i++){
		if(winSecond[i].isOpen()){
			winSecond[i].close();
		}
	}
	tableMixer.clear();
	mixerSelect=65535;
	mixerActif=mixerSelect;
}
void onClickSelectGrp(sf::Event e){
	int id=(e.mouseButton.x/59)+((e.mouseButton.y/32)*8);
	tableMixer[mixerActif].setGroupe(id);
	defautGroupe=id;
	defGroupe.setTexture(groupe[defautGroupe]); 
	winSelectGrp.close();
}
void onClickSelectBkg(sf::Event e){
	int id=(e.mouseButton.x/59)+((e.mouseButton.y/32)*4);
	std::cout<<"pvActif3 "<<pvActif << "id: " << id << std::endl;
	tablePave[pvActif].img=id;
	winSelectBkg.close();
}
void drawSurface(){
}
void apropos(){
	std::cout <<"a propos "<< std::endl;
	string s=defInstall+"/Theme/Defaut/Defaut";
	newApropos.drawApropos(defInstall,prefTheme,defGui,prefLang,"firefox");
}
void onClickPave(sf::Event e){
	std::cout << "the left button was pressed" << std::endl;
   std::cout << "mouse x: " << e.mouseButton.x << std::endl;
   std::cout << "mouse y: " << e.mouseButton.y << std::endl;
   stringstream adr;
	if(e.mouseButton.x>100 && e.mouseButton.x<124 && e.mouseButton.y>274 && e.mouseButton.y<298){
		winSelectGrp.create(sf::VideoMode(473,128), "Groupe");
	}
	if(e.mouseButton.x>64 && e.mouseButton.x<214 && e.mouseButton.y>11 && e.mouseButton.y<41){
		char * cstr = new char [home.length()+1]; //   Lecture du dossier home
		std::strcpy (cstr,home.c_str());
		int ncwd=chdir(cstr);
		string path=getcwd(NULL,0);
   	string s=path;
   	SF_INFO sfinfo;
		fileSelector.initSelector(s);
		string rtf2=fileSelector.selector();
		path=getcwd(NULL,0);
		tableMixer[mixerActif].setPath(path);
		tableMixer[mixerActif].setFile(rtf2);
		paramFile.setString(rtf2);
		paramFile.setPosition(262-paramFile.getLocalBounds().width-4,18);
		std::cout << "path: " << tableMixer[mixerActif].getPath()<< "file :"<<tableMixer[mixerActif].getFile()<< std::endl;
		string sFileName=tableMixer[mixerActif].getPath()+"/"+tableMixer[mixerActif].getFile();
      const char *dpath=sFileName.c_str();
      SNDFILE* sndfile=sf_open(dpath, SFM_READ, &sfinfo) ;
      sf_close(sndfile) ;
      cout <<path <<" canaux : " << sfinfo.channels<<" Sample rate = "<< sfinfo.samplerate<<" frames : "<<sfinfo.frames<< endl;
	}
	if(e.mouseButton.x>278 && e.mouseButton.x<296 && e.mouseButton.y>10 && e.mouseButton.y<30){
		readFileDSP("",tableMixer[mixerActif].getPath(),tableMixer[mixerActif].getFile());
	}
	if(e.mouseButton.x>278 && e.mouseButton.x<296 && e.mouseButton.y>32 && e.mouseButton.y<52){
		fileDelete();
	}
	if(e.mouseButton.x>104 && e.mouseButton.x<120 && e.mouseButton.y>70 && e.mouseButton.y<82){
	 	bool p=tableMixer[mixerActif].getDaw();
	   if(p==0){
	   	tableMixer[mixerActif].setDaw(1);
	   	defSelectDaw.setTexture(bNoSelect);
	   }else{
	   	tableMixer[mixerActif].setDaw(0);
	   	defSelectDaw.setTexture(bSelect);
	   }
	}
	if(e.mouseButton.x>254 && e.mouseButton.x<268 && e.mouseButton.y>206 && e.mouseButton.y<220){
	 	bool p=tableMixer[mixerActif].getFx();
	   if(p==0){
	   	tableMixer[mixerActif].setFx(1);
	   	fxSelect.setTexture(bNoSelect);
	   }else{
	   	tableMixer[mixerActif].setFx(0);
	   	fxSelect.setTexture(bSelect);
	   }
	 }
	 if(e.mouseButton.x>254 && e.mouseButton.x<268 && e.mouseButton.y>70 && e.mouseButton.y<82){
	 	bool p=tableMixer[mixerActif].getSpeed(); 
	 	if(p==0){
	 		tableMixer[mixerActif].setSpeed(1);
	 		defSpeed.setTexture(bNoSelect);
	 	}else{
	 		tableMixer[mixerActif].setSpeed(0);
	 		defSpeed.setTexture(bSelect);
	 	}
	 }
	 if(e.mouseButton.x>254 && e.mouseButton.x<268 && e.mouseButton.y>170 && e.mouseButton.y<184){
	 	bool p=tableMixer[mixerActif].getRevSpeed(); 
	 	if(p==0){
	 		tableMixer[mixerActif].setRevSpeed(1);
	 		defRevSpeed.setTexture(bNoSelect);
	 	}else{
	 		tableMixer[mixerActif].setRevSpeed(0);
	 		defRevSpeed.setTexture(bSelect);
	 	}
	 }
	 if(e.mouseButton.x>76 && e.mouseButton.x<91 && e.mouseButton.y>242 && e.mouseButton.y<256){
	 	bool p=tableMixer[mixerActif].getRevGain(); 
	 	if(p==0){
	 		tableMixer[mixerActif].setRevGain(1);
	 		defRevGain.setTexture(bNoSelect);
	 	}else{
	 		tableMixer[mixerActif].setRevGain(0);
	 		defRevGain.setTexture(bSelect);
	 	}
	 }
	 if(e.mouseButton.x>196 && e.mouseButton.x<211 && e.mouseButton.y>242 && e.mouseButton.y<256){
	 	bool p=tableMixer[mixerActif].getRelatif(); 
	 	if(p==0){
	 		tableMixer[mixerActif].setRelatif(1);
	 		defRelatif.setTexture(bNoSelect);
	 	}else{
	 		tableMixer[mixerActif].setRelatif(0);
	 		defRelatif.setTexture(bSelect);
	 	}
	 }
	 if(e.mouseButton.x>106 && e.mouseButton.x<120 && e.mouseButton.y>134 && e.mouseButton.y<148){
	 	bool p=tableMixer[mixerActif].getGain();
	 	if(p==0){
	 		tableMixer[mixerActif].setGain(1);
	 		defSelectGain.setTexture(bNoSelect);
	 	}else{
	 		tableMixer[mixerActif].setGain(0);
	 		defSelectGain.setTexture(bSelect);
	 	}
	 }
	 if(e.mouseButton.x>134 && e.mouseButton.x<144 && e.mouseButton.y>102 && e.mouseButton.y<112){
   	int tk=tableMixer[mixerActif].getTrack();
   	tk=tk+1;
   	if(tk>64){
   		tk=64;
   	}
   	tableMixer[mixerActif].setTrack(tk);
   	paramTrack.setString(to_string(tk));
   	paramTrack.setPosition(130-paramTrack.getLocalBounds().width-2,102);
   }
   if(e.mouseButton.x>134 && e.mouseButton.x<144 && e.mouseButton.y>112 && e.mouseButton.y<122){
   	int tk=tableMixer[mixerActif].getTrack();
   	tk=tk-1;
   	if(tk<0){
   		tk=0;
   	}
   	tableMixer[mixerActif].setTrack(tk);
   	paramTrack.setString(to_string(tk));
   	paramTrack.setPosition(130-paramTrack.getLocalBounds().width-2,102);
   }
   if(e.mouseButton.x>282 && e.mouseButton.x<294 && e.mouseButton.y>102 && e.mouseButton.y<112){
   	int tk=tableMixer[mixerActif].getSpeedMin();
   	tk=tk+1;
   	if(tk>1){
   		tk=1;
   	}
   	tableMixer[mixerActif].setSpeedMin(tk);
   	paramSpeedMin.setString(to_string(tk));
   	paramSpeedMin.setPosition(278-paramSpeedMin.getLocalBounds().width-2,102);
   }
   if(e.mouseButton.x>282 && e.mouseButton.x<294 && e.mouseButton.y>112 && e.mouseButton.y<122){
   	int tk=tableMixer[mixerActif].getSpeedMin();
   	tk=tk-1;
   	if(tk<0){
   		tk=0;
   	}
   	tableMixer[mixerActif].setSpeedMin(tk);
   	paramSpeedMin.setString(to_string(tk));
   	paramSpeedMin.setPosition(278-paramSpeedMin.getLocalBounds().width-2,102);
   }
   if(e.mouseButton.x>282 && e.mouseButton.x<294 && e.mouseButton.y>136 && e.mouseButton.y<146){
   	int tk=tableMixer[mixerActif].getSpeedMax();
   	tk=tk+1;
   	if(tk>2){
   		tk=2;
   	}
   	tableMixer[mixerActif].setSpeedMax(tk);
   	paramSpeedMax.setString(to_string(tk));
   	paramSpeedMax.setPosition(278-paramSpeedMax.getLocalBounds().width-2,137);
   }
   if(e.mouseButton.x>282 && e.mouseButton.x<294 && e.mouseButton.y>146 && e.mouseButton.y<156){
   	int tk=tableMixer[mixerActif].getSpeedMax();
   	tk=tk-1;
   	if(tk<0){
   		tk=0;
   	}
   	tableMixer[mixerActif].setSpeedMax(tk);
   	paramSpeedMax.setString(to_string(tk));
   	paramSpeedMax.setPosition(278-paramSpeedMax.getLocalBounds().width-2,137);
   }
   if(e.mouseButton.x>136 && e.mouseButton.x<148 && e.mouseButton.y>170 && e.mouseButton.y<180){
   	float dp=tableMixer[mixerActif].getGainMin();
   	dp=dp+0.01;
   	if(dp>tableMixer[mixerActif].getGainMax()){
   		dp=tableMixer[mixerActif].getGainMax()-0.01;
   	}
   	tableMixer[mixerActif].setGainMin(dp);
   	adr << std::fixed<< std::setprecision(2)<<dp;
   	paramGainMin.setString(adr.str());
   	paramGainMin.setPosition(132-paramGainMin.getLocalBounds().width-2,170);
   	adr.clear();
		adr.str("");
   }
   if(e.mouseButton.x>136 && e.mouseButton.x<148 && e.mouseButton.y>180 && e.mouseButton.y<190){
   	float dp=tableMixer[mixerActif].getGainMin();
   	dp=dp-0.01;
   	if(dp<0.01){
   		dp=0.01;
   	}
   	tableMixer[mixerActif].setGainMin(dp);
   	adr << std::fixed<< std::setprecision(2)<<dp;
   	paramGainMin.setString(adr.str());
   	paramGainMin.setPosition(132-paramGainMin.getLocalBounds().width-2,170);
   	adr.clear();
		adr.str("");
   }
   if(e.mouseButton.x>136 && e.mouseButton.x<148 && e.mouseButton.y>204 && e.mouseButton.y<215){
   	float dp=tableMixer[mixerActif].getGainMax();
   	dp=dp+0.01;
   	if(dp>4.0){
   		dp=4.0;
   	}
   	tableMixer[mixerActif].setGainMax(dp);
   	adr << std::fixed<< std::setprecision(2)<<dp;
   	paramGainMax.setString(adr.str());
   	paramGainMax.setPosition(132-paramGainMax.getLocalBounds().width-2,206);
   	adr.clear();
		adr.str("");
   }
   if(e.mouseButton.x>136 && e.mouseButton.x<148 && e.mouseButton.y>215 && e.mouseButton.y<225){
   	float dp=tableMixer[mixerActif].getGainMax();
   	dp=dp-0.01;
   	if(dp<0.01){
   		dp=0.01;
   	}
   	tableMixer[mixerActif].setGainMax(dp);
   	adr << std::fixed<< std::setprecision(2)<<dp;
   	paramGainMax.setString(adr.str());
   	paramGainMax.setPosition(132-paramGainMax.getLocalBounds().width-2,206);
   	adr.clear();
		adr.str("");
   }
   if(e.mouseButton.x>76 && e.mouseButton.x<136  && e.mouseButton.y>170 && e.mouseButton.y<190){
   	paveChampIndex=1;
   	apptxt="";
   	//paramGainMin.setString("");
   	winPaveCurseur.setPosition(133,172);
	 	flagPaveCurseur=1;
	 	clockCurseur.restart();
   }
   if(e.mouseButton.x>76 && e.mouseButton.x<136  && e.mouseButton.y>204 && e.mouseButton.y<224){
   	paveChampIndex=2;
   	apptxt="";
   	winPaveCurseur.setPosition(133,206);
	 	flagPaveCurseur=1;
	 	//paramGainMax.setString("");
	 	clockCurseur.restart();
   }
   if(e.mouseButton.x>234 && e.mouseButton.x<280  && e.mouseButton.y>100 && e.mouseButton.y<122){
   	paveChampIndex=3;
   	apptxt="";
   	//paramGainMin.setString("");
   	winPaveCurseur.setPosition(279,102);
	 	flagPaveCurseur=1;
	 	clockCurseur.restart();
   }
   if(e.mouseButton.x>234 && e.mouseButton.x<280  && e.mouseButton.y>134 && e.mouseButton.y<156){
   	paveChampIndex=4;
   	apptxt="";
   	winPaveCurseur.setPosition(279,137);
	 	flagPaveCurseur=1;
	 	//paramGainMax.setString("");
	 	clockCurseur.restart();
   }
   if(tableMixer[mixerActif].getFx()==0 && e.mouseButton.x>244 && e.mouseButton.x<274  && e.mouseButton.y>236 && e.mouseButton.y<268){
   	string name="Graphe"+to_string(mixerActif);
		nameSecond[mixerActif]=name;
		winSecond[mixerActif].create(sf::VideoMode(1000,600), name);
		sf::FloatRect area(0, 0, 1000,600);
		winSecond[mixerActif].setView(sf::View(area));
		int sy=(tableLabelModule.size()*18)-214;
		if(sy<0){
			sy=214;
		}else{
			sy=214-(sy/18);
		}
		
		menuFxSlider.setSize(sf::Vector2f(14,sy));
   }
   if(e.mouseButton.x>186 && e.mouseButton.x<242  && e.mouseButton.y>296 && e.mouseButton.y<324){
   	winPave.close();
   }
   if(e.mouseButton.x>246 && e.mouseButton.x<296  && e.mouseButton.y>296 && e.mouseButton.y<324){
   	mixerPaveValid();
   }
}
void mixerPaveValid(){
	winPave.close();
}
void fileDelete(){
	tableMixer[mixerActif].setPath("");
	tableMixer[mixerActif].setFile("");
	tableMixer[mixerActif].setSpeedMin(1);
	tableMixer[mixerActif].setSpeedMax(0);
	paramFile.setString("");
	paramFile.setPosition(262-paramFile.getLocalBounds().width-4,18);
	paramSpeedMin.setString("1");
   paramSpeedMin.setPosition(296-paramSpeedMin.getLocalBounds().width-4,65);
   paramSpeedMax.setString("");
   paramSpeedMax.setPosition(296-paramSpeedMax.getLocalBounds().width-4,100);
}
void newPaveText(sf::Event e){
   int key=e.key.code;
   std::cout << "key: " << e.key.code << std::endl;
   stringstream adr;
   if (e.text.unicode < 128){
		if(key==8){
	      apptxt=apptxt.substr(0,apptxt.length()-1);
	 	}else{
			apptxt=apptxt+static_cast<char>(e.text.unicode);
			flagTxt=1;
		}
		switch (paveChampIndex){
       		case 1:
       			adr << std::fixed<< std::setprecision(2)<<apptxt;
       			paramGainMin.setString(adr.str());
       			paramGainMin.setPosition(132-paramGainMin.getLocalBounds().width-2,170);
       			break;
       		case 2:
       			adr << std::fixed<< std::setprecision(2)<<apptxt;
       			paramGainMax.setString(adr.str());
       			paramGainMax.setPosition(132-paramGainMax.getLocalBounds().width-2,206);
       			break;
       		case 3:
       			adr << std::fixed<< std::setprecision(2)<<apptxt;
       			paramSpeedMin.setString(adr.str());
       			paramSpeedMin.setPosition(278-paramSpeedMin.getLocalBounds().width-2,102);
       			break;
       		case 4:
       			adr << std::fixed<< std::setprecision(2)<<apptxt;
       			paramSpeedMax.setString(adr.str());
       			paramSpeedMax.setPosition(278-paramSpeedMax.getLocalBounds().width-2,137);
       			break;
       		default:
       			break;
	    }
	    
   }
   adr.clear();
  	adr.str(""); 
}
void onPaveMove(sf::Event e){
   std::cout << "mouse x: " << e.mouseMove.x << std::endl;
   std::cout << "mouse y: " << e.mouseMove.y << std::endl;
   string ds;
   stringstream adr;
	if(flagTxt==1 && e.mouseMove.x>76 && e.mouseMove.x<298 && e.mouseMove.y>96 && e.mouseMove.y<230){
		switch (paveChampIndex){
       		case 1:
       			ds=paramGainMin.getString();
	   			if(stof(ds)<0 || stof(ds)>1){
	   				std::cout << "erreur " << std::endl;
	   				flagTxt=0;
	  					adr << std::fixed<< std::setprecision(2)<<tableMixer[mixerActif].getGainMin();
	   			}else{
	   				adr << std::fixed<< std::setprecision(2)<<stof(ds);
	   			}
	   			tableMixer[mixerActif].setGainMin(stof(ds));
	   			paramGainMin.setString(adr.str());
					paramGainMin.setPosition(132-paramGainMin.getLocalBounds().width-2,170);
					adr.clear();
	  				adr.str("");
       			break;
       		case 2:
       			ds=paramGainMax.getString();
	   			if(stof(ds)<0 || stof(ds)>2){
	   				std::cout << "erreur " << std::endl;
	   				flagTxt=0;
	  					adr << std::fixed<< std::setprecision(2)<<tableMixer[mixerActif].getGainMax();
	   			}else{
	   				adr << std::fixed<< std::setprecision(2)<<stof(ds);
	   			}
	   			tableMixer[mixerActif].setGainMax(stof(ds));
	   			paramGainMax.setString(adr.str());
	   			paramGainMax.setPosition(132-paramGainMax.getLocalBounds().width-2,206);
	   			adr.clear();
	  				adr.str("");
       			break;
       		case 3:
       			ds=paramSpeedMin.getString();
	   			if(stof(ds)<0 || stof(ds)>1){
	   				std::cout << "erreur " << std::endl;
	   				flagTxt=0;
	  					adr << std::fixed<< std::setprecision(2)<<tableMixer[mixerActif].getSpeedMin();
	   			}else{
	   				adr << std::fixed<< std::setprecision(2)<<stof(ds);
	   			}
	   			tableMixer[mixerActif].setSpeedMin(stof(ds));
	   			paramSpeedMin.setString(adr.str());
					paramSpeedMin.setPosition(278-paramSpeedMin.getLocalBounds().width-2,102);
					adr.clear();
	  				adr.str("");
       			break;
       		case 4:
       			ds=paramSpeedMax.getString();
	   			if(stof(ds)<0 || stof(ds)>2){
	   				std::cout << "erreur " << std::endl;
	   				flagTxt=0;
	  					adr << std::fixed<< std::setprecision(2)<<tableMixer[mixerActif].getSpeedMax();
	   			}else{
	   				adr << std::fixed<< std::setprecision(2)<<stof(ds);
	   			}
	   			tableMixer[mixerActif].setSpeedMax(stof(ds));
	   			paramSpeedMax.setString(adr.str());
	   			paramSpeedMax.setPosition(278-paramSpeedMax.getLocalBounds().width-2,137);
	   			adr.clear();
	  				adr.str("");
       			break;	
       		default:
       		break;
      }
	}
	flagTxt=0;
}
//*********************************************************************************************************************
//													Modules
//*********************************************************************************************************************
void readListModules(){
	string path=defInstall;
	std::cout << "base : " << path << std::endl;
	string file=path+"/listeFx.txt";
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
	     p = std::strtok(NULL,"=");
       }
       if(pf[0]=="Plugin"){
      	pf2=explode(pf[1], ',');
      	std::cout << "pf1 ok "<<pf[1]<<" size=  "<<pf2.size()<< std::endl;
      	tableFx[pf2[0]].label=pf2[0];
      	tableFx[pf2[0]].name=pf2[1];
      	tableFx[pf2[0]].type=stoi(pf2[2]);
      	tableFx[pf2[0]].input=stoi(pf2[3]);
      	tableFx[pf2[0]].output=stoi(pf2[4]);
    		ns.setString(pf2[0]);
         tableLabelModule.push_back(ns);
         id++;
       }
       if(pf[0]=="Param"){
      	pf3=explode(pf[1], ';');
      	for(int m=0;m<pf3.size();m++){
      		std::cout << "size "<<pf3.size()<<"param pf3: "<<pf3[m]  << std::endl;
      	}
      	for(int m=0;m<pf3.size();m++){
      		pf4=explode(pf3[m], ',');
 	     		tableFx[pf2[0]].tableParam.push_back(pr);
 	     		tableFx[pf2[0]].tableParam.back().etat=stoi(pf4[0]);
 	     		tableFx[pf2[0]].tableParam.back().label=pf4[1];
 	     		tableFx[pf2[0]].tableParam.back().min=stof(pf4[2]);
 	     		tableFx[pf2[0]].tableParam.back().max=stof(pf4[3]);
 	     		tableFx[pf2[0]].tableParam.back().scale=stoi(pf4[4]);
 	     		tableFx[pf2[0]].tableParam.back().rev=stoi(pf4[5]);
 	     		tableFx[pf2[0]].tableParam.back().path=pf4[6];
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
	for(int q=0;q<tableLabelModule.size();q++){
		tableLabelModule[q].setFont(font);
		tableLabelModule[q].setCharacterSize(12);
		tableLabelModule[q].setFillColor(sf::Color(0,0,0));
   }
}
void readListExterne(){
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
const vector<string>  explode(const string& s, const char& c){
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
//*********************************************************************************************************************
//													Controleurs
//*********************************************************************************************************************
void controlGainMixer(sf::Event e,int id){
	float x=(windowAppWidth/2)*(windowAppWidth/2);
	float y=((windowAppHeight-60)/2)*((windowAppHeight-60)/2);
	float dx=sqrt(x+y);
	float dt;
	float dt2;
	float mx;
	string cm;
	mx=(tableMixer[id].getGainMax()-tableMixer[id].getGainMin())/dx;
	dt2=distance(windowAppWidth/2,windowAppHeight/2,tableMixer[id].getPosX(),tableMixer[id].getPosY());
	if(tableMixer[id].getRevGain()==1){
		dt=tableMixer[id].getGainMax()-(dt2*mx);
	}else{
		dt=tableMixer[id].getGainMin()+(dt2*mx);	
	}
	if(dt<tableMixer[id].getGainMin()){
		dt=tableMixer[id].getGainMin();
	}
	if(dt>tableMixer[id].getGainMax()){
		dt=tableMixer[id].getGainMax();
	}
	cm="/fileFx/Mixer/M"+to_string(id)+"/Player/Param/gain_"+to_string(id);
	oscSendGain(cm,dt);
	controlParamsMixer(e,id,dx,dt2);
	uptdateGreffon(e,id,dx,dt2);
	cout <<" dx1 : " << dx<<" mx = "<< mx<<" distance "<<dt2<<" gain : "<<dt<< endl;
	controlGainPave(e);
}
void controlGainRelatif(sf::Event e,int id){
	float newR;
	float cx;
	float cy;
	float dt;
	float dt2;
	float mx;
	float g=0;
	int nb=0;
	string cm;
	for(int i=0;i<tablePave.size();i++){
		newR=tablePave[i].rayon*tablePave[i].scale;
		cx=tablePave[i].pavX+newR;
		cy=tablePave[i].pavY+newR;
		dt2=distance(cx,cy,tableMixer[id].getPosX(),tableMixer[id].getPosY());
		if(dt2<newR){
			if(tableMixer[id].getRevGain()==1){
				dt=tableMixer[id].getGainMax()-(dt2/newR);
			}else{
				dt=tableMixer[id].getGainMin()+(dt2/newR);	
			}
			if(dt<tableMixer[id].getGainMin()){
				dt=tableMixer[id].getGainMin();
			}
			if(dt>tableMixer[id].getGainMax()){
				dt=tableMixer[id].getGainMax();
			}
			g=g+dt;
			nb++;
		}
	}
	g=g/nb;
	for(int i=0;i<tablePave.size();i++){
		newR=tablePave[i].rayon*tablePave[i].scale;
		cx=tablePave[i].pavX+newR ;
		cy=tablePave[i].pavY+newR ;
		dt2=distance(cx,cy,tableMixer[id].getPosX(),tableMixer[id].getPosY());
		if(dt2<newR){
			cm="/fileFx/Mixer/M"+to_string(id)+"/Player/Param/gain_"+to_string(id);
			oscSendGain(cm,g);
			controlParamsMixer(e,id,newR,dt2);
			uptdateGreffon(e,id,newR,dt2);
			controlGainPave(e);
		}
		cout <<" dx1 : " << newR<<" distance "<<dt2<<" gain : "<<g<< endl;
	}
}

void controlGainPave(sf::Event e){
	float mx;
	float dt;
	float newR;
	int nbc=0;
	float maxDt=0;
	for(int i=0;i<tablePave.size();i++){
		newR=tablePave[i].rayon*tablePave[i].scale;
		for(int j=0;j<tableMixer.size();j++){
			mx=tablePave[i].gainMax/newR;
			dt=distance(tablePave[i].pavX+newR,tablePave[i].pavY+newR,tableMixer[j].getPosX(),tableMixer[j].getPosY());
			if(dt<newR){
				maxDt=maxDt+tablePave[i].gainMax-(dt*mx);
			if(maxDt>tablePave[i].gainMax){
				maxDt=tablePave[i].gainMax;
			}
			string cm="/fileFx/Mixer/P"+to_string(i)+"/Player/Param/gain_"+to_string(i);
			oscSendGain(cm,maxDt);
			}
		}
	}
}
void controlParamsMixer(sf::Event e,int id,float dx, float dt2){
	float amp;
	float mx;
	float dt;
	int idh=0;
	string cm;
	if(tableMixer[id].getDaw()==1 && tableMixer[id].getFx()==0){
		for(int i=2;i<tableMixer[id].listModules.size();i++){
			if(tableMixer[id].listModules[i].type==0){
				for(int j=0;j<tableMixer[id].listModules[i].tableContraintes.size();j++){
					if(tableMixer[id].listModules[i].tableContraintes[j].etat==1){
						amp=tableMixer[id].listModules[i].tableContraintes[j].max-tableMixer[id].listModules[i].tableContraintes[j].min;
						mx=amp/dx;
						if(tableMixer[id].listModules[i].tableContraintes[j].rev==0){
							dt=tableMixer[id].listModules[i].tableContraintes[j].max-(dt2*mx);
						}else{
							dt=tableMixer[id].listModules[i].tableContraintes[j].min+(dt2*mx);
						}
						if(dt<tableMixer[id].listModules[i].tableContraintes[j].min){
							dt=tableMixer[id].listModules[i].tableContraintes[j].min;
						}
						if(dt>tableMixer[id].listModules[i].tableContraintes[j].max){
							dt=tableMixer[id].listModules[i].tableContraintes[j].max;
						}
						
						cm=tableMixer[id].listModules[i].path+tableMixer[id].listModules[i].tableContraintes[j].path;
						//cm="/metaFx/Mixer/M"+to_string(id)+"/v0/h"+to_string(idh)+"/"+tableMixer[id].listModules[i].tableContraintes[j].path;
						cout <<" module path2 " << tableMixer[id].listModules[i].path<< endl;
						oscSendGain(cm, dt);
						idh++;
					}
				}
			}
		}	
		cout <<" module size " << tableMixer[mixerActif].listModules.size()<< endl;
	}
}
void uptdateGreffon(sf::Event e, int id, float dx,float dt2){
	float amp;
	float mx;
	float dt;
	string cm="";
	int piste;
	int ordre;
	lo::Address a(prefServeur, prefPortD);
	if(tableMixer[id].getDaw()==0 && tableMixer[id].listExterne.size()>0){
		for(int i=0;i<tableMixer[id].listExterne.size();i++){
			for(int j=0;j<tableMixer[id].listExterne[i].tableParam.size();j++){
				if(tableMixer[id].listExterne[i].tableParam[j].etat==1){
					amp=tableMixer[id].listExterne[i].tableParam[j].max-tableMixer[id].listExterne[i].tableParam[j].min;
					mx=amp/dx;
					if(tableMixer[id].listExterne[i].tableParam[j].rev==0){
						dt=tableMixer[id].listExterne[i].tableParam[j].max-(dt2*mx);
					}else{
						dt=tableMixer[id].listExterne[i].tableParam[j].min+(dt2*mx);
					}
					if(dt<tableMixer[id].listExterne[i].tableParam[j].min){
						dt=tableMixer[id].listExterne[i].tableParam[j].min;
					}
					if(dt>tableMixer[id].listExterne[i].tableParam[j].max){
						dt=tableMixer[id].listExterne[i].tableParam[j].max;
					}
					// Ardour /strip/plugin/parameter iiif ssid piid param value
					piste=tableMixer[id].getTrack();
					if(prefDAW==0){
						cm="/strip/plugin/parameter";
						a.send(cm, "iiif",piste,i+1,j+1,dt);
					}else{
						cm="/track/"+to_string(piste)+"/fx/"+to_string(i+1)+"/fxparam/"+to_string(j+1)+"/value";
						a.send(cm, "f",dt/tableExterne[tableMixer[id].listExterne[i].label].tableParam[j].max);
					}
					std::cout<<"prefDAW "<< prefDAW<<" commande "<<cm<<" Piste "<<piste<<" ordre "<<i+1<<" param "<< j+1<<" f "<<dt<<" dt "<<dt/tableMixer[id].listExterne[i].tableParam[j].max<< std::endl;
				}
			}
		}
	}
}
void oscSendGain(string cm, float dt){
	lo::Address a("localhost", "5510");
	a.send(cm, "f",dt);
	std::cout <<" commande "<<cm<<" f "<<dt<< std::endl;
}
float distance(float x1, float y1, float x2, float y2){
	float nx=x1-x2;
	float ny=y1-y2;
	float dt=sqrt((nx*nx)+(ny*ny));
	return dt;
}
//*********************************************************************************************************************
//													Faust factory
//*********************************************************************************************************************
bool readFileDSP(string st1, string pathf, string filef){
  SF_INFO sfinfo;
  //string sFileName=tableMixer[mixerActif].getPath()+"/"+tableMixer[mixerActif].getFile();
  string sFileName=pathf+"/"+filef;
  const char *path=sFileName.c_str();
  cout <<sFileName <<endl;
  SNDFILE* sndfile=sf_open(path, SFM_READ, &sfinfo) ;
  sf_close(sndfile) ;
  cout <<path <<" canaux : " << sfinfo.channels<<" Sample rate = "<< sfinfo.samplerate<<" frames : "<<sfinfo.frames<< endl;
  string nameFile="simplePlayer.dsp";
  if(sfinfo.channels>1){
  string prog;
  prog=prog+"import(\"stdfaust.lib\");";
  prog=prog+"import(\""+defInstall+"/faust/soundfiles.lib\");\n";
  prog=prog+"ds=soundfile(\"[url:{\'"+sFileName+"\'}]\","+to_string(sfinfo.channels)+");";
  prog=prog+"vmeter(x)= attach(x, envelop(x) : vbargraph(\"[2][unit:dB]\", -70, +5));";
  prog=prog+"envelop = abs : max ~ -(1.0/ma.SR) : max(ba.db2linear(-70)) : ba.linear2db;";
  prog=prog+"sample1 = so.sound(ds, 0);";
  prog=prog+"gain = vslider(\"[0]gain\",0.1,0,2,0.01) : si.smoo;";
  prog=prog+"gate = button(\"[1]gate\");";
  prog=prog+"gdec(x) = hgroup(\"Player\",x);";
  prog=prog+"tdec(x) = gdec(vgroup(\"[1]\",x));";
  prog=prog+"lect(x) = gdec(hgroup(\"[2]\",x));";
  prog=prog+"lgain=tdec(gain);";
  prog=prog+"lgate=tdec(gate);";
  prog=prog+"lmet=lect(par(j,"+to_string(sfinfo.channels)+",hgroup(\"c%2j\",vmeter)));";
  prog=prog+"smp1 = display_progress(sample1.play_progress(lgain,lgate)):lmet;";
  prog=prog+"process = smp1;";
  cout <<sFileName <<" prog : " << prog<< endl;
  saveDsp(prog, "-osc");
  //createDSP(prog,"");
  }
}
/*
void createDSP(std::string localDsp, std::string st1){
  char name[256];
  char nameAudio[256];
  char rcfilename[256];
  char* home = getenv("HOME");
  string s = "metaFxPlayer.dsp";
  char filename[s.length() + 1];
  strcpy(filename, s.c_str());
  snprintf(name, 255, "%s","metaFxplayer");
  snprintf(rcfilename, 255, "%s/.%s-rc", home, name);
  std::cout << "name : " << name<<"filename "<<filename<<"rcfilename "<<rcfilename<< std::endl;
  
  bool is_osc;
  if(st1=="-osc"){
    is_osc = 1;
  }else{
  	 is_osc = 0;
  } 
  dsp_factory* factory = nullptr;
  dsp* DSP = nullptr;
  MidiUI* midiinterface = nullptr;
  jackaudio_midi audio;
  GUI* oscinterface = nullptr;
  string error_msg;
    
  cout << "Libfaust version : " << getCLibFaustVersion () << endl;
  factory = createDSPFactoryFromString(filename,localDsp, 0,NULL, "", error_msg, -1);
  if (!factory) {
     cerr << "Cannot create factory : " << error_msg;
     exit(EXIT_FAILURE);
  }
  std::cout << "Factory : " << factory<< std::endl;
  cout << "getCompileOptions " << factory->getCompileOptions() << endl;
  
  DSP = factory->createDSPInstance();
  if (!DSP) {
     cerr << "Cannot create instance "<< endl;
     exit(EXIT_FAILURE);
  }
  GUI* interface = new GTKUI(filename,0,NULL);
  DSP->buildUserInterface(interface);
  FUI* finterface = new FUI();
  DSP->buildUserInterface(finterface);
  SoundUI* soundinterface = new SoundUI();
  DSP->buildUserInterface(soundinterface);
  
  if (is_osc) {
  	 int argc1=5;
  	 char* argv1[64];
  	 argv1[0]=filename;
  	 argv1[1]=(char*)"-xmit";
  	 argv1[2]=(char*)"0";
  	 argv1[3]=(char*)"-port";
  	 string s=baseOSC;
  	 argv1[4]=(char*)s.c_str();//"5540"
  	 cout << "osc " << argv1[4] << endl;
    oscinterface = new OSCUI(filename,argc1,argv1);
    DSP->buildUserInterface(oscinterface);
  }

  if (!audio.init(basename(filename), DSP)) {
     cout << "audio.init : " << 0<< endl;
  }
  finterface->recallState(rcfilename);
  audio.start();
  if (is_osc) {
  		oscinterface->run();
  }
  interface->run();
  appRun.setTexture(actRun);
  appFlag=0;
  audio.stop();
  finterface->saveState(rcfilename);
  delete DSP;
  delete interface;
  delete finterface;
  delete oscinterface;
  delete soundinterface;
  
  deleteDSPFactory(static_cast<llvm_dsp_factory*>(factory));
}
*/
void defStringDSP(){
	int nbcanaux=1;
	int nbsorties=1;
	int nbCanauxMax=1;
	string s;
	string prog;
	string nfx;
	string nameFile="metaFxPlayer.dsp";
	string path=getcwd(NULL,0);
	
	nbcanaux=nbCanauxMax;
	nbsorties=nbcanaux;
	cout << "nbCanaux " << nbcanaux<< endl;
	nfx="ds=soundfile(\"[url:{";
	for(int i=0;i<tableMixer.size();i++){
	   if(tableMixer[i].getFile()==""){
	   	nfx=nfx+"\'"+defInstall+"/sound/vide.wav\';";
	   }else{
	   	nfx=nfx+"\'"+tableMixer[i].getPath()+"/"+tableMixer[i].getFile()+"\';";
	   }
	}
	
	for(int i=0;i<tablePave.size();i++){
	   if(tablePave[i].file==""){
	   	nfx=nfx+"\'"+defInstall+"/sound/vide.wav\';";
	   }else{
	   	nfx=nfx+"\'"+tablePave[i].path+"/"+tablePave[i].file+"\';";
	   }
	}
	nfx=nfx.substr(0,nfx.length()-1);
	nfx=nfx+"}]\","+to_string(nbcanaux)+");\n";
	
	
	prog=prog+"import(\"stdfaust.lib\");\n";
	prog=prog+"import(\""+defInstall+"/faust/soundfiles.lib\");\n";
	prog=prog+"import(\""+defInstall+"/faust/metaSurfaceFaust.lib\");\n";
	prog=prog+nfx;
	
	prog=prog+"vmeter(i,x)= attach(x, envelop(x) : vbargraph(\"%2i[unit:dB]\", -70, +5));\n";
	prog=prog+"envelop = abs : max ~ -(1.0/ma.SR) : max(ba.db2linear(-70)) : ba.linear2db;\n";
	prog=prog+"sample(x) = so.sound(ds, x);\n";
	prog=prog+"ingain(i) = vslider(\"[0]gain%2i\",0.1,0,2,0.01) : si.smoo;\n";
	prog=prog+"sgain(i) = vslider(\"[0]gain%2i\",0.1,0,2,0.01) : si.smoo;\n";
	prog=prog+"sspeed(i)=vslider(\"[1]speed%2i\",1.0,0,2,0.01) : si.smoo;\n";
	prog=prog+"lect2(x)=hgroup(\"Player\",x);\n";
	
	prog=prog+"tdec(x)=lect2(hgroup(\"[0]Param\",x));\n";
	prog=prog+"lect(x)=lect2(hgroup(\"[1]Meter\",x));\n";
	
	prog=prog+"lgain(i)=tdec(sgain(i));\n";
	prog=prog+"lgainp(i)=tdec(ingain(i));\n";
	prog=prog+"lspeed(i)=tdec(sspeed(i));\n";
	nbsorties=0;																							// on force les sorties à 1
	if(nbsorties==0){
		prog=prog+"lmet=lect(vmeter(1));\n";
		nbcanaux=1;
	}else{
		prog=prog+"lmet=lect(par(j,"+to_string(nbsorties)+",vmeter(j)));\n";
	}
	prog=prog+"base=tgroup(\"Mixer\",";
	int vblock=0;
	string defName;
	std::vector<int>  pile2;
	std::vector<string> pileS;
	std::vector<int> graphString;
	int nbvoix=0;
	int suivant=0;
	int suivant2=0;
	int cas=0;
	//prog=prog+"tgroup(\"Pad";
	for(int i=0;i<tableMixer.size();i++){        // 											 On boucle sur tous les mixer
		if(tableMixer[i].getFile()==""){
			prog=prog+"hgroup(\"M"+to_string(i)+"[0]\",_:*(lgainp("+to_string(i)+")):lmet";  
		}else{
			prog=prog+"hgroup(\"M"+to_string(i)+"[0]\",sample("+to_string(i)+").loop_speed_level(lspeed("+to_string(i)+"),lgain("+to_string(i)+")):>lmet";  
		}
		if(tableMixer[i].getFx()==0){					//												Si le mixer dispose de graphes Fx
			
			inPort=moduleSearchId("Entree",i);    //												On identifie le module d'entrée
			cout << "entree : " <<inPort<<" voix  "<<tableMixer[i].listModules[0].label<<"  :  "<<tableMixer[i].listModules[0].out.size()<< endl;
			string s="";
			int d;
			int nbsuivant;
			nbvoix=tableMixer[i].listModules[0].out.size();
			for(int q=0;q<nbvoix;q++){
				graphString.push_back(tableMixer[i].listModules[0].out[q]);
			}
			cout << "graphString : " <<graphString.size()<< endl;
			int initGraphe=1;
			int sv=0;
			int precedent=0;
			while(sv!=2){
				int k;
				for(k=0;k<nbvoix;k++){															// on utilise la taille de la nouvelle liste de module
					if(tableMixer[i].listModules[graphString[k]].out.size()!=0){
							d=graphString[k];
							cout << "tableMixer[i].listModules[d].out.size() "<<tableMixer[i].listModules[d].out.size()<< endl;
							if(tableMixer[i].listModules[d].out.size()==1){
								if(tableMixer[i].listModules[d].type==0){
									suivant=tableMixer[i].listModules[d].out[0];
									defName=tableFx[tableMixer[i].listModules[d].label].name;
									cout << "name1 "<< endl;
									
									if(d!=precedent){
										if(defName=="passThrough"){
											s="_";
										}else{
											s="hgroup(\"h"+to_string(k)+"\","+defName+")";
										}
										pileS.push_back(s);
										tableMixer[i].listModules[d].path="/fileFx/Mixer/M"+to_string(i)+"/v"+to_string(vblock)+"/h"+to_string(k)+"/";
									}
									precedent=d;
									if(suivant!=1){
										for(int q=0;q<tableMixer[i].listModules[d].out.size();q++){
											pile2.push_back(tableMixer[i].listModules[d].out[q]);
										}	
									}else{
										sv=2;
									}
								}
								
							}else{
								if(tableMixer[i].listModules[d].out.size()>1){
									for(int q=0;q<tableMixer[i].listModules[d].out.size();q++){
										if(tableMixer[i].listModules[d].type==0){
											suivant=tableMixer[i].listModules[d].out[q];
											defName=tableFx[tableMixer[i].listModules[d].label].name;
											cout << "name 2 "<<tableFx[tableMixer[i].listModules[d].label].name<< endl;
											if(defName=="passThrough"){
												s="_";
											}else{
												s="hgroup(\"h"+to_string(k)+"\","+defName+")";
											}
											pileS.push_back(s);
											tableMixer[i].listModules[d].path="/fileFx/Mixer/M"+to_string(i)+"/v"+to_string(vblock)+"/h"+to_string(q)+"/";
											if(suivant!=1){
												if(suivant!=suivant2){
													pile2.push_back(suivant);
												}
												suivant2=suivant;
											}else{
												sv=2;
											}
										}
									}
								}
							}
							
							//tableMixer[i].listModules[k+2].path="/metaFx/Mixer/M"+to_string(i)+"/v"+to_string(vblock)+"/h"+to_string(k)+"/";
							cout << "module path: i"<< i <<" k "<< k+2<< " " <<tableMixer[i].listModules[k+2].path<< endl;
							//cout << "module "<<tableMixer[i].listModules[d].label<< " name "<< defName<< endl;
							//cout << "module path: " <<tableMixer[i].listModules[k].path<< endl;
							//cout << "nb suivant "<<nbsuivant<< endl;
							
							//cout << "suivant: " <<suivant<< " d "<< d<< endl;
					}else{
						cout << "Il y a une erreur sur le graphe. Un module ne peut pas être sans destination"<< endl;
					}
				}
				cout << "pileS.size() "<<pileS.size()<<" graphString.size() "<<graphString.size()<< endl;
				if(pileS.size()==graphString.size()){
					cas=1;
				}
				if(pileS.size()<graphString.size()){
					cas=2;
					float dt=graphString.size()/pileS.size();
					dt=graphString.size()-(dt*pileS.size());
					if(dt!=0){
						cout << "Il y a une erreur sur le graphe. Un bloc a un nombre d'entrées qui n'est pas un multiple du précédent"<< endl;
					}
				}
				if(pileS.size()>graphString.size() || initGraphe==1){
					cas=3;
					float dt=pileS.size()/graphString.size();
					dt=pileS.size()-(dt*graphString.size());
					if(dt!=0){
						cout << "Il y a une erreur sur le graphe. Un bloc a un nombre de sorties qui n'est pas un multiple du précédent"<< endl;
					}
				}
				
				switch(cas){
					case 1:
						cout << "cas1" << endl;
						prog=prog+":>";
						prog=prog+"vgroup(\"v"+to_string(vblock)+"\",";
						for(int q=0;q<pileS.size();q++){
							prog=prog+pileS[q]+",";
						}
						prog=prog.substr(0,prog.length()-1);
						
						prog=prog+")";
						
						for(int q=0;q<pile2.size();q++){
							graphString[q]=pile2[q];
						}
						nbvoix=pileS.size();
						pile2.clear();
						pileS.clear();
						k=0;
						vblock++;
						break;
					case 2:
						cout << "cas2" << endl;
						prog=prog+":>";
						prog=prog+"vgroup(\"v"+to_string(vblock)+"\",";
						for(int q=0;q<pileS.size();q++){
							prog=prog+pileS[q]+",";
						}
						prog=prog.substr(0,prog.length()-1);
						prog=prog+")";
						nbvoix=pileS.size();
						for(int q=0;q<pile2.size();q++){
							graphString[q]=pile2[q];
						}
						pile2.clear();
						pileS.clear();
						//vblock++;
						k=0;
						break;
					case 3:
						cout << "cas3" << endl;
						//prog=prog.substr(0,prog.length()-1);
						prog=prog+"<:";
						
						prog=prog+"vgroup(\"v"+to_string(vblock)+"\",";
						
						for(int q=0;q<pileS.size();q++){
							prog=prog+pileS[q]+",";
						}
						prog=prog.substr(0,prog.length()-1);
						prog=prog+")";
						for(int q=0;q<pile2.size();q++){
							graphString[q]=pile2[q];
						}
						nbvoix=pileS.size();
						pile2.clear();
						pileS.clear();
						k=0;
						initGraphe=0;
						vblock++;
						break;
					default:
       				break;	
				}
				suivant2=0;
				//prog=prog.substr(0,prog.length()-1);
				
			}

				
			prog=prog.substr(0,prog.length()-1);
			prog=prog+")";
			
			prog=prog+":>_";
	  	prog=prog+"),\n";
	  	prog=prog.substr(0,prog.length()-1);
		//prog=prog+",\n";
	}else{
			prog=prog+":>_),";
	}
	prog=prog.substr(0,prog.length()-1);
	prog=prog+",\n";
  	vblock=0;
	defName="";
	pile2.clear();
	pileS.clear();
	graphString.clear();
	nbvoix=0;
	suivant=0;
	suivant2=0;
	cas=0;
	}
	prog=prog.substr(0,prog.length()-1);
	for(int i=0;i<tablePave.size();i++){
		if(tablePave[i].file==""){
			prog=prog+"hgroup(\"P"+to_string(i)+"[0]\",_:*(lgainp("+to_string(i)+")):>lmet:_),"; 
		}else{
			prog=prog+"hgroup(\"P"+to_string(i)+"[0]\",sample("+to_string(i+tableMixer.size())+").loop_speed_level(lspeed("+to_string(i+tableMixer.size())+"),lgain("+to_string(i)+")):>lmet:_),";  
		}
	}
	prog=prog.substr(0,prog.length()-1);
	prog=prog+");\n";
	prog=prog+"smp1=vgroup(\"[0]\",base);\n";
  	prog=prog+"process = smp1;\n";
	cout << "prog : " << prog<< endl;
	
	saveDsp(prog, "-osc");
	/*
	std::thread tplayer(&createDSP,prog,"-osc");
	tplayer.detach();
	*/
}
void saveDsp(string dsp, string param){
	string s=home+"/.config/metaFx";
	char * cstr = new char [s.length()+1];
   std::strcpy (cstr, s.c_str());
   int ncwd=chdir(cstr);
   string wfile=s+"/fileFx.dsp";
   ofstream fichier(wfile, ios::out | ios::trunc);	
	
   if(fichier){
   	fichier<<dsp<< endl;
   }	
   fichier.close();
   string cmd="./dynamic-jack-gtk -llvm -osc "+wfile+" &";
   char* dest = new char[cmd.length() + 1];
   std::copy(cmd.begin(), cmd.end(), dest);
   int id=system(dest);
   /*
   string cmd2="./fileFx &";
   char* dest2 = new char[cmd2.length() + 1];
   std::copy(cmd2.begin(), cmd2.end(), dest2);
   id=system(dest2);
   */
   std::cout << "id " <<id<< std::endl;
}
int moduleSearchId(string s,int id){
	int rt;
	for(int i=0;i<tableMixer[id].listModules.size();i++){
		if(tableMixer[id].listModules[i].label==s){
			rt=i;
			exit;
		}
	}
	return rt;
}
//************************************************************************
//   windows externe
//************************************************************************
void onWinExterneClick(sf::Event e){
	std::cout << "the left button was pressed" << std::endl;
   std::cout << "mouse x: " << e.mouseButton.x << std::endl;
   std::cout << "mouse y: " << e.mouseButton.y << std::endl;
   if(e.mouseButton.x>186 && e.mouseButton.x<206 && e.mouseButton.y>14 && e.mouseButton.y<404){
   	moduleExterneId=e.mouseButton.y/35;
   	winListExterne.create(sf::VideoMode(180,320), labelGreffon);
   	std::cout << "id externe= "<<moduleExterneId << std::endl;
   }
   if(e.mouseButton.x>30 && e.mouseButton.x<50 && e.mouseButton.y>14 && e.mouseButton.y<404){
   	moduleExterneId=e.mouseButton.y/35;
   	if(moduleExterneId<tableMixer[idAjout].listExterne.size()){
   		std::cout << "sup id externe= "<<tableMixer[idAjout].listExterne[moduleExterneId].label << std::endl;
   		tableMixer[idAjout].listExterne.erase(tableMixer[idAjout].listExterne.begin()+moduleExterneId);
   	}
   }
   if(e.mouseButton.x>214 && e.mouseButton.x<244 && e.mouseButton.y>14 && e.mouseButton.y<404){
   	int index=e.mouseButton.y/35;
   	ViewGreffon greffonView(defGui);
		Mixer rtmixer;
		std::cout << "id externe= "<<tableMixer[idAjout].listExterne[index].label << std::endl;
		 rtmixer=greffonView.drawGreffon(tableMixer[idAjout],index,tableExterne);
		 std::cout << "retour newPlug: "<<rtmixer.listExterne[index].label<< " newPlug size: "<<rtmixer.listExterne.size()<< std::endl;
		//rtmixer=greffonView.drawGreffon(tableMixer[idAjout],index,tableFx);
		
		for(int q=0;q<rtmixer.listExterne[index].tableParam.size();q++){
			tableMixer[idAjout].listExterne[index].tableParam[q].etat=rtmixer.listExterne[index].tableParam[q].etat;
 	   	tableMixer[idAjout].listExterne[index].tableParam[q].min=rtmixer.listExterne[index].tableParam[q].min;
 	   	tableMixer[idAjout].listExterne[index].tableParam[q].max=rtmixer.listExterne[index].tableParam[q].max;
 	   	tableMixer[idAjout].listExterne[index].tableParam[q].scale=rtmixer.listExterne[index].tableParam[q].scale;
 	   	tableMixer[idAjout].listExterne[index].tableParam[q].rev= rtmixer.listExterne[index].tableParam[q].rev;
		}
		for(int q=0;q<rtmixer.listExterne[index].tableParam.size();q++){
			std::cout << "module: etat "<<tableMixer[idAjout].listExterne[index].tableParam[q].etat << std::endl;
			std::cout << "module: label "<<tableMixer[idAjout].listExterne[index].tableParam[q].label << std::endl;
	 	   std::cout << "module: min "<<tableMixer[idAjout].listExterne[index].tableParam[q].min << std::endl;
	 	   std::cout << "module: max "<<tableMixer[idAjout].listExterne[index].tableParam[q].max << std::endl;
	 	   std::cout << "module: scale "<<tableMixer[idAjout].listExterne[index].tableParam[q].scale << std::endl;
	 	   std::cout << "module: rev "<<tableMixer[idAjout].listExterne[index].tableParam[q].rev << std::endl;
	 	   std::cout << "module: path "<<tableMixer[idAjout].listExterne[index].tableParam[q].path<< std::endl;
		}
		std::cout << " newExterne size: "<<tableMixer[idAjout].listExterne.size()<< std::endl;
   }
}
void onWinExterneUp(sf::Event e){
}
void onWinExterneMove(sf::Event e){
	std::cout << "mouse x: "<< e.mouseMove.x << std::endl;
   std::cout << "mouse y: "<< e.mouseMove.y << std::endl;
}
void externeAjouter(sf::Event e, int id){
	menuFx1Flag=0;
	idAjout=id;
	winExterne.create(sf::VideoMode(270, 454), "Modules Externes");
}
void externeNouveau(sf::Event e){
	 std::cout << "Nouveau" << std::endl;
	 bool nContraintes=1;
	 NewGreffon newExterne(defInstall,defGui);
	 nContraintes=newExterne.drawGreffon(home);
	 std::cout << "Contraintes" <<nContraintes<< std::endl;
	 if(nContraintes==0){
		 tableExterne.clear();
		 tableLabelExterne.clear();
		 readListExterne();
	 }
}
void externeModif(sf::Event e){
	std::cout << "Modifier" << std::endl;
	winListExterne.create(sf::VideoMode(180,320), labelGreffon);
}
void externeSupprimer(sf::Event e){
	std::cout << "Supprimer" << std::endl;
	winListExterne.create(sf::VideoMode(180,320), labelGreffon);
}
void onListExterneClick(sf::Event e){
	std::cout << "the left button was pressed" << std::endl;
   std::cout << "mouse x: " << e.mouseButton.x << std::endl;
   std::cout << "mouse y: " << e.mouseButton.y << std::endl;
   int index;
   Fx newFx;
   string s;
   bool nContraintes;
   NewGreffon newExterne(defInstall,defGui);
   if(e.mouseButton.x>1 && e.mouseButton.x<158 && e.mouseButton.y>1 && e.mouseButton.y<319){
	   switch (rtIndex){
	   	case 0:
		   	index=e.mouseButton.y/18;
		   	s=tableLabelExterne[index].getString();
		   	std::cout <<" new greffon "<<s<< std::endl;
		   	newFx=tableExterne[s];
		   	winListExterne.close();
		   	tableMixer[idAjout].listExterne.push_back(newFx);
		   	break;
	   	case 1:
	   		index=e.mouseButton.y/18;
	   		std::cout <<" modif greffon "<< index<< std::endl;
	   		nContraintes=1;
	   		winListExterne.close();
	 			nContraintes=newExterne.modifContraintes(index,home);
	 			std::cout << "Contraintes" <<nContraintes<< std::endl;
	   		break;
	   	case 2:
	   		index=e.mouseButton.y/18;
	   		std::cout <<" supprimer greffon "<< index<< std::endl;
	   		winListExterne.close();
	   		newInfo.setTxtPosX(0, 100);
	   		newInfo.setTxtPosY(0, 30);
	   		newInfo.setTxtPosX(1, 100);
	   		newInfo.setTxtPosY(1, 50);
	   		newInfo.setInfoTxtSize(14);
	   		newInfo.setTxt(0,supExterne1);
	   		newInfo.setTxt(1,supExterne2);
		   	bool rt=newInfo.drawInfo(0, "Alerte");
		   	if(rt==1){
		   		std::cout << "rtf "<<rt << std::endl;
	 				nContraintes=newExterne.supContraintes(index,home);
	 				tableExterne.clear();
					tableLabelExterne.clear();
					readListExterne();
	 			}
	   		break;
	   }
	}
}
void onListExterneMove(sf::Event e){
	std::cout << "mouse x: "<< e.mouseMove.x << std::endl;
   std::cout << "mouse y: "<< e.mouseMove.y << std::endl;
   if(e.mouseMove.x>1 && e.mouseMove.x<158 && e.mouseMove.y>1 && e.mouseMove.y<319){
		int vpos=(e.mouseMove.y/18)*18;
		extSelectBox.setPosition(sf::Vector2f(1,vpos));

		//index=((e.mouseMove.y-30)/18)+baseIndex;
	}
}
//************************************************************************
//   windows graph
//************************************************************************

void onWinSecondClick(sf::Event e, int id){
	std::cout << "the left button was pressed" << std::endl;
   std::cout << "mouse x: " << e.mouseButton.x << std::endl;
   std::cout << "mouse y: " << e.mouseButton.y << std::endl;
   idFx=id;
			std::cout << "new win: "<<winSecond[id].getSize().x<< std::endl;
	if(e.mouseButton.x>10 && e.mouseButton.x<56 && e.mouseButton.y<28){
		menuFx1Flag=1;
		menuFx2Flag=0;
	}
	if(e.mouseButton.x>90 && e.mouseButton.x<140 && e.mouseButton.y<28){
		menuFx1Flag=0;
		menuFx2Flag=1;
	}
	if(menuFx1Flag==1 && e.mouseButton.x>94 && e.mouseButton.x<112 && e.mouseButton.y>30 && e.mouseButton.y<232){
		menuFxSliderFlag=1;
		lastPos1=e.mouseButton.y;
	}
	if(e.mouseButton.x>150 && e.mouseButton.x<174 && e.mouseButton.y>4 && e.mouseButton.y<30){
		initGraph(id);
	}
	if(menuFx1Flag==1 && e.mouseButton.x>4 && e.mouseButton.x<94 && e.mouseButton.y>30 && e.mouseButton.y<232){
		fxIndex=((e.mouseButton.y-30)/18)+baseFxIndex;
		std::cout << "index: " << fxIndex << std::endl;
		string s=tableLabelModule[fxIndex+2].getString();
		std::cout << "index name: "<<tableFx[s].name<< std::endl;
		menuFx1Flag=0;
		addModule(e, fxIndex+2, id);
	}
	if(menuFx2Flag==1 && e.mouseButton.x>94 && e.mouseButton.x<184 && e.mouseButton.y>30 && e.mouseButton.y<124){
		fx2Index=((e.mouseButton.y-30)/18)+baseFx2Index;
		std::cout << "index: " << fx2Index << std::endl;
		//string s=tableListExterne[fx2Index+2].getString();
		menuFx2Flag=0;
		switch (fx2Index){
			case 0:
				rtIndex=0;
				externeAjouter(e, id);
				break;
			case 1:
				break;
			case 2:
				externeNouveau(e);
				break;
			case 3:
				rtIndex=1;
				externeModif(e);
				break;
			case 4:
				rtIndex=2;
				externeSupprimer(e);
				break;
		}
	}
	if(e.mouseButton.x>0 && e.mouseButton.x<winSecond[id].getSize().x && e.mouseButton.y>30 && e.mouseButton.y<winSecond[id].getSize().y){
		for(int i=0;i<tableMixer[id].listModules.size();i++){
			if(e.mouseButton.x>tableMixer[id].listModules[i].x+4 && e.mouseButton.x<tableMixer[id].listModules[i].x+144 && e.mouseButton.y>tableMixer[id].listModules[i].y && e.mouseButton.y<tableMixer[id].listModules[i].y+60){
				moduleActif[id]=i;
				cout << "module label click: "<<tableMixer[id].listModules[i].label<< endl;
				if(e.mouseButton.button == sf::Mouse::Right && i!=0 && i!=1 && tableMixer[id].listModules[i].type==0){
					ViewContraintes contraintesView(defGui);
					Module rtmodule;
					rtmodule=contraintesView.drawContraintesFx(tableMixer[id].listModules[i],tableFx);
					for(int q=0;q<rtmodule.tableContraintes.size();q++){
						tableMixer[id].listModules[i].tableContraintes[q].etat=rtmodule.tableContraintes[q].etat;
			 	   	tableMixer[id].listModules[i].tableContraintes[q].min=rtmodule.tableContraintes[q].min;
			 	   	tableMixer[id].listModules[i].tableContraintes[q].max=rtmodule.tableContraintes[q].max;
			 	   	tableMixer[id].listModules[i].tableContraintes[q].scale=rtmodule.tableContraintes[q].scale;
			 	   	tableMixer[id].listModules[i].tableContraintes[q].rev= rtmodule.tableContraintes[q].rev;
					}
					for(int q=0;q<rtmodule.tableContraintes.size();q++){
						std::cout << "module: etat "<<tableMixer[id].listModules[i].tableContraintes[q].etat << std::endl;
						std::cout << "module: label "<<tableMixer[id].listModules[i].tableContraintes[q].label << std::endl;
				 	   std::cout << "module: min "<<tableMixer[id].listModules[i].tableContraintes[q].min << std::endl;
				 	   std::cout << "module: max "<<tableMixer[id].listModules[i].tableContraintes[q].max << std::endl;
				 	   std::cout << "module: scale "<<tableMixer[id].listModules[i].tableContraintes[q].scale << std::endl;
				 	   std::cout << "module: rev "<<tableMixer[id].listModules[i].tableContraintes[q].rev << std::endl;
				 	   std::cout << "module: path "<<tableMixer[id].listModules[i].tableContraintes[q].path<< std::endl;
					}
				}
			}
			
		}
		
		std::cout << "module Actif: "<<moduleActif[id]<< std::endl;
		
	}
	if(e.mouseButton.x>0 && e.mouseButton.x<winSecond[id].getSize().x && e.mouseButton.y>30 && e.mouseButton.y<winSecond[id].getSize().y){
		for(int i=0;i<tableMixer[id].listModules.size();i++){
			if(e.mouseButton.x>tableMixer[id].listModules[i].x+144 && e.mouseButton.x<tableMixer[id].listModules[i].x+154 && e.mouseButton.y>tableMixer[id].listModules[i].y+34 && e.mouseButton.y<tableMixer[id].listModules[i].y+44){
				cubicFlag=1;
				defConnect.defConnector(i,65535,0,tableMixer[id].listModules[i].x+148, tableMixer[id].listModules[i].y+38, e.mouseButton.x+1, e.mouseButton.x+1, e.mouseButton.x, e.mouseButton.y, e.mouseButton.x+1, (e.mouseButton.y+1));
				std::cout << "cubic start x: " << e.mouseButton.x<< " y : "<< e.mouseButton.y << std::endl;
			}
			if(i!=0 && i!= 1 && e.mouseButton.x>tableMixer[id].listModules[i].x+130 && e.mouseButton.x<tableMixer[id].listModules[i].x+150 && e.mouseButton.y>tableMixer[id].listModules[i].y && e.mouseButton.y<tableMixer[id].listModules[i].y+20){
				moduleDelete(id,i);
			}
		}
		for(int i=0;i<tableMixer[id].tableConnectors.size();i++){
			if(e.mouseButton.x>tableMixer[id].tableConnectors[i].startX &&  e.mouseButton.x<tableMixer[id].tableConnectors[i].endX && e.mouseButton.y>tableMixer[id].tableConnectors[i].startY &&  e.mouseButton.y<tableMixer[id].tableConnectors[i].endY){	
				connectorDel(id, i);
			}
			if(e.mouseButton.x>tableMixer[id].tableConnectors[i].startX &&  e.mouseButton.x<tableMixer[id].tableConnectors[i].endX && e.mouseButton.y<tableMixer[id].tableConnectors[i].startY &&  e.mouseButton.y>tableMixer[id].tableConnectors[i].endY){	
				connectorDel(id, i);
			}
		}
	}
	
}
void onWinSecondUp(sf::Event e,int id){
	if(cubicFlag==1){
		cubicFlag=0;
		for(int i=0;i<tableMixer[id].listModules.size();i++){
			if(e.mouseButton.x>tableMixer[id].listModules[i].x-4 && e.mouseButton.x<tableMixer[id].listModules[i].x+6 && e.mouseButton.y>tableMixer[id].listModules[i].y+34 && e.mouseButton.y<tableMixer[id].listModules[i].y+44){
				defConnect.sortie=i;
				defConnect.setConnectEndX(tableMixer[id].listModules[i].x+2);
				defConnect.setConnectEndY(tableMixer[id].listModules[i].y+38);
				tableMixer[id].listModules[i].in.push_back(defConnect.entree);
				tableMixer[id].listModules[defConnect.entree].out.push_back(defConnect.sortie);
				tableMixer[id].tableConnectors.push_back(defConnect);
			}
		}
		points.clear();
		vertices[id].clear();
		
	}
	menuFxSliderFlag=0;
	moduleActif[id]=65535;
}
void onWinSecondMove(sf::Event e, int id){
	std::cout << "the left button was pressed" << std::endl;
   std::cout << "mouse x: "<<id<<"   " << e.mouseMove.x << std::endl;
   std::cout << "mouse y: " <<id<<"    "<< e.mouseMove.y << std::endl;
   idFx=id;
   if(menuFx1Flag==1 && e.mouseMove.x>112 || e.mouseMove.y>232){
		menuFx1Flag=0;
	}
	if(menuFx2Flag==1 && e.mouseMove.x<90 || e.mouseMove.x>204 || e.mouseMove.y>124){
		menuFx2Flag=0;
	}
	if(e.mouseMove.x>154 && e.mouseMove.x<172 && e.mouseMove.y<30){
			flagToolTip2=1;
			toolTip.setString(sToolTip[9]);
			float wx=(204-toolTip.getLocalBounds().width)/2;
			toolTip.setPosition(sf::Vector2f(wx,32));
			winToolTip.setSize(sf::Vector2f(200,20));
			winToolTip.setPosition(sf::Vector2f(10,30));
	}
	if(e.mouseMove.y>30){
		flagToolTip2=0;
	}
	if(menuFx1Flag==1 && e.mouseMove.x>4 && e.mouseMove.x<94 && e.mouseMove.y>30 && e.mouseMove.y<232){
		int vpos=(((e.mouseMove.y-30)/18)*18)+30;
		menuFxSelectBox.setPosition(sf::Vector2f(1,vpos));
		//index=((e.mouseMove.y-30)/18)+baseIndex;
		std::cout << "index: " << fxIndex << std::endl;
	}
	if(menuFx2Flag==1 && e.mouseMove.x>94 && e.mouseMove.x<184 && e.mouseMove.y>30 && e.mouseMove.y<124){
		int vpos=(((e.mouseMove.y-30)/18)*18)+30;
		menuFx2SelectBox.setPosition(sf::Vector2f(91,vpos));
		//index=((e.mouseMove.y-30)/18)+baseIndex;
	}
	if(menuFxSliderFlag==1 && e.mouseMove.x>menuFxSlider.getPosition().x && e.mouseMove.x<menuFxSlider.getPosition().x+14 && e.mouseMove.y>menuFxSlider.getPosition().y && e.mouseMove.y<menuFxSlider.getPosition().y+menuFxSlider.getSize().y){
     int nposy=menuFxSlider.getPosition().y+(e.mouseMove.y-lastPos1);
     if(nposy<menuFxBkgSlider.getPosition().y+2){
   		nposy=menuFxBkgSlider.getPosition().y+2;
     }
     if(nposy+menuFxSlider.getSize().y>30+menuFxBkgSlider.getSize().y){
   		nposy=menuFxBkgSlider.getSize().y-menuFxSlider.getSize().y+30;
     }
     menuFxSlider.setPosition(menuFxSlider.getPosition().x,nposy);
     baseFxIndex=(int)nposy-30;
     lastPos1=e.mouseMove.y;  
     std::cout << "base index: " << baseFxIndex << std::endl;
   }
   if(moduleActif[id]!=65535 &&e.mouseMove.x>0 && e.mouseMove.x<winSecond[id].getSize().x && e.mouseMove.y>30 && e.mouseMove.y<winSecond[id].getSize().y){
		tableMixer[id].listModules[moduleActif[id]].x=e.mouseMove.x-75;
		tableMixer[id].listModules[moduleActif[id]].y=e.mouseMove.y-30;
		
		for(int i=0;i<tableMixer[id].tableConnectors.size();i++){
			if(tableMixer[id].tableConnectors[i].entree==moduleActif[id]){
   			tableMixer[id].tableConnectors[i].startX=tableMixer[id].listModules[moduleActif[id]].x+146;
				tableMixer[id].tableConnectors[i].startY=tableMixer[id].listModules[moduleActif[id]].y+36;
   		}
   		if(tableMixer[id].tableConnectors[i].sortie==moduleActif[id]){
   			tableMixer[id].tableConnectors[i].endX=tableMixer[id].listModules[moduleActif[id]].x;
				tableMixer[id].tableConnectors[i].endY=tableMixer[id].listModules[moduleActif[id]].y+36;
   		}
  	  	 }	 
	}
	if(cubicFlag==1 &&e.mouseMove.x>0 && e.mouseMove.x<winSecond[id].getSize().x && e.mouseMove.y>30 && e.mouseMove.y<winSecond[id].getSize().y){
		defConnect.setConnectEndX(e.mouseMove.x);
		defConnect.setConnectEndY(e.mouseMove.y);
		viewConnector(id);
	}
}
void onWinSecondResized(sf::Event e, int id){
	std::cout << "new width: " << e.size.width << std::endl;
    std::cout << "new height: " << e.size.height << std::endl;
	sf::FloatRect area(0, 0,  e.size.width , e.size.height);	//fenêtre
   winSecond[id].setView(sf::View(area));
   menuBar.setSize(sf::Vector2f( e.size.width ,menuBarHeight));
}
void initGraph(int id){
	tableMixer[id].listModules.clear();
   tableMixer[id].tableConnectors.clear();
   addEntree(id);
   addSortie(id);
}
void addEntree(int id){
	string s=tableLabelModule[0].getString();
	Module nModule;
	nModule.initModule(s, 0, 10, 200, 0, 36, 146, 36);
	tableMixer[id].listModules.push_back(nModule);
}
void addSortie(int id){
	string s=tableLabelModule[1].getString();
	Module nModule;
	nModule.initModule(s,0, 800, 200, 0, 36, 146, 36);
	tableMixer[id].listModules.push_back(nModule);
}
void addModule(sf::Event e, int ind,int id){
	string s=tableLabelModule[ind].getString();
	Module nModule;
	Contrainte pr;
	nModule.initModule(s, 0, e.mouseButton.x, e.mouseButton.y, 0, 36, 146, 36);
	tableMixer[id].listModules.push_back(nModule);
	std::cout << "add module: " << tableMixer[id].listModules.back().label << std::endl;
	std::vector<Contrainte> plugin=tableFx[s].tableParam;
	std::cout << "param: " << plugin.size() << std::endl;
	for(int i=0;i<plugin.size();i++){
		std::cout << "param: etat "<<tableFx[s].tableParam[i].etat << std::endl;
		std::cout << "param: label "<<tableFx[s].tableParam[i].label << std::endl;
 	   std::cout << "param: min "<<tableFx[s].tableParam[i].min << std::endl;
 	   std::cout << "param: max "<<tableFx[s].tableParam[i].max << std::endl;
 	   std::cout << "param: scale "<<tableFx[s].tableParam[i].scale << std::endl;
 	   std::cout << "param: rev "<<tableFx[s].tableParam[i].rev << std::endl;
 	   std::cout << "param: path "<<tableFx[s].tableParam[i].path<< std::endl;
	}
	
	for(int i=0;i<plugin.size();i++){
		tableMixer[id].listModules.back().tableContraintes.push_back(pr);
		tableMixer[id].listModules.back().tableContraintes.back().etat=tableFx[s].tableParam[i].etat;
		tableMixer[id].listModules.back().tableContraintes.back().label=tableFx[s].tableParam[i].label;
 	   tableMixer[id].listModules.back().tableContraintes.back().min=tableFx[s].tableParam[i].min;
 	   tableMixer[id].listModules.back().tableContraintes.back().max=tableFx[s].tableParam[i].max;
 	   tableMixer[id].listModules.back().tableContraintes.back().scale=tableFx[s].tableParam[i].scale;
 	   tableMixer[id].listModules.back().tableContraintes.back().rev=tableFx[s].tableParam[i].rev;
 	   tableMixer[id].listModules.back().tableContraintes.back().path=tableFx[s].tableParam[i].path;
 	   std::cout << "param2: label "<<tableMixer[id].listModules.back().tableContraintes[i].label << std::endl;
	}

}
void viewModule(int i, int id){

	boxModule.setSize(sf::Vector2f(150,60));
	boxModule.setFillColor(sf::Color(203,213,217));
	boxModule.setOutlineThickness(1);
	boxModule.setOutlineColor(sf::Color(0,0,0));
	int px=tableMixer[id].listModules[i].x;
	int py=tableMixer[id].listModules[i].y;
	boxModule.setPosition(sf::Vector2f(px,py));
	TitleBoxModule.setSize(sf::Vector2f(boxModule.getSize().x,20));
	TitleBoxModule.setPosition(sf::Vector2f(px,py));
	TitleBoxModule.setFillColor(sf::Color(138,146,149));
	titleModule.setFont(font);
	titleModule.setCharacterSize(12);
	titleModule.setFillColor(sf::Color(0,0,0));
	titleModule.setPosition(sf::Vector2f(px+10,py+2));
	titleModule.setRotation(0.f);
	titleModule.setString(tableMixer[id].listModules[i].label);
	if(tableMixer[id].listModules[i].label!="Entree"){
		boxConnectorModule.setSize(sf::Vector2f(4,4));
		boxConnectorModule.setFillColor(sf::Color(255,0,0));
		boxConnectorModule.setOutlineThickness(1);
		boxConnectorModule.setOutlineColor(sf::Color(0,0,0));
		boxConnectorModule.setPosition(sf::Vector2f(tableMixer[id].listModules[i].inX+boxModule.getPosition().x,tableMixer[id].listModules[i].inY+boxModule.getPosition().y));
	}	
	winSecond[id].draw(boxModule);
	winSecond[id].draw(TitleBoxModule);
	winSecond[id].draw(titleModule);
	if(tableMixer[id].listModules[i].label!="Entree" && tableMixer[id].listModules[i].label!="Sortie"){
		winCloser.setPosition(sf::Vector2f(px+135,py+4));
		winSecond[id].draw(winCloser);
	}
	winSecond[id].draw(boxConnectorModule);
	if(tableMixer[id].listModules[i].label!="Sortie"){
		boxConnectorModule.setFillColor(sf::Color(0,255,0));
		boxConnectorModule.setPosition(sf::Vector2f(tableMixer[id].listModules[i].outX+boxModule.getPosition().x,tableMixer[id].listModules[i].outY+boxModule.getPosition().y));
		winSecond[id].draw(boxConnectorModule);
	}
}

void viewConnector(int id){
points.clear();
vertices[id].clear();
	points =
    defConnect.CalcCubicBezier(
        sf::Vector2f(defConnect.startX,defConnect.startY),
        sf::Vector2f(defConnect.endX, defConnect.endY),
        sf::Vector2f(defConnect.startX+60, defConnect.startY),
        sf::Vector2f(defConnect.endX-60, defConnect.endY),
        80);
	
// Append the points as vertices to the vertex array
	for (std::vector<sf::Vector2f>::const_iterator a = points.begin(); a != points.end(); ++a){
    vertices[id].append(sf::Vertex(*a, sf::Color::White));
	}

}
void drawConnector(Connector &c, int id){
points1.clear();
vertices1.clear();
	points1 =
    c.CalcCubicBezier(
        sf::Vector2f(c.startX,c.startY),
        sf::Vector2f(c.endX,c.endY),
        sf::Vector2f(c.startX+60, c.startY),
        sf::Vector2f(c.endX-60, c.endY),
        80);
	
// Append the points as vertices to the vertex array
	for (std::vector<sf::Vector2f>::const_iterator a = points1.begin(); a != points1.end(); ++a){
    vertices1.append(sf::Vertex(*a, sf::Color::White));
	}
}
void moduleDelete(int id, int dest){
	int rt;
	int ct;
	if(tableMixer[id].listModules[dest].label!="Entree" && tableMixer[id].listModules[dest].label!="Sortie"){
		for(int i=0;i<tableMixer[id].tableConnectors.size();i++){
			ct=tableMixer[id].tableConnectors[i].entree;
			if(ct==dest){
				connectorDel(id, i);
			}
		}
		for(int i=0;i<tableMixer[id].tableConnectors.size();i++){
			ct=tableMixer[id].tableConnectors[i].sortie;
			if(ct==dest){	
				connectorDel(id, i);
			}
		}
		tableMixer[id].listModules.erase(tableMixer[id].listModules.begin()+dest);
	}
	
}
void connectorDel(int id, int dest){
	int orig=tableMixer[id].tableConnectors[dest].entree;
	int suiv=tableMixer[id].tableConnectors[dest].sortie;
	std::cout <<"orig "<< orig<< " suiv " << suiv << std::endl;
	for(int q=0;q<tableMixer[id].listModules[orig].out.size();q++){
		if(tableMixer[id].listModules[orig].out[q]==suiv){
			tableMixer[id].listModules[orig].out.erase(tableMixer[id].listModules[orig].out.begin()+q);
			std::cout << "list out " << tableMixer[id].listModules[orig].out[q] << std::endl;
		}
		std::cout <<"dest "<< tableMixer[id].listModules[orig].label<< " list out " << tableMixer[id].listModules[tableMixer[id].listModules[orig].out[q]].label << std::endl;
	}
	for(int q=0;q<tableMixer[id].listModules[suiv].in.size();q++){
		if(tableMixer[id].listModules[suiv].in[q]==orig){
			tableMixer[id].listModules[suiv].in.erase(tableMixer[id].listModules[suiv].in.begin()+q);
		}
	}
	tableMixer[id].tableConnectors.erase(tableMixer[id].tableConnectors.begin()+dest);
}


