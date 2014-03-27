//Programma om een oplossing voor een rechthoek te controleren
//Er is mogelijkheid om files te importeren en daarna te controleren
//TODO Oplossingen genereren.

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
		bool bevat_info;
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
	bevat_info = false;
	info = false;
	algeteld = false;
	for(i = 0; i<4; i++)
		buren[i] = NULL;
}//vakje::vakje

//De rand van een oplossing met zwarte vakjes vanaf "vanaf" en tot "tot"
//kloksgewijs. Er wordt telkens gekozen voor een rand die niet de linkerboven-
//hoek bevat (dus de linkerbovenhoek is wit).
class rand{
	public:
		vakje* vanaf;
		vakje* tot;
		vakje* volgend(vakje*);
		vakje* vorig(vakje*);
		vakje* spie(vakje*, bool);
		bool bevatlb(vakje*, vakje*);
		bool eerderv(vakje*, vakje*);
		bool eerderr(vakje*, vakje*, vakje*, vakje*);
		bool eerste();
		bool ineenhoek(vakje* p);
		bool vierkant2x2();
		void negatief(vakje*&, vakje*&);
		void spiegel(vakje*&, vakje*&, bool);
		void nrand();
		rand();
};

rand::rand(){
	vanaf = new vakje;
}

//Geeft het volgende vakje in de rand (kloksgewijs gezien)
vakje* rand::volgend(vakje* rvakje){
	for(int i = 0; i < 4; i++)
		if(rvakje->buren[i] == NULL && rvakje->buren[(i+1) % 4] != NULL)
			return rvakje->buren[(i+1) % 4];
}

//Geeft het vorige vakje in de rand (kloksgewijs gezien)
vakje* rand::vorig(vakje* rvakje){
	for(int i = 0; i < 4; i++)
		if(rvakje->buren[i] != NULL && rvakje->buren[(i+1) % 4] == NULL)
			return rvakje->buren[i];
}

//Returnt true desda de rand de linkerbovenhoek bevat.
bool rand::bevatlb(vakje* kvanaf, vakje* ktot){
	vakje* p = vorig(kvanaf);
	while(p != ktot){
		p = volgend(p);
		if (p->buren[2] == NULL && p->buren[3] == NULL)
			return true;
	}
	return (p->buren[2] == NULL && p->buren[3] == NULL);
}

//Maakt de zwarte vakjes in de rand wit en andersom.
void rand::negatief(vakje*& kvanaf, vakje*& ktot){
	vakje* temp = kvanaf;
	kvanaf = volgend(ktot);
	ktot = vorig(temp);
}

//Spiegelt een vakje verticaal(true) of horizontaal(false) (snelheid: O(h/b)).
//Hulpfunctie voor spiegel.
vakje* rand::spie(vakje* rvakje, bool vertic){
	int teller = 0;
	vakje* p = rvakje;
	while(p->buren[vertic] != NULL){
		p = p->buren[vertic];
		teller++;
	}
	p = rvakje;
	while(p->buren[vertic + 2] != NULL)
		p = p->buren[vertic + 2];
	for(int i = 0; i < teller; i++)
		p = p->buren[vertic];
	return p;
}


//Spiegelt de rand verticaal(true) of horizontaal(false).
void rand::spiegel(vakje*& kvanaf, vakje*& ktot, bool vertic){
	vakje* temp = kvanaf;
	kvanaf = spie(ktot, vertic);
	ktot = spie(temp, vertic);
	if(bevatlb(kvanaf, ktot))
		negatief(kvanaf, ktot);
}

//Returnt true desda je vakje1 eerder of tegelijk tegenkomt vanaf het vakje
//rechts van de linkerbovenhoek (kloksgewijs bekeken). Gelijkheid geeft true.
bool rand::eerderv(vakje* vakje1, vakje* vakje2){
	while(vakje2->buren[2] != NULL || vakje2->buren[3] != NULL){
		if(vakje2 == vakje1)
			return true;
		vakje2 = vorig(vakje2);
	}
	return (vakje2 == vakje1);
}//rand::eerderv

