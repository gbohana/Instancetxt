/*
 * input.cpp
 *
 *  Created on: 9 de jul de 2018
 *      Author: adamr
 */

#include "input.h"

Input::Input() {

	num_vertices = 0;
}


double Input::euclidian2D(double x1, double y1, double x2, double y2){
	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

double Input::geo(double x1, double y1, double x2, double y2){

	latitude_x1 = computeGeo(x1);
	longitude_y1 = computeGeo(y1);
	latitude_x2 = computeGeo(x2);
	longitude_y2 = computeGeo(y2);

	q1 = cos(longitude_y1 - longitude_y2);
	q2 = cos(latitude_x1 - latitude_x2);
	q3 = cos(latitude_x1 + latitude_x2);

	return (rrr * acos( 0.5 * ( (1+q1) * q2 - (1 - q1) * q3) ) + 1);
}

double Input::computeGeo(double a){

	degree = a;
	min = a - degree;

	return (pi * (degree + 5 * min / 3) / 180);
}

void Input::addCoordenates(int id, double x, double y){

	coordinates[id].first = x;
	coordinates[id].second = y;
}

double Input::setBuild(double x1, double y1, double x2, double y2){

	if(type == "EUC_2D")
		return euclidian2D(x1,y1,x2,y2);

	return geo(x1,y1,x2,y2);
}

void Input::build(){

	pair<double,double> v1,v2;


		for(unsigned int i = 0; i < num_vertices; i++){
			for(unsigned int j = 0; j < num_vertices; j++){
				v1 = coordinates[i];
				v2 = coordinates[j];

				distance_matrix[i][j] = 
				distance_matrix[j][i] =
										round(setBuild(v1.first, v1.second,
												v2.first, v2.second));
				
			}
		}

}

/*vector<string> Input::breakString(string s, char splitChar){
  stringstream stream (s);
  vector<string> tokens;
  for (string each; getline(s, each, splitChar); tokens.push_back(each));
  return tokens;
}*/

void Input::load(string name){

	ifstream file(name.c_str());
  OnRequest r;
  Shift shf;
  Staff stf;
  DaysOff dO;
  OffRequest rr;
  Cover co;
  int num_shifts=0, num_staff=0;
	if( file.is_open() ){

    //cout<<"Consegui abrir o arquivo!"<<endl;

		while(file >> reader){
      //cout<<reader<<endl;


      // TODO: cria função para descartar linhas de comentário
      // # indica a presenca de comentario
      if(reader == "#"){
        // Descarta a linha do comentario
        getline(file,lixo);
      }

      // Captura a informacao do Horizonte
      if( reader == "SECTION_HORIZON"){
        do{
          file >> reader;
            if(reader == "#")
              getline(file,lixo);
        }while(reader == "#");
        stringstream geek(reader); 
        geek >> this->horizon_;
                
      }

      
      if( reader == "SECTION_SHIFTS"){
        do{
          do{
            // Leitura da linha
            file >> reader;
            // tratamento de comentarios
            if(reader[0] == '#') 
              getline(file,lixo);
          }while(reader[0] == '#');
          
          if(reader != "SECTION_STAFF"){
          
          stringstream geek(reader);
          
          getline(geek, shf.shift_id_, ',' );
          getline(geek, shf.length_in_min_, ',' );
          while (geek){
            string notfollow;
            getline(geek, notfollow, ',');
            shf.not_follow_shift_.push_back(notfollow);
          }

          shifts_.push_back(shf);
          num_shifts++;
          }
        } while(reader != "SECTION_STAFF");       
        num_total_shifts_ = num_shifts;
      }
      
      if( reader == "SECTION_STAFF"){ cout<<"staff"<<endl;
        do{
          do{
            // Leitura da linha
            file >> reader;
            // tratamento de comentarios
            if(reader[0] == '#') 
              getline(file,lixo);
          }while(reader[0] == '#');
          
          if(reader != "SECTION_DAYS_OFF"){
          
          stringstream geek(reader);
          
          getline(geek, stf.employee_id_, ',' );
          
          for (int i = 0;i<num_total_shifts_;i++){
            string s;
            pair<string, string> p;
            getline(geek, s, '=' );
            p.first = s;
            if (i!=num_total_shifts_-1){
              getline(geek, s, '|' );
              p.second = s;
            } else {
              getline(geek, s, ',' );
              p.second = s;
            }
            stf.max_shifts_.push_back(p);
          }
          getline(geek, stf.max_total_minutes_, ',' );
          getline(geek, stf.min_total_minutes_, ',' );
          getline(geek, stf.max_consecutive_shifts_, ',' );
          getline(geek, stf.min_consecutive_shifts_, ',' );
          getline(geek, stf.min_consecutive_days_off_, ',' );
          getline(geek, stf.max_weekends_, ',' );

          staff_.push_back(stf);
          num_staff++;
          }

        } while(reader != "SECTION_DAYS_OFF");
        num_total_staff_ = num_staff;       
      }

      if( reader == "SECTION_DAYS_OFF"){
        cout<<"daysoff"<<endl;
        do{
          do{
            // Leitura da linha
            file >> reader;
            // tratamento de comentarios
            if(reader[0] == '#') 
              getline(file,lixo);
          } while(reader[0] == '#');
          
          if(reader != "SECTION_SHIFT_ON_REQUESTS"){
          
          stringstream geek(reader);
          
          getline(geek, dO.employee_id_, ',' );
          string str;

          //str sendo posto de forma duplicada em dayIndex
          while(geek){
            getline(geek, str, ',' ); cout<<str<<endl;
            dO.day_index_.push_back(str);
          }

          days_off_.push_back(dO);
          }
        } while(reader != "SECTION_SHIFT_ON_REQUESTS");   
        
      }

      if( reader == "SECTION_SHIFT_ON_REQUESTS"){
        do{
          do{
            // Leitura da linha
            file >> reader;
            // tratamento de comentarios
            if(reader[0] == '#') 
              getline(file,lixo);
          }while(reader[0] == '#');

          // TODO: parar se reader == SECTION_
          if(reader != "SECTION_SHIFT_OFF_REQUESTS"){
            // Faz a quebra da string
            stringstream geek(reader);
            
            // Faca a sobrecarda do operador>>
            //geek >> r;

            getline(geek, r.employee_id_, ',' );
            getline(geek, r.day_, ',' );
            getline(geek, r.shift_id_, ',' );
            getline(geek, r.weight_, ',' );
          }
          shift_request_.push_back(r);
        }
        while(reader != "SECTION_SHIFT_OFF_REQUESTS");        
      }
      if( reader == "SECTION_SHIFT_OFF_REQUESTS"){
        do{
          do{
            // Leitura da linha
            file >> reader;
            // tratamento de comentarios
            if(reader[0] == '#') 
              getline(file,lixo);
          }while(reader[0] == '#');

          // TODO: parar se reader == SECTION_
          if(reader != "SECTION_COVER"){
            // Faz a quebra da string
            stringstream geek(reader);
            
            // Faca a sobrecarda do operador>>
            //geek >> r;

            getline(geek, rr.employee_id_, ',' );
            getline(geek, rr.day_, ',' );
            getline(geek, rr.shift_id_, ',' );
            getline(geek, rr.weight_, ',' );
          }
          shift_off_request_.push_back(rr);
        }
        while(reader != "SECTION_COVER");        
      }
      
      if( reader == "SECTION_COVER"){
        //int totalCover é numEmployees*numShifts
        int num_cover = num_shifts*num_staff;
          for(int i=0;i<num_cover;i++){
          do{
            // Leitura da linha
            file >> reader;
            // tratamento de comentarios
            if(reader[0] == '#') 
              getline(file,lixo);
          }while(reader[0] == '#');

          stringstream geek(reader);
          getline(geek, co.day_, ',' );
          getline(geek, co.shift_id_, ',' );
          getline(geek, co.requirement_, ',' );
          getline(geek, co.weight_for_under_, ',' );
          getline(geek, co.weight_for_over_, ',' );          
          cover_.push_back(co);
        
       }
      }

					if(reader == "DIMENSION"){

						file >> reader; // :
						file >> num_vertices;

						coordinates.resize( num_vertices );
						distance_matrix.resize( num_vertices, vector<int>(num_vertices) );
					}

					if(reader == "EDGE_WEIGHT_TYPE"){

						file >> reader; // :
						file >> type;

					}

					if(reader == "NODE_COORD_SECTION"){
						int id;
						double x,
							   y;

						for(unsigned int i = 0; i < num_vertices; i++){

							file >> id >> x >> y;


							addCoordenates(id-1,x,y);

						}

						build();
					}


		}

		file.close();
	}else
  cout<<"Problema na abertura do arquivo"<<endl;


}

