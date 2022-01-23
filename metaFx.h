/* metaFx - .
 *
 * This is the main function for the MetaFx.
 *
 * Copyright 2022-2023 dominique Blanchemain
 *
 *
 * metaFx is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.
 *
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

* You should have received a copy of the GNU General Public License along with this program;
* If not, see http://www.gnu.org/licenses.
*/


#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

#ifndef DEF_METAFX
#define DEF_METAFX
#endif
#include "mixer.h"
#include "selectFile.h"
#include "fx.h"
#include "module.h"
#include "graph.h"
#include "connector.h"
#include "pave.h"
#include "apropos.h"

sf::Font font;
sf::RenderWindow winPrincipale;	
sf::RenderWindow winSelectGrp;
sf::RenderWindow winPave;
sf::RenderWindow winBkg;
sf::RenderWindow winSelectBkg;
sf::RenderWindow winExterne;
sf::RenderWindow winListExterne;
sf::RenderWindow winPref;

//WindowsFx winFx;
Graph listGraphs[64];
std::map<std::string, Fx> tableFx;
std::map<std::string, Fx> tableExterne;
std::vector<sf::Text> tableLabelModule;
std::vector<sf::Text> tableLabelExterne;

Info newInfo;
std::vector<Pave> tablePave;
std::vector<sf::Texture> tableTexturePv;
sf::Sprite paveSprite;
int pvActif;
int pvSelect;

sf::RenderWindow winSecond[64];
sf::Event eventSecond[64];
std::string nameSecond[64];
std::string defBaseLang;
sf::Text listMenu2[4];

//************************************************************************
//                           paramètres de l'application
//************************************************************************	
std::string defGui;
std::string defInstall;
std::string home;
std::string pref;	
std::string prefLang;
std::string prefTheme;
int prefDAW;
std::string prefServeur;
sf::Text defServeur;
int prefPortD;
sf::Text defPort;
selectFile fileSelector; 
std::string baseOSC;

std::string sessionPath;
std::string sessionFile;

std::string nameSurface;
sf::Text menuFichier;
sf::Text menuModule;

sf::CircleShape selectMixer;

sf::Texture groupe[32];
int defautGroupe;
int baseIdCompte;
Mixer newMixer;
int mixerActif;
int mixerSelect;
std::vector<Mixer> tableMixer;
sf::Sprite mixerSprite;

Apropos newApropos;
//************************************************************************
//                           paramètres Interface surface
//************************************************************************	
int windowAppWidth;
int windowAppHeight;
int workSpaceWidth;
int workSpaceHeight;
int typeMove;
int oldMoveX;
int oldMoveY;
sf::Color AppBackGround;
sf::Color AppBackGroundMenuBar;
sf::RectangleShape menuBar;
int menuBarHeight;
int paveChampIndex;
std::string apptxt;
bool flagTxt;
bool appFlag;

sf::RectangleShape winPaveCurseur;
sf::RectangleShape winToolTip;
bool flagPaveCurseur;
sf::Clock clockCurseur;

sf::RectangleShape menu1;
sf::RectangleShape menu2;
sf::RectangleShape menuSelectBox;
sf::Color menu1BkgColor;

