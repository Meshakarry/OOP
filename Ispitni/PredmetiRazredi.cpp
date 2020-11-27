#include<iostream>
#include<vector>
#include<string>
#include<thread>
#include<regex>
#include<mutex>

using namespace std;
enum Predmeti { Matematika, Historija, Geografia, Hemija, Biologija, Engleski, Muzicko, Fizicko };
enum Razredi { I1, I2, I3, II1, II2, II3, III1, III2, III3, IV1, IV2, IV3 };
const char* ispisPredmeti[] = { "Matematika", "Historija", "Geografia", "Hemija", "Biologija", "Engleski", "Muzicko", "Fizicko" };
const char* ispisRazredi[] = { "I1", "I2", "I3", "II1", "II2", "II3", "III1", "III2", "III3", "IV1", "IV2", "IV3" };
const char* crt = "\n--------------------------------------------\n";
bool ProvjeriValidnostBrojaTelefona(string telefon) {//globalna funkcija
	/*
	broj telefona treba biti u formatu: +387(61)-222-333

	*/
	string pravilo = "[+](\\d{3})(\\(\\d{2}\\))[-](\\d{3})[-](\\d{3})";
	if (regex_match(telefon, regex(pravilo)))
		return true;

	return false;


}
template<class T1, class T2>
class Kolekcija {
	T1* _elementi1;
	T2* _elementi2;
	int _trenutno;
public:
	Kolekcija()
	{
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutno = 0;
	}
	Kolekcija(const Kolekcija& kol)
	{
		_trenutno = kol._trenutno;
		_elementi1 = new T1[_trenutno];
		_elementi2 = new T2[_trenutno];
		for (size_t i = 0; i < kol._trenutno; i++)
		{
			_elementi1[i] = kol._elementi1[i];
			_elementi2[i] = kol._elementi2[i];
		}
	}
	Kolekcija& operator=(Kolekcija& kol)
	{
		if (this != &kol)
		{
			delete[] _elementi1;
			delete[] _elementi2;
			_trenutno = kol._trenutno;
			_elementi1 = new T1[_trenutno];
			_elementi2 = new T2[_trenutno];
			for (size_t i = 0; i < kol._trenutno; i++)
			{
				_elementi1[i] = kol._elementi1[i];
				_elementi2[i] = kol._elementi2[i];
			}

		}
		return *this;
	}
	void AddElement(T1  a, T2 b) {
		T1* temp = new T1[_trenutno + 1];
		T2* temp2 = new T2[_trenutno + 1];
		for (size_t i = 0; i < _trenutno; i++)
		{
			temp[i] = _elementi1[i];
			temp2[i] = _elementi2[i];

		}
		delete[] _elementi1;
		delete[] _elementi2;
		_elementi1 = temp;
		_elementi2 = temp2;
		_elementi1[_trenutno] = a;
		_elementi2[_trenutno] = b;
		_trenutno++;

	}
	void RemoveElement(int lokacija)
	{
		for (size_t i = lokacija; i < _trenutno - 1; i++)
		{
			_elementi1[i] = _elementi1[i + 1];
			_elementi2[i] = _elementi2[i + 1];
		}
		_trenutno--;
	}
	T1 GetElement1(int i)const
	{
		return _elementi1[i];
	}
	T2 GetElement2(int i)const
	{
		return _elementi2[i];
	}
	int GetTrenutno()const { return _trenutno; }
	friend ostream& operator<<(ostream& output, Kolekcija& obj)
	{
		for (size_t i = 0; i < obj.GetTrenutno(); i++)
		{
			output << obj.GetElement1(i) << " " << obj.GetElement2(i) << endl;
		}
		return output;
	}
	~Kolekcija()
	{
		delete[]_elementi1;
		delete[]_elementi2;
	}
};
class Datum {
	int* _dan, * _mjesec, * _godina;
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}

	Datum(const Datum& obj) {
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
	}
	Datum& operator=(Datum& d)
	{
		if (this != &d)
		{
			*_dan = *d._dan;
			*_mjesec = *d._mjesec;
			*_godina = *d._godina;
		}
		return *this;
	}

	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}


	friend ostream& operator<< (ostream& COUT, const Datum& obj) {
		COUT << *obj._dan << " " << *obj._mjesec << " " << *obj._godina;
		return COUT;
	}
};
class Osoba {
protected:
	char* _imePrezime;
	Datum* _datumRodjenja;
	string _brojTelefona;
public:
	Osoba(const char* imePrezime, Datum datum) {
		_imePrezime = new char[strlen(imePrezime) + 1];
		strcpy_s(_imePrezime, strlen(imePrezime) + 1, imePrezime);
		_datumRodjenja = new Datum(datum);
		_brojTelefona = "";

	}
	Osoba(const Osoba& obj) :_brojTelefona(obj._brojTelefona) {
		_imePrezime = new char[strlen(obj._imePrezime) + 1];
		strcpy_s(_imePrezime, strlen(obj._imePrezime) + 1, obj._imePrezime);
		_datumRodjenja = new Datum(*obj._datumRodjenja);
	}
	~Osoba() {
		delete[]_imePrezime; delete _datumRodjenja;
		_imePrezime = nullptr;
		_datumRodjenja = nullptr;

	}
	Osoba& operator=(Osoba& obj)
	{
		if (this != &obj)
		{
			delete[] _imePrezime;
			_imePrezime = new char[strlen(obj._imePrezime) + 1];
			strcpy_s(_imePrezime, strlen(obj._imePrezime) + 1, obj._imePrezime);
			*_datumRodjenja = *obj._datumRodjenja;
			_brojTelefona = obj._brojTelefona;

		}
		return *this;
	}
	friend ostream& operator<<(ostream& output, Osoba& o)
	{
		output << "Ime i prezime ->" << o._imePrezime << endl;
		output << "Broj telefona ->" << o._brojTelefona << endl;
		output << " Datum rodjenja : " << *o._datumRodjenja << endl;
		return output;

	}
	bool setBrojTelefona(string brojTelefona) {
		if (ProvjeriValidnostBrojaTelefona(brojTelefona)) {
			_brojTelefona = brojTelefona;
			return true;
		}
		return false;
	}
	bool operator==(const Osoba& o)
	{
		return strcmp(_imePrezime, o._imePrezime) == 0;
	}
	char* GetIme()const { return _imePrezime; }

};

