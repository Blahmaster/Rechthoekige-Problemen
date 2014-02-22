#include<iostream>
#include<fstream>
#include<string>

using namespace std;

//Coverteert oplossing naar TikZ code
void converteer(){
	int i, j;
	int info;
	int hoogte,breedte;
	ifstream invoer;
	ofstream uitvoer;
	string infile;
	string uitfile;
	cout << "Welke file moet geimporteerd worden?" << endl;
	cin >> infile;
	invoer.open(infile.c_str(), ios::in); //Invoerfile
	if(invoer.fail()){ //Checkt of invoer goed is
		cout << "File niet geopend" << endl;
		return;
	}
	cout << "Welke file moet uitgevoerd worden?" << endl;
	cin >> uitfile;
	uitvoer.open(uitfile.c_str(), ios::out);

	invoer >> hoogte;
	cout << hoogte << endl;
	invoer >> breedte;
	cout << breedte << endl;

	uitvoer << "\\newcounter{row}" << '\n';
	uitvoer << "\\newcounter{col}" << '\n';

	uitvoer << "\\newcommand\\lijn[" << breedte << "]{" << '\n';
	uitvoer << "	\\setcounter{col}{0}" << '\n';
	uitvoer << "	\\foreach \\n in {";
	for(i = 0; i < breedte; i++){
		uitvoer << "#" << i+1;
		if(i != breedte - 1)
			uitvoer << ",";
	}
	uitvoer << "} {" << '\n';
	uitvoer << "		\\edef\\x{\\value{col}}" << '\n';
   uitvoer << "	 	\\edef\\y{";
	uitvoer << hoogte - 1 << " - \\value{row}}" << '\n';
   uitvoer << "	 	\\ifnum \\n = 1 \\draw[fill=black,scale=0.5] (\\x,\\y) rectangle (\\x+1,\\y+1);" << '\n';
   uitvoer << "	 	\\fi" << '\n';
	uitvoer << "		\\stepcounter{col}" << '\n';
	uitvoer << "	}" << '\n';
	uitvoer << "	\\stepcounter{row}" << '\n';
	uitvoer << "}" << '\n';
	uitvoer << "\\begin{tikzpicture}" << '\n';
	for(i = 0; i < hoogte; i++){
		uitvoer << "\\lijn";
		for(j = 0; j < breedte; j++){
			invoer >> info;
			if(info == 1)
				uitvoer << "{1}";
			else
				uitvoer << "{0}";
		}
		uitvoer << '\n';
	}
	uitvoer << "\\draw[scale=0.5] (0,0) grid (" << breedte << "," << hoogte << ");" << '\n';
	uitvoer << "\\end{tikzpicture}" << '\n';
	invoer.close();
	uitvoer.close();
}//converteer

// Main van het programma
int main(){
	converteer();
	return 0;
}//main
