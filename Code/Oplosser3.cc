#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

class bord
{
private:
	int static const hoogte = 7;
	int static const breedte = 9;
	bool A[hoogte][breedte];
	bool nutteloos;
	int x, y;
public:
	bord();
	bool einde();
	void invullen();
	void volgende();
	void vorige();
	bool diagcheck(int bovx, int bovy);
	bool diagonaal();
	void randvolgende (int& xcoor, int& ycoor);
	bool randvoorwaarde();
	void nieuw_A(bool mislukt);
	void print();
	bool subvierkant(int linksboven_x, int linksboven_y, int zijde);
	bool zwartwit_verspreid(int copyx, int copyy);
	void kopieerarray(bool informatie[][breedte], int legearray[][breedte]);
	bool eenoppervlak(bool eenofnul);
	int hokjestellen(bool eenofnul);
	void doodlopend_pad(int kopie[][breedte], int &pos_x, int &pos_y, bool &posveranderd);
	bool startpositie(int kopie[][breedte], int &pos_x, int &pos_y, bool eenofnul);
	int routebepaling(int kopie[][breedte], int pos_x, int pos_y, bool eenofnul);
	bool voldoet();
	bool oplossing();
};

bord::bord()
{
	for(int i = 0; i < hoogte; i++)
		for(int j = 0; j < breedte; j++)
			A[i][j] = false;
	x = y = 0;
	A[0][0] = true; // komt handig uit als deze alvast is ingevuld
	nutteloos = false;
}

bool bord::einde()
{
	return (x == breedte - 1 && y == hoogte - 1);
}

void bord::invullen()
{
	char kar;
	bool geplaatst = false;
	kar = cin.get();
	for (int i = 0; i < hoogte; i++)
	{
		cout << "Geef nu rij " << i << " op:";
		for (int j = 0; j < breedte; j++)
		{
			kar = cin.get();
			if (kar == '1')
				A[i][j] = true;
			else
				A[i][j] = false;
		}
		while (kar != '\n')
			kar = cin.get();
	}

	y = hoogte - 1;
	while (!geplaatst && y >= 0)
	{
		x = breedte - 1;
		while (!geplaatst && x >= 0)
		{
			if (A[y][x] == true)
				geplaatst = true;
			x--;
		}
		y--;
	}
	x++;
	y++;
}

void bord::volgende() //returnt true als er een volgende is.
{
	if (x < breedte - 1)
		x++;
	else if (y < hoogte - 1)
	{
		x = 0;
		y++;
	}
}

void bord::vorige() //vind het vorige zwarte hokje.
{
	bool gevonden = false;
	A[y][x] = false;
	while (!gevonden)
	{
		if (x > 0)
			x--;
		else if (y > 0)
		{
			x = breedte - 1;
			y--;
		}
		if (A[y][x])
			gevonden = true;
	}
}

void bord::nieuw_A(bool mislukt) //mislukt geeft aan of A (huidig) fout is.
{
	if (mislukt)
		A[y][x] = false;
	volgende();
	A[y][x] = true;
}

bool bord::diagcheck(int bx, int by) //bovenhoek x, bovenhoeky
{
	if ((A[by][bx] + A[by + 1][bx + 1] + A[by][bx + 1] + A[by + 1][bx]) % 2 != 0)
		return true;
	if (A[by][bx] != A[by + 1][bx + 1])
		return true;
	return false;
}

bool bord::diagonaal()
{
	bool klopt_nog = true, klaar = false;
	int bovy = 0, bovx = 0;
	while (klopt_nog && bovy <= hoogte - 2 && !klaar)
	{
		bovx = 0;
		while (klopt_nog && bovx <= breedte - 2 && !klaar)
		{
			if ((bovx >= x && bovy + 1 == y) || bovy + 1 > y)
				klaar = true;
			else
				klopt_nog = diagcheck(bovx, bovy);
			bovx++;
		}
		bovy++;
	}
	return klopt_nog;
}

void bord::randvolgende(int& xcoor, int& ycoor)
{ //Gegeven een randvakje. Bepaalt naastliggend randvakje, tegen de klok in.
	if (xcoor == breedte - 1 && ycoor > 0)
		ycoor--;
	else if (xcoor > 0 && ycoor == 0)
		xcoor--;
	else if (xcoor == 0 && ycoor < hoogte - 1)
		ycoor++;
	else if (xcoor < breedte - 1 && ycoor == hoogte - 1)
		xcoor++;
}

