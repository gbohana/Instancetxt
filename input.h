/*
 * input.h
 *
 *  Created on: 9 de jul de 2018
 *      Author: adamr
 */

#ifndef INPUT_H_
#define INPUT_H_

#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <string>
#include <cmath>
#include <iomanip>
#include <sstream>

using namespace std;

class Shift {
public:
  string shiftID;
  string lengthInMin;
  vector <string> notFollowShift;
};

class Staff {
public:
  string employeeID;
  vector <pair<string, string> > maxShifts;
  string maxTotalMinutes;
  string minTotalMinutes;
  string maxConsecutiveShifts;
  string minConsecutiveShifts;
  string minConsecutiveDaysOff; 
  string maxWeekends;
};

class DaysOff {
public:
  string employeeID;
  vector <string> dayIndex;
};

class OnRequest {
public:
   string employeeID;
   string day;
   string shiftID;
   string weight;
};

class OffRequest {
public:
   string employeeID;
   string day;
   string shiftID;
   string weight;
};

class Cover {
public:
  string day;
  string shiftID;
  string requirement;
  string weightForUnder;
  string weightForOver;
};

class Input {
public:

  string lixo;

  //Variavel
  int horizon_;

  int numTotalShifts_;
  int numTotalStaff_;

  // Funcao de recuperacao de informacoes
  int horizonGet(){
    return horizon_;
  }

  // Funcao para definicao de informacoes
  int horizonSet(int value);

	unsigned int num_vertices;

  int degree;

	double q1;
  double q2;
	double q3;
	double latitude_x1,
		   longitude_y1,
		   latitude_x2,
		   longitude_y2,
		   min,
		   pi = 3.141592,
		   rrr = 6378.388;

  vector<OnRequest> shift_request;
  vector<Shift> shifts;
  vector<Staff> staff;
  vector<DaysOff> daysoff;
  vector<string> breakString(string s, char splitChar);
  vector<OffRequest> shift_off_request;
  vector <Cover> cover;
  
  ////

	vector<vector<int> > distance_matrix;

	//Represents the euclidian2D or geo coordinates of vertex i.
	vector<pair<double,double> > coordinates;

	string reader;

	string type;

	Input();

	double euclidian2D(double x1, double y1, double x2, double y2);

	double geo(double x1, double y1, double x2, double y2);

	double setBuild(double x1, double y1, double x2, double y2);

	double computeGeo(double a);

	void load(string name);

	void addCoordenates(int id, double x, double y);

	void build();

	friend ostream& operator>>(ostream& os, ifstream& file);

};



#endif /* INPUT_H_ */
