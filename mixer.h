/* metaFx - .
 *
 * This is the main function for the MetaSurface.
 *
 * Copyright 2020-2021 dominique Blanchemain
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
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include "graph.h"
#include "externe.h"
#include "fx.h"

#ifndef DEF_MIXER
#define DEF_MIXER

class Mixer{

 public :
  
  Mixer();
 
  ~Mixer();
  
  std::vector<Fx> listExterne; 
  std::vector<Module> listModules;
  std::vector<Connector> tableConnectors; 
  
    
  void setId(int nid);
  void setGroupe(int grp);
  void setPosX(int nx);
  void setPosY(int ny);
  void setDaw(bool Daw);
  void setFx(bool Fx);
  void setGain(bool Gain);
  void setSpeed(bool Speed);
  void setTrack(int Tk);
  void setGainMin(float gm);
  void setGainMax(float gm);
  void setPath(std::string Path);
  void setFile(std::string File);
  void setSpeedMin(int nc);
  void setSpeedMax(float dt);
  void setRevSpeed(bool RevS);
  void setRelatif(bool Rel);
  void setRevGain(bool RevG);
  
  int getId();
  int getGroupe();
  int getPosX();
  int getPosY();
  bool getDaw();
  bool getFx();
  bool getGain();
  bool getSpeed();
  int getTrack();
  float getGainMin();
  float getGainMax();
  std::string getPath();
  std::string getFile();
  float getSpeedMin();
  float getSpeedMax();
  bool getRevSpeed();
  bool getRelatif();
  bool getRevGain();
  
 private :
  
	int mixerId;
	int groupe;
	int x;
	int y;
	bool daw;
	bool fx;
	bool gain; 
	bool revGain; 
	bool relatif;
	bool speed;
	bool revSpeed;
	int track;
	float gainMin;
	float gainMax;
	float speedMin;
	float speedMax;
	std::string path;
	std::string file;
	
};
#endif