#include<iostream>
#include<string>
#include<vector>
#include<thread>
#include<regex>
#include<mutex>
#include<sstream>

using namespace std;

const char *crt = "\n-------------------------------------------\n";
enum stringFormat { DDMMGG, DDMMGGGG, DDMMGGSSMM, DDMMGGGGSSMM };
enum prikazPoruka { SVE_PORUKE, SAKRIJ_POBRISANE, PRIKAZI_SAMO_POBRISANE };
vector<string> nedozvoljeneRijeci;
/*
DDMMGG - 29.8.17
DDMMGGGG - 29.8.2017
DDMMGGGGSSMM - 29.8.2017 10:15
*/

template<class T1, class T2>
class Kolekcija {
	T1 * _elementi1;
	T2 * _elementi2;
	int * _trenutno;
public:
	Kolekcija()
	{
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutno = new int(0);
	}
	Kolekcija(const Kolekcija& orig)
	{
		_elementi1 = new T1[*orig._trenutno];
		_elementi2 = new T2[*orig._trenutno];
		_trenutno = new int(*orig._trenutno);

		for (int i = 0; i < *_trenutno; i++)
		{
			_elementi1[i] = orig._elementi1[i];
			_elementi2[i] = orig._elementi2[i];
		}
	}

	Kolekcija & operator=(Kolekcija<T1, T2> & orig)
	{
		if (this != &orig)
		{
			delete[]_elementi1;
			delete[]_elementi2;

			_elementi1 = new T1[*orig._trenutno];
			_elementi2 = new T2[*orig._trenutno];
			*_trenutno = *orig._trenutno;

			for (int i = 0; i < *_trenutno; i++)
			{
				_elementi1[i] = orig._elementi1[i];
				_elementi2[i] = orig._elementi2[i];
			}
		}
		return *this;
	}

	bool AddElement(T1 elem1, T2 elem2)
	{
		T1 * temp1 = new T1[*_trenutno + 1];
		T2 * temp2 = new T2[*_trenutno + 1];

		for (int i = 0; i < *_trenutno; i++) {
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}

		delete[] _elementi1;
		delete[] _elementi2;

		_elementi1 = temp1;
		_elementi2 = temp2;

		_elementi1[*_trenutno] = elem1;
		_elementi2[*_trenutno] = elem2;
		(*_trenutno)++;
		return true;
	}
	~Kolekcija()
	{
		delete[]_elementi1; _elementi1 = nullptr;
		delete[]_elementi2; _elementi2 = nullptr;
		delete _trenutno; _trenutno = nullptr;
	}

	T1 * getElementi1Pok() { return _elementi1; }
	T2 * getElementi2Pok() { return _elementi2; }
	T1 & getElement1(int lokacija) { return _elementi1[lokacija]; }
	T2 & getElement2(int lokacija) { return _elementi2[lokacija]; }
	int getTrenutno() { return *_trenutno; }

