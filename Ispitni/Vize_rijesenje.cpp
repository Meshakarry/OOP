#include<iostream>
#include <vector>
#include <string>
#include <regex>
#include <exception>
#include <thread>
#include <chrono>

const char *crt = "\n-----------------------------------------------\n";

using namespace std;

enum Status { ZAHTJEV_ZAPRIMLJEN, ODUSTAO, ODBIJENA, IZDATA, ISTEKLA, PONISTENA };
const char * StatusChar[] = { "ZAHTJEV_ZAPRIMLJEN", "ODUSTAO", "ODBIJENA", "IZDATA", "ISTEKLA", "PONISTENA" };
enum Sortiranje { RASTUCI, OPADAJUCI };
const char * SortiranjeChar[] = { "RASTUCI", "OPADAJUCI" };
enum Drzava { SAD, AUSTRALIJA, ENGLESKA };
const char * DrzavaChar[] = { "SAD", "AUSTRALIJA", "ENGLESKA" };

class Datum {
	int *_dan, *_mjesec, *_godina;
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	Datum(const Datum& original) {
		_dan = new int(*original._dan);
		_mjesec = new int(*original._mjesec);
		_godina = new int(*original._godina);
	}
	int getDan() { return *_dan; }
	int getMjesec() { return *_mjesec; }
	int getGodina() { return *_godina; }
	Datum& operator++() { (*_dan)++; return *this; }
	Datum& operator=(const Datum& drugi) {
		if (this != &drugi) {
			*_dan = *drugi._dan;
			*_mjesec = *drugi._mjesec;
			*_godina = *drugi._godina;


		}
		return *this;
	}
	bool operator==(const Datum& drugi) {
		return *_dan == *drugi._dan && *_mjesec == *drugi._mjesec && *_godina == *drugi._godina;
	}
	friend ostream& operator<< (ostream &COUT, Datum &obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
		return COUT;
	}
};
bool MladiOd(Datum * d1, Datum *  d2) {
	if ((d1->getGodina() - d2->getGodina()) > 10) return true;
	if ((d1->getGodina() - d2->getGodina()) == 10 && d1->getMjesec() > d2->getMjesec()) return true;
	return false;
}
bool ManjiDatum(Datum * d1, Datum * d2) {
	if (d1->getGodina() < d2->getGodina()) return true;
	if (d1->getGodina() == d2->getGodina() && d1->getMjesec() < d2->getMjesec()) return true;
	if (d1->getGodina() == d2->getGodina() && d1->getMjesec() == d2->getMjesec() && d1->getDan() < d2->getDan()) return true;
	return false;
}

template<class T1, class T2, int max>
class FITKolekcija {
	T1 * _elementi1[max];
	T2 * _elementi2[max];
	int _trenutno;
public:
	FITKolekcija() {
		for (size_t i = 0; i < max; i++) {
			_elementi1[i] = nullptr;
			_elementi2[i] = nullptr;
		}
		_trenutno = 0;
	}
	~FITKolekcija() {
		for (size_t i = 0; i < max; i++) {
			if(_elementi1[i]!=nullptr && _elementi2[i]!=nullptr){
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
			}
		}
	}
	void Dodaj(T1 & el1, T2 & el2) {
		if (_trenutno == max)
			throw exception("Prekoracen maksimalan broj elemenata!");

		_elementi1[_trenutno] = new T1(el1);
		_elementi2[_trenutno] = new T2(el2);
		_trenutno++;
	}
	