bool bord::randvoorwaarde()
{ //checkt of de rand van begin (xcoor,ycoor) tot eind (tegen de klok in) klopt.
	int xcoor, ycoor, eindx = 0, eindy = y, stadium = 1;
	if (y == hoogte - 1)
		eindx = x;
	if (y > 0)
	{
		xcoor = breedte - 1;
		if (x < breedte - 1)
			ycoor = y - 1;
		else
			ycoor = y;
	}
	else
	{
		xcoor = x;
		ycoor = 0;
	}
	if (A[ycoor][xcoor] == true)
		stadium++; //geeft aan wat er gezocht moet worden.
	while (stadium != 4 && !(xcoor == eindx && ycoor == eindy))
	{ //zoekt eerst een 1, dan 0, dan 1 in de rand -> fout
		randvolgende(xcoor, ycoor);
		if (A[ycoor][xcoor] == stadium % 2)
			stadium++;
	}
	return (stadium != 4);
}

bool bord::subvierkant(int linksboven_x, int linksboven_y, int zijde)
{
    int aantal_zwart = 0, aantal_wit = 0, minimaal;
    minimaal = (zijde * (zijde -1))/2;
    for (int i = 0; i < zijde; i++)
        for (int j = 0; j < zijde; j++)
        {
            if (A[linksboven_y+i][linksboven_x+j])
                aantal_zwart++;
            else
                aantal_wit++;
        }
    return (aantal_zwart >= minimaal && aantal_wit >= minimaal);
}

bool bord::zwartwit_verspreid(int copyx, int copyy)
{
	nutteloos = false;
	int linksboven_x = 0, linksboven_y = 0, zijde = 2, onderx, ondery;
	bool klopt_nog = true, klaar;
	while (klopt_nog && zijde <= hoogte)
	{
	  linksboven_y = 0;
	  klaar = false;
	  while (klopt_nog && linksboven_y <= hoogte - zijde && !klaar)
	  {
			linksboven_x = 0;
			while (klopt_nog && linksboven_x <= breedte - zijde && !klaar)
			{
				onderx = linksboven_x + zijde - 1; //de rechteronderhoek x-coord.
				ondery = linksboven_y + zijde - 1; // rechteronderhoek y-coord.
				if ((onderx > copyx && ondery == copyy) || ondery > copyy)
					klaar = true;
				else
					 klopt_nog = subvierkant(linksboven_x, linksboven_y, zijde);
				linksboven_x++;
			}
			linksboven_y++;
	  }
	  zijde++;
	}
	if (!klopt_nog && (x > onderx || y > ondery)) //
		nutteloos = true;
	return klopt_nog;
}

void bord::print()
{
    for (int i = 0; i < hoogte; i++)
    {
        for (int j = 0; j < breedte; j++)
            cout << ' ' << A[i][j];
        cout << endl;
    }
    cout << endl;
}

void bord::kopieerarray(bool informatie[][breedte], int legearray[][breedte])
{
    for(int i = 0; i < hoogte; i++)
        for (int j = 0; j < breedte; j++)
            legearray[i][j] = informatie[i][j];
}