	friend ostream & operator<<(ostream & COUT, Kolekcija<T1, T2> & obj)
	{
		for (size_t i = 0; i < *obj._trenutno; i++)
		{
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		}
		return COUT;
	}
};
class DatumVrijeme {
	int *_dan, *_mjesec, *_godina, *_sati, *_minuti;
public:
	DatumVrijeme(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 0, int minuti = 0) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
		_sati = new int(sati);
		_minuti = new int(minuti);
	}
	DatumVrijeme(const DatumVrijeme & d)
	{
		_dan = new int(*d._dan);
		_mjesec = new int(*d._mjesec);
		_godina = new int(*d._godina);
		_sati = new int(*d._sati);
		_minuti = new int(*d._minuti);
	}
	DatumVrijeme & operator = (DatumVrijeme & d)
	{
		if (this != &d) {

			*_dan = *d._dan;
			*_mjesec = *d._mjesec;
			*_godina = *d._godina;
			*_sati = *d._sati;
			*_minuti = *d._minuti;
		}
		return *this;

	}
	~DatumVrijeme() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
		delete _sati; _sati = nullptr;
		delete _minuti; _minuti = nullptr;
	}
	string ToString(stringFormat ovaj)
	{
		stringstream obj;
		if (ovaj == DDMMGGGG)
		{
			obj << *_dan << "." << *_mjesec << "." << *_godina << endl;

		}
		else if (ovaj == DDMMGGGGSSMM)
		{
			obj << *_dan << "." << *_mjesec << "." << *_godina << " " << *_sati << ":" << *_minuti << endl;
		}
		return obj.str();
	}
	friend ostream & operator<< (ostream & COUT, DatumVrijeme & obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina << " " << *obj._sati << ":" << *obj._minuti << endl;
		return COUT;
	}
	int operator -(const DatumVrijeme& d)
	{
		return ((*_dan + *_mjesec * 30 + *_godina * 365) * 1440 + *_sati * 60 + *_minuti) - ((*d._dan + *d._mjesec * 30 + *d._godina * 365) * 1440 + *d._sati * 60 + *d._minuti);
	}
	
};
class Poruka {
	string _posiljalac;
	string _primalac;
	string _sadrzaj;
	DatumVrijeme _vrijeme;
public:
	Poruka() :_posiljalac("---"), _primalac("---"), _sadrzaj("---") {}
	Poruka(string posiljalac, string primalac, string sadrzaj, DatumVrijeme vrijeme) :_vrijeme(vrijeme)
	{
		_posiljalac = posiljalac;
		_primalac = primalac;
		_sadrzaj = sadrzaj;
	}
	friend ostream & operator<< (ostream & COUT, Poruka & obj) {
		COUT << "(" << obj._vrijeme << ")" << endl;

			COUT<< obj._posiljalac << " -> ";
		COUT << obj._sadrzaj;
		return COUT;
	}
	string getPrimalac() { return _primalac; }
	string getPosiljalac() { return _posiljalac; }
	string getSadrzaj() { return _sadrzaj; }
	DatumVrijeme getTime() { return _vrijeme; }
};
class Komunikacija {
	string _naslov;
	int _ocjena; //ocjena komunikacije 1 - 5 se automatski izracunava
	Kolekcija<Poruka, bool> _poruke;//bool se odnosi na podatak da li je poruka pobrisana tj. nece biti vidljiva u prikazu ukoliko se drugacije ne navede
public:
	Komunikacija(string naslov = "") {
		_naslov = naslov;
		_ocjena = -1;
	}
	friend ostream & operator<< (ostream & COUT, Komunikacija & obj) {
		COUT << "(" << obj._ocjena << ") " << obj._naslov << " -> ";
		for (size_t i = 0; i < obj._poruke.getTrenutno(); i++)
		{
			if (!obj._poruke.getElement2(i))
			{
				COUT << obj._poruke.getElement1(i) << endl;
			}
		}
		return COUT;
	}
	Kolekcija<Poruka, bool> * getPoruke() { return &_poruke; }
	int getOcjena() { return _ocjena; }
	void SetOcjena(int ocjena) { _ocjena = ocjena; }
	string getNaslov() { return _naslov; }
};

class Korisnik {
	string _korisnickoIme;
	char * _imePrezime;
	//char * - se odnosi na naziv predmeta ili tema gdje će se čuvati kompletna komunikacija sa nastavnim osobljem 
	//ili korisnikom na određenom predmetu/temi
	Kolekcija<char *, Komunikacija> _komunikacija;
public:
	Korisnik(const char * korisnickoIme, const char * imePrezime)
	{
		int vel = strlen(imePrezime) + 1;
		_imePrezime = new char[vel];
		strcpy_s(_imePrezime, vel, imePrezime);
		_korisnickoIme = korisnickoIme;
	}
	Korisnik(Korisnik & original) :_komunikacija(original._komunikacija)
	{
		int vel = strlen(original._imePrezime) + 1;
		_imePrezime = new char[vel];
		strcpy_s(_imePrezime, vel, original._imePrezime);
		_korisnickoIme = original._korisnickoIme;
	}

