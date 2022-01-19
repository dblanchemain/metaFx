/* MetaFx  .
 *
 * this file is part of the MetaFxe application
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

#include "connector.h"

using namespace std;

Connector::Connector(){
 
}

Connector::~Connector(){
}

void Connector::defConnector(int startS, int endS,int t,int sX, int sY, int eX, int eY, int cSX, int cSY, int cEX, int cEY){
	entree=startS;
	sortie=endS;
	startX=sX;
	startY=sY;
	endX=eX;
	endY=eY;
	ctStartX=cSX;
	ctStartY=cSY;
	ctEndX=cEX;
	ctEndY=cEY;
}
std::vector<sf::Vector2f> Connector::CalcCubicBezier(
        const sf::Vector2f &start,
        const sf::Vector2f &end,
        const sf::Vector2f &startControl,
        const sf::Vector2f &endControl,
        const size_t numSegments)
{
    std::vector<sf::Vector2f> ret;
    if (!numSegments) // Any points at all?
        return ret;

    ret.push_back(start); // First point is fixed
    float p = 1.f / numSegments;
    float q = p;
    for (size_t i = 1; i < numSegments; i++, p += q) // Generate all between
        ret.push_back(p * p * p * (end + 3.f * (startControl - endControl) - start) +
                      3.f * p * p * (start - 2.f * startControl + endControl) +
                      3.f * p * (startControl - start) + start);
    ret.push_back(end); // Last point is fixed
    return ret;
}
int Connector::calculBezier(int x1, int x2){
	return x1 - (x1 - x2) / 2;
}
void Connector::setConnectEndX(int eX){
	endX=eX;
}
void Connector::setConnectEndY(int eY){
	endY=eY;
}