	void Sortiraj(Sortiranje sortNacin) {
		bool promjena = true;
		
		while (promjena) {
			promjena = false;
			for (int i = 0; i < _trenutno-1; i++) {
				if (sortNacin == RASTUCI) {
					if (*_elementi1[i] > *_elementi1[i + 1]) {
						swap(*_elementi1[i], *_elementi1[i + 1]);
						swap(*_elementi2[i], *_elementi2[i + 1]);
						promjena = true;
					}
				}
				else if (sortNacin == OPADAJUCI) {
					if (*_elementi1[i] < *_elementi1[i + 1]) {
						swap(*_elementi1[i], *_elementi1[i + 1]);
						swap(*_elementi2[i], *_elementi2[i + 1]);
						promjena = true;
					}
				}
			}
			
		}
	}
	T1 ** GetT1() { return _elementi1; }
	T1 * GetEl1(int index) {
		if (index >= 0 && index < _trenutno)
			return _elementi1[index];
		throw exception("Index out of range!");
	}
	T2 ** GetT2() { return _elementi2; }
	T2 * GetEl2(int index) {
		if (index >= 0 && index < _trenutno)
			return _elementi2[index];
		throw exception("Index out of range!");
	}
	int GetTrenutno() { return _trenutno; }

	friend ostream& operator<< (ostream &COUT, FITKolekcija &obj) {
		for (size_t i = 0; i < obj._trenutno; i++)
			COUT << *obj._elementi1[i] << " " << *obj._elementi2[i] << endl;
		return COUT;
	}
};

class Viza {
protected:
	Drzava _drzava;
	Datum _vazenjeOD;
	Datum _vazenjeDO;
	char * _napomena;
	// string se odnosi na razlog promjene statusa, a moguće je samo 10 izmjena statusa
	FITKolekcija<Status, string, 10> _statusi;
public:
	Viza(Drzava drzava, Datum _OD, Datum _DO, const char * napomena = nullptr) :_vazenjeDO(_DO), _vazenjeOD(_OD)
	{
		if (napomena != nullptr) {
			_napomena = new char[strlen(napomena) + 1];
			strcpy_s(_napomena, strlen(napomena) + 1, napomena);
		}
		else
			_napomena = nullptr;
		_drzava = drzava;
	}
	virtual ~Viza() { delete[] _napomena; _napomena = nullptr; }
	Viza(Viza& druga) {
		_drzava = druga._drzava;
		_vazenjeOD = druga._vazenjeOD;
		_vazenjeDO = druga._vazenjeDO;
		if (druga._napomena != nullptr) {
			int len = strlen(druga._napomena) + 1;
			_napomena = new char[len];
			strcpy_s(_napomena, len, druga._napomena);
		}
		else
			_napomena = nullptr;
		
	}
	Drzava getDrzavu() { return _drzava; }
	Datum GetVazenjeOD() { return _vazenjeOD; }
	Datum GetVazenjeDO() { return _vazenjeDO; }
	string GetNapomenaString() { return _napomena; }
	char * GetNapomenaChar() { return _napomena; }
	FITKolekcija<Status, string, 10> & GetStatuse() { return _statusi; }
	virtual void Info() = 0;
	Viza& operator=(Viza& druga) {
		if (this != &druga) {
			_drzava = druga._drzava;
			_vazenjeOD = druga._vazenjeOD;
			_vazenjeDO = druga._vazenjeDO;
			delete[] _napomena;
			int len = strlen(druga._napomena) + 1;
			_napomena = new char[len];
			strcpy_s(_napomena, len, druga._napomena);
			
		}
		return *this;
	}
	bool operator==(const Viza& druga) {
		if (_drzava == druga._drzava && _vazenjeOD == druga._vazenjeOD && _vazenjeDO == druga._vazenjeDO)
		{
			if (_napomena != nullptr && druga._napomena != nullptr)
			{
				if (strcmp(_napomena, druga._napomena) == 0)
					return true;
			}
		}
		else {
			return false;
		}
			
	}
};