class Nastavnik :public Osoba {
	Kolekcija<Predmeti, Razredi>* _predmetiRazredi;
public:
	Nastavnik(const char* ime, Datum  d) :Osoba(ime, d)
	{
		_predmetiRazredi = new Kolekcija < Predmeti, Razredi>();
	}
	Nastavnik(const Nastavnik& n) :Osoba(n)
	{
		_predmetiRazredi = new Kolekcija < Predmeti, Razredi>(*n._predmetiRazredi);

	}
	Nastavnik& operator=(Nastavnik& n)
	{
		if (this != &n)
		{
			(Osoba&)(*this) = n;
			*_predmetiRazredi = *n._predmetiRazredi;

		}
		return *this;
	}
	bool operator==(Nastavnik& novi)
	{
		return static_cast<Osoba>(novi) == static_cast<Osoba>(*this);
	}
	/*NASTAVNIK MOZE PREDAVATI ISTI PREDMET, ALI TO MORA BITI U RAZLICITIM ODJELJENJIMA I NAJVISE U DVA RAZREDA (NA DVIJE RAZLICITE GODINE)*/

	bool AddPredmetRazred(Predmeti p, Razredi r)
	{
		int brojac = 0;
		for (size_t i = 0; i < _predmetiRazredi->GetTrenutno(); i++)
		{
			if (_predmetiRazredi->GetElement1(i) == p && _predmetiRazredi->GetElement2(i) == r)
				return false;
			if (_predmetiRazredi->GetElement1(i) == p)
				brojac++;
		}
		if (brojac == 2)
			return false;
		_predmetiRazredi->AddElement(p, r);
		return true;
	}
	bool RemovePredmetRazred(Predmeti izbrisat)
	{
		for (size_t i = 0; i < _predmetiRazredi->GetTrenutno(); i++)
		{
			if (_predmetiRazredi->GetElement1(i) == izbrisat)
				_predmetiRazredi->RemoveElement(i);
			return true;
		}
		return false;
	}
	Kolekcija < Predmeti, Razredi>& getPredmetiRazredi() { return *_predmetiRazredi; }
	friend ostream& operator<<(ostream& cout, Nastavnik& n)
	{
		cout << (Osoba&)(n) << endl;
		for (size_t i = 0; i < n._predmetiRazredi->GetTrenutno(); i++)
		{
			cout << ispisPredmeti[n._predmetiRazredi->GetElement1(i)] << " " << ispisRazredi[n._predmetiRazredi->GetElement2(i)] << endl;
		}
		return cout;

	}


};

