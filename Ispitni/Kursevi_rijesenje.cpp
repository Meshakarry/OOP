#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <ctime>
using namespace std;

////bool ProvjeriFormatTelefona(string telefon) {
////	return regex_match(telefon, regex("(\\()(\\d{3})(\\))(\\d{3})(\\-)(\\d{3})"));
////}
bool ProvjeriFormatTelefona(string tel)
{
	//(000)000-000
	string pravilo = "(\\((\\d{3})\\))(\\d{3})[-](\\d{3})";
	if (regex_match(tel, regex(pravilo)))
		return true;
	
	
		return false;
	
}

class Datum {
	int *_dan, *_mjesec, *_godina;
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	Datum(const Datum &d) {
		_dan = new int(*d._dan);
		_mjesec = new int(*d._mjesec);
		_godina = new int(*d._godina);
	}
	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	Datum &operator=(const Datum &d) {
		*_dan = *d._dan;
		*_mjesec = *d._mjesec;
		*_godina = *d._godina;
		return *this;
	}
	bool operator == (const Datum &d) {
		return *_dan == *d._dan && *_mjesec == *d._mjesec && *_godina == *d._godina;
	}
	bool operator > (const Datum &d) {
		int day1 = *_dan + *_mjesec * 30 + *_godina * 365;
		int day2 = *d._dan + *d._mjesec * 30 + *d._godina * 365;
		return day1 > day2;
	}
	friend ostream& operator<< (ostream &COUT, const Datum &obj) {
		COUT << *obj._dan << " " << *obj._mjesec << " " << *obj._godina;
		return COUT;
	}
};

class Izuzetak :public exception {
	Datum _datum;//KORISTITE DANASNJI DATUM
	string _funkcija;//FUNKCIJA U KOJOJ JE NASTAO IZUZETAK
	int _linija;//LINIJA CODE-A U KOJOJ JE NASTAO IZUZETAK
public:
	Izuzetak(string fun, int line, const char * poruka) :exception(poruka), _funkcija(fun), _linija(line) {
		time_t trenutno = time(0);
		tm localcurrent;
		localtime_s(&localcurrent, &trenutno);
		Datum d(localcurrent.tm_mday, localcurrent.tm_mon, localcurrent.tm_year);
		_datum = d;
	}
	friend ostream &operator <<(ostream &output, const Izuzetak &i) {
		output << "Datum -> " << i._datum << endl;
		output << "Funkcija -> " << i._funkcija << endl;
		output << "Linija -> " << i._linija << endl;
		output << "Poruka -> " << i.what() << endl;
		return output;
	}
};

template<class T1, class T2, int max>
class Kolekcija {
	T1 _elementi1[max];
	T2 _elementi2[max];
	int * _trenutno;
public:
	Kolekcija() :_trenutno(new int(0)) {}
	~Kolekcija() { delete _trenutno; }
	Kolekcija(const Kolekcija &k)  {
		_trenutno = new int(*k._trenutno);

		if (max < k.GetMax()) throw Izuzetak(__FUNCTION__, __LINE__, "Nemoguce kopiranje objekata");
		for (size_t i = 0; i < *k._trenutno; i++)
		{
			_elementi1[i] = k._elementi1[i];
			_elementi2[i] = k._elementi2[i];
		}
	}
	void AddElement(T1 el1, T2 el2) {
		
		if (*_trenutno == max) throw Izuzetak(__FUNCTION__, __LINE__, "Prekoracili ste kapacitet kolekcija");
		_elementi1[*_trenutno] = el1;
		_elementi2[*_trenutno] = el2;
		(*_trenutno)++;
	}
	Kolekcija &operator=(const Kolekcija &k) {
		*_trenutno = *k._trenutno;
		for (size_t i = 0; i < *k._trenutno; i++)
		{
			_elementi1[i] = k._elementi1[i];
			_elementi2[i] = k._elementi2[i];
		}
		return *this;
	}
	int GetMax()const { return max; }
	T1 GetElement1(int i)const {
		
		if (i < 0 || i >= *_trenutno) throw Izuzetak(__FUNCTION__, __LINE__, "Nepostojeci element");
		return _elementi1[i];
	}
	T2 GetElement2(int i)const {
		
		if (i < 0 || i >= *_trenutno) throw Izuzetak(__FUNCTION__, __LINE__, "Nepostojeci element");
		return _elementi2[i];
	}
	T1 &operator[](int i) {
		
		if (i < 0 || i >= *_trenutno) throw Izuzetak(__FUNCTION__, __LINE__, "Nepostojeci element");
		return _elementi1[i];
	}
	int GetTrenutno()const { return *_trenutno; }
	void Clear() { *_trenutno = 0; }
	friend ostream& operator<< (ostream &output, const Kolekcija &k) {
		for (size_t i = 0; i < k.GetTrenutno(); i++)
			output << k.GetElement1(i) << " " << k.GetElement2(i) << endl;
		return output;
	}
};