class PoslovnaViza :public Viza {
	string _kompanija; //naziv kompanije koja potvrđuje da posluje sa aplikantom
public:
	PoslovnaViza(Drzava drzava, Datum _OD, Datum _DO, string kompanija, const char * napomena = nullptr) : Viza(drzava, _OD, _DO, napomena) {
		_kompanija = kompanija;
	}
	PoslovnaViza(PoslovnaViza& original) :Viza(original) {
		_kompanija = original._kompanija;
	}
	void Info() {
		cout << *this;
	}
	PoslovnaViza& operator=(PoslovnaViza& druga) {
		if (this != &druga) {
			(Viza&)(*this) = druga;
			_kompanija = druga._kompanija;
		}
		return *this;
	}
	bool operator==(const PoslovnaViza& druga) {
		return _drzava == druga._drzava && _vazenjeOD == druga._vazenjeOD && _vazenjeDO == druga._vazenjeDO
			&& strcmp(_napomena, druga._napomena) == 0 && _kompanija == druga._kompanija;
	}
	friend ostream & operator << (ostream & COUT, PoslovnaViza & obj) {
		COUT << DrzavaChar[obj._drzava] << " " << obj._vazenjeOD << " " << obj._vazenjeDO << " " << obj._kompanija << endl;
		COUT << ((obj._napomena != nullptr) ? obj._napomena : "") << endl;
		COUT << "STATUSI -> " << obj._statusi << endl;
		return COUT;

	}

};
class TuristickaViza : public Viza {
	string _adresa; //adresa hotele ili drugog smještaja na kojoj će aplikant boraviti
public:
	TuristickaViza(Drzava drzava, Datum _OD, Datum _DO, string adresa, const char * napomena = nullptr) : Viza(drzava, _OD, _DO, napomena) {
		_adresa = adresa;
	}
	TuristickaViza(TuristickaViza& original) :Viza(original) {
		_adresa = original._adresa;
	}
	void Info() {
		cout << *this;
	}
	TuristickaViza& operator=(TuristickaViza& druga) {
		if (this != &druga) {
			(Viza&)(*this) = druga;
			_adresa = druga._adresa;
		}
		return *this;
	}
	bool operator==(const TuristickaViza& druga) {
		return _drzava == druga._drzava && _vazenjeOD == druga._vazenjeOD && _vazenjeDO == druga._vazenjeDO
			&& strcmp(_napomena, druga._napomena) == 0 && _adresa == druga._adresa;
	}
	friend ostream & operator << (ostream & COUT, TuristickaViza & obj) {
		COUT << DrzavaChar[obj._drzava] << " " << obj._vazenjeOD << " " << obj._vazenjeDO << " " << obj._adresa << endl;
		COUT << ((obj._napomena != nullptr) ? obj._napomena : "") << endl;
		COUT << "STATUSI -> " << obj._statusi << endl;
		return COUT;

	}
};