//Returnt true desda rand1 eerder zou worden gegenereerd door de "nieuwe rand"-
//functie dan rand2. Gelijkheid van de randen geeft false.
bool rand::eerderr(vakje* vanaf1, vakje* tot1, vakje* vanaf2, vakje* tot2){
	if(!eerderv(vanaf2, vanaf1))
		return true;
	else if(!eerderv(vanaf1, vanaf2))
		return false;
	else if(!eerderv(tot2, tot1))
		return true;
	else
		return false;
}

//Returnt true desda deze rand de eerste is van al zijn spiegelingen.
bool rand::eerste(){
	vakje *kvanaf = vanaf, *ktot = tot;
	for(int i = 0; i < 3; i++){
		spiegel(kvanaf, ktot, i % 2);
		if(eerderr(kvanaf, ktot, vanaf, tot))
			return false;
	}
	return true;
}//rand::eerste

//Geeft true desda p een "hoekvakje" aanwijst.
bool rand::ineenhoek(vakje* p){
	for(int i = 0; i < 4; i++)
		if(p->buren[i] == NULL && p->buren[(i+1)%4] == NULL)
			return true;
	return false;
}//rand::ineenhoek

//Maakt een volgende rand en let op symmetrie.
void rand::nrand(){
	do{
		tot = volgend(tot);
		if(tot->buren[2] == NULL && tot->buren[3] == NULL){
			vanaf = volgend(vanaf);
			tot = vanaf;
		}
	}while(!eerste());
}//rand::nrand

//Deze class zorgt ervoor dat vakjes naar elkaar kunnen wijzen.
class hokje{
	public:
		vakje* Vakje;
		hokje* volgende;
		hokje();
		~hokje();
};

//Constructor voor hokje
hokje::hokje(){
	Vakje = NULL;
	volgende = NULL;
}//hokje::hokje

//Destructor voor hokje
hokje::~hokje(){
	Vakje->bevat_info = false;
	volgende = NULL;
	Vakje = NULL;
}//hokje::~hokje

//Telkens als een gok gedaan wordt, wordt er een nieuwe groep aangemaakt.
//In die groep worden alle vakjes gestopt waarvan het programma de kleur 
//"weet". De groep wordt gedeletet als de gok leidt tot een fout.
//Het vakje dat werd gegokt gaat als "eerste" in de groep.
class groep{
	public:
		hokje* eerste;
		hokje* laatste;
		groep* hierna;
		void voeg_toe(vakje*);
		groep();
		~groep();
};

//Constructor voor groep
groep::groep(){
	eerste = NULL;
	laatste = NULL;
	hierna = NULL;
}//groep::groep

//Zorgt ervoor dat de hele groep wordt verwijderd en alle vakjes die erin staan
//geen info meer bevatten.
groep::~groep(){
	hokje *p = laatste, *q;
	while(p != NULL){
		q = p->volgende;
		delete p;
		p = q;
	}//while
	eerste = NULL;
	laatste = NULL;
	hierna = NULL;
}//groep::~groep

//Voegt een hokje met daarin een bepaald vakje toe aan de groep.
//De eerste keer dat een hokje wordt toegevoegd, wordt "eerste" dit Hokje.
void groep::voeg_toe(vakje* p){
	hokje* Hokje = new hokje;
	if(eerste == NULL)
		eerste = Hokje;
	Hokje->volgende = laatste;
	laatste = Hokje;
	Hokje->Vakje = p;
}

//Stapelt groepen op elkaar zodat de laatste "gokgeneratie" netjes
//kan worden verwijderd en indien nodig ook die daarvoor.
class stapel{
	public:
		groep* bovenste;
		void nieuw_op_stapel();
		void haal_van_stapel();
		stapel();
};//stapel

//Constructor voor stapel
stapel::stapel(){
	bovenste = new groep;
}//stapel::stapel

//Zet een nieuwe groep bovenop de stapel.
void stapel::nieuw_op_stapel(){
	groep* Groep = new groep;
	Groep->hierna = bovenste;
	bovenste = Groep;
}//stapel::nieuw_op_stapel

//Verwijderd netjes de bovenste groep.
void stapel::haal_van_stapel(){
	groep* p = bovenste;
	if(p != NULL){
		bovenste = p->hierna;
		delete p;
	}//if
}//stapel::haal_van_stapel