enum enumKursevi { HtmlCSSJavaScript=0, SoftwareEngeneeringFundamentals=1, MasteringSQL=2, WindowsSecurity=3 };
const char * kursevi[] = { "HTMLCSSJAVA","SOFTWAREENGINEERING","BAZEMASTERING","WINDOWSSECURITY" };

class Kurs {
	enumKursevi _kurs;
	Datum _pocetak;
	Datum _kraj;
	char * _imePredavaca;
public:
	Kurs()
	{
		_imePredavaca = nullptr;
	}
	~Kurs() {
		delete[] _imePredavaca;
	}
	Kurs(enumKursevi tecaj, const char * ime, Datum  start, Datum  end):_kurs(tecaj)
	{
		int size = strlen(ime) + 1;
		_imePredavaca = new char[size];
		strcpy_s(_imePredavaca, size, ime);

		

		_pocetak = start;
		_kraj = end;
	}
	Kurs(const Kurs & k)
	{
		int size = strlen(k._imePredavaca) + 1;
		_imePredavaca = new char[size];
		strcpy_s(_imePredavaca, size, k._imePredavaca);
		_kurs = k._kurs;
		_pocetak = k._pocetak;
		_kraj = k._kraj;

	}
	Kurs & operator=(const Kurs & k)
	{
		if (this != &k)
		{
			delete[] _imePredavaca;
			int size = strlen(k._imePredavaca) + 1;
			_imePredavaca = new char[size];
			strcpy_s(_imePredavaca, size, k._imePredavaca);
			_kurs = k._kurs;
			_pocetak = k._pocetak;
			_kraj = k._kraj;

		}
		return *this;
	}
	bool operator==(const Kurs & novi)
	{
		return strcmp(_imePredavaca, novi._imePredavaca) == 0 && _kurs == novi._kurs && _pocetak == novi._pocetak && _kraj == novi._kraj;
	}
	friend ostream & operator<<(ostream & ispis, Kurs & k)
	{
		ispis << " Naziv kursa ->" << kursevi[k._kurs] << endl;
		ispis << " Naziv predavaca na tom kursu ->" << k._imePredavaca << endl;
		ispis << " Kurs pocinje ->" << k._pocetak << endl;
		ispis << " Kurs traje do ->" << k._kraj << endl;
		return ispis;
	}
	Datum GetPocetakKursa() {
		return _pocetak;
	}
	Datum Getzavrsetak() {
		return _kraj;
	}
	char * GetIme() { return _imePredavaca; }


};
class Polaznik {
	static int ID;
	const int _polaznikID;
	//SVAKOM NOVOM POLAZNIKU AUTOMATSKI DODIJELITI NOVI ID (AUTOINCREMENT) POCEVSI OD BROJA 1
	char * _imePrezime;
	string _kontaktTelefon; //BROJ TELEFONA TREBA BITI U FORMATU (06X)XXX-XXX 
	Kolekcija<Kurs *, int, 10> _uspjesnoOkoncaniKursevi;
	//INT PREDSTAVLJA OSTVARENI PROCENAT NA ISPITU, A JEDAN POLAZNIK MOZE POLOZITI NAJVISE 10 KURSEVA
public:
	Polaznik(const char * imePrezime, string telefon) :_polaznikID(ID++) {
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
		_kontaktTelefon = telefon;
	}
	~Polaznik() {
		delete[] _imePrezime; _imePrezime = nullptr;
		for (size_t i = 0; i < _uspjesnoOkoncaniKursevi.GetTrenutno(); i++)
		{
			delete _uspjesnoOkoncaniKursevi[i];
			_uspjesnoOkoncaniKursevi[i] = nullptr;
		}
	}
	Polaznik(const Polaznik & p): _polaznikID(p._polaznikID),_kontaktTelefon(p._kontaktTelefon),_uspjesnoOkoncaniKursevi(p._uspjesnoOkoncaniKursevi)
	{
		int size = strlen(p._imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, p._imePrezime);
	}
	int GetID() { return _polaznikID; }
	Polaznik & operator=(const Polaznik & p)
	{
		if (this != &p)
		{
			delete[] _imePrezime;
			int size = strlen(p._imePrezime) + 1;
			_imePrezime = new char[size];
			strcpy_s(_imePrezime, size, p._imePrezime);
			_uspjesnoOkoncaniKursevi.Clear();
			_kontaktTelefon = p._kontaktTelefon;
			for (size_t i = 0; i < _uspjesnoOkoncaniKursevi.GetTrenutno(); i++)
			{
				_uspjesnoOkoncaniKursevi.AddElement(new Kurs(*p._uspjesnoOkoncaniKursevi.GetElement1(i)), p._uspjesnoOkoncaniKursevi.GetElement2(i));
			}
		}
		return *this;
	}
	bool operator ==(const Polaznik & po)
	{
		if (_uspjesnoOkoncaniKursevi.GetTrenutno() != _uspjesnoOkoncaniKursevi.GetTrenutno() || strcmp(_imePrezime, po._imePrezime) != 0)
			return false;
		for (size_t i = 0; i < _uspjesnoOkoncaniKursevi.GetTrenutno(); i++)
		{
			if (_uspjesnoOkoncaniKursevi.GetElement1(i) != po._uspjesnoOkoncaniKursevi.GetElement1(i))
				return false;
		}
		return true;

	}
	Kolekcija<Kurs *, int, 10> GetuspjesnoOkoncaniKursevi() { return _uspjesnoOkoncaniKursevi; }
	void SetUspjesnoOkoncaneKurseve(Kurs k, int procenat)
	{
		_uspjesnoOkoncaniKursevi.AddElement(new Kurs(k), procenat);
	}

