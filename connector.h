/* MetaFx  .
 *
 * this file is part of the MetaFx application
 *
 * Copyright 2020-2022 dominique Blanchemain
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
#ifndef DEF_CONNECTOR
#define DEF_CONNECTOR

#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <string>


class Connector{

 public :
  
  Connector();
  ~Connector();
  
	int entree=65535;
	int sortie=65535;
	int type;
	int startX;
	int startY;
	int endX;
	int endY;
	int ctStartX;
	int ctStartY;
	int ctEndX;
	int ctEndY;
   
  
  std::vector<sf::Vector2f> CalcCubicBezier(const sf::Vector2f &start,
        const sf::Vector2f &end,
        const sf::Vector2f &startControl,
        const sf::Vector2f &endControl,
        const size_t numSegments);
  void defConnector(int startS,int endS,int t,int sX, int sY, int eX, int eY, int cSX, int cSY, int cEX, int cEY);
  void setConnectEndX(int eX);
  void setConnectEndY(int eY);
  int calculBezier(int x1, int x2);
  
 private :
  
   

};
#endif
