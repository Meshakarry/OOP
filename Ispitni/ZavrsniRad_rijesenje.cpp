#include<iostream>
#include<string>
#include<vector>
#include<thread>
#include<mutex>

/****************************************************************************
1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ĆE BITI OZNACENO KAO "RE"
3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEČILO GUBLJENJE URAĐENOG ZADATKA
4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIČNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUČAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOĆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
5. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAČENO.
****************************************************************************/

using namespace std;

enum Oblast { SoftverskiInzinjering, KomunikacijskiSistemi, SigurnostInformacijskihSistema, InteligentniSistemi };
const char* oblast_txt[] = { "Softverski inzinjering", "Komunikacijski sistemi", "Sigurnost informacijskih sistema", "Inteligentni sistemi" };

template<class T1, class T2, int max>
class Kolekcija {
	T1 _elementi1[max];
	T2 _elementi2[max];
	int _trenutno;
public:
	Kolekcija()
	{
		_trenutno = 0;
	}

	void AddElement(const T1& elem1, const T2& elem2)
	{
		if (_trenutno == max)
			throw exception("Dostigli ste maksimalan broj elemenata u kolekciji!");

		for (int i = 0; i < _trenutno; i++)
		{
			if (_elementi1[i] == elem1 || _elementi2[i] == elem2)
				throw exception("Nije moguce dodati duple elemente u kolekciju!");
		}

		_elementi1[_trenutno] = elem1;
		_elementi2[_trenutno++] = elem2;

	}

	T1 * getElementi1Pok() { return _elementi1; }
	T2 * getElementi2Pok() { return _elementi2; }
	T1 & getElement1(int lokacija) { return _elementi1[lokacija]; }
	T2 & getElement2(int lokacija) { return _elementi2[lokacija]; }
	int getTrenutno() { return _trenutno; }

	friend ostream & operator<<(ostream & COUT, Kolekcija<T1, T2, max> & obj)
	{
		for (size_t i = 0; i < obj._trenutno; i++)
		{
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		}
		return COUT;
	}
};

//Deklaracija klase Nastavnik omogucava njeno koristenje u klasi ZavrsniRad, a definicija je data naknadno
class Nastavnik;

class ZavrsniRad
{
	char* _nazivTeme;
	Oblast* _oblastTeme;
	string _datumPrijave; //Format: dd.MM.gggg
	//U vector pohraniti samo adresu objekta tipa Nastavnik, dakle bez alokacije nove memorije
	vector<Nastavnik*> _komisija;
	string _datumOdbrane; //Format: dd.MM.gggg (najranije 2 mjeseca od datuma prijave)
	int _ocjena;
public:
	ZavrsniRad() :_nazivTeme(nullptr), _oblastTeme(nullptr), _datumPrijave("NEMA VRIJEDNOST"), _datumOdbrane("NEMA VRIJEDNOST"), _ocjena(5)
	{ }

	ZavrsniRad(string nazivTeme, Oblast oblastTeme, string datumPrijave) : _datumPrijave(datumPrijave), _oblastTeme(new Oblast(oblastTeme))
	{
		_nazivTeme = new char[nazivTeme.size() + 1];
		strcpy_s(_nazivTeme, nazivTeme.size() + 1, nazivTeme.c_str());
	}
	ZavrsniRad(const ZavrsniRad & zr)
	{
		int vel = strlen(zr._nazivTeme) + 1;
		_nazivTeme = new char[vel];
		strcpy_s(_nazivTeme, vel, zr._nazivTeme);
			_oblastTeme = new Oblast(*zr._oblastTeme);
			_datumPrijave = zr._datumPrijave;
			_komisija = zr._komisija;
			_ocjena = zr._ocjena;

	}
	ZavrsniRad & operator=(const ZavrsniRad & zr)
	{
		if (this != &zr)
		{
			delete[] _nazivTeme;
			int vel = strlen(zr._nazivTeme) + 1;
			_nazivTeme = new char[vel];
			strcpy_s(_nazivTeme, vel, zr._nazivTeme);
			delete _oblastTeme;
			_oblastTeme = new Oblast(*zr._oblastTeme);
			_datumPrijave = zr._datumPrijave;
			_komisija = zr._komisija;
			_ocjena = zr._ocjena;
		}
		return *this;
	}

	~ZavrsniRad()
	{
		delete[] _nazivTeme; _nazivTeme = nullptr;
		delete _oblastTeme; _oblastTeme = nullptr;
	}