//Class van de mogelijke oplossing.
class oplossing{
	private:
		int hoogte, breedte;
	public:
		vakje* start;
		bool ingevuld;//Tijdens zoeken: geeft aan of er wel of niet iets is ~
		bool fout;
		stapel Stapel;
		rand Rand;//				1 1 1 1	0	linksboven is de eerste
		int Hoek[4];//De volgende hoek geeft 4: 1 ? ? ?	?	hoek in de array.
		void drukaf();//			1 ? ? ? ?	Daarna rechtsboven,
		void bouwbord();//			1 ? ? ?	?	linksonder, rechtsonder
		void bouwrij(vakje*);
		void rits(vakje*, vakje*);
		void rand_kleur(rand);
		void instellen();
		void uitfile();
		void naarfile();
		void niets_geteld(bool);
		void vul_in(vakje*, bool);
		void diagc(vakje* A[]);
		void diagcheck(vakje*);
		void diag();//snelheid: O(4n^2)
		void maak_Hoek();
		void strook(vakje*, int, int, bool);
		void hoek();//snelheid: O(n^2)
		void vierk(int, vakje*, bool);
		void vierkant(int, vakje*);
		void spreiding();//snelheid: O(n^5/30)
		void c_rand(int, vakje*, bool);
		void check_rand(int, vakje*);
		void rand_eis();//snelheid: O(n^4/6)
		void tel_opp(vakje*, bool);
		void ingesloten();
		void vul_opp(bool);
		void vul_rand(rand);
		void zoek_vakje();
		void gok();
		void zoek_opl();
		int aantalinsub(int,vakje*);
		vakje* eerste();
		bool vol();
		bool check_subvierkanten();
		oplossing();
};//oplossing

// Constructor voor oplossing
oplossing::oplossing(){
	hoogte = 8;
	breedte = 10;
	start = Rand.vanaf;
}//oplossing::oplossing

// Regelt het bouwen van de rijen en het ritsen van het hele bord
void oplossing::bouwbord(){
	vakje *boven = start, *onder;
	bouwrij(boven);
	for(int i = 1; i < hoogte; i++){
		onder = new vakje;
		bouwrij(onder);
		rits(boven, onder);
		boven = onder;
	}//for
	//De volgende vier regels zouden in een rand::functie moeten komen.
	Rand.vanaf = Rand.tot = start->buren[0];
	Rand.nrand();
	vul_rand(Rand);
	maak_Hoek();
}//oplossing::bouwbord

// Bouwt een dubbel verbonden pointer lijst en geeft de ingang hiervan terug
void oplossing::bouwrij(vakje* ingang){
	vakje *p = ingang, *q = ingang;
	for(int i = 1; i < breedte; i++){
		p = new vakje;
		p->buren[2] = q;
		if(q != NULL)
			q->buren[0] = p;
		q = p;
	}//for
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

//geeft eerste ongetelde vakje met kleur.
vakje* oplossing::eerste(){
	vakje *p = start, *q = start;
	for(int i = 0; i < hoogte; i++){
		p = q;
		for(int j = 0; j < breedte; j++){
			if(!p->algeteld && p->bevat_info)
				return p;
			p = p->buren[0];
		}//for
		q = q->buren[1];
	}//for
	if(p != NULL)
		return p;
	else
		return start;
}//oplossing::eerste

//Telt het aantal aan elkaar gesloten vakjes
void oplossing::tel_opp(vakje* help, bool zwart){
	int i;
	if((help->info == zwart || !help->bevat_info) && !help->algeteld)
		help->algeteld = true;
	else
		return;
	for(i = 0; i < 4; i++)
		if(help->buren[i] != NULL && help->buren[i]->algeteld == false)
			tel_opp(help->buren[i], zwart);
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
		}//for
		q = q->buren[1];
		p = q;
		cout << endl;
	}//for
	return aantal;
}//oplossing::aantalinsub

// Checkt of alle subvierkanten kloppen
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
			}//for
			q = q->buren[1];
			p = q;
		}//for
	}//for
	return true;
}//oplossing::check_subvierkanten