bool menu1Flag;
sf::RectangleShape barOutils;
int barOutilsHeight;
sf::Color barOutilsBackGround;
sf::Texture barOutilsDef1;
sf::Texture barOutilsPave;
sf::Texture barOutilsDef2;
sf::Texture barOutilsDef3;
sf::Texture barOutilsDef4;
sf::Texture barOutilsMv1;
sf::Texture barOutilsMv2;
sf::Texture barOutilsMv3;
sf::Texture selectC;
sf::Texture selectMx;
sf::Texture selectMx2;
sf::Texture selectMx3;
sf::Texture selectMx4;
sf::Texture bNoSelect;
sf::Texture bSelect;
sf::Texture actRun;
sf::Texture actStop;
sf::Texture ldCloser;
sf::Texture npv;
sf::Texture nTextureInit;
sf::Sprite mixerIntNew;
sf::Sprite selectPave;
sf::Sprite selectPave2;
sf::Sprite barTrash1;
sf::Sprite barTrash2;
sf::Sprite barTrash3;
sf::Sprite barMv1;
sf::Sprite barMv2;
sf::Sprite barMv3;
sf::Sprite selectGrp;
sf::Sprite defGroupe;
sf::Sprite bkgWinMixer;
sf::Sprite bkgWinPave;
sf::Sprite bkgWinExterne;
sf::Sprite fxSelect;
sf::Sprite defSelectGain;
sf::Sprite defSpeed;
sf::Sprite defSelectDaw;
sf::Sprite appRun;
sf::Sprite winCloser;
sf::Sprite selectBkg;
sf::Sprite defTrouNoir;
sf::Sprite reGraph;
sf::Sprite defRevGain;
sf::Sprite defRelatif;
sf::Sprite defRevSpeed;
sf::Sprite fondPref;

sf::Text fichier;
sf::Text menuAPropos;
sf::Text menuAide;
sf::Text menuLab[6];
sf::Text labelFichierSon;
sf::Text labelDaw;
sf::Text labelSpeedMax;
sf::Text labelMixerGain;
sf::Text labelRevGain;
sf::Text labelRelatif;
sf::Text labelGainMin;
sf::Text labelGainMax;
sf::Text labelGainMin2;
sf::Text labelGainMax2;
sf::Text labelRayon;
sf::Text labelSpeed;
sf::Text labelSpeedMin;
sf::Text labelRevSpeed;
sf::Text labelPiste;
sf::Text labelFx;
sf::Text toolTip;
sf::Text canalTextF1;
sf::Color genTcolor[32];

sf::Text labelGroupe;
std::string labelGreffon;
sf::Text paramTrack;
sf::Text paramGainMin;
sf::Text paramGainMax;
sf::Text paramGainMin2;
sf::Text paramGainMax2;
sf::Text paramRayon;
sf::Text paramFile;
sf::Text paramSpeedMin;
sf::Text paramSpeedMax;
sf::Text labelImg;
sf::Text labelTrouNoir;
sf::Text labelPlugin;
sf::Text labelToolTip;

std::wstring sToolTip[10];
bool flagToolTip;
bool flagToolTip2;
sf::Text paramFileBkg;
int bkgChampIndex;
bool flagBkgCurseur;
sf::RectangleShape winBkgCurseur;
//************************************************************************
//                           Alerte
//************************************************************************
std::string supExterne1;
std::string supExterne2;
std::string supNouv1;
std::string supNouv2;
std::string supQuit1;
//************************************************************************
//                           Fenêtres Fx
//************************************************************************
sf::Text selectFxName;
bool menuFx1Flag;
bool menuFx2Flag;
sf::Text menuLabelFx;
sf::Text menuLabel2Fx;
sf::Color menuFx1BkgColor;
sf::RectangleShape menuFx1;
sf::RectangleShape menuFx2;
sf::RectangleShape menuFxBkgSlider;
sf::RectangleShape menuFxSlider;
sf::RectangleShape menuFxBkgSliderLineLeft;
sf::RectangleShape menuFxBkgSliderLineBottom;
sf::RectangleShape menuFx2BkgSlider;
sf::RectangleShape menuFx2Slider;
sf::RectangleShape menuFx2BkgSliderLineLeft;
sf::RectangleShape menuFx2BkgSliderLineBottom;
sf::RectangleShape menuFxSelectBox;
sf::RectangleShape menuFx2SelectBox;
sf::RectangleShape extSelectBox;
sf::RectangleShape extBkgSlider;
sf::RectangleShape extSlider;
sf::RectangleShape extBkgSliderLineLeft;
sf::RectangleShape extBkgSliderLineBottom;
sf::RectangleShape menuFx2SelectSep;
sf::CircleShape circleDaw;

bool menuFxSliderFlag;
int lastPos1;
int baseFxIndex;
int baseFx2Index;
int moduleExtId;
int extIndex;
int fxIndex;
int fx2Index;
int vpos;
int idFx;
int idAjout;
int moduleExterneId;