void Print(const char * imePrezime, Datum  * datumRodjenja, const char * brojPasosa, vector<Viza * >vize, Viza * viza) {
	cout << crt << "Ime i prezime aplikanta: " << imePrezime << endl;
	this_thread::sleep_for(chrono::milliseconds(500));
	cout << "Datum rodjenja: " << *datumRodjenja << endl;
	this_thread::sleep_for(chrono::milliseconds(500));
	cout << "Broj pasosa: " << brojPasosa << endl;
	this_thread::sleep_for(chrono::milliseconds(500));
	vector<Viza*>::iterator it;
	for (it = vize.begin(); it != vize.end(); ++it) {
		if (*viza == (**it)) {
			TuristickaViza * pok = dynamic_cast<TuristickaViza *>(viza);
			if (pok != nullptr)
				pok->Info();
			else {
				PoslovnaViza * poslovna = dynamic_cast<PoslovnaViza *>(viza);
				poslovna->Info();
			}
		}
	}
}
class Aplikant {
	char * _imePrezime;
	Datum * _datumRodjenja;
	char * _brojPasosa;
	vector<Viza *> _vize;
public:
	Aplikant(const char * imePrezime, Datum datumRodjenja, const char * brPasosa) {
		int vel = strlen(imePrezime) + 1;
		_imePrezime = new char[vel];
		strcpy_s(_imePrezime, vel, imePrezime);
		vel = strlen(brPasosa) + 1;
		_brojPasosa = new char[vel];
		strcpy_s(_brojPasosa, vel, brPasosa);
		_datumRodjenja = new Datum(datumRodjenja);
	}
	~Aplikant() {
		delete[]_imePrezime; _imePrezime = nullptr;
		delete[]_brojPasosa; _brojPasosa = nullptr;
		delete _datumRodjenja; _datumRodjenja = nullptr;
		for (size_t i = 0; i < _vize.size(); i++)
		{
			delete _vize[i]; _vize[i] = nullptr;
		}
	}
	bool DodajVizu(Viza * viza) {
		if (!MladiOd(&viza->GetVazenjeOD(), _datumRodjenja))
			return false;

		vector<Viza*>::iterator it;
		for (it = _vize.begin(); it != _vize.end(); it++) {
			//za jednu drzavu u periodu od Do i viza me smije imati status ponistena ili istek
			if (viza->getDrzavu() == (*it)->getDrzavu()) {
				if (ManjiDatum(&viza->GetVazenjeOD(), &(*it)->GetVazenjeOD()) && ManjiDatum(&(*it)->GetVazenjeDO(), &viza->GetVazenjeDO())) {
					if (*(*it)->GetStatuse().GetEl1(((*it)->GetStatuse().GetTrenutno())) == PONISTENA ||
						*(*it)->GetStatuse().GetEl1(((*it)->GetStatuse().GetTrenutno())) == ISTEKLA)
						return false;
				}
			}
		}

		if (viza->getDrzavu() == SAD) {
			for (it = _vize.begin(); it != _vize.end(); it++) {
				if (*(*it)->GetStatuse().GetEl1(((*it)->GetStatuse().GetTrenutno())) == PONISTENA)
					return false;
			}
		}
		PoslovnaViza * pokazivac = dynamic_cast<PoslovnaViza*>(viza);
		if (pokazivac != nullptr)
			_vize.push_back(new PoslovnaViza(*pokazivac));
		else {
			TuristickaViza * turistViza = dynamic_cast<TuristickaViza *>(viza);
			_vize.push_back(new TuristickaViza(*turistViza));
		}
		return true;
	}
	/*STATUSI SE MOGU DODAVATI SAMO U REDOSLIJEDU U KOJEM SE NALAZE U ENUMERACIJI NPR.
		- NIJE MOGUĆE ODBITI VIZU AKO PRETHODNO NIJE IMALA STATUS DA JE ZAHTJEV_ZAPRIMLJEN;
	-NIJE MOGUĆE PONIŠTITI VIZU KOJA PRETHODNO NIJE IZDATA*/
	bool DodajStatus(Viza * viza, Status status, string razlogPromjeneStatusa = "") {
		
		vector<Viza*>::iterator it;
		for (it = _vize.begin(); it != _vize.end(); ++it) {
			if ((**it) == *viza) {
				if ((*it)->GetStatuse().GetTrenutno() >= 10)
					return false;
				if ((*it)->GetStatuse().GetTrenutno() > 0) {
					if (status == ODUSTAO || status == ODBIJENA || status == IZDATA &&
						*(*it)->GetStatuse().GetEl1((*it)->GetStatuse().GetTrenutno()-1) != ZAHTJEV_ZAPRIMLJEN)
						return false;
					if (status == ISTEKLA || status == PONISTENA && *(*it)->GetStatuse().GetEl1((*it)->GetStatuse().GetTrenutno()-1) != IZDATA)
						return false;

				}
				if ((*it)->GetStatuse().GetTrenutno() == 0) {
					if (status > 0)
						return false;
				}
				(*it)->GetStatuse().Dodaj(status, razlogPromjeneStatusa);
				return true;
			}
		}
		
		return false;


	}