class Ucenik : public Osoba {
	int _brojUDnevniku;
	Razredi _razred;
	vector<Nastavnik*> _nastavnici;
	Kolekcija<Predmeti, int> _predmetiOcjene;
public:
	Ucenik(const char* ime, Datum d, int broj, Razredi razred) :Osoba(ime, d), _razred(razred)
	{
		_brojUDnevniku = broj;
	}
	Ucenik(const Ucenik& u) :Osoba(u), _predmetiOcjene(u._predmetiOcjene)
	{
		_brojUDnevniku = u._brojUDnevniku;
		_razred = u._razred;
		_nastavnici = u._nastavnici;


	}
	//	/*VODITI RACUNA DA SE DODAJU REFERENCE NA NASTAVNIKE KAKO BI SVI UCENICI DIJELILI ISTE INFORMACIJE O NASTAVNICIMA 
	//U SLUCAJU DA SE NASTAVNIKU DODA NEKI PREDMET*/
	  /*ZA USPJESNO DODAVANJE, NASTAVNIK MORA POSJEDOVATI (PREDAVATI) NAJMANJE JEDAN PREDMET U TOM RAZREDU */

	bool AddNastavnik(Nastavnik& novi)
	{
		bool predajebarjedan = false;
		for (size_t i = 0; i < _nastavnici.size(); i++)
		{
			if (*_nastavnici[i] == novi)
				return false;
			for (size_t j = 0; j < novi.getPredmetiRazredi().GetTrenutno(); j++)
			{
				if (novi.getPredmetiRazredi().GetElement2(j) == _razred && novi.getPredmetiRazredi().GetElement1(j) < 1)
					return predajebarjedan;
			}

		}
		_nastavnici.push_back(&novi);
		return true;

	}
	
	bool AddPredmetOcjenu(Predmeti p, int ocjena)
	{
		bool predajem = false;
		for (size_t j = 0; j < _nastavnici.size(); j++)
		{
			if (_nastavnici[j]->getPredmetiRazredi().GetElement1(j) == p)
				predajem = true;
		}
		if (!predajem)
			return false;

		_predmetiOcjene.AddElement(p, ocjena);
		return true;

	}
	friend ostream& operator<<(ostream& output, Ucenik& u)
	{
		output << (Osoba&)(u) << endl;
		output << " broj u dnevniku je ->" << u._brojUDnevniku << endl;
		output << " Razred koji pohada je ->" << u._razred << endl;
		for (size_t i = 0; i < u._predmetiOcjene.GetTrenutno(); i++)
		{
			output << ispisPredmeti[u._predmetiOcjene.GetElement1(i)] << " " << u._predmetiOcjene.GetElement2(i) << endl;
		}
		return output;
	}
	vector<Nastavnik*>& getNastavnici() { return _nastavnici; }
	Kolekcija<Predmeti, int>& getPredmetiOcjene() { return _predmetiOcjene; }
	float GetProsjekByNastavnik(const  char* nastavnik)
	{
		float prosjek = 0.0;
		int broj = 0;
		for (size_t i = 0; i < _nastavnici.size(); i++)
		{
			if (strcmp(_nastavnici[i]->GetIme(), nastavnik) == 0)
			{
				for (size_t j = 0; j < _predmetiOcjene.GetTrenutno(); j++)
				{
					for (size_t k = 0; k < _nastavnici[i]->getPredmetiRazredi().GetTrenutno(); k++)
					{


						if (_nastavnici[i]->getPredmetiRazredi().GetElement1(k) == _predmetiOcjene.GetElement1(j))
						{
							prosjek += _predmetiOcjene.GetElement2(i);
							broj++;
						}
					}
				}
			}
		}
		return prosjek / float(broj);
	}

};