//Drukt af
void oplossing::drukaf(){
	vakje* p = start;
	vakje* q = start;
	cout << endl;
	q = start;
	while(q != NULL){
		p = q;
		while(p != NULL){
			if(p->bevat_info)
				cout << p->info;
			else
				cout << 'x';
			p = p->buren[0];
		}//while
		cout << endl;
		q = q->buren[1];
	}//while
}//oplossing::drukaf

//Mogelijke oplossing uit een file lezen
void oplossing::uitfile(){
	int i, j;
	int info;
	vakje* p = NULL;
	vakje* q = NULL;
	ifstream invoer;
	string infile;
	do{
		cout << "Welke file moet geimporteerd worden?" << endl;
		cin >> infile;
		invoer.open(infile.c_str(), ios::in); //Invoerfile
	}while(invoer.fail());
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
		}//for
		q = q->buren[1];
		p = q;
	}//for
	invoer.close();
}//oplossing::uitfile

//Schrijft een oplossing weg naar een tekstbestand.
void oplossing::naarfile(){
	char antwoord;
	string file_naam;
	vakje *q = start, *p;
	int i, j;
	cout << "Moet deze oplossing weggeschreven worden (Y/n)? ";
	cin >> antwoord;
	if(antwoord == 'n')
		return;
	cout << "Geef de naam van het tekstbestand op: ";
	cin >> file_naam;
	ofstream tekstbestand (file_naam.c_str());
	tekstbestand << hoogte << ' ' << breedte << endl;
	for(i = 0; i < hoogte; i++){
		p = q;
		for(j = 0; j < breedte; j++){
			tekstbestand << char(p->info + '0');
			if(j < breedte - 1)
				tekstbestand << ' ';
			p = p->buren[0];
		}
		if(i < hoogte - 1)
			tekstbestand << endl;
		q = q->buren[1];
	}
	tekstbestand.close();
}

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

//Zorgt ervoor dat voor ieder vakje* p geldt (p->bevat_info == false).
void oplossing::niets_geteld(bool slechts_leeg){
	vakje *q = start, *p;
	for(int i = 0; i < hoogte; i++){
		p = q;
		for(int j = 0; j < breedte; j++){
			if(!slechts_leeg || p->bevat_info == false)
				p->algeteld = false;
			p = p->buren[0];
		}//for
		q = q->buren[1];
	}//for
}//oplossing::niets_geteld

//Kleurt een vakje zwart(true) of wit(false) in.
void oplossing::vul_in(vakje* p, bool zwart){
	if(!p->bevat_info){
		p->bevat_info = ingevuld = true;
		p->info = zwart;
		Stapel.bovenste->voeg_toe(p);
	}
}//oplossing::kleur

//Vult een oppervlak zwart(true) als wit niet met dit oppervlak kan verbinden.
void oplossing::vul_opp(bool zwart){
	vakje *q = start, *p;
	for(int i = 0; i < hoogte; i++){
		p = q;
		for(int j = 0; j < breedte; j++){
			if(!p->algeteld)
				vul_in(p, zwart);
			p = p->buren[0];
		}//for
		q = q->buren[1];
	}//for
}//oplossing::vul_opp

//Kleurt de rand zwart in van "vanaf" tot en met "tot". De rest wordt wit.
void oplossing::vul_rand(rand Rand){
	vakje* p = Rand.vorig(Rand.vanaf);
	while(p != Rand.tot){
		p = Rand.volgend(p);
		p->bevat_info = true;
		p->info = true;
	}//while
	p = Rand.volgend(p);
	while(p != Rand.vanaf){
		p->bevat_info = true;
		p->info = false;
		p = Rand.volgend(p);
	}//while
}//oplossing::vul_rand

//Hulpfunctie voor diagcheck. Vult in als er wat ingevuld kan worden.
void oplossing::diagc(vakje* A[]){
	if(!fout){
		fout = true;
		for(int i = 0; i < 4; i++)
			if(A[i]->info == A[(i+1)%4]->info)
				fout = false;
	}
}//oplossing::diagc

