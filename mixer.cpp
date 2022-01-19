/* metaFx  .
 *
 * this file is part of the metaFx application
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
#include <sstream>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <stdlib.h>
#include <ctgmath>
#include <string>
#include <cstring>
#include <boost/filesystem.hpp>

#include "mixer.h"


using namespace std;

Mixer::Mixer(){
	
}

Mixer::~Mixer(){
}
void Mixer::setId(int nid){
	mixerId=nid;
}
void Mixer::setGroupe(int grp){
	groupe=grp;
}
void Mixer::setPosX(int nx){
	x=nx;
}
void Mixer::setPosY(int ny){
	y=ny;
}
void Mixer::setDaw(bool Daw){
	daw=Daw;
}
void Mixer::setFx(bool Fx){
	fx=Fx;
}
void Mixer::setSpeed(bool Speed){
	speed=Speed;
}
void Mixer::setRevSpeed(bool RevS){
	revSpeed=RevS;
}
void Mixer::setGain(bool Gain){
	gain=Gain;
}
void Mixer::setRevGain(bool RevG){
	revGain=RevG;
}
void Mixer::setRelatif(bool Rel){
	relatif=Rel;
}
void Mixer::setTrack(int Tk){
	track=Tk;
}
void Mixer::setGainMin(float gm){
	gainMin=gm;
}
void Mixer::setGainMax(float gm){
	gainMax=gm;
}
void Mixer::setSpeedMin(int nc){
	speedMin=nc;
}
void Mixer::setSpeedMax(float dt){
	speedMax=dt;
}
void Mixer::setPath(std::string Path){
	path=Path;
}
void Mixer::setFile(std::string File){
	file=File;
}

int Mixer::getId(){
	return mixerId;
}
int Mixer::getGroupe(){
	return groupe;
}
int Mixer::getPosX(){
	return x;
}
int Mixer::getPosY(){
	return y;
}
bool Mixer::getDaw(){
	return daw;
}
bool Mixer::getFx(){
	return fx;
}
bool Mixer::getGain(){
	return gain;
}
bool Mixer::getSpeed(){
	return speed;
}
int Mixer::getTrack(){
	return track;
}
float Mixer::getGainMin(){
	return gainMin;
}
float Mixer::getGainMax(){
	return gainMax;
}
bool Mixer::getRevGain(){
	return revGain;
}
string Mixer::getPath(){
	return path;
}
string Mixer::getFile(){
	return file;
}
float Mixer::getSpeedMin(){
	return speedMin;
}
float Mixer::getSpeedMax(){
	return speedMax;
}
bool Mixer::getRevSpeed(){
	return revSpeed;
}
bool Mixer::getRelatif(){
	return relatif;
}