int bord::routebepaling(int kopie[][breedte], int pos_x, int pos_y, bool eenofnul)
{
    int richting = 0;
    bool splitsing = false;
    kopie[pos_y][pos_x] = 2;
    if (pos_x < breedte - 1 && kopie[pos_y][pos_x+1] == eenofnul)
        richting = 1;

    if(pos_y < hoogte - 1 && kopie[pos_y+1][pos_x] == eenofnul)
    {
        if(richting)
            splitsing = true;
        else
            richting = 2;
    }
    if(pos_x > 0 && kopie[pos_y][pos_x-1] == eenofnul)
    {
        if(richting)
            splitsing = true;
        else
            richting = 3;
    }
    if(pos_y > 0 && kopie[pos_y-1][pos_x] == eenofnul)
    {
        if(richting)
            splitsing = true;
        else
            richting = 4;
    }
    if (splitsing)
        kopie[pos_y][pos_x] = 3;
    return richting;
}
void hokje_ernaast(int &pos_x, int &pos_y, int richting)
{
    switch (richting)
    {
        case 1: pos_x++; break;
        case 2: pos_y++; break;
        case 3: pos_x--; break;
        case 4: pos_y--; break;
    }
}
void bord::doodlopend_pad(int kopie[][breedte], int &pos_x, int &pos_y, bool &posveranderd)
{
    pos_x = pos_y = 0;
    posveranderd = false;
    while(pos_y < hoogte && !posveranderd)
    {
        pos_x = 0;
        while(pos_x < breedte && !posveranderd)
        {
            if (kopie[pos_y][pos_x] == 3)
                posveranderd = true;
            pos_x++;
        }
        pos_y++;
    }
    pos_x--;
    pos_y--;
}
bool bord::startpositie(int kopie[][breedte], int &pos_x, int &pos_y, bool eenofnul)
{
    bool gevonden = false;
    while(pos_y < hoogte && !gevonden)
    {
        pos_x = 0;
        while(pos_x < breedte && !gevonden)
        {
            if (kopie[pos_y][pos_x] == eenofnul)
                gevonden = true;
            pos_x++;
        }
        pos_y++;
    }
    pos_x--;
    pos_y--;
    return gevonden;
}
int bord::hokjestellen(bool eenofnul)
{
    int aantal_hokjes = 0;
    for(int i = 0; i < hoogte; i++)
        for(int j = 0; j < breedte; j++)
            if (A[i][j] == eenofnul)
                aantal_hokjes++;
    return aantal_hokjes;
}
bool bord::eenoppervlak(bool eenofnul)
{ //eenofnul geeft het te controleren oppervlak aan.
    int kopie[hoogte][breedte];
    int pos_x = 0, pos_y = 0, richting, grootte_opp = 1, aantal_hokjes, koe = 0;
    bool posveranderd = true;
    kopieerarray(A,kopie);
    aantal_hokjes = hokjestellen(eenofnul);
    if(startpositie(kopie, pos_x, pos_y, eenofnul))
    {
        while (posveranderd)
        {
            richting = routebepaling(kopie, pos_x, pos_y, eenofnul);
            if (richting)
                hokje_ernaast(pos_x, pos_y, richting);
            else
                doodlopend_pad(kopie, pos_x, pos_y, posveranderd);
            if (posveranderd && richting > 0)
                grootte_opp++;
        }
    }
    return (grootte_opp == aantal_hokjes);
}

bool bord::voldoet()
{
	return (zwartwit_verspreid(x, y) && eenoppervlak(true) && eenoppervlak(false));
}

bool bord::oplossing()
{
	bool mislukt, zwartwit;
	long int teller = 0;
	while (1)
	{
		if (!A[0][0]) // Z.d.a.t.s. geldt dat het eerste vakje zwart is.
			return false;
		nutteloos = false; //geeft aan of de fout zit in de laatstgeplaatste 1
		zwartwit = zwartwit_verspreid(x, y); //deze functie moet worden uitgevoerd voor "nutteloos"
		mislukt = false; // mislukt geeft aan of A[][] tot nu toe goed is.

		if (!randvoorwaarde() || !diagonaal())
			mislukt = true;

		if (x == breedte - 1 && y == hoogte - 1 && voldoet())
			return true; //oplossing gevonden
		else if (x == breedte - 1 && y == hoogte - 1)
			nutteloos = true;

		if (nutteloos) //zorgt ervoor dat de laatste twee zetten worden weggehaald
		{
			vorige();
			mislukt = true;
		}
		else if (!zwartwit || !eenoppervlak(false))
			mislukt = true;
		else if (eenoppervlak(true) && zwartwit_verspreid(breedte - 1, hoogte - 1))
			return true; //oplossing gevonden

		nieuw_A(mislukt); // maakt een nieuwe mogelijke oplossing aan
		if (teller % 200000 == 0) //om de zoveel tijd, toon A[][].
			print();
		teller++;
	}
}


int main()
{
	bord Bord;
	bool gelukt, doorzoeken = true, ingevuld;
	cout << "Beginnen vanaf een ingevulde rechthoek? (1 = ja, 0 = nee) ";
	cin >> ingevuld;
	if (ingevuld)
		Bord.invullen();
	Bord.print();
	while (doorzoeken)
	{
		doorzoeken = false;
		gelukt = Bord.oplossing();
		if (gelukt)
		{
			cout << "Er is een oplossing gevonden: " << endl << endl;
			Bord.print();
			cout << "Naar een andere oplossing zoeken? (0 = nee, 1 = ja) ";
			cin >> doorzoeken;
			if (doorzoeken)
			{
				if (Bord.einde())
				{
					Bord.vorige();
					Bord.nieuw_A(true);
				}
				else
					Bord.nieuw_A(false);
			}
		}
		else
			cout << "Geen gevonden oplossingen." << endl << endl;
	}
	return 0;
}