	friend ostream& operator <<(ostream & ispis, Polaznik & pol)
	{
		ispis << " ID polaznika ->" << pol._polaznikID << endl << " Ime i prezime ->" << pol._imePrezime << endl;
		ispis << " Kontakt telefon : " << pol._kontaktTelefon << endl;
		ispis << " lista polozenih kurseva" << pol._uspjesnoOkoncaniKursevi << endl;
		return ispis;
	}
};
int Polaznik::ID = 1;

class SkillsCentar {
	string _nazivCentra;
	vector<Kurs> _kursevi;//KURSEVI KOJE NUDI ODREDJENI CENTAR
	Kolekcija<Kurs, Polaznik *, 150> _aplikanti;
public:
	SkillsCentar(string naziv) { _nazivCentra = naziv; }
	/*~SkillsCentar() {
		for (size_t i = 0; i < _aplikanti.GetTrenutno(); i++)
		{
			delete _aplikanti.GetElement2(i);
			
		}
	}*/
	SkillsCentar(const SkillsCentar & obj) :_kursevi(obj._kursevi), _aplikanti(obj._aplikanti) {
		_nazivCentra = obj._nazivCentra;
	}
	bool ProvjeriKoliziju(Kurs kurs)
	{
		for (size_t i = 0; i < _kursevi.size(); i++)
		{
			if (_kursevi[i].Getzavrsetak() > kurs.GetPocetakKursa())
			{
				return false;
			}
		}
		return true;
	}
	void AddKurs(Kurs novi)
	{
		for (size_t i = 0; i < _kursevi.size(); i++)
		{
			if (_kursevi[i] == novi)
				throw Izuzetak(__FUNCTION__, __LINE__, "kurs vec postoji");
		}
		_kursevi.push_back(novi);
	}
	void AddAplikaciju(Kurs novi, Polaznik & starter)
	{
		bool postoji = false;
		for (size_t i = 0; i < _kursevi.size(); i++)
		{
			if (_kursevi[i] == novi) {
				postoji = true;
			}
		}
		if (!postoji)
			throw Izuzetak(__FUNCTION__, __LINE__, "kurs ne postoji u centru");
		for (size_t i = 0; i < _aplikanti.GetTrenutno(); i++)
		{
			if (_aplikanti.GetElement1(i) == novi && *_aplikanti.GetElement2(i) == starter)
			{
				throw Izuzetak(__FUNCTION__, __LINE__, "aplikacija dakle vec postoji");
			}
		}
		_aplikanti.AddElement(novi, &starter);


	}
	//FUNKCIJA JE ZADUZENA DA POLAZNIKU DODA INFORMACIJU O USPJESNO POLOZENOM KURSU KOJI JE POSLAN KAO PARAMETAR
		//. PREDUSLOV ZA DODAVANJE JE DA JE POLAZNIK PRETHODNO APLICIRAO ZA TAJ KURS, TE DA JE NA ISPITU OSTVARIO VISE OD 55%
	void DodajUspjesnoOkoncanKurs(int ajDi, Kurs novi, int procent)
	{
		
			for (size_t j = 0; j < _aplikanti.GetTrenutno(); j++)
			{
				if (_aplikanti.GetElement1(j) == novi && _aplikanti.GetElement2(j)->GetID() == ajDi && procent >= 55) {
					_aplikanti.GetElement2(j)->SetUspjesnoOkoncaneKurseve(novi, procent);
				}
			}
		
	}
	vector<Polaznik> GetPolazniciByPredavac(const char* predavac, enumKursevi kursevi)
	{
		vector<Polaznik> novi;
		for (size_t i = 0; i < _aplikanti.GetTrenutno(); i++)
		{
			for (size_t j = 0; j < _aplikanti.GetElement2(i)->GetuspjesnoOkoncaniKursevi().GetTrenutno(); j++)
			{
				if (strcmp(_aplikanti.GetElement2(i)->GetuspjesnoOkoncaniKursevi().GetElement1(j)->GetIme(), predavac) == 0 ) {
					novi.push_back(*_aplikanti.GetElement2(i));
				}
			}
		}
		return novi;
	}
	bool RemoveKurs(Kurs kurs)
	{
		bool trebaIzbrisat = true;
		for (size_t i = 0; i < _aplikanti.GetTrenutno(); i++)
		{
			if (_aplikanti.GetElement1(i) == kurs)
				trebaIzbrisat = false;
		}
		if (!trebaIzbrisat)
			return false;
		for (vector<Kurs>::iterator i = _kursevi.begin(); i != _kursevi.end(); i++)
		{
			if (*i == kurs)
				_kursevi.erase(i);
			return true;
		}
		return false;

	 }
	friend ostream & operator<<(ostream & ispis, SkillsCentar & sc)
	{
		ispis << " Naziv centra je " << sc._nazivCentra << endl;
		ispis << " kursevi koje ova jcentar ima u ponudi ->";
		for (size_t i = 0; i <sc._kursevi.size(); i++)
		{
			ispis << sc._kursevi[i] << endl;
		}
		return ispis;
	}

};
const char *crt = "\n---------------------------------------\n";
void main()
{
	/****************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ĆE BITI OZNACENO KAO "RE"
	3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEČILO GUBLJENJE URAĐENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKOĐER NALAZI U FAJLU CODE.TXT
	5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIČNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUCAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOĆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
	6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAČENO.
	****************************************************************************/

#pragma region TestiranjeDatuma
	Datum danas(26, 11, 2015);
	Datum sutra(danas);
	Datum prekosutra;
	prekosutra = danas;
	cout << danas << endl << sutra << endl << prekosutra << crt;
#pragma endregion
#pragma region TestiranjeKolekcije
	Kolekcija<int, int, 10> kolekcija1;
	try
	{
		for (size_t i = 0; i < kolekcija1.GetMax(); i++)
			kolekcija1.AddElement(i, i + 2);
		//FUNKCIJA TREBA BACITI IZUZETAK U SLUCAJ DA NEMA VISE PROSTORA ZA DODAVANJE NOVIH ELEMENATA
		cout << kolekcija1.GetElement1(0) << " " << kolekcija1.GetElement2(0) << endl;
		cout << kolekcija1.GetElement1(20) << " " << kolekcija1.GetElement2(20) << endl;
		//FUNKCIJA TREBA DA BACI IZUZETAK UKOLIKO ELEMENT NA ZAHTIJEVANOJ LOKACIJI NE POSTOJI
	}
	catch (Izuzetak & err)
	{
		cout << "Greska -> " << err << endl;
	}

	cout << kolekcija1 << endl;
	Kolekcija<int, int, 10> kolekcija2;
	kolekcija2 = kolekcija1;
	cout << kolekcija2 << crt;
	if (kolekcija1.GetTrenutno() == kolekcija2.GetTrenutno())
		cout << "ISTI BROJ ELEMENATA" << endl;
	Kolekcija<int, int, 10> kolekcija3(kolekcija2);
	cout << kolekcija3 << crt;
#pragma endregion
#pragma region TestiranjeKursa
	Kurs sef(SoftwareEngeneeringFundamentals, "Jasmin Azemovic", Datum(28, 1, 2016), Datum(15, 2, 2016));
	cout << sef << endl;
	Kurs msql(MasteringSQL, "Adel Handzic", Datum(28, 2, 2016), Datum(15, 3, 2016));
	sef = msql;
	if (sef == msql)//KURSEVI SU ISTI AKO POSJEDUJU IDENTICNE VRIJEDNOSTI SVIH ATRIBUTA
		cout << "ISTI SU KURSEVI" << endl;
	Kurs msql2(msql);
	cout << msql2 << endl;
#pragma endregion
#pragma region TestiranjePolaznika
	string telefon1 = "(061)111-111";
	string telefon2 = "(061)111222";
	string telefon3 = "(061)111-333";

	if (!ProvjeriFormatTelefona(telefon1))
		cout << "Broj " << telefon1 << " nije validan" << endl;
	if (!ProvjeriFormatTelefona(telefon2))
		cout << "Broj " << telefon2 << " nije validan" << endl;
	if (!ProvjeriFormatTelefona(telefon3))
		cout << "Broj " << telefon3 << " nije validan" << endl;
//
//
	Polaznik denis("Denis Music", telefon1);
	Polaznik denis2("Denis2 Music2", telefon3);

#pragma endregion

#pragma region TestiranjeCentra
	try {
		SkillsCentar mostar("Skills Center Mostar");
		if (mostar.ProvjeriKoliziju(sef))
			//JEDAN KURS NE SMIJE POCETI DOK DRUGI TRAJE TJ. VRIJEME ODRZAVANJA KURSA SE NE SMIJE POKLAPATI
			mostar.AddKurs(sef);
		if (mostar.ProvjeriKoliziju(msql))
			mostar.AddKurs(msql);
		if (mostar.ProvjeriKoliziju(msql2))
			mostar.AddKurs(msql2);
		/*mostar.AddKurs(sef);*/
//		//ONEMOGUCITI DODAVANJE IDENTICNIH KURSEVA. FUNKCIJA TREBA DA BACI IZUZETAK UKOLIKO SE POKUSA DODATI IDENTICAN KURS
		mostar.AddAplikaciju(sef, denis);
		//ONEMOGUCITI APLICIRANJE ZA KURSEVE KOJI NISU REGISTROVANI U CENTRU
	//	mostar.AddAplikaciju(msql, denis);
	//	mostar.AddAplikaciju(msql, denis);
		
//		//ONEMOGUCITI APLICIRANJE ZA ISTI KURS. FUNKCIJA BAZA IZUZETAK
		cout << endl;
		cout << denis2 << endl;
		denis2 = denis;
		if (denis == denis2)
			//POLAZNICI SU ISTI AKO POSJEDUJU ISTO IME I AKO SU POHADJALI ISTE KURSEVE
			cout << "ISTI SU" << endl;
//
		mostar.DodajUspjesnoOkoncanKurs(1, sef, 60);//BROJ 1 OZNACAVA ID POLAZNIKA.
		//FUNKCIJA JE ZADUZENA DA POLAZNIKU DODA INFORMACIJU O USPJESNO POLOZENOM KURSU KOJI JE POSLAN KAO PARAMETAR
		//. PREDUSLOV ZA DODAVANJE JE DA JE POLAZNIK PRETHODNO APLICIRAO ZA TAJ KURS, TE DA JE NA ISPITU OSTVARIO VISE OD 55%
		mostar.DodajUspjesnoOkoncanKurs(1, msql, 83);
		vector<Polaznik> listaPolaznika = mostar.GetPolazniciByPredavac("Jasmin Azemovic", SoftwareEngeneeringFundamentals);//VRACA INFORMACIJE O POLAZNICIMA KOJI SU KOD ODREDJENOG PREDAVACA USPJESNO OKONCALI ODREDJENI KURS
		 
		

		if (mostar.RemoveKurs(sef))//ONEMOGUCITI UKLANJANJE KURSA KOJI SU VEC PRIJAVILI NEKI OD POLAZNIKA
			cout << "Kurs " << sef << " uspjesno uklonjen iz centra " << mostar << endl;
//
		SkillsCentar sarajevo = mostar;
		cout << sarajevo << endl;
	}
	catch (Izuzetak & err) {
		cout << err << endl;
	}
#pragma endregion
	system("pause");
}