//Hulpfunctie voor oplossing::diag. Parameter is het vakje v/d linkerbovenhoek.
void oplossing::diagcheck(vakje* p){
	bool kleur;
	int som = 0, i;
	vakje* A[4] = {p, p->buren[0], p->buren[0]->buren[1], p->buren[1]};
	for(i = 0; i < 4; i++)//Telt het aantal vakjes met info.
		som += A[i]->bevat_info;
	if (som != 3 && som != 4)//Als het aantal vakjes met info niet 2 is, return.
		return;
	if(som == 4){
		diagc(A);
		return;
	}
	do{//Zoekt het vakje zonder info.
		i--;
	}while(A[i]->bevat_info);
	kleur = A[(i+2)%4]->info;
	if((A[(i+1)%4]->info == kleur) || (A[(i+3)%4]->info == kleur))
		return;
	vul_in(A[i], !kleur);
}//oplossing::diagcheck

//				0 1	
//Als deze functie bijv. 	1 x	tegenkomt, vult deze x = 1 in.
//Gaat voor alle 2x2 vierkanten na wat er (op deze manier) kan worden ingevuld.
void oplossing::diag(){
	vakje *q = start, *p;
	for(int i = 0; i < hoogte - 1; i++){
		p = q;
		for(int j = 0; j < breedte - 1; j++){
			diagcheck(p);
			p = p->buren[0];
		}//for
		q = q->buren[1];
	}//for
}//oplossing::diag

//Geeft invulling aan Hoek[].
void oplossing::maak_Hoek(){
	int teller;
	vakje *p = start, *q1, *q2;
	for(int i = 0; i < 4; i++){
		teller = 0;
		while(p->buren[(i+3)%4] != NULL)
			p = p->buren[(i+3)%4];
		q1 = q2 = p;
		while(q1 != NULL && q2 != NULL && q1->info == p->info &&
				q2->info == p->info){
			teller++;
			q1 = q1->buren[i];
			q2 = q2->buren[(i+1)%4];
		}//while
		Hoek[i] = teller;
	}//for
}//oplossing::maak_Hoek

//Kleurt in een strook de lege vakjes zwart(true) of wit(false).
void oplossing::strook(vakje* q1, int i, int j, bool zwart){
	for(int k = 0; k < 2*j-1; k++){
		q1 = q1->buren[(i+1+(k>=j))%4];
		vul_in(q1, zwart);
	}//for
}//oplossing::strook

//		1 0 0 0
//		1 a b c
//		1 0 d e
//In bijv. 	1 1 1 1		worden d en e door deze functie beide gelijk aan 1.
//Kijkt in de hoeken of er vakjes kunnen worden ingevuld.
void oplossing::hoek(){
	vakje *p = start, *q1, *q2;
	int aant_zwart, aant_wit;
	for(int i = 0; i < 4; i++){ //Gaat alle vier de hoeken na. Eerst linksboven.
		while(p->buren[(i+3)%4] != NULL) //Zorgt ervoor dat p in de hoek komt.
			p = p->buren[(i+3)%4];
		q1 = p->buren[i];
		for(int j = 1; j < Hoek[i]; j++){ //Hierdoor worden alle stroken gecheckt.
			q2 = q1;
			aant_zwart = aant_wit = 0;
			for(int k = 0; k < 2*(j - 1); k++){ //De stroken worden bekeken.
				q2 = q2->buren[(i+1+(k>=j))%4];
				aant_zwart += q2->bevat_info * q2->info;
				aant_wit += q2->bevat_info * (!q2->info);
			}//for
			if(aant_zwart == j - p->info) //Aantal zwarte vakjes is maximaal.
				strook(q1, i, j, false);
			else if(aant_wit == j - (!p->info)) //Aantal witte vakjes is maximaal.
				strook(q1, i, j, true);
			else if(aant_zwart > j - p->info || aant_wit > j - (!p->info))
				fout = true;
			q1 = q1->buren[i];
		}//for
	}//for
}//oplossing::hoek

