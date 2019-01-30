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
  string shift_id_;
  string length_in_min_;
  vector <string> not_follow_shift_;
};

class Staff {
public:
  string employee_id_;
  vector <pair<string, string> > max_shifts_;
  string max_total_minutes_;
  string min_total_minutes_;
  string max_consecutive_shifts_;
  string min_consecutive_shifts_;
  string min_consecutive_days_off_; 
  string max_weekends_;
};

class DaysOff {
public:
  string employee_id_;
  vector <string> day_index_;
};

class OnRequest {
public:
   string employee_id_;
   string day_;
   string shift_id_;
   string weight_;
};

class OffRequest {
public:
   string employee_id_;
   string day_;
   string shift_id_;
   string weight_;
};

class Cover {
public:
  string day_;
  string shift_id_;
  string requirement_;
  string weight_for_under_;
  string weight_for_over_;
};

class Input {
public:

  string lixo;

  //Variavel
  int horizon_;

  int num_total_shifts_;
  int num_total_staff_;

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

  vector<OnRequest> shift_request_;
  vector<Shift> shifts_;
  vector<Staff> staff_;
  vector<DaysOff> days_off_;
  vector<string> breakString(string s, char splitChar);
  vector<OffRequest> shift_off_request_;
  vector <Cover> cover_;
  
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