	~Korisnik()
	{
		delete[] _imePrezime; _imePrezime = nullptr;

	}
	friend ostream & operator<< (ostream &COUT, Korisnik &obj)
	{
		COUT << obj._imePrezime << endl << obj._komunikacija << endl;
		return COUT;
	}
	void PrikaziKomunikacijuPoNaslovu(string naslov, prikazPoruka vidljivost)
	{
		if (vidljivost == SVE_PORUKE) {
			for (size_t i = 0; i < _komunikacija.getTrenutno(); i++)
			{
				if (strcmp(_komunikacija.getElement1(i), naslov.c_str()) == 0)
				{
					for (size_t j = 0; j < _komunikacija.getElement2(i).getPoruke()->getTrenutno(); j++)
					{
						if (!_komunikacija.getElement2(i).getPoruke()->getElement2(j))
						{
							cout << _komunikacija.getElement2(i).getPoruke()->getElement1(j) << crt;
						}
						else {
							cout << "poruka je uspjesno izbrisana";
						}
					}
				}
			}
		}
		if (vidljivost == SAKRIJ_POBRISANE)
		{
			for (size_t i = 0; i < _komunikacija.getTrenutno(); i++)
			{
				if (strcmp(_komunikacija.getElement1(i), naslov.c_str()) == 0)
				{
					for (size_t j = 0; j < _komunikacija.getElement2(i).getPoruke()->getTrenutno(); j++)
					{
						if (!_komunikacija.getElement2(i).getPoruke()->getElement2(j))
						{
							cout << _komunikacija.getElement2(i).getPoruke()->getElement1(j) << crt;
						}
					}
				}
			}
		}
		if (vidljivost == PRIKAZI_SAMO_POBRISANE)
		{
			for (size_t i = 0; i < _komunikacija.getTrenutno(); i++)
			{
				if (strcmp(_komunikacija.getElement1(i), naslov.c_str()) == 0)
				{
					for (size_t j = 0; j < _komunikacija.getElement2(i).getPoruke()->getTrenutno(); j++)
					{
						if (_komunikacija.getElement2(i).getPoruke()->getElement2(j))
						{
							cout << _komunikacija.getElement2(i).getPoruke()->getElement1(j) << crt;
						}
					}
				}
			}
		}
	}
	int PrikaziKomunikacijuSaKorisnikom(string username)
	{
		int brojporuka = 0;

		for (size_t i = 0; i < _komunikacija.getTrenutno(); i++)
		{
			for (size_t j = 0; j < _komunikacija.getElement2(i).getPoruke()->getTrenutno(); j++)
			{
				if (_komunikacija.getElement2(i).getPoruke()->getElement1(j).getPosiljalac() == username || _komunikacija.getElement2(i).getPoruke()->getElement1(j).getPrimalac() == username)
				{
					cout << endl;
					cout << _komunikacija.getElement2(i).getPoruke()->getElement1(j) << crt;
					brojporuka++;
				}
			}
		}
		return brojporuka;
	}
	//prosjek > 20 minuta = ocjena 1
	//prosjek > 15 minuta = ocjena 2
	//prosjek > 10 minuta = ocjena 3
	//prosjek > 5 minuta = ocjena 4
	//prosjek <=5 minuta = ocjena 5
	void OcijeniKomunikaciju(string naslov)
	{
		float prosjek = 0;
		int brojim = 0;
		for (size_t i = 0; i < _komunikacija.getTrenutno(); i++)
		{
			if (_komunikacija.getElement1(i) == naslov) {
				for (size_t j = 0; j < _komunikacija.getElement2(i).getPoruke()->getTrenutno() - 1; j++)
				{
					prosjek += (_komunikacija.getElement2(i).getPoruke()->getElement1(j).getTime() - _komunikacija.getElement2(i).getPoruke()->getElement1(j + 1).getTime());
					brojim++;
				}
				prosjek /= float(brojim);
				if (prosjek > 20)
					_komunikacija.getElement2(i).SetOcjena(1);
				else if (prosjek > 15)
					_komunikacija.getElement2(i).SetOcjena(2);
				else if (prosjek > 10)
					_komunikacija.getElement2(i).SetOcjena(3);
				else if (prosjek > 5)
					_komunikacija.getElement2(i).SetOcjena(4);
				else 
					_komunikacija.getElement2(i).SetOcjena(5);
				

				
					
			}
		}
		
	}
	Kolekcija<char*, Komunikacija> * GetKomunikacija() { return &_komunikacija; }
	string getKorisnickoIme() { return _korisnickoIme; }
};
//sve poruke se cuvaju u komunikaciji posiljaoca i primaoca tako da u svakom momentu oba ucesnika u komunikaciji posjeduju kompletnu historiju poruka.poruke se grupisu na osnovu naslova, pa ce poruke pod naslovom "Sastanak" izmedju dva korisnika sadrzavati sve poruke koje su razmijenjene na tu temu
//u slucaju da posiljalac ili primalac ne postoje funkcija treba baciti izuzetak sa odgovarajucom porukom. takodjer,
//koristeci regex sve poruke koje sadrze neku od zabranjenih rijeci trebaju biti oznacene kao
//*
void PosaljiPoruku(Korisnik ** user, int vel, string naziv, string primaoc, string posiljaoc, string sadrzaj, DatumVrijeme  dejt) {

	string postoji = "";
	bool prvaOsoba = false;
	bool DrugaOsoba = false;
	bool komunikacija = false;
	Poruka zaslanje(primaoc, posiljaoc, sadrzaj, dejt);

	for (size_t i = 0; i < nedozvoljeneRijeci.size(); i++)
	{
		postoji += nedozvoljeneRijeci[i] + "|";
	}
	postoji += nedozvoljeneRijeci[nedozvoljeneRijeci.size() - 1];

	if (regex_search(sadrzaj, regex(postoji)))
		throw exception("u sadrzaju postoji zabranjena rijec");
	for (size_t i = 0; i < vel; i++)
	{
		if (user[i]->getKorisnickoIme() == primaoc)
			DrugaOsoba = true;
		if (user[i]->getKorisnickoIme() == posiljaoc)
			prvaOsoba = true;

	}
	if (!prvaOsoba || !DrugaOsoba)
		throw exception("nema utvrdene komunikacije");
	for (size_t i = 0; i < vel; i++)
	{
		if (user[i]->getKorisnickoIme() == primaoc && user[i]->getKorisnickoIme() == posiljaoc)
		{

			for (size_t j = 0; j < user[i]->GetKomunikacija()->getTrenutno(); j++)
			{


				if (strcmp(naziv.c_str(), user[i]->GetKomunikacija()->getElement1(j)) == 0) {

					user[i]->GetKomunikacija()->getElement2(j).getPoruke()->AddElement(zaslanje, false);
					komunikacija = true;

				}

			}
		}
		if (!komunikacija)
		{
			char *naslov = new char[strlen(naziv.c_str()) + 1];
			strcpy_s(naslov, strlen(naziv.c_str()) + 1, naziv.c_str());

			Komunikacija nova(naslov);
			Kolekcija<Poruka, bool> *porukice = nova.getPoruke();
			porukice->AddElement(zaslanje, false);
			user[i]->GetKomunikacija()->AddElement(naslov, nova);

		}


	}

}
void main()
{

	/****************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ĆE BITI OZNACENO KAO "RE"
	3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEČILO GUBLJENJE URAĐENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKOĐER NALAZI U FAJLU CODE.TXT
	5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIČNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUČAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOĆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
	6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAČENO.
	****************************************************************************/


	/*riječi koje su zabranjene u komunikaciji*/
	nedozvoljeneRijeci.push_back("ubiti");
	nedozvoljeneRijeci.push_back("mrzim");
	nedozvoljeneRijeci.push_back("rat");

	DatumVrijeme temp,
		datum19062017_1015(19, 6, 2017, 10, 15),
		datum19062017_1020(19, 6, 2017, 10, 20),
		datum19062017_1021(19, 6, 2017, 10, 21),
		datum19062017_1030(19, 6, 2017, 10, 30);

	cout << datum19062017_1015.ToString(DDMMGGGG) << endl;//vraca string: 19.6.2017
	cout << datum19062017_1015.ToString(DDMMGGGGSSMM) << endl;//vraca string: 19.6.2017 10:15


	const int max = 3;
	Korisnik * korisnici[max];
	korisnici[0] = new Korisnik("jasmin", "Jasmin Azemovic");
	korisnici[1] = new Korisnik("adel", "Adel Handzic");
	korisnici[2] = new Korisnik("denis", "Denis Music");

	///*
	//sve poruke se cuvaju u komunikaciji posiljaoca i primaoca tako da u svakom momentu oba ucesnika u komunikaciji posjeduju kompletnu historiju poruka. poruke se grupisu na osnovu naslova, pa ce poruke pod naslovom "Sastanak" izmedju dva korisnika sadrzavati sve poruke koje su razmijenjene na tu temu
	//u slucaju da posiljalac ili primalac ne postoje funkcija treba baciti izuzetak sa odgovarajucom porukom. takodjer,
	//koristeci regex sve poruke koje sadrze neku od zabranjenih rijeci trebaju biti oznacene kao
	//*


	PosaljiPoruku(korisnici, max, "Sastanak",
		korisnici[0]->getKorisnickoIme(), korisnici[1]->getKorisnickoIme(),
		"Danas imamo sastanak u 10h!", datum19062017_1015);

	PosaljiPoruku(korisnici, max, "Sastanak",
		korisnici[1]->getKorisnickoIme(), korisnici[0]->getKorisnickoIme(),
		"Naravno, vidimo se u sali za sastanke!", datum19062017_1020);

	PosaljiPoruku(korisnici, max, "Sastanak",
		korisnici[0]->getKorisnickoIme(), korisnici[1]->getKorisnickoIme(),
		"Sa sobom ponesi radni materijal!", datum19062017_1021);

	///*ispisuje sve poruke koje je korisnik razmijenio pod naslovom definisanim parametrom. ispis komunikacije
	//treba biti organizovan po datumu na nacin da se prvo prikaze datum, a nakon toga sve poruke koje su razmijenjene tog datuma.
	//Na mjestima pobrisanih poruka prikazati tekst "PORUKA IZBRISANA"
	//*/
	korisnici[0]->PrikaziKomunikacijuPoNaslovu("Sastanak", SVE_PORUKE);

	///*funkcija prikazuje sve poruke sa korisnikom cije korisnicko ime je definisano parametrom, te vraca ukupan broj (razmijenjenih poruka sa tim korisnikom)*/
	int ukupnoPoruka = korisnici[0]->PrikaziKomunikacijuSaKorisnikom("adel");
	cout << endl;
	cout <<"ukupan broj poruka je -> "<< ukupnoPoruka << endl;

	///*formira ocjenu na nivou komunikacije na nacin da izracunava prosjecno vrijeme odgovora drugog ucesnika izrazeno u minutama i to prema sljedecoj skali
	//prosjek > 20 minuta = ocjena 1
	//prosjek > 15 minuta = ocjena 2
	//prosjek > 10 minuta = ocjena 3
	//prosjek > 5 minuta = ocjena 4
	//prosjek <=5 minuta = ocjena 5
	//*/
	korisnici[0]->OcijeniKomunikaciju("Sastanak");


	for (size_t i = 0; i < max; i++)
	{
		delete korisnici[i];
		korisnici[i] = nullptr;
	}

	system("PAUSE");
}
