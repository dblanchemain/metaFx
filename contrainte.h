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
#ifndef DEF_CONTRAINTE
#define DEF_CONTRAINTE

#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <string>


class Contrainte{

 public :
  
  Contrainte();
  ~Contrainte();
  
	bool etat;
	std::string label;
	int index;
	float min;
	float max;
	float scale;
	bool rev;
	std::string path;
 

 private :
  
   

};
#endif
