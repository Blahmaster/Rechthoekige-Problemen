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
	invoer >> breedte;

	uitvoer << "\\newcommand\\vakje[2]{" << '\n';
	uitvoer << "	\\edef\\x{#1}" << '\n';
	uitvoer << "	\\edef\\y{";
	uitvoer << hoogte - 1 << " - #2}" << '\n';
	uitvoer << "	\\draw[fill=black,scale=0.5] (\\x,\\y) rectangle (\\x+1,\\y+1);" << '\n';
	uitvoer << "}" << '\n';
	uitvoer << "\\begin{tikzpicture}" << '\n';
	for(i = 0; i < hoogte; i++){
		for(j = 0; j < breedte; j++){
			invoer >> info;
			if(info == 1)
				uitvoer << "\\vakje{" << j << "}{" << i << "} ";
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
	cout << "Done!" << endl;
	return 0;
}//main