//Hulpfunctie voor vierkant(). Vult vakjes in als ze ingevuld kunnen worden.
void oplossing::vierk(int zijde, vakje* q, bool zwart){
	vakje* p = q;
	for(int i = 0; i < zijde; i++){
		q = p;
		for(int j = 0; j < zijde; j++){
			vul_in(q, zwart);
			q = q->buren[0];
		}//for
		p = p->buren[1];
	}//for
}//oplossing::vierk

//Hulpfunctie voor spreiding. Handelt één vierkantje af. De pointer wijst naar
//de linkerbovenhoek van dit vierkant.
void oplossing::vierkant(int zijde, vakje* q){
	int aant_zwart = 0, aant_wit = 0, maxi;
	vakje *p = q, *r = q;
	for(int i = 0; i < zijde; i++){
		q = p;
		for(int j = 0; j < zijde; j++){
			aant_zwart += q->bevat_info * q->info;
			aant_wit += q->bevat_info * (!q->info);
			q = q->buren[0];
		}//for
		p = p->buren[1];
	}//for
	maxi = zijde * (zijde + 1) / 2;
	if(aant_zwart == maxi) //Aamtal zwarte vakjes is maximaal.
		vierk(zijde, r, false);
	else if(aant_wit == maxi) //Aantal witte vakjes is maximaal.
		vierk(zijde, r, true);
	else if (aant_zwart > maxi || aant_wit > maxi)
		fout = true;
}//oplossing::vierkant

//In bijv.	1 1 1	worden a, b en c gelijk aan 0 gemaakt.
//		a b c
//		1 1 1
//Deze functie zorgt voor invulling als dat kan. Er kan dus wat worden ingevuld
//als er in een nxn subvierkant n(n+1)/2 zwarte of witte vakjes zitten.
void oplossing::spreiding(){
	vakje *p, *q;
	int maximaal = breedte * (breedte >= hoogte) + hoogte * (breedte < hoogte);
	for(int zijde = 2; zijde <= maximaal; zijde++){
		p = start;
		for(int i = 0; i < hoogte - zijde + 1; i++){
			q = p;
			for(int j = 0; j < breedte - zijde + 1; j++){
				vierkant(zijde, q);
				q = q->buren[0];
			}//for
			p = p->buren[1];
		}//for
	}//for
}//oplossing::spreiding

//Hulpfunctie voor rand_eis. Vult vakjes in waarvan de kleur met zeker is.
void oplossing::c_rand(int zijde, vakje* q, bool zwart){
	for(int i = 0; i < 4 * (zijde - 1); i++){
		vul_in(q, zwart);
		q = q->buren[i / (zijde - 1)];
	}//for
}//oplossing::c_rand

//Hulpfunctie voor rand_eis. Handelt één subvierkant af.
void oplossing::check_rand(int zijde, vakje* q){
	int aant_zwart = 0, aant_wit = 0;
	for(int i = 0; i < 4 * (zijde - 1); i++){
		aant_zwart += q->bevat_info * q->info;
		aant_wit += q->bevat_info * (!q->info);
		q = q->buren[i / (zijde - 1)];
	}//for
	if(aant_zwart == 3 * (zijde - 1)) //Aamtal zwarte vakjes is maximaal.
		c_rand(zijde, q, false);
	else if(aant_wit == 3 * (zijde - 1)) //Aantal witte vakjes is maximaal.
		c_rand(zijde, q, true);
	else if(aant_zwart > 3 * (zijde - 1) || aant_wit > 3 * (zijde - 1))
		fout = true;
}//oplossing::check_rand

//		1 1 1 1
//		1 0 a b
//		c d e f
//In bijv.	1 1 1 1		worden b, c en f gelijk aan 1 gemaakt.
//Controleert of in iedere rand van een nxn subvierkant tenminste n-1 zwarte en
//witte vakjes zitten. Als zo'n rand 3n-1 zwarte vakjes bevat wordt de rest wit. 
void oplossing::rand_eis(){
	vakje *p, *q;
	int maximaal = breedte * (breedte >= hoogte) + hoogte * (breedte < hoogte);
	for(int zijde = 2; zijde <= maximaal; zijde++){
		p = start;
		for(int i = 0; i < hoogte - zijde + 1; i++){
			q = p;
			for(int j = 0; j < breedte - zijde + 1; j++){
				check_rand(zijde, q);
				q = q->buren[0];
			}//for
			p = p->buren[1];
		}//for
	}//for
}//oplossing::rand_eis