	char* GetNazivTeme()const { return _nazivTeme; }
	Oblast GetOblastTeme()const { return *_oblastTeme; }
	string GetDatumPrijave()const { return _datumPrijave; }
	string GetDatumOdbrane() { return _datumOdbrane; }
	void SetDatumOdbrane(string datum)
	{
		_datumOdbrane = datum;
	}
	void setOcjena(int ocjena)
	{
		_ocjena = ocjena;
	}
	int GetOcjena() { return _ocjena; }
	vector<Nastavnik*>& GetKomisija() { return _komisija; };
	bool operator==(const ZavrsniRad & zr)
	{
		return strcmp(_nazivTeme, zr._nazivTeme) == 0;
	}
	friend ostream& operator<<(ostream& COUT, ZavrsniRad& r)
	{
		COUT << "Tema rada: " << r._nazivTeme << endl;
		COUT << "Oblast teme: " << oblast_txt[*r._oblastTeme] << endl;
		COUT << "Datum prijave rada: " << r._datumPrijave << endl;
		if (r._ocjena > 0)
		{
			COUT << r._ocjena << endl;
			COUT << r._datumOdbrane << endl;
		}
		//Podatke o nastavnicima nije moguce ispisati ovdje jer klasa jos nije definisana
		return COUT;
	}
};

