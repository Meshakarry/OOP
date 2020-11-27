#include<iostream>
#include<vector>
#include<string>
#include<thread>
#include<regex>
#include<mutex>

const char *crt = "\n---------------------------------------\n";
using namespace std;


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
	DatumVrijeme& operator=(DatumVrijeme&drugi)
	{
		if (this != &drugi) {
			*_dan = *drugi._dan;
			*_mjesec = *drugi._mjesec;
			*_godina = *drugi._godina;
			*_sati = *drugi._sati;
			*_minuti = *drugi._minuti;
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
	friend ostream& operator<< (ostream &COUT, DatumVrijeme &obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina << " " << *obj._sati << ":" << *obj._minuti;
		return COUT;
	}
	bool operator==(DatumVrijeme & d1)
	{
		return *d1._dan == *_dan && *d1._mjesec == *_mjesec && *d1._godina == *_godina && *d1._sati == *_sati;

	}
};

template<class T1, class T2 = int>
class FITKolekcija {
	T1 * _elementi1;
	T2 * _elementi2;
	int _trenutno;
public:
	FITKolekcija() {
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutno = 0;
	}
	~FITKolekcija() {
		delete[] _elementi1; 
		_elementi1 = nullptr;
		delete[] _elementi2;
		_elementi2 = nullptr;
	}
	FITKolekcija(const FITKolekcija & kol)
	{
		
		_trenutno = kol._trenutno;
		_elementi1 = new T1[kol._trenutno];
		_elementi2 = new T2[kol._trenutno];
		for (size_t i = 0; i < kol._trenutno; i++)
		{
			_elementi1[i] = kol._elementi1[i];
			_elementi2[i] = kol._elementi2[i];
		}
	}
	FITKolekcija<T1,T2> & operator=(const FITKolekcija & kol)
	{
		if (this != &kol)
		{
			if (_elementi1 != nullptr)
				delete[] _elementi1;
			if (_elementi2 != nullptr)
				delete[] _elementi2;

			_trenutno = kol._trenutno;
			_elementi1 = new T1[kol._trenutno];
			_elementi2 = new T2[kol._trenutno];
			for (size_t i = 0; i < kol._trenutno; i++)
			{
				_elementi1[i] = kol._elementi1[i];
				_elementi2[i] = kol._elementi2[i];
			}

		}
		return *this;
	}
	void Dodaj(T1 &a, T2 & b)
	{
		T1 *temp = new T1[_trenutno + 1];
		T2 * temp2 = new T2[_trenutno + 1];

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
	void Sortiraj() {
		bool promjena = true;
		while (promjena) {
			promjena = false;
			for (size_t i = 0; i < _trenutno - 1; i++)
			{

				if (_elementi1[i] > _elementi1[i + 1])
				{
					swap(_elementi1[i], _elementi1[i + 1]);
					swap(_elementi2[i], _elementi2[i + 1]);
					promjena = true;
				}
			}
		}
	}
	void DodajNaLokaciju(T1 & a, T2 &b, int lokejsen)
	{
		T1 *jedan = new T1[_trenutno + 1];
		T2 *dva = new T2[_trenutno + 1];
		jedan[lokejsen] = a;
		dva[lokejsen] = b;

		for (size_t i = 0; i < _trenutno+1; i++)
		{
			if (i < lokejsen)
			{
				jedan[i] = _elementi1[i];
				dva[i] = _elementi2[i];
			}
			else if (i > lokejsen)
			{
				jedan[i] = _elementi1[i - 1];
				dva[i] = _elementi2[i - 1];
			}
		}
		delete[] _elementi1;
		delete[] _elementi2;
		_elementi1 = jedan;
		_elementi2 = dva;
		_trenutno++;

	}
	FITKolekcija<T1, T2> * GetJedinstveni() {

		FITKolekcija < T1, T2 > * jedinstven= new FITKolekcija<T1, T2>;
		for (size_t i = 0; i < _trenutno; i++)
		{
			jedinstven->Dodaj(_elementi1[i], _elementi2[i]);
			if (_elementi1[i] == _elementi1[i + 1] && _elementi2[i] == _elementi2[i + 1])
				i++;
		}
		return jedinstven;
	}
	T1 * GetT1() { return _elementi1; }
	T2 * GetT2() { return _elementi2; }
	int GetTrenutno() { return _trenutno; }
	void remove(int lok)
	{
		for (size_t i = lok; i < _trenutno-1; i++)
		{
			_elementi1[i] = _elementi1[i + 1];
			_elementi2[i] = _elementi2[i + 1];
		}
		_trenutno--;

	}
	friend ostream& operator<< (ostream &COUT, FITKolekcija &obj) {
		for (size_t i = 0; i < obj._trenutno; i++)
			COUT << obj._elementi1[i] << " " << obj._elementi2[i] << endl;
		return COUT;
	}
};

enum vrstaDokumenta { PDF, DOC, TXT, HTML };


class Dokument {
	vrstaDokumenta _vrsta;
	string _naziv;
	string _sadrzaj;
	int _brojStranica;
public:
	Dokument() { _brojStranica = 0; }
	Dokument(vrstaDokumenta vrsta, string naziv) {
		_naziv = naziv; _vrsta = vrsta; _brojStranica = 0;
	}
	string GetNaziv() { return _naziv; };
	string GetSadrzaj() { return _sadrzaj; };
	vrstaDokumenta GetVrsta() { return _vrsta; };
	void UvecajBrojStranica() { _brojStranica++; }
	int GetBrojStranica() { return _brojStranica; }

	friend ostream & operator<<(ostream&COUT, Dokument & obj) {
		COUT << obj._naziv << "." << obj._vrsta << " (" << obj._brojStranica << ")\n" << obj._sadrzaj << endl;
		return COUT;
	}
	void DodajSadrzaj(string txt)
	{
		float stranica = txt.length() / 30.;
		//ceil vraca integer vrijednost koja je najbliza ovoj koju smo dobili u stranica(npr 10.25 bilo bi 11)//
		_brojStranica+= ceil(stranica);
		_sadrzaj += txt;

	}
};
class Uredjaj {
protected:
	char * _proizvodjac;
	char * _model;
public:
	Uredjaj(const char * proizvodjac, const char * model) {
		int vel = strlen(proizvodjac) + 1;
		_proizvodjac = new char[vel];
		strcpy_s(_proizvodjac, vel, proizvodjac);
		vel = strlen(model) + 1;
		_model = new char[vel];
		strcpy_s(_model, vel, model);
	}
	~Uredjaj() {
		delete[] _proizvodjac; _proizvodjac = nullptr;
		delete[] _model; _model = nullptr;
	}
	Uredjaj(const Uredjaj & u)
	{
		int vel = strlen(u._proizvodjac) + 1;
		_proizvodjac = new char[vel];
		strcpy_s(_proizvodjac, vel, u._proizvodjac);
		vel = strlen(u._model) + 1;
		_model = new char[vel];
		strcpy_s(_model, vel, u._model);
	}
	Uredjaj & operator=(Uredjaj & u)
	{
		if (this != &u)
		{
			if (_proizvodjac != nullptr)
				delete[] _proizvodjac;
			if (_model != nullptr)
				delete[] _model;
			int vel = strlen(u._proizvodjac) + 1;
			_proizvodjac = new char[vel];
			strcpy_s(_proizvodjac, vel, u._proizvodjac);
			vel = strlen(u._model) + 1;
			_model = new char[vel];
			strcpy_s(_model, vel, u._model);

		}
		return *this;
	}
	friend ostream & operator<<(ostream&COUT, Uredjaj & obj) {
		COUT << obj._proizvodjac << "." << obj._model << endl;
		return COUT;
	}
};
mutex stedim;
class Printer: public Uredjaj {
	FITKolekcija<DatumVrijeme, Dokument> _printaniDokumenti;
	vector<string> _zabranjeneRijeci;
public:
	Printer(const char * proizvod, const char * model):Uredjaj(proizvod,model) {

	}
	Printer(const Printer& drugi) :Uredjaj(drugi) {
		_printaniDokumenti = drugi._printaniDokumenti;
		_zabranjeneRijeci = drugi._zabranjeneRijeci;
	}
	void ispis(string sadrzaj)
	{
		stedim.lock();
		for (size_t i = 0; i < sadrzaj.size(); i++)
		{
			if (i == 30)
			{
				this_thread::sleep_for(chrono::seconds(2));
			}
			cout << sadrzaj[i];
		}
		
		cout << endl;
		stedim.unlock();
	}
	void DodajZabranjenuRijec(string rijecica){

		vector<string>::iterator it;
		for ( it = _zabranjeneRijeci.begin(); it != _zabranjeneRijeci.end(); it++)
		{
			if (*it == rijecica)
				return;


		}
		_zabranjeneRijeci.push_back(rijecica);

	}
	void Printaj(DatumVrijeme & d, Dokument doc)
	{
		_printaniDokumenti.Dodaj(d, doc);
	
		
		string pravilo = "([a-zA-Z]{5,15})([.]{1})(pdf|PDF|doc|DOC|txt|TXT|html|HTML)";


		string zabranjene = "";
		for (size_t i = 0; i < _zabranjeneRijeci.size()-1; i++)
		{
			zabranjene += _zabranjeneRijeci[i] + "|";
		
	   }
		zabranjene += _zabranjeneRijeci[_zabranjeneRijeci.size() - 1];

		if (!regex_match(doc.GetNaziv(), regex(pravilo)) || regex_search(doc.GetSadrzaj(), regex(zabranjene))) {
			throw exception("Sadrzaj sadrzi zabranjenu rijec ili ne odgovara trazenom formatu!");
		}
		
		else {
			thread zaPrintanje(&Printer::ispis, this, doc.GetSadrzaj());
			zaPrintanje.join();
		}
		

	}
	int GetProsjecanBrojStranicaPoDatumu(DatumVrijeme & dateAndHour) {
		int printanoDanas = 0;
		for (size_t i = 0; i < _printaniDokumenti.GetTrenutno(); i++)
		{
			if (_printaniDokumenti.GetT1()[i]==dateAndHour){

				printanoDanas += _printaniDokumenti.GetT2()[i].GetBrojStranica();
				}
		}
		return printanoDanas;

    }

	string GetTopZabranjenuRijec() {
		string repeat;
		int brojPonavljanjarijeciMAX = 0;
		int *nizRijeci = new int[_zabranjeneRijeci.size()];
		for (size_t i = 0; i < _zabranjeneRijeci.size(); i++)
		{
			nizRijeci[i] = 0;
		}

		string pravilo = "";
		for (int i = 0; i < _zabranjeneRijeci.size(); i++) {
			pravilo = _zabranjeneRijeci[i];
			for (int j = 0; j < _printaniDokumenti.GetTrenutno(); j++) {
				if (regex_search(_printaniDokumenti.GetT2()[j].GetSadrzaj(), regex(pravilo))) {
					nizRijeci[i]++;
				}
			}
		}

		for (size_t i = 0; i < _zabranjeneRijeci.size(); i++)
		{
			if (nizRijeci[i] > brojPonavljanjarijeciMAX) {

				brojPonavljanjarijeciMAX = nizRijeci[i];
				repeat = _zabranjeneRijeci[i];

			}
		}

		return repeat;

	}
	void UkloniDokumente() {
		string pravilo = "([a-zA-Z]{5,15})([.]{1})(pdf|PDF|doc|DOC|txt|TXT|html|HTML)";

		string zabranjene = "";
		for (size_t i = 0; i < _zabranjeneRijeci.size() - 1; i++)
		{
			zabranjene += _zabranjeneRijeci[i] + "|";

		}
		zabranjene += _zabranjeneRijeci[_zabranjeneRijeci.size() - 1];
		for (size_t i = 0; i < _printaniDokumenti.GetTrenutno(); i++)
		{


			if (!regex_match(_printaniDokumenti.GetT2()[i].GetNaziv(), regex(pravilo))
			|| regex_search(_printaniDokumenti.GetT2()[i].GetSadrzaj(), regex(zabranjene)))
			{
				_printaniDokumenti.remove(i);
			}
		}
			
	}
	friend ostream& operator<<(ostream& COUT, Printer & p)
	{
		COUT << (Uredjaj&)(p) << endl;
		COUT << " DOKUMENTI ::"  << endl;
		COUT << p._printaniDokumenti << endl;
		COUT << " :: Zabranjene Rijeci :: " << endl;

		for (size_t i = 0; i < p._zabranjeneRijeci.size(); i++)
		{
			COUT << p._zabranjeneRijeci[i] << endl;
		}
		return COUT;
	}
	FITKolekcija<DatumVrijeme, Dokument> & GetPrintaniDokumenti() { return _printaniDokumenti; };
	vector<string> & GetZabranjeneRijeci() { return _zabranjeneRijeci; };
};

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

#pragma region TestiranjeDatumVrijeme
	DatumVrijeme danas(1, 2, 2017, 10, 15);
	DatumVrijeme sutra(danas);
	DatumVrijeme prekosutra;
	prekosutra = danas;
	cout << danas << endl << sutra << endl << prekosutra << crt;
#pragma endregion
#pragma region FITKolekcija
	int v6 = 6, v13 = 13, v32 = 32, v63 = 63, v98 = 98, v109 = 109, v196 = 196;

	FITKolekcija<int, int> brojevi;
	brojevi.Dodaj(v196, v6);
	brojevi.Dodaj(v13, v32);
	brojevi.Dodaj(v98, v196);
	brojevi.Dodaj(v63, v13);
	brojevi.Dodaj(v98, v196);
	brojevi.Dodaj(v196, v6);

	cout << brojevi << crt;
	//SORTIRANJE ČLANOVA KOLEKCIJE SE VRŠI U RASTUĆEM REDOSLIJEDU NA OSNOVU VRIJEDNOSTI ELEMENTA TIPA T1
	brojevi.Sortiraj();
	cout << brojevi << crt;
	//BROJ 2 SE ODNOSI NA LOKACIJU/INDEKS UNUTAR LISTE NA KOJI JE POTREBNO DODATI NOVE ELEMENTE
	brojevi.DodajNaLokaciju(v109, v6, 2);
	cout << brojevi << crt;
	brojevi.Sortiraj();
	cout << brojevi << crt;
	///*METODA GetJedinstveni VRAĆA LISTU JEDINSTVENIH ELEMENATA TJ. ELEMENATA KOJI NE SADRŽE DUPLIKATE
	//(POJAM DUPLIKAT SE ODNOSI NA ISTE VRIJEDNOSTI ELEMENATA T1 I T2). ELEMENTI KOJI SE DUPLIRAJU SE U OVOJ LISTI TREBAJU POJAVITI SAMO JEDNOM.*/
	FITKolekcija<int, int> * jedinstveni = brojevi.GetJedinstveni();
	cout << *jedinstveni << crt;
	*jedinstveni = brojevi;
	cout << *jedinstveni << crt;

	Dokument ispitPRIII(DOC, "ispitPRIII.doc");
//	//BROJ STRANICA DOKUMENTA SE AUTOMATSKI ODREĐUJE PRILIKOM DODAVANJA SADRŽAJA. ZA POTREBE ISPITA PRETPOSTAVLJAMO DA NA JEDNU STRANICU MOŽE STATI 30 
	//ZNAKOVA UKLJUČUJUĆI I RAZMAKE
	ispitPRIII.DodajSadrzaj("NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ĆE BITI OZNACENO KAO RE");
	cout << "Broj stranica -> " << ispitPRIII.GetBrojStranica() << endl;
	Dokument ispitBaze(DOC, "ispitBaze.doc");
	ispitBaze.DodajSadrzaj("PROGRAMSKI CODE SE TAKOĐER NALAZI U FAJLU CODE.TXT");
	cout << ispitBaze << endl;//ISPISUJE SVE DOSTUPNE PODATKE O DOKUMENTU
//
	Printer hp3200("HP", "3200");
//	//PRINTER NEĆE DOZVOLITI PRINTANJE DOKUMENATA U ČIJEM SADRŽAJU SE NALAZI NEKA OD ZABRANJENIH RIJEČI
	hp3200.DodajZabranjenuRijec("RE");// :)
	hp3200.DodajZabranjenuRijec("RAT");
	hp3200.DodajZabranjenuRijec("UBITI");
	hp3200.DodajZabranjenuRijec("RE");// ONEMOGUĆITI PONAVLJANJE ZABRANJENIH RIJEČI, KORISTITI ITERATORE
	hp3200.DodajZabranjenuRijec("MRZITI");
	try
	{
		/*
		DA BI PRINTER ISPRINTAO NEKI DOKUMENT MORAJU BITI ZADOVOLJENA SLJEDEĆA PRAVILA:
		1. NAZIV DOKUMENTA MORA BITI U SLJEDEĆEM FORMATU npr: ispitPRIII.doc
		NAZIV DOKUMENTA MOŽE SADRŽAVATI SAMO SLOVA (NAJMANJE 5, NAJVIŠE 15), A EKSTENZIJA MOŽE
		BITI SAMO NEKA OD DOZVOLJENIH VRSTOMDOKUMENTA
		2. SADRŽAJ DOKUMENTA U SEBI NE SMIJE POSJEDOVATI NITI JEDNU ZABRANJENU RIJEČ
		UKOLIKO NEKI OD NAVEDENIH USLOVA NIJE ZADOVOLJEN FUNKCIJA TREBA DA BACI EXCEPTION SA
		ODGOVARAJUĆOM PORUKOM, ALI ĆE DOKUMENT BITI POHRANJEN U KOLEKCIJU _printaniDokumenti.
		UKOLIKO DOKUMENT ISPUNJAVA SVE USLOVE ZA PRINTANJE POTREBNO JE NA KONZOLU ISPISATI
		SADRŽAJ DOKUMENTA KOJI SE PRINTA
		*/
		hp3200.Printaj(danas, ispitPRIII);
		hp3200.Printaj(danas, ispitBaze);

		/*
		IMPLEMENTIRATI PRINTANJE U ZASEBNIM NITIMA UZ KORIŠTENJE sleep_for FUNKCIJE
		VODITI RAČUNA O DIJELJENJU ZAJEDNIČKIH RESURSA
		*/

	}
	catch (exception& err)
	{
		cout << err.what() << endl;
	}
//
//	//KOD POREĐENJA DATUMA PRINTANJA NIJE POTREBNO POREDITI MINUTE, SAMO DATUM I SATE
	cout << "Prosjecan broj printanih stranica za " << danas << " je -> " << hp3200.GetProsjecanBrojStranicaPoDatumu(danas) << crt;
	cout << "Najcesce koristena zabranjena rijec je -> " << hp3200.GetTopZabranjenuRijec() << crt;
//
	Printer hp4000(hp3200);
	cout << "Prosjecan broj printanih stranica za " << danas << " je -> " << hp4000.GetProsjecanBrojStranicaPoDatumu(danas) << crt;
	cout << "Najcesce koristena zabranjena rijec je -> " << hp4000.GetTopZabranjenuRijec() << crt;
	hp4000.UkloniDokumente();//UKLANJA SVE DOKUMENTE KOJI NISU ZADOVOLJILI USLOVE ZA PRINTANJE
	cout << hp4000 << crt;
//
#pragma endregion
	cin.get();
	system("pause");
}