//		1 1 a b
//		0 c 1 d
//In bijv.	0 1 e 1		worden a, b en d gelijk aan 1.
//		0 0 0 1		(wit is ingesloten)
//Zoekt een ingesloten oppervlak en kleurt de rest anders dan de kleur v/h opp.
void oplossing::ingesloten(){
	int aantal_opp = 0;
	niets_geteld(false);
	vakje* p = eerste();
	while(!p->algeteld){
		niets_geteld(true);
		aantal_opp++;
		tel_opp(p, p->info);
		vul_opp(!p->info);
		p = eerste();
	}//while
	if(aantal_opp > 2)
		fout = true;
}//oppervlak::ingesloten

//Zoekt naar vakjes die kunnen worden ingevuld.
void oplossing::zoek_vakje(){
	do{
		ingevuld = false;
		hoek();
		diag();
		rand_eis();
		spreiding();
		ingesloten();
	}while(ingevuld);
}//oplossing::zoek_vakje

//Gokt dat een bepaald vakje zwart is. Zal dit proberen in de linkerbovenhoek
//te doen.
void oplossing::gok(){
	int i = 1, j;
	bool hgb = hoogte > breedte;
	vakje *q = start, *p;
	if(hgb){
		while(q->buren[0] != NULL)
			q = q->buren[0];
	}
	while(q->buren[hgb]->buren[hgb] != NULL){
		q = q->buren[hgb];
		j = 0;
		p = q;
		while(j < 2*i-1 && (p->buren[1+(j<=i)+hgb]->buren[hgb+1] != NULL)){
			p = p->buren[1+(j>=i)+hgb];
			if(!p->bevat_info){
				Stapel.nieuw_op_stapel();
				vul_in(p, true);
				return;
			}//if
			j++;
		}//while
		i++;
	}//while
}//oplossing::gok

//Returnt true desda alle vakjes in de oplossing info bevatten.
bool oplossing::vol(){
	vakje *p, *q = start;
	for(int i = 0; i < hoogte; i++){
		p = q;
		for(int j = 0; j < breedte; j++){
			if(!p->bevat_info)
				return false;
			p = p->buren[0];
		}//for
		q = q->buren[1];
	}//for
	return true;
}//oplossing::vol

//Zoekt naar een oplossing voor het probleem.
void oplossing::zoek_opl(){
	int teller = 0;
	while(true){
		fout = false;
		while(!fout){
			fout = false;
			zoek_vakje();
			gok();
			if(vol()){
				zoek_vakje();
				if(!fout){
					cout << "Gevonden! (" << hoogte << 'x' << breedte << ")" << endl;
					drukaf();
					naarfile();
					return;
				}//if
			}//if
		}//while
		//De zooi die hierna komt zou in een stapel-functie moeten komen.
		//Hierin worden de "fouten" afgehandeld.
		while(Stapel.bovenste != NULL && //delete foute groepen.
				Stapel.bovenste->eerste->Vakje->info == false){
			Stapel.haal_van_stapel();
		}//while
		if(Stapel.bovenste != NULL){//De gok moest wit ipv zwart zijn...
			vakje* temp = Stapel.bovenste->eerste->Vakje;
			Stapel.haal_van_stapel();
			Stapel.nieuw_op_stapel();
			Stapel.bovenste->voeg_toe(temp);
			temp->info = !temp->info;
			temp->bevat_info = true;
		}
		else{//Er moet een nieuwe rand komen, want deze is fout.
			Rand.nrand();
			vul_rand(Rand);
			if(start->info){
				cout << "Geen oplossing gevonden." << endl;
				return;
			}
			Stapel.nieuw_op_stapel();
			maak_Hoek();
		}//else
		if ((teller + 1) % 10000 == 0){
			cout << "tussenstand: " << teller << endl;
			drukaf();
		}//if
		teller++;
	}//while
}//oplossing::zoek_opl

//main van het programma
int main(){
	int aantal = 1, teller = 0;
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
	Oplossing.zoek_opl();
	return 0;
}//main