class Nastavnik
{
	string _imePrezime;
	Oblast _oblastIzboraUZvanje;
	//Parametar string predstavlja broj indeksa studenta koji prijavljuje zavrsni rad kod odredjenog nastavnika
	Kolekcija<string, ZavrsniRad, 10> _teme;

public:
	Nastavnik(string imePrezime, Oblast oblastIzboraUZvanje) : _imePrezime(imePrezime), _oblastIzboraUZvanje(oblastIzboraUZvanje)
	{}
	Nastavnik(const Nastavnik & n)
	{
		_imePrezime = n._imePrezime;
		_oblastIzboraUZvanje = n._oblastIzboraUZvanje;

	}
	//indeks, naslov, oblast, datum prijave
	void DodajZavrsniRad(string index, string naslov, Oblast ova, string datumPrijave)
	{
		ZavrsniRad novi(naslov.c_str(), ova, datumPrijave);
		if (_teme.getTrenutno() > 0) {

			for (size_t i = 0; i < _teme.getTrenutno(); i++)
			{

				if (_oblastIzboraUZvanje != ova)
				
					throw exception("nemate odgovarajuce zvanje");
				
				if (strcmp(_teme.getElement2(i).GetNazivTeme(), naslov.c_str()) == 0)
					throw exception("ne mogu se dodati dva sa istim nazivom");
			}
		}
		_teme.AddElement(index,novi);
	}
	/*Funkcija ZakaziOdbranuRada ima zadatak da studentu sa proslijedjenim brojem indeksa zakaze odbranu zavrsnog rada sto podrazumijeva
		//definisanje datuma odbrane i liste clanova komisije pred kojima ce student braniti zavrsni rad.
		//Odbrana rada se moze zakazati samo studentu koji je rad prethodno prijavio.
		//Komisiju trebaju ciniti najmanje 2 nastavnika koji imaju izbor u zvanje u oblasti kojoj pripada tema rada.
		//Datum odbrane ne smije biti manji od datuma prijave. U slucaju da bilo koji od navedenih uslova nije ispunjen funkcija treba da baci izuzetak*/
	void ZakaziOdbranuRada(string indeks,string datum,vector<Nastavnik*> komisija) {
		int brojacLjudi = 0;
		//moze i na ovaj nacin//
	/*	int postojiStudent = false;*/
		for (size_t i = 0; i < _teme.getTrenutno(); i++)
		{
			if (_teme.getElement1(i) == indeks)
			{
				/*postojiStudent = true;*/

				if (_teme.getElement2(i).GetDatumPrijave() == "NEMA VRIJEDNOST")
					throw exception("niste prijavili rad");
				for (vector<Nastavnik*>::iterator it = komisija.begin(); it != komisija.end(); it++)
				{
					Nastavnik * u = *it;
					if (u->_oblastIzboraUZvanje == _oblastIzboraUZvanje)
						brojacLjudi++;
				}
				if (brojacLjudi < 2)
					throw exception("nemate dovoljno raje u komisiji");
				if (_teme.getElement2(i).GetDatumPrijave() > _teme.getElement2(i).GetDatumOdbrane())
					throw exception("ne moze datum rijave bit veci od datuma odbrane hajvanu");
				
			}
			/*if (!postojiStudent)
				throw exception("nije prijavio ispit");*/
			_teme.getElement2(i).GetKomisija() = komisija;
			_teme.getElement2(i).SetDatumOdbrane(datum);
		}
	}
	friend ostream & operator<<(ostream & cout, Nastavnik & n)
	{
		cout << " ime i prezime ->" << n._imePrezime << endl;
		cout << " oblast u kojem predajem ->" << n._oblastIzboraUZvanje << endl;
		cout << " Indeks studenta  I Njegova tema zavrsnog rada" << endl;
		for (size_t i = 0; i < n._teme.getTrenutno(); i++)
		{
			cout << n._teme.getElement1(i) << " " << n._teme.getElement2(i) << endl;
		}
		return cout;
	}
	/*Studentu sa brojem indeksa IB130011 dodjeljuje ocjenu 8 na zavrsnom radu.
		//Uslov za dodjelu ocjene je da student posjeduje definisan datum odbrane i listu clanova komisije.
		//U zavisnosti od uspjesnosti izvrsenja, funkcija vraca true ili false*/
	bool operator()(string index, int ocjena)
	{
		for (size_t i = 0; i < _teme.getTrenutno(); i++)
		{
			if (_teme.getElement1(i) == index)
			{
				if (_teme.getElement2(i).GetDatumOdbrane() == "NEMA VRIJEDNOST")
					return false;
				if (_teme.getElement2(i).GetKomisija().size() == 0)
					return false;

				_teme.getElement2(i).setOcjena(ocjena);
				return true;
			}
		}
		return false;
	}
	string GetImePrezime() { return _imePrezime; }
	Oblast GetOblast() { return _oblastIzboraUZvanje; }
	Kolekcija<string, ZavrsniRad, 10>& GetTeme() { return _teme; };
};
mutex m;
void Sms(string sadrzaj)
{
	m.lock();
	cout << sadrzaj << endl;
	m.unlock();
}
//*Funkcija PronadjiNajStudenta ima zadatak da pronadje broj indeksa studenta koji je
//na zavrsnom radu ostvario najvecu ocjenu kod nastavnika koji posjeduje najvecu prosjecnu ocjenu 
//na zavrsnim radovima. Ukoliko se kod nastavnika sa najvecom prosjecnom ocjenom pojavi 
//vise studenata sa istom ocjenom, onda funkcija vraca broj indeksa prvog pronadjenog studenta.
//Svim studentima koji su odbranili rad kod nastavnika sa najvecom prosjecno ocjenom,
//u zasebnom thread.u, poslati email poruku (mail adresa: brojIndeksa@edu.fit.ba)
//sa sadrzajem da su svoj zavrsni rad uspjesno odbranili sa vecom ili manjom ocjenom od prosjecne.
//Ukoliko niti jedan od nastavnika ne posjeduje evidentirano mentorstvo na zavrsnom radu, funkcija vraca tekst: NIJE PRONADJEN*/
string PronadjiNajStudenta(Nastavnik ** ja, int max)
{
	float najvecaOcjena = 0.0;
	int broj = 0;
	float najveciPRosjek = 3.5;
	string index = "not_set";
	Nastavnik *najbolji = nullptr;
	for (size_t i = 0; i < max; i++)
	{
		for (size_t j = 0; j < ja[i]->GetTeme().getTrenutno(); j++)
		{
			najvecaOcjena += ja[i]->GetTeme().getElement2(j).GetOcjena();
			broj++;
		}
		float prosjek = najvecaOcjena / broj;
		if (prosjek > najveciPRosjek)
		{
			najveciPRosjek = prosjek;
			najbolji = (*ja + i);

		}
		for (size_t i = 0; i < najbolji->GetTeme().getTrenutno(); i++)
		{
			najveciPRosjek = najbolji->GetTeme().getElement2(i).GetOcjena();
			index = najbolji->GetTeme().getElement1(i);
		}
	   
		if (prosjek > najveciPRosjek)
		{
			string sadrzaj = index + "@edu.fit.ba"+
			 "ostvarili ste uspjeh veci od prosjecnog"+to_string(prosjek);
			thread t1(Sms, sadrzaj);
			t1.join();
		}
		else {
			string sadrzajj = index + "@edu.fit.ba" +
				"ostvarili ste uspjeh veci od prosjecnog" + to_string(prosjek);
			thread t2(Sms, sadrzajj);
			t2.join();
		}

		return index;
	}
}
void main()
{
	const int max = 4;
	Nastavnik* nastavnici[max];

	nastavnici[0] = new Nastavnik("Denis Music", SoftverskiInzinjering);
	nastavnici[1] = new Nastavnik("Zanin Vejzovic", KomunikacijskiSistemi);
	nastavnici[2] = new Nastavnik("Jasmin Azemovic", SigurnostInformacijskihSistema);
	nastavnici[3] = new Nastavnik("Emina Junuz", SoftverskiInzinjering);

	try
	{
	//	/*Funkcija DodajZavrsniRad ima zadatak da odredjenom nastavniku dodijeli 
		//mentorstvo na zavrsnom radu. 
		//Sprijeciti dodavanje zavrsnih radova sa istom temom vise puta. 
		//Nastavnik moze imati (mentorisati) samo radove iz oblasti za koju posjeduje izbor u zvanje.
		//U slucaju da se nastavniku pokusa dodati rad koji nije iz njegove oblasti funkcija treba da baci izuzetak sa odgovarajucom porukom */

	//	//indeks, naslov, oblast, datum prijave
		nastavnici[0]->DodajZavrsniRad("IB130011", "Multimedijalni informacijski sistem za visoko-obrazovnu ustanovu", SoftverskiInzinjering, "01.04.2017");
		nastavnici[0]->DodajZavrsniRad("IB120051", "Sistem za podršku rada kablovskog operatera", SoftverskiInzinjering, "03.03.2017");

		nastavnici[1]->DodajZavrsniRad("IB140102", "Praktična analiza sigurnosti bežičnih računarskih mreža", KomunikacijskiSistemi, "22.08.2017");

		nastavnici[2]->DodajZavrsniRad("IB140002", "Primjena teorije informacija u procesu generisanja kriptografskih ključeva", SigurnostInformacijskihSistema, "10.09.2017");

		vector<Nastavnik*> komisija;//formira se komisija
		komisija.push_back(nastavnici[0]);
		komisija.push_back(nastavnici[2]);
		komisija.push_back(nastavnici[3]);

	//	/*Funkcija ZakaziOdbranuRada ima zadatak da studentu sa proslijedjenim brojem indeksa zakaze odbranu zavrsnog rada sto podrazumijeva 
		//definisanje datuma odbrane i liste clanova komisije pred kojima ce student braniti zavrsni rad.
		//Odbrana rada se moze zakazati samo studentu koji je rad prethodno prijavio. 
		//Komisiju trebaju ciniti najmanje 2 nastavnika koji imaju izbor u zvanje u oblasti kojoj pripada tema rada.
		//Datum odbrane ne smije biti manji od datuma prijave. U slucaju da bilo koji od navedenih uslova nije ispunjen funkcija treba da baci izuzetak*/
		nastavnici[0]->ZakaziOdbranuRada("IB130011", "25.09.2017", komisija);
		nastavnici[0]->ZakaziOdbranuRada("IB130111", "25.09.2017", komisija);//student sa brojem indeksa IB130111 jos uvijek nije prijavio rad
	

	//	/*Studentu sa brojem indeksa IB130011 dodjeljuje ocjenu 8 na zavrsnom radu.
		//Uslov za dodjelu ocjene je da student posjeduje definisan datum odbrane i listu clanova komisije.
		//U zavisnosti od uspjesnosti izvrsenja, funkcija vraca true ili false*/

		if ((*nastavnici[0])("IB130011", 8))
			cout << "Uspjesno ste ocijenili zavrsni rad!" << endl;

	//	/*Ispisuje sve podatke o nastavniku i njegovim mentorstvima. Za clanove komisije je dovoljno ispisati samo ime i prezime.*/
		cout << *nastavnici[0] << endl;

	//	/*Funkcija PronadjiNajStudenta ima zadatak da pronadje broj indeksa studenta koji je 
		//na zavrsnom radu ostvario najvecu ocjenu kod nastavnika koji posjeduje najvecu prosjecnu ocjenu 
		//na zavrsnim radovima. Ukoliko se kod nastavnika sa najvecom prosjecnom ocjenom pojavi 
		//vise studenata sa istom ocjenom, onda funkcija vraca broj indeksa prvog pronadjenog studenta.
		//Svim studentima koji su odbranili rad kod nastavnika sa najvecom prosjecno ocjenom,
		//u zasebnom thread.u, poslati email poruku (mail adresa: brojIndeksa@edu.fit.ba)
		//sa sadrzajem da su svoj zavrsni rad uspjesno odbranili sa vecom ili manjom ocjenom od prosjecne.
		//Ukoliko niti jedan od nastavnika ne posjeduje evidentirano mentorstvo na zavrsnom radu, funkcija vraca tekst: NIJE PRONADJEN*/

		cout << "Najsupjesniji student: " << PronadjiNajStudenta(nastavnici, max) << endl;

		//Baca izuzetak zbog neadekvatnog izbora u zvanje, odnosno oblasti
		nastavnici[2]->DodajZavrsniRad("IB150008", "Razvoj sistema autentifikacije na osnovu biometrije glasa", InteligentniSistemi, "15.05.2017");
	}
	catch (exception& ex)
	{
		cout << "GRESKA -> " << ex.what() << endl;
	}

	for (int i = 0; i < max; i++)
	{
		delete nastavnici[i];
		nastavnici[i] = nullptr;
	}
	system("pause>0");
}
