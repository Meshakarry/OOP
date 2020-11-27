#include<iostream>
#include<thread>
#include<mutex>
#include<regex>
#include<vector>
#include<string>
#include<sstream>


using namespace std;
//text-text@hotmail.com|outlok.com|gmail.com
//text podrazumijeva i mala i velika slova
bool ValidirajEmail(string adresa)
{
	string pravilo = "([a-zA-Z])[-]([a-zA-Z])[@](hotmail.com|outlook.com|fit.ba|gmail.com)";
	if (regex_match(adresa, regex(pravilo)))
		return true;
	return false;
}
const char *crt = "\n-------------------------------------------\n";
enum GodinaStudija { PRVA = 1, DRUGA, TRECA };

char * Alociraj(const char * sadrzaj) {
	if (sadrzaj == nullptr)return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

template<class T1, class T2>
class Dictionary {
	T1 * _elementi1;
	T2 * _elementi2;
	int * _trenutno;
public:
	Dictionary() {
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutno = new int(0);
	}

	Dictionary(const Dictionary & d)
	{
		_trenutno = new int(*d._trenutno);
		_elementi1 = new T1[*d._trenutno];
		_elementi2 = new T2[*d._trenutno];

		for (size_t i = 0; i < *d._trenutno; i++)
		{
			_elementi1[i] = d._elementi1[i];
			_elementi2[i] = d._elementi2[i];

		}
	}
	Dictionary & operator=(Dictionary & d)
	{
		if (this != &d)
		{
			*_trenutno = *d._trenutno;
			if (_elementi1 != nullptr)
				delete[] _elementi1;
			if (_elementi2 != nullptr)
				delete[] _elementi2;
			_elementi1 = new T1[*d._trenutno];
			_elementi2 = new T2[*d._trenutno];

			for (size_t i = 0; i < *d._trenutno; i++)
			{
				_elementi1[i] = d._elementi1[i];
				_elementi2[i] = d._elementi2[i];

			}

		}
		return *this;
	}
	~Dictionary() {
		delete[]_elementi1; _elementi1 = nullptr;
		delete[]_elementi2; _elementi2 = nullptr;
		delete _trenutno; _trenutno = nullptr;
	}
	void AddElement(T1 a, T2 b)
	{
		T1 *pok = new T1[*_trenutno + 1];
		T2 *temp = new T2[*_trenutno + 1];

		for (size_t i = 0; i < *_trenutno; i++)
		{
			pok[i] = _elementi1[i];
			temp[i] = _elementi2[i];
		}
		delete[] _elementi1;
		delete[] _elementi2;
		_elementi1 = pok;
		_elementi2 = temp;
		_elementi1[*_trenutno] = a;
		_elementi2[*_trenutno] = b;
		(*_trenutno)++;

	}
	//vraca elemente kolekcije koji se nalaze na lokacijama definisanim vrijednostima parametara (npr. 2 - 7). 
	//funkcija baca izuzetak u slucaju da se zahtijeva lokacija koja ne postoji ili je vrijednost posljednje lokacije manja od pocetne

	Dictionary<T1, T2> getRange(int prvaLok, int drugaLok)
	{
		Dictionary<T1, T2> opseg;
		if (prvaLok < 0 || drugaLok < 0 || drugaLok < prvaLok)
			throw exception("nevalidni argumenti");
		if (prvaLok > *_trenutno || drugaLok > *_trenutno)
			throw exception("previse elemenata");
		for (size_t i = prvaLok; i <= drugaLok; i++)
		{
			opseg.AddElement(_elementi1[i], _elementi2[i]);
		}
		return opseg;
	}

	T1& getElement1(int lokacija)const { return _elementi1[lokacija]; }
	T2& getElement2(int lokacija)const { return _elementi2[lokacija]; }
	int getTrenutno() { return *_trenutno; }
	friend ostream& operator<< (ostream &COUT, const Dictionary &obj) {
		for (size_t i = 0; i < *obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
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
	~DatumVrijeme() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
		delete _sati; _sati = nullptr;
		delete _minuti; _minuti = nullptr;
	}
	DatumVrijeme(const DatumVrijeme & dv)
	{
		_dan = new int(*dv._dan);
		_mjesec = new int(*dv._mjesec);
		_godina = new int(*dv._godina);
		_sati = new int(*dv._sati);
		_minuti = new int(*dv._minuti);
	}
	DatumVrijeme & operator=(DatumVrijeme & d)
	{
		if (this != &d)
		{
			*_dan = *d._dan;
			*_mjesec = *d._mjesec;
			*_godina = *d._godina;
			*_sati = *d._sati;
			*_minuti = *d._minuti;
		}
		return *this;
	}
	bool operator==(const DatumVrijeme &d1)
	{
		return *_dan == *d1._dan && *_mjesec == *d1._mjesec && *_godina == *d1._godina && *_sati == *d1._sati && *_minuti == *d1._minuti;
	}
	char * ToCharArray()
	{
		stringstream ispis;
		ispis << *_dan << "/" << *_mjesec << "/" << *_godina << " " << *_sati << ":" << *_minuti;

		return Alociraj(ispis.str().c_str());
	}
	friend ostream& operator<< (ostream &COUT, const DatumVrijeme &obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina << " " << *obj._sati << ":" << *obj._minuti << endl;
		return COUT;
	}
	int getDateInHours()
	{
		return *_sati + *_dan * 24 + *_mjesec * 720 + *_godina * 8640;
	}
};

class Predmet {
	char * _naziv;
	int _ocjena;
	string _napomena;
public:
	Predmet(const char * naziv = "", int ocjena = 0, string napomena = "") {
		_naziv = Alociraj(naziv);
		_ocjena = ocjena;
		_napomena = napomena;
	}
	Predmet(const Predmet & p)
	{
		_naziv = Alociraj(p._naziv);
		_ocjena = p._ocjena;
		_napomena = p._napomena;
	}
	Predmet & operator=(Predmet & p)
	{
		if (this != &p)
		{
			delete[] _naziv;

			_naziv = Alociraj(p._naziv);
			_ocjena = p._ocjena;
			_napomena = p._napomena;
		}
		return *this;
	}
	~Predmet() {
		delete[] _naziv; _naziv = nullptr;
	}
	friend ostream& operator<< (ostream &COUT, Predmet &obj) {
		COUT << obj._naziv << " (" << obj._ocjena << ") " << obj._napomena << endl;
		return COUT;
	}
	string GetNapomena() { return _napomena; }
	char * GetNaziv() { return _naziv; }
	int GetOcjena() { return _ocjena; }

	void DodajNapomenu(string napomena) {
		_napomena += " " + napomena;
	}
	bool operator ==(const Predmet & p)
	{
		return strcmp(_naziv, p._naziv) == 0;
	}
};
class Uspjeh {
	GodinaStudija * _godina;
	//datumvrijeme se odnosi na vrijeme evidentiranja polozenog predmeta
	Dictionary<Predmet, DatumVrijeme> _predmeti;
public:
	Uspjeh(GodinaStudija godina) {
		_godina = new GodinaStudija(godina);
	}
	Uspjeh(const Uspjeh & u) :_predmeti(u._predmeti) {
		_godina = new GodinaStudija(*u._godina);
	}
	~Uspjeh() { delete _godina; _godina = nullptr; }

	Dictionary<Predmet, DatumVrijeme> * GetPredmeti() { return &_predmeti; }
	GodinaStudija * GetGodinaStudija() { return _godina; }
	friend ostream& operator<< (ostream &COUT, const Uspjeh &obj) {
		COUT << *obj._godina << " " << obj._predmeti << endl;
		return COUT;
	}
};
mutex m;
void Mail(string sadrzaj)
{
	m.lock();
	cout << sadrzaj << endl;
	m.unlock();
	
}
class Student {
	char * _imePrezime;
	string _emailAdresa;
	string _brojTelefona;
	vector<Uspjeh> _uspjeh;
public:
	Student(const char * imePrezime, string emailAdresa, string brojTelefona) {
		_imePrezime = Alociraj(imePrezime);
		try {
			if (ValidirajEmail(emailAdresa))
			{
				_emailAdresa = emailAdresa;
			}
		}
		catch (exception &err)
		{
			cout << err.what() << endl;
		}
		_brojTelefona = brojTelefona;
	}
	~Student() {
		delete[] _imePrezime; _imePrezime = nullptr;
	}
	friend ostream& operator<< (ostream &COUT, Student &obj) {
		COUT << obj._imePrezime << " " << obj._emailAdresa << " " << obj._brojTelefona << endl;
		ostream_iterator<Uspjeh> ja(COUT, crt);

		copy(obj._uspjeh.begin(), obj._uspjeh.end(), ja);

		return COUT;
	}
	vector<Uspjeh> * GetUspjeh() { return &_uspjeh; }
	string GetEmail() { return _emailAdresa; }
	string GetBrojTelefona() { return _brojTelefona; }
	char * GetImePrezime() { return _imePrezime; }

	bool AddPredmet(Predmet & p, GodinaStudija year, DatumVrijeme & d)
	{
		bool dodan = false;
		for (size_t i = 0; i < _uspjeh.size(); i++)
		{
			if (*_uspjeh[i].GetGodinaStudija() == year)
			{
				for (size_t j = 0; j < _uspjeh[i].GetPredmeti()->getTrenutno(); j++)
				{
					if (_uspjeh[i].GetPredmeti()->getElement1(j) == p)
						return false;
					if (d.getDateInHours() <= _uspjeh[i].GetPredmeti()->getElement2(j).getDateInHours())
						return false;
				}
				_uspjeh[i].GetPredmeti()->AddElement(p, d);


				dodan = true;
			}
		}
		if (!dodan)
		{
			Uspjeh novi(year);
			novi.GetPredmeti()->AddElement(p, d);
			_uspjeh.push_back(novi);
			
			//FROM:info@fit.ba
	//TO: emailStudenta
	//Postovani ime i prezime, evidentirali ste uspjeh za X godinu studija.
	//Pozdrav.
	//FIT Team.
		}
		stringstream ispis;
		ispis << "FROM:info@fit.ba" << endl << "TO:emailStudenta" << endl << "Postovani" << _imePrezime << "," << "evidentirali ste uspjeh za " << year << " godinu studija." << endl << "Pozdrav." << endl << "Fit tim";
		thread tred(&Mail,ispis.str());
		string uspjeh;
		float konacna = 0.0;
		int borj = 0;
		for (vector<Uspjeh>::iterator it = _uspjeh.begin(); it != _uspjeh.end(); it++)
		{
			if (year == *it->GetGodinaStudija())
			{
				for (size_t i = 0; i < it->GetPredmeti()->getTrenutno(); i++)
				{
					konacna += it->GetPredmeti()->getElement1(i).GetOcjena();
					borj++;
				}
				float prosjek = konacna / borj;

				if (prosjek > 8)
				{
					 uspjeh = "svaka cast za uspjeh" + to_string(prosjek) + "ostvaren na godinu studija" + to_string(year);

				}
			}
		}
		thread djani(&Mail, uspjeh);

		tred.join();
		djani.join();
		
		
		return true;
	}
	int BrojPonavljanjaRijeci(string ova)
	{
		int brojac = 0;
		for (size_t i = 0; i < _uspjeh.size(); i++)
		{

			for (size_t j = 0; j < _uspjeh[i].GetPredmeti()->getTrenutno(); j++)
			{


				regex r("(""\\b" + ova + "\\b|\\B" + ova + "\\B"")");
				string napomena = _uspjeh[i].GetPredmeti()->getElement1(j).GetNapomena();
				auto pocetak = sregex_iterator(napomena.begin(),napomena.end(), r);
				auto end = sregex_iterator();
				brojac += distance(pocetak, end);

			}

		}
		return brojac;
	}
	vector<Predmet> operator ()(DatumVrijeme * od, DatumVrijeme * Do)
	{
		vector<Predmet> svi;

		for (size_t i = 0; i < _uspjeh.size(); i++)
		{
			for (size_t j = 0; j < _uspjeh[i].GetPredmeti()->getTrenutno(); j++)
			{
				if (_uspjeh[i].GetPredmeti()->getElement2(j).getDateInHours() > od->getDateInHours() && Do->getDateInHours() > _uspjeh[i].GetPredmeti()->getElement2(j).getDateInHours()) {
					svi.push_back(_uspjeh[i].GetPredmeti()->getElement1(j));
				}
			}
		}
		return svi;
	}
	Uspjeh *operator[](string godina)
	{
		for (size_t i = 0; i < _uspjeh.size(); i++)
		{
			if (godina == "PRVA"&& *_uspjeh[i].GetGodinaStudija() == PRVA)
				return &_uspjeh[i];
			if (godina == "DRUGA"&& *_uspjeh[i].GetGodinaStudija() == DRUGA)
				return &_uspjeh[i];
			if (godina == "TRECA"&& *_uspjeh[i].GetGodinaStudija() == TRECA)
				return &_uspjeh[i];
		}
		return nullptr;
	}
		
};

void main() {
	/****************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ĆE BITI OZNACENO KAO "RE"
	3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEČILO GUBLJENJE URAĐENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKOĐER NALAZI U FAJLU CODE.TXT
	5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIČNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUČAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOĆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
	6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAČENO.
	****************************************************************************/
	cout << "NA KRAJU ISPITA, RAD PREDAJTE U ODGOVARAJUCI FOLDER NA FTP SERVERU (INTEGRALNI)!" << endl;
	DatumVrijeme temp,
		datum19062019_1015(19, 6, 2019, 10, 15),
		datum20062019_1115(20, 6, 2019, 11, 15),
		datum30062019_1215(30, 6, 2019, 12, 15),
		datum05072019_1231(5, 7, 2019, 12, 31);
	//funkcija ToCharArray vraca datum i vrijeme kao char *. konverziju izvrsiti koristeci stringstream objekat.
	//voditi racuna o tome da se izmedju datuma i vremena nalazi samo jedan razmak, te da su vrijednosti dana i mjeseca iskazane kao dvije cifre
	cout << datum19062019_1015.ToCharArray() << endl;//treba ispisati: 19/06/2019 10:15
	temp = datum05072019_1231;
	cout << temp.ToCharArray() << endl;//treba ispisati: 05/07/2019 12:31

	const int DictionaryTestSize = 9;
	Dictionary<int, int> Dictionary1;
	for (size_t i = 0; i < DictionaryTestSize; i++)
		Dictionary1.AddElement(i + 1, i*i);

	try {
		//vraca elemente kolekcije koji se nalaze na lokacijama definisanim vrijednostima parametara (npr. 2 - 7). 
		//funkcija baca izuzetak u slucaju da se zahtijeva lokacija koja ne postoji ili je vrijednost posljednje lokacije manja od pocetne
		cout << "ja sam kolekcija u tom opsegu -----" << endl;
		Dictionary<int, int> opseg = Dictionary1.getRange(2, 7);
		cout << opseg << endl;
		Dictionary1.getRange(7, 11);
	}
	catch (exception& err) {
		cout << err.what() << endl;
	}
	cout << Dictionary1 << endl;

	Dictionary<int, int> Dictionary2 = Dictionary1;
	cout << Dictionary2 << crt;

	Dictionary<int, int> Dictionary3;
	Dictionary3 = Dictionary1;
	cout << Dictionary3 << crt;

	//napomena se moze dodati i prilikom kreiranja objekta
	Predmet MAT("Matematika", 7, "Ucesce na takmicenju"),
		UIT("Uvod u informacijske tehnologije", 9),
		RM("Racunarske mreze", 8),
		EN("Engleski jezik", 6);
	UIT.DodajNapomenu("Pohvala za ostvareni uspjeh");
	cout << MAT << endl;

	///*
	//email adresa mora biti u formatu: text-text@ nakon cega slijedi neka od sljedecih domena: hotmail.com ili outlook.com ili fit.ba. Pod text se podrazumijeva bilo koje slovo, malo ili veliko.
	//u slucaju da email adresa nije validna, postaviti je na defaultnu: notSet@fit.ba
	//za provjeru koristiti regex
	//*/
	Student jasmin("Jasmin Azemovic", "jasmin-azemovic@hotmail.com", "033 281 172");
	Student adel("Adel Handzic", "adel-handzic@fit.ba", "033 281 170");
	Student emailNotValid("Ime Prezime", "korisnik@lazna.ba", "033 281 170");

	///*
	//uspjeh se dodaje za svaki predmet na nivou godine studija.
	//tom prilikom onemoguciti:
	//- dodavanje istoimenih predmeta na nivou jedne godine,
	//- dodavanje vise predmeta u kratkom vremenskom periodu (na nivou jedne godine, razmak izmedju dodavanja pojedinih predmeta mora biti najmanje 1 sat).
	//godine (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za drugu godinu, pa onda za prvu godinu i sl.).
	//Funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
	//*/
	if (jasmin.AddPredmet(UIT, DRUGA, datum20062019_1115))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(RM, DRUGA, datum30062019_1215))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(EN, PRVA, datum19062019_1015))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(MAT, PRVA, datum20062019_1115))
		cout << "Predmet uspjesno dodan!" << crt;
	//ne treba dodati MAT jer je vec dodana u prvoj godini
	if (jasmin.AddPredmet(MAT, PRVA, datum05072019_1231))
		cout << "Predmet uspjesno dodan!" << crt;
	//ne treba dodati UIT jer nije prosao 1 sat od dodavanja posljednjeg predmeta
	if (jasmin.AddPredmet(UIT, PRVA, datum20062019_1115))
		cout << "Predmet uspjesno dodan!" << crt;
	///*nakon evidentiranja uspjeha na bilo kojem predmetu tj. prilikom uspjesno izvrsene funkcije AddPredmet, Studentu se salje email sa sadrzajem:
	//FROM:info@fit.ba
	//TO: emailStudenta
	//Postovani ime i prezime, evidentirali ste uspjeh za X godinu studija.
	//Pozdrav.
	//FIT Team.
	//ukoliko je, nakon dodavanja predmeta, prosjek na nivou te godine veci od 8.0 Studentu se, pored email-a, salje i SMS sa sadrzajem: "Svaka cast za uspjeh X.X ostvaren u X godini studija".
	//slanje poruka i emailova implemenitrati koristeci zasebne thread-ove.
	//*/
	cout << "USPJEH ISPISATI KORISTEĆI OSTREAM_ITERATOR" << endl;
	cout << jasmin << endl;
	////vraca broj ponavljanja odredjene rijeci u napomenama, koristiti sregex_iterator
	cout << "Rijec takmicenje se pojavljuje " << jasmin.BrojPonavljanjaRijeci("takmicenju") << " puta." << endl;

	////vraca niz predmeta koji su evidentiranih u periodu izmedju vrijednosti proslijedjenih parametara
	vector<Predmet> jasminUspjeh = jasmin(new DatumVrijeme(18, 06, 2019, 10, 15), new DatumVrijeme(21, 06, 2019, 10, 10));
	for (Predmet u : jasminUspjeh)
		cout << u << endl;

	Uspjeh * uspjeh_I_godina = jasmin["PRVA"];//vraca uspjeh Studenta ostvaren u prvoj godini studija
	if (uspjeh_I_godina != nullptr)
		cout << *uspjeh_I_godina << endl;

	cin.get();
	system("pause>0");
}