	void PrintajVizu(Viza * viza) {
		PoslovnaViza * pok = dynamic_cast<PoslovnaViza*>(viza);
		if (pok != nullptr)
		{
			thread meshail(&Print, this->_imePrezime, this->_datumRodjenja, this->_brojPasosa, this->GetVize(), pok);
			meshail.join();
		}
		else {
			TuristickaViza * pok2 = dynamic_cast<TuristickaViza*>(viza);
			thread djani(&Print, this->_imePrezime, this->_datumRodjenja, this->_brojPasosa, this->GetVize(), pok2);
			djani.join();

		}
	}
	vector<Viza*> GetVizeByDrzava(Drzava drzava, Status status) {
		vector<Viza*> povratniVektor;
		vector<Viza*>::iterator it;
		for (it = _vize.begin(); it != _vize.end(); ++it) {
			if ((*it)->getDrzavu() == drzava) {
				for (int i = 0; i < (*it)->GetStatuse().GetTrenutno(); i++) {
					if (*(*it)->GetStatuse().GetEl1(i) == status)
						povratniVektor.push_back((*it));
				}
			}
		}
		return povratniVektor;
	}
	void ZamijeniRijec(const char * trazena, const char * zamjenska) {
		string pravilo = "\\b";
		pravilo = pravilo + trazena;

		pravilo += "\\b";
		string zamjeniME = zamjenska;
		vector<Viza*>::iterator it;
		for (it = _vize.begin(); it != _vize.end(); ++it) {
			for (int j = 0; j < (*it)->GetStatuse().GetTrenutno(); j++) {
				*(*it)->GetStatuse().GetEl2(j) = regex_replace(*(*it)->GetStatuse().GetEl2(j), regex(pravilo), zamjeniME);
			}
		}
	}
	friend ostream & operator << (ostream & COUT, Aplikant & obj) {
		COUT << obj._imePrezime << " " << *obj._datumRodjenja << " " << obj._brojPasosa << endl;
		COUT << "VIZE -> " << endl;
		for (size_t i = 0; i < obj._vize.size(); i++) {
			obj._vize[i]->Info();
		}
		return COUT;
	}

	vector<Viza * > GetVize() { return _vize; }
};