sf::RectangleShape boxModule;
sf::RectangleShape TitleBoxModule;
sf::Text titleModule;
sf::RectangleShape boxConnectorModule;
int moduleActif[64];
bool cubicFlag;
Connector defConnect;
sf::VertexArray vertices1;
std::vector<sf::Vector2f> points1;
sf::VertexArray vertices[64];
std::vector<sf::Vector2f> points;
std::vector<Connector> tableConnectors;
//************************************************************************
//   Faust Factory
//************************************************************************
int inPort;
//************************************************************************
//                           paramètres Macro Objets
//************************************************************************
void readFilePref(char* arg1[]);
void readFileTheme(std::string theme);	
void readFileLang(std::string lang);

void nouveau();
void ouvrir();
void sauvegarder();
void sauvegarderSous();
void preferences();
void quitter();
void saveMetaFx();
void savePref();
//************************************************************************
//													Controleurs
//************************************************************************
void controlGainMixer(sf::Event e,int id);
void controlGainRelatif(sf::Event e,int id);
void controlSpeedMixer(sf::Event e,int id);
void controlParamsMixer(sf::Event e,int id, float dx, float dt);
void controlGainPave(sf::Event e);
void uptdateGreffon(sf::Event e, int id, float dx,float dt2);
void oscSendGain(std::string ms, float dt);
float distance(float x1, float y1, float x2, float y2);
//************************************************************************
//   Interface
//************************************************************************
void onWineClose();
void onEventResized(sf::Event e);
void onClick(sf::Event e);
void onMouseUp(sf::Event e);
void onMouseMove(sf::Event e);
void mouseWheel(sf::Event e);
void onKeyPressed(sf::Event e);
void onTextEvent(sf::Event e);
void drawSurface();
void apropos();

void createMixer();
void createPave();
void readParamMixer();
void deleteMixer();
void deleteGrpMixer();
void deleteAllMixer();

void onClickSelectGrp(sf::Event e);
void onClickSelectBkg(sf::Event e);
void onClickPave(sf::Event e);
void onPaveMove(sf::Event e);
void newPaveText(sf::Event e);
void mixerPaveValid();

void fileDelete();

void readListModules();
void readListExterne();
const std::vector<std::string>  explode(const std::string& s, const char& c);
//************************************************************************
//   Preferences
//************************************************************************
void onPrefClick(sf::Event e);
void onPrefMove(sf::Event e);
void onTextPref(sf::Event e);
//************************************************************************
//   Faust Factory
//************************************************************************
bool readFileDSP(std::string st1, std::string pathf, std::string filef);
void createDSP(std::string localDsp, std::string st1);
void defStringDSP();
int moduleSearchId(std::string s, int id);
void saveDsp(std::string dsp, std::string param);
//************************************************************************
//   windows bkg
//************************************************************************

void onClickBkg(sf::Event e);
void onMoveBkg(sf::Event e);
void newBkgText(sf::Event e);
//************************************************************************
//   windows externe
//************************************************************************
void onWinExterneClick(sf::Event e);
void onWinExterneUp(sf::Event e);
void onWinExterneMove(sf::Event e);
void externeAjouter(sf::Event e,int id);
void externeNouveau(sf::Event e);
void externeModif(sf::Event e);
void externeSupprimer(sf::Event e);
void onListExterneClick(sf::Event e);
void onListExterneMove(sf::Event e);

int rtIndex;
//************************************************************************
//   windows graph
//************************************************************************

void onWinSecondUp(sf::Event e,int id);
void onWinSecondClick(sf::Event e, int id);
void onWinSecondMove(sf::Event e, int id);
void onWinSecondResized(sf::Event e, int id);
void onMouseWheel(sf::Event e);

void addModule(sf::Event e, int ind,int id);
void moduleDelete(int id, int dest);
void connectorDel(int id, int dest);
void addEntree(int id);
void addSortie(int id);
void viewModule(int i, int id);

void viewConnector(int id);
void drawConnector(Connector &c, int i);
void initGraph(int id);
