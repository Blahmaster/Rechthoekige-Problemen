//Programma om een oplossing voor een rechthoek te controleren
//Er is mogelijkheid om files te importeren en daarna te controleren
//TODO Checken of zwart en wit beide een oppervlak zijn

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Checkt of het aantal zwart en wit in het subvierkant goed is
// aantalz = aantal geteld. nsub is groote subvierkant
bool goed_aantal(int aantalz, int nsub){
	int aantalw = nsub*nsub - aantalz;
	int driehoeks = (nsub*(nsub - 1))/2;
	if(aantalz >= driehoeks && aantalw >= driehoeks)
		return true;
	return false;
}//goed_aantal

//Leest het volgende karakter uit en checkt of het een getal is
//parameter geeft aantal decimalen die maximaal door gegeven worden
int leesgetal(int maxdec){
	int x = 0, i = 0;
	char kar;
	while((i < maxdec) && !((kar == '\n') && (i>0))){
		cin.get(kar);
		if ((kar >= '0') && (kar <='9')){
			x = 10*x + kar - '0';
			i++;
		}
	}
	return x;
} //leesgetal

//Leest het volgende karakter uit als een char en returned het als het een letter is
char leesltr(){
	char kar;
	do{
		cin.get(kar);
	}while((kar == toupper(kar)) && (kar == tolower(kar)));
	return kar;
}//leesltr

class vakje{
	public:
		vakje();
		bool info;
		bool algeteld;
		vakje* buren[4];
};//vakje

// Buren
//	 3
//	2 0
//	 1

vakje::vakje(){
	int i;
	info = false;
	algeteld = false;
	for(i = 0; i<4; i++)
		buren[i] = NULL;
}//vakje::vakje

//Class van de mogelijke oplossing
class oplossing{
	private:
		int hoogte, breedte;
		vakje* ingang;
		vakje* halp;

	public:
		vakje* start;
		void drukaf();
		void bouwbord();
		void rits(vakje*, vakje*);
		void instellen();
		void uitfile();
		int aantalinsub(int,vakje*);
		void in_een_oppervlak(int&,vakje*);
		bool check_subvierkanten();
		vakje* bouwrij();
		oplossing();
};//oplossing

oplossing::oplossing(){
	hoogte = 8;
	breedte = 10;
	ingang = NULL;
	halp = NULL;
	start = NULL;
}//oplossing::oplossing

// Regelt het bouwen van de rijen en het ritsen van het hele bord
void oplossing::bouwbord(){
	vakje* boven;
	vakje* onder;
	int i;
	boven = bouwrij();
	start = boven;
	for(i = 1; i < hoogte; i++){
		onder = bouwrij();
		rits(boven, onder);
		boven = onder;
	}//for
}//oplossing::maakbord

// Bouwt een dubbel verbonden pointer lijst en geeft de ingang hiervan terug
vakje* oplossing::bouwrij(){
	int j;
	vakje* q = halp;
	for(j = 1; j <= breedte; j++){
		vakje* p = new vakje;
		if(j == 1){
			ingang = p;
		}
		else
			p->buren[2] = q;
		if(j == breedte){
			p->buren[0] = NULL;
			halp = p;
		}
		if(j > 1)
			q->buren[0] = p;
		if(j < breedte)
			q = p;
	}
	return ingang;
}//oplossing::bouwlijst

// De twee parameters zijn beginnen van twee dubbel verbonden pointer lijsten en worden aan elkaar geritst
void oplossing::rits(vakje* boven, vakje* onder){
	while(boven != NULL){
		boven->buren[1] = onder;
		onder->buren[3] = boven;

		boven = boven->buren[0];
		onder = onder->buren[0];
	}//while
}//oplossing::rits


// Nog niet getest !!!!!!!!