int main() {

	/****************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ĆE BITI OZNACENO KAO "RE"
	3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEČILO GUBLJENJE URAĐENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKOĐER NALAZI U FAJLU CODE.TXT
	5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIČNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUČAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOĆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
	6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAČENO.
	****************************************************************************/



	Datum danas(15, 2, 2017);
	Datum sutra(++danas); // inkrementiranjem se uvecava dan za 1
	Datum prekosutra;
	Datum dat15022017(15, 2, 2017), dat18082017(18, 8, 2017), dat11052018(11, 5, 2018), dat11122017(11, 12, 2017);


	prekosutra = danas;
	if (prekosutra == danas)
		cout << "Datumi su jednaki" << endl;
	cout << danas << endl << sutra << endl << prekosutra << crt;

	int v6 = 6, v13 = 13, v32 = 32, v63 = 63, v98 = 98, v109 = 109, v196 = 196;

	FITKolekcija<int, int, 7> brojevi;
	brojevi.Dodaj(v196, v6);
	brojevi.Dodaj(v13, v32);
	brojevi.Dodaj(v98, v196);
	brojevi.Dodaj(v63, v13);
	brojevi.Dodaj(v98, v196);
	brojevi.Dodaj(v196, v6);

	cout << brojevi << crt;
	//NA KOJI NAČIN ĆE SE VRŠITI SORTIRANJE PRVENSTVENO ZAVISI OD VRIJEDNOSTI PARAMETRA, A VRŠI SE NA OSNOVU VRIJEDNOSTI TIPA T1
	brojevi.Sortiraj(RASTUCI);
	cout << brojevi << crt;
	brojevi.Sortiraj(OPADAJUCI);
	cout << brojevi << crt;

	try
	{
		//U SLUČAJU PREKORAČENJA DEFINISANE VELIČINE KOLEKCIJE, POTREBNO JE BACITI IZUZETAK
		brojevi.Dodaj(v13, v196);
		//brojevi.Dodaj(v13, v196);
	}
	catch (exception & err)
	{
		cout << "Error -> " << err.what() << endl;
	}

	Aplikant jasmin("Jasmin Azemovic", Datum(15, 5, 1979), "BS6985645");
	//cout << jasmin << crt;
	PoslovnaViza poslovna(SAD, dat15022017, dat18082017, "Microsoft");
	//cout << poslovna << crt;

	/*
	NOVA VIZA SE NE MOŽE DODATI UKOLIKO:
	- JE APLIKANT MLAĐI OD 10 GODINA
	- U TOM PERIODU (ODNOSI SE NA PERIOD VIZE OD - DO) NE POSTOJI VAŽEĆA (POJAM VAŽEĆA PODRAZUMIJEVA DA JE POSLJEDNJI STATUS VIZE BIO IZDATA
	TJ. DA VIZA NIJE PONIŠTENA ILI ISTEKLA) VIZA ZA TU DRŽAVU, BEZ OBZIRA NA NJENU VRSTU.
	- JE NEKA OD RANIJIH VIZA PONIŠTENA (OVO SE ODNOSI SAMO NA VIZE ZA DRŽAVU: SAD)
	*/
	if (jasmin.DodajVizu(&poslovna))
		cout << "Podaci o vizi uspješno zaprimljeni" << endl;
	/*
	STATUSI SE MOGU DODAVATI SAMO U REDOSLIJEDU U KOJEM SE NALAZE U ENUMERACIJI NPR.
	- NIJE MOGUĆE ODBITI VIZU AKO PRETHODNO NIJE IMALA STATUS DA JE ZAHTJEV_ZAPRIMLJEN;
	- NIJE MOGUĆE PONIŠTITI VIZU KOJA PRETHODNO NIJE IZDATA
	*/
	if (jasmin.DodajStatus(&poslovna, ZAHTJEV_ZAPRIMLJEN, "provjeriti navode sa predstavnicima kompanije Microsoft"))
		cout << "Status vize uspijesno promijenjen" << endl;
	else
		cout << "Aplikant " << jasmin << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;

	if (jasmin.DodajStatus(&poslovna, IZDATA, "provjeriti navode sa predstavnicima kompanije Microsoft"))
		cout << "Status vize uspijesno promijenjen" << endl;
	if (jasmin.DodajStatus(&poslovna, PONISTENA, "predstavnicima kompanije Microsoft nemaju informaciju o posjeti navedenog aplikanta"))
		cout << "Status vize uspijesno promijenjen" << endl;
	else
		cout << "Aplikant " << jasmin << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;

	TuristickaViza turisticka(ENGLESKA, dat18082017, dat11122017, "Whitehall Pl, London SW1A 2BD, UK");

	if (jasmin.DodajVizu(&turisticka))
		cout << "Podaci o vizi uspješno zaprimljeni" << endl;
	if (jasmin.DodajStatus(&turisticka, ZAHTJEV_ZAPRIMLJEN))
		cout << "Status vize uspijesno promijenjen" << endl;
	else
		cout << "Aplikant " << jasmin << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;

	if (jasmin.DodajStatus(&turisticka, IZDATA)) {
		cout << "Status vize uspijesno promijenjen" << endl;
		//U ZASEBNOJ NITI URADITI PRINTANJE VIZE...KORISTITI METODU SLEEP_FOR
		jasmin.PrintajVizu(&turisticka);
	}

	else
		cout << "Aplikant " << jasmin << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;
	//VRAĆA PODATKE O SVIM VIZAMA KOJE SU IZDATE ZA ODREĐENU DRŽAVU, A KOJE SU IMALE ODREĐENI STATUS
	vector<Viza *> jasminVize = jasmin.GetVizeByDrzava(SAD, PONISTENA);
	vector<Viza*>::iterator it;
	for (it = jasminVize.begin(); it != jasminVize.end(); ++it)
		(*it)->Info();

	//UNUTAR NAPOMENE MIJENJA SADRŽAJ "nemaju informaciju" SA "su upoznti sa" - KORISTITI REGEX
	jasmin.ZamijeniRijec("nemaju informaciju", "su upoznti sa");
	for (it = jasminVize.begin(); it != jasminVize.end(); ++it)
		(*it)->Info();

	system("PAUSE");
}
