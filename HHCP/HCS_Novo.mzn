include "alldifferent.mzn";
include "circuitpreds.mzn";
include "circuit.mzn";

int: nbNodes;       % number of locations
set of int: Locations = 1..nbNodes; %conjunto de locais com o hospital
set of int: Visits = 2..nbNodes-1; %conjunto de pacientes

int: nbVehi;
set of int: Enfermeiras = 1..nbVehi; %rotas = numero de enfermeiras

int: nbServi;
set of int: Services = 1..nbServi; %conjunto de servicos

% travel times between locations
%2d array 
array[Locations,Locations] of int: d; 

%qualification of staff members
array[Enfermeiras,Services] of 0..1: a;

% successor variables
%proximo local a ser visitado pela enfermeira
%array[Locations] of var 1..nbNodes+1: succ;
array[Enfermeiras,Locations] of var 1..nbNodes+1: prox; 

% começa o percurso no hospital
var Locations: start = 1;
var Locations: end;

%service requirement of each patient
array[Locations,Services] of bool: rs;

var float: obj; %onde está guardado o calculo de distancia

var int: Z;  %onde está guardado o calculo de atendimentos

%Inicio do serviço s ao patient i realizado pela enfermeira v
%3Darray com unidades de tempo
% DECISION VARIABLE 
array[Locations,Enfermeiras,Services] of var int: t;

%Janela de tempo
%tempo de começo e fim dos serviços nessas locações
array[Services, Locations] of 0..480: e;
array[Services, Locations] of 0..600: l;

%1 se a enfermeira vai de i para j realizar o serviço s
% 4D array
% DECISION VARIABLE
array[Locations,Locations,Enfermeiras,Services] of var 0..1: x; 

% DECISION VARIABLE
%distancia total percorrida?
array[Enfermeiras] of var float: rota; %calculo dos pesos do caminho

% DECISION VARIABLE
%é uma média?
array[Enfermeiras] of var 0.0..480.0: dayWl; %tempo total de servico diario

% DECISION VARIABLE
array[Enfermeiras] of var 0.0..480.0: servico; %tempo total de serviço de cada enfermeira

%tempo de cada servico(set) prestado a cada paciente(lines) por cada enfermeira(column)
array[Locations, Enfermeiras, Services] of int: p;

% DECISION VARIABLE
array[Enfermeiras] of var int: C; %visitas por enfermeiras

%distancias
%distancia total percorrida - a ser minimizada - NAO UTILIZADO 
obj = sum(r in Enfermeiras)(rota[r]);     

%atendimentos
%numero total de atendimentos a ser maximizado
Z = sum(r in Enfermeiras)(C[r]);

var 0.0..480.0: maximum;
var 0.0..480.0: minimum;
constraint forall(r,s in Enfermeiras where r<s) (if dayWl[r]>dayWl[s] then maximum = dayWl[r] endif);

constraint forall(r,s in Enfermeiras where r<s) (if dayWl[r]<dayWl[s] then minimum = dayWl[r] endif);


%constraint maximum = max(r in Enfermeiras)(dayWl[r]); % max e min dão runtime error em empty arrays

%var 0.0..480.0: differ =  min([dayWl[f] | f in Enfermeiras]);

solve minimize -Z + (maximum-minimum);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                     Constraints                                                 %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%1111111111111111111111111111111111111111111111111111111111111111111111111111111111
%                         criando o percurso                                      %     
%1111111111111111111111111111111111111111111111111111111111111111111111111111111111

constraint forall(r in Enfermeiras)( path(row(prox,r), start, end) );

%2222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222
%janela de tempo                                                                                       %
%2 - 3 determine the compliance of service start times with the time windows                           %
%(adjusts the service time to the time window)                                                         %
%333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333%

constraint
  forall(i in Locations, r in Enfermeiras, s in Services)(
    t[i,r,s] >= e[s, i]
  );
  
constraint 
  forall(i in Locations, r in Enfermeiras, s in Services)(
    t[i,r,s] <= l[s, i] 
  );  
  
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                     Referente a carga horaria                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%  calculo do tempo de servico %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

constraint
  forall(r in Enfermeiras)(trace( "p[i,r,s] * x[i,prox[r,i],r,s] < 480.0",
    servico[r] = sum(i in Visits, s in Services)( p[i,r,s] * x[i,prox[r,i],r,s] )
 ));


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%  calculo dos pesos de cada aresta  %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
constraint
  forall(r in Enfermeiras)(
      rota[r] = sum(i in Visits, s in Services)( d[i,prox[r,i]] * x[i,prox[r,i],r,s] )
  );

%C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3%
% Cattafi - Calculo de carga horaria diaria                                                            %
%C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3%

