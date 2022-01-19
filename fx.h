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
#ifndef DEF_FX
#define DEF_FX

#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <string>
#include "contrainte.h"

class Fx{

 public :
  
  Fx();
  ~Fx();
  
	std::string label;
	std::string name;
	int type;
   int input;
   int output;
   int nbpar;
   std::vector<Contrainte> tableParam; 
  
  
  void setFx(std::string lb,std::string nm, int tp, int inp, int out);
  
  

 private :
  
   

};
#endif