//Telt het aantal aan elkaar gesloten vakjes
void oplossing::in_een_oppervlak(int & teller, vakje* help){
	int i;
	if(help->info == true){
		if(!help->algeteld){
			teller++;
			help->algeteld = true;
		}
	}
	else
		return;

	for(i = 0; i < 4; i++){
		if(help->buren[i] != NULL && help->buren[i]->algeteld == false){
			help = help->buren[i];
			in_een_oppervlak(teller, help);
		}
	}
	return;
}

//Telt het aantal zwarte vakjes in een subvierkant. Parameters zijn groote van subvierkant en een pointer naar de linkerbovenhoek van het vierkant
int oplossing::aantalinsub(int n, vakje* sub){
	int i, j;
	int aantal = 0;
	vakje* p = sub;
	vakje* q = sub;
	for(i = 0; i < n; i++){
		for(j = 0; j < n; j++){
			if(p->info)
				cout << "1";
			else
				cout << "0";

			if(p->info)
				aantal++;
			p = p->buren[0];
		}
		q = q->buren[1];
		p = q;
		cout << endl;
	}
	return aantal;
}//oplossing::aantalinsub

bool oplossing::check_subvierkanten(){
	int i, j;
	int maxn = hoogte;
	int n;
	int aantal;
	vakje* p = start;
	vakje* q = start;
	if(hoogte > breedte)
		maxn = breedte;
	for(n = 2; n <= maxn; n++){
		p = start;
		q = start;
		for(i = 0; i <= hoogte - n; i++){
			for(j = 0; j <= breedte - n; j++){
				cout << endl;
				aantal = aantalinsub(n, p);
				if(!goed_aantal(aantal, n))
					return false;
				p = p->buren[0];
			}
			q = q->buren[1];
			p = q;
		}
	}
	return true;
}//oplossing::check_subvierkanten

//Drukt af
void oplossing::drukaf(){
	int i, j;
	vakje* p = start;
	vakje* q = start;
	for(i = 0; i < hoogte; i++){
		for(j = 0; j < breedte; j++){
			if(p->info)
				cout << "1";
			else
				cout << "0";
			p = p->buren[0];
		}
		q = q->buren[1];
		p = q;
		cout << endl;
	}
}//oplossing::drukaf

//Mogelijke oplossing uit een file lezen
void oplossing::uitfile(){
	int i, j;
	int info;
	vakje* p = NULL;
	vakje* q = NULL;
	ifstream invoer;
	string infile;
	cout << "Welke file moet geimporteerd worden?" << endl;
	cin >> infile;
	invoer.open(infile.c_str(), ios::in); //Invoerfile
	if(invoer.fail()){ //Checkt of invoer goed is
		cout << "File niet geopend" << endl;
		return;
	}
	invoer >> hoogte;
	invoer >> breedte;
	bouwbord();
	p = start;
	q = start;
	for(i = 0; i < hoogte; i++){
		for(j = 0; j < breedte; j++){
			invoer >> info;
			if(info == 1)
				p->info = true;
			else
				p->info = false;
			p = p->buren[0];
		}
		q = q->buren[1];
		p = q;
	}
	invoer.close();
}//oplossing::uitfile

// Gebruiker kan parameters van de mogelijke oplossing instellen
void oplossing::instellen(){
	cout << "Hoogte: ";
	do{
		cin >> hoogte;
	}while(hoogte < 2);

	cout << endl << "Breedte: ";
	do{
		cin >> breedte;
	}while(breedte < 2);
	cout << endl;
	return;
}//oplossing::instellen

//main van het programma
int main(){
	int aantal = 1;
	char temp;
	oplossing Oplossing;
	cout << "Uitfile?(Y/n): ";
	temp = leesltr();
	if(temp == 'Y' || temp == 'y')
		Oplossing.uitfile();
	else{
		Oplossing.instellen();
		Oplossing.bouwbord();
	}
	Oplossing.drukaf();
	if(Oplossing.check_subvierkanten())
		cout << "GOED!!!" << endl;
	else
		cout << "Helaas :(";
	return 0;
}//main