constraint
  forall(r in Enfermeiras)( trace( "dayWl[r] < 480.0",
    dayWl[r] = servico[r] + rota[r]
 )); 

%6666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666
% constraint 6 - inflow-outflow conditions, which ensure that an employee v, who visits patient i, has %
% to leave this patient after the service                                                              %
% CONTRIBUIÇÃO: esta restrição é uma contribuição do trabalho, lembre-se de colocar isso no seu texto. %
%6666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666

constraint
  forall(i in Visits, r in Enfermeiras)(
    sum(j in Locations, s in Services)( x[i,j,r,s] ) == sum(j in Locations, s in Services)(x[j,i,r,s]) 
  );  
 
% nodes 1 e nbNodes, representando o hospital, só podem ser, respectivavemente, de saída e de chegada  
constraint 
  forall(i, j in Locations, r in Enfermeiras, s in Services) (
    x[nbNodes, j, r, s] + x[i, 1, r, s] = 0
  );

constraint
  sum(r in Enfermeiras, j in Locations, s in Services)(x[1,j,r,s]) <= nbVehi ;


                        
%7777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777
% constraint 7 - cada serviço é realizado por uma enfermeira qualificada                               %
% CONTRIBUIÇÃO: esta restrição é uma contribuição do trabalho, lembre-se de colocar isso no seu texto. %
%7777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777
constraint 
  forall(j in Visits, s in Services)(trace("a[r,s] * x[i,j,r,s]) > rs[j,s]",     
   sum(r in Enfermeiras, i in Locations)(a[r,s] * x[i,j,r,s]) <= rs[j,s] 
  ));   


%7777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777
% constraint 8 - Tiago                                                                                 %
% CONTRIBUIÇÃO: esta restrição é uma contribuição do trabalho, lembre-se de colocar isso no seu texto. %
%7777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777
constraint 
  forall(i,j in Visits, v in Enfermeiras, s1,s2 in Services)(
      trace("x[i,j,r,s2] 1 <-> (t[i,v,s1] + p[i,v,s1] + d[i,j]) <= t[j,v,s2,k]",     
      x[i,j,v,s2] = 1 -> ( (t[i,v,s1] + p[i,v,s1] + d[i,j]) <= t[j,v,s2] )
  ));


%conta quantas visitas foram realizadas por cada enfermeira em cada dia
%não conta a primeira visita de cada enfermeira partindo do hospital porque começa com i=2 (set Visits)
%mas conta o retorno x->12 como uma visita - OK
constraint 
    forall(r in Enfermeiras)
          (C[r] = sum(i in Visits, s in Services)
                        (trace("bool = \(x[i,prox[r,i],r,s]), \(i), \(prox[r,i])\n", x[i,prox[r,i],r,s]) ) 
           );                     

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%     a quantidade de visitas realizadas deve ser no máximo a quantidade de serviços solicitados    %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

constraint forall(i in Locations, j in Visits, r in Enfermeiras, s in Services)( x[i,j,r,s] <= a[r,s] * rs[j,s]  );

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%constraint forall();
    
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%      Funções objetivo       %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%solve satisfy;



%solve minimize obj;

% include "minisearch.mzn";
% array[int] of var int: obj_array = [Z, - obj];
% solve search minimize_lex(obj_array);

%%%%%%%%%%%%%%%%%%%%%%
%      Outputs       %
%%%%%%%%%%%%%%%%%%%%%%

output["Visitas = \(Z) \n"];

output["Distancia = \(obj)\n"];

% output["Dist total= \(rota)\n"];

% output["Tempo total= \(dayWl)\n"];

% output["Maximum e minimum:\(maximum) , \(minimum)\n"];
% output["\(Z), \(obj)"];

%   output[
%       if fix(x[i,j,r,s])=1 then
%              if r = 1 then
%                "\\draw [black,->] (P\(i)) edge (P\(j));" ++ "\n" 
%              else 
%                if r= 2 then
%                  "\\draw [red,->] (P\(i)) edge (P\(j));" ++ "\n" 
%                else 
%                  if r = 3 then
%                    "\\draw [blue,->] (P\(i)) edge (P\(j));" ++ "\n" 
%                  else 
%                    if r = 4 then
%                      "\\draw [yellow,->] (P\(i)) edge (P\(j));" ++ "\n" 
%                    else "\\draw [grey,->] (P\(i)) edge (P\(j)); \(r)" ++ "\n" endif
%                  endif
%                 endif
%              endif
%       else 
%         "" 
%       endif
%       | 
%       r in Enfermeiras, i, j in Locations, s in Services
%      ];

% j != 1 e i!= nbNodes