void main() {

	/****************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR CE BITI OZNACENO KAO "RE"
	3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKODJER NALAZI U FAJLU CODE_.TXT
	5. NAZIV I LISTA PARAMETARA FUNKCIJA MORAJU BITI IDENTIČNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U.
	6. NIJE POTREBNO BACATI IZUZETKE. SVE FUNKCIJE VRACAJU TRUE ILI FALSE
	****************************************************************************/

#pragma region TestiranjeDatuma
	Datum danas(26, 11, 2015);
	Datum sutra(danas);
	Datum prekosutra;
	prekosutra = danas;
	cout << danas << endl << sutra << endl << prekosutra << crt;
#pragma endregion
#pragma region TestiranjeKolekcije
	Kolekcija<int, int> kolekcija1;
	int brojElemenata = 15;
	for (size_t i = 0; i < brojElemenata; i++)
		kolekcija1.AddElement(i, i);
	kolekcija1.RemoveElement(0);//uklanja element na osnovu lokacije
	cout << kolekcija1.GetElement1(0) << " " << kolekcija1.GetElement2(0) << endl;
	cout << kolekcija1 << endl;
	Kolekcija<int, int> kolekcija2;
	kolekcija2 = kolekcija1;
	cout << kolekcija2 << crt;
	if (kolekcija1.GetTrenutno() == kolekcija2.GetTrenutno())
		cout << "ISTI BROJ ELEMENATA" << endl;
	Kolekcija<int, int> kolekcija3(kolekcija2);
	cout << kolekcija3 << crt;
#pragma endregion
#pragma region TestiranjeNastavnika
	Nastavnik jasmin("Jasmin Azemovic", Datum(15, 2, 1980));
	Nastavnik adel("Adel Handzic", Datum(15, 10, 1981));
	if (jasmin.AddPredmetRazred(Matematika, I1))
		cout << "Predmeti dodan" << endl;
	if (jasmin.AddPredmetRazred(Historija, I1))
		cout << "Predmeti dodan" << endl;

	if (jasmin.AddPredmetRazred(Geografia, I1))
		cout << "Predmeti dodan" << endl;

	if (jasmin.AddPredmetRazred(Geografia, I2))
		cout << "Predmeti dodan" << endl;

	if (adel.AddPredmetRazred(Historija, III1))
		cout << "Predmeti dodan" << endl;
	if (jasmin.AddPredmetRazred(Geografia, I3))
		cout << "Predmeti dodan" << endl;

	if (adel.RemovePredmetRazred(Historija))
		cout << "Predmet uklonjen!" << endl;
	//
	Nastavnik goran(adel);
	if (goran == adel)
		cout << "Nastavnici identicni" << endl;
	goran = jasmin;
	if (goran == jasmin)
		cout << "Nastavnici identicni" << endl;

	Kolekcija<Predmeti, Razredi>& predmetiRazredi = jasmin.getPredmetiRazredi();
	//
	if (!jasmin.setBrojTelefona("+38761111222"))
		cout << "Broj telefona nije dodan!" << endl;
	if (!jasmin.setBrojTelefona("+387-61-111-222"))
		cout << "Broj telefona nije dodan!" << endl;
	if (jasmin.setBrojTelefona("+387(61)-111-222"))
		cout << "Broj telefona uspjesno dodan!" << endl;
#pragma endregion
	//
#pragma region TestiranjeUcenika
	Ucenik denis("Denis Music", Datum(8, 10, 1990), 3, III1);
	Ucenik elmin("Elmin Sudic", Datum(15, 6, 1993), 1, I1);
	Ucenik adil("Adil Joldic", Datum(13, 8, 1992), 2, II1);
	if (denis.AddNastavnik(jasmin))
		cout << "Nastavnik uspjesno dodan" << endl;
	/*ONEMOGUCITI DODAVANJE ISTIH NASTAVNIKA; POJAM ISTI PODRAZUMIJEVA IDENTICNE VRIJEDNOSTI SVIH ATRIBUTA*/
	if (denis.AddNastavnik(adel))
		cout << "Nastavnik uspjesno dodan" << endl;
	//	/*DA BI SE DODALA OCJENA ZA ODREDJENI PREDMET UCENIK MORA POSJEDOVATI NASTAVNIKA KOJI PREDAJE TAJ PREDMET*/
	if (denis.AddPredmetOcjenu(Matematika, 3))
		cout << "Ocjena uspjesno dodana" << endl;
	if (denis.AddPredmetOcjenu(Historija, 4))
		cout << "Ocjena uspjesno dodana" << endl;
	//
	vector<Nastavnik*>& nastavnici = denis.getNastavnici();
	cout << "Ucenik Denis ima dodijeljena " << nastavnici.size() << " nastavnika" << endl;
	//
	Kolekcija<Predmeti, int>& uspjeh = denis.getPredmetiOcjene();
	cout << "Denis ima " << uspjeh.GetTrenutno() << " polozenih predmeta" << endl;
	cout << "Prosjecna ocjena kod nastavnika Jasmin Azemovic je " << denis.GetProsjekByNastavnik("Jasmin Azemovic") << endl;
	//
	Ucenik denis2(denis);
	cout << denis2 << endl;
	//
#pragma endregion
	system("pause");
}
