#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include<cmath>
using namespace std;

// for((i = 1; i<11; i++)); do ./a.out 40 $i; done
// ./a.out
pair<double,double> makeCoordenada(){
    pair <double,double> coordenada;
    double r = rand()%31;
    double theta = -M_PI + ((double)rand() / (RAND_MAX + 1.0))*(2.0*M_PI);
    double x = r*cos(theta);
    double y = r*sin(theta);
    coordenada = make_pair(x,y);
    return coordenada;
}

int distancia(pair<int,int> a, pair<int,int> b){
    return sqrt(pow(a.first - b.first,2.0) + pow(a.second - b.second,2.0));
}

vector<string> split (string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

int main(int argc, char* argv[]){

    srand(time(0));

    string argv1(argv[1]);
    string argv2(argv[2]);

    string output_filename("Instanz_HCSRP_"+argv1+"_"+argv2+".dzn");
    ofstream saida(output_filename.c_str(),ios::out);
    int nbNodes = 12;
    int nbVehi = 3;
    int nbServi = 6;
    int sequencia = atoi(argv[2]);

    switch (atoi(argv[1]))
    {
        case 10:
             nbNodes = 12;
             nbVehi = 3;
             nbServi = 4;
            break;
        case 20:
            nbNodes=22;
            nbVehi=5;
            nbServi = 5;
            break;
        case 30:
             nbNodes = 32;
             nbVehi = 10;
             nbServi = 6;
            break;
        case 40:
             nbNodes = 42;
             nbVehi = 10;
             nbServi = 7;
            break;
        default:
            break;
    }

    saida<<"nbNodes = "<<nbNodes<<";"<<endl;
    saida<<"nbVehi = "<<nbVehi<<";"<<endl;
    saida<<"nbServi = "<<nbServi<<";"<<endl;
    saida<<"numdays = "<<5<<";"<<endl;

    int e[5][nbNodes];
    int l[5][nbNodes];

    for(int i = 0; i < 5; i++)
        for (int j = 0; j < nbNodes; j++)
        {
            if(j == 0 || j == nbNodes-1){
                e[i][j] = 0;
                l[i][j] = 480;
            }else{
            e[i][j] = rand()%481;
            l[i][j] = e[i][j]+60+rand()%61;
            }
        }

    // Inicio da impressao do e
    saida<<"\ne = \n [|";
    for(int i = 0; i < 5; i++)
    {
        for (int j = 0; j < nbNodes; j++)
        {
            saida<<setw(5)<< e[i][j]<<",";
        }
        saida<<endl<<" |";

    }
    saida<< "];\n"<<endl;
    // Fim da impressao do e

    // Inicio da impressao do l
    saida<<"l = \n [|";
    for(int i = 0; i < 5; i++)
    {
        for (int j = 0; j < nbNodes; j++)
            saida<<setw(4)<< l[i][j]<<",";
        saida<<endl<<" |";
    }
    saida<< "];\n";
    // Fim da impressao do l


    // for( sequencia = 1; sequencia <= 10; sequencia++)    {

        int a[nbVehi][nbServi];
        int minimo = min(nbVehi,nbServi);
        //Garantir que tenha pelo menos uma enfermeira para cada serviço
        for(int i = 0; i < nbVehi; i++)
        {
            for(int j = 0; j < nbServi; j++)
            {
                a[i][j] = 0;
                if(i%minimo == j%minimo)
                    a[i][j] = 1;
                else
                {
                    if(rand()%100 < sequencia*10)
                        a[i][j] = 1;
                }
            }
        }

        // Inicio da impressao da tabela a
        saida<<"\na = \n [|";
        for(int i = 0; i < nbVehi; i++){
            for(int j = 0; j < nbServi; j++)
                saida<<setw(3)<< a[i][j]<<",";
            saida<<endl<<" |";
        }
        saida<< "];\n";

    // }

    int p[nbNodes][nbVehi][nbServi];
    for(int i = 0; i < nbNodes; i++)
    {
        for(int j = 0; j < nbVehi; j++)
        {
            for(int k = 0; k < nbServi; k++)
            {
                if( i == 0 || i == nbNodes-1){
                    p[i][j][k] = 0;
                }
                else
                {
                    //Calcular a menor janela de tempo - minTimeWindow
                    int minTimeWindow = 480;
                    for(int dia = 0; dia < 5; dia++){
                        minTimeWindow = min(minTimeWindow,l[dia][i]-e[dia][i]);
                    }
                    p[i][j][k] = min(10+rand()%(int(double(minTimeWindow)*((11-sequencia)/10.0))),minTimeWindow);
                }

            }
        }
    }


    saida<<"\np = array3d(1..nbNodes,1..nbVehi,1..nbServi,\n[";
    for(int i = 0; i < nbNodes; i++)
    {
        for(int j = 0; j < nbVehi; j++)
        {
            for(int k = 0; k < nbServi; k++)
            {
                saida<<setw(3)<<p[i][j][k]<<",";
            }
            saida<<"\t";
        }
        saida<<endl;
    }
    saida<< "]);\n";


	/***********************     paciente - dia - servico  **************************/

	string rs[nbNodes][nbServi];
	int minimoPaciente = min(5, nbServi);

	for (int i = 0; i < nbNodes; i++)
	{
		for (int k = 0; k < nbServi; k++)
			{
				rs[i][k] = "false";

				if (i == 0 || i == nbNodes - 1) {
					rs[i][k] = "true";
				}
		}
	}

    for (int i = 0; i < nbNodes; i++)
		for (int j = 0; j < 5; j++)
            rs[i][j][rand()%nbServi] = 1;

	saida << "\nrs = \n[|";
	for (int i = 0; i < nbNodes; i++)
	{
		    for (int k = 0; k < nbServi; k++)
			{
				saida << setw(3) << rs[i][k] << ",";
			}
			saida<<endl<<" |";
    }
	saida<< "];\n"<<endl;

    int matriz[nbNodes][nbNodes];
    vector<pair<int,int> > local;
    for(int i=0; i < nbNodes; i++)
        local.push_back(makeCoordenada());

    local[0].first = 0;
    local[0].second = 0;
    local[nbNodes-1] = local[0];

    saida<<"coord=[ ";
    for(int i=0; i < nbNodes; i++){
            saida<<"("<<local[i].first<<",";
            saida<<local[i].second<<"), ";
    }
    saida<< "];\n"<<endl;


    for(int i=0; i < nbNodes; i++){
        for(int j=0; j < nbNodes; j++){
            matriz[i][j] = distancia(local[i],local[j]);
        }
    }

    saida<<"d=[";
    for(int i=0; i < nbNodes; i++){
        saida<<"|";
        for(int j=0; j < nbNodes; j++){
            saida<<int(matriz[i][j])<<", ";
        }
        saida<<endl;
    }
    saida<<"|];"<<endl;

    for(int i=0; i < nbNodes; i++){
        cout<<"("<<local[i].first<<","<<local[i].second<<")"<<endl;
    }

}
