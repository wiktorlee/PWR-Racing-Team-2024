#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<cmath>		/*DLA FUNKCJI ATAN 2 ORAZ PI*/
#include<utility>	/*DLA STD::PAIR*/
using namespace std;

/*Funkcja, która znajduje punkt najbardziej wychylony w lewą stronę, czyli którego współrzędna x jest najmniejsza.
  Jeśli punkty mają takie same współrzędne x, wybierany jest punky z wiekszą współrzędną y. Jest to zgodne z zamysłem algorytmu Jarvisa.*/

pair<double, double> max_left_choose(const vector<vector<double>>& wspolrzedne, int liczba_punktow) {
	double max_left_x = wspolrzedne[0][0];
	double max_left_y = wspolrzedne[0][1];												
	for (int i = 1; i < liczba_punktow; i++) {
		if (wspolrzedne[i][0] < max_left_x) {
			max_left_x = wspolrzedne[i][0];
			max_left_y = wspolrzedne[i][1];

		}
		if (wspolrzedne[i][0] == max_left_x) {
			if (wspolrzedne[i][1] > max_left_x) {
				max_left_x = wspolrzedne[i][0];
				max_left_y = wspolrzedne[i][1];
			}
		}
	}
	return make_pair(max_left_x,max_left_y);

}

/*	Funkcja mająca na celu wybór punktu, z którym obecnie wybrany punkt (poczatkowo jest to punkt max_left wyznaczony w funkcji max_left_choose) utworzy najmniejszy kąt
  liczony w orientacji pionowej zgodnie z ruchem wskazówek zegara. Miała być to moja własna interpretacja algorytmu Jarvisa, pozwalającego na znalezienie
  otoczki wypukłej zbioru punktów. Algorym miał za zadanie dążyć do stworzenia otoczki za pomoca jak najmniejszej ilości punktów. Pierwszy punkt wybiera sie poprawnie, 
  gorzej z kolejnymi. Źródłem błedu jest zapewne niewydajna, prowizoryczna pętla while, która miała pełnić bardziej ,,testową'' rolę. 
	Funkcja oblicza różnicę wektorów dwóch punktów a następnie wylicza w radianach kąt odcinka je łączącego za pomocą funkcji atan2. Argumenty funkcji atan2 są wpisane na odwrót
  nie bez powodu. Domyślnie atan2 oblicza kąt względem osi x. Odwrócenie argumentów pozwala na obliczenie kąta względem osi y, a zatem wedle mojego zamysłu. Jesli badany kąt jest ujemny,
  zostaje przekształcony na odpowiadającą mu wartość dodatnią. Następnie jeśli otrzymany kąt jest mniejszy od domyślnego 2*pi resetującego się co jedno wykonanie pętli, zostaje wstępnie 
  wybrany jako potencjalny nowy punkt tworzący otoczkę. Po sprawdzeniu wszystkich połączeń, wybrany punkt miał być dodawany do vectora, a następnie na samym końcu programy wypisany.
  Niestety obecna forma pętli nie pozwala na skuteczne umieszczenie tego vectora. Po powrocie do punktu startowego, czyli w zamyśle, po stworzeniu pełnej otoczki wypukłej, pętla miała się
  zatrzymać. Niestety, nie udało mi się rozgryźć i naprawić tego problemu.
  */

vector<vector<double>> siatka(pair<double, double>max_left, const vector<vector<double>>& wspolrzedne, int liczba_punktow) {
	const double pi = 3.14159265358979323846;
	double baza_x = max_left.first;
	double baza_y = max_left.second;
	vector<vector<double>> siatka_polaczen;
	siatka_polaczen.push_back({ baza_x, baza_y });
	 
	bool koniec = true;
	while (koniec) {
		for (int i = 0; i < liczba_punktow; i++) {
			double default_angle = 2 * pi;

			double dx = wspolrzedne[i][0] - baza_x;
			double dy = wspolrzedne[i][1] - baza_y;
			double d_angle = atan2(dx, dy);
			if (d_angle < 0) {
				d_angle += 2 * pi;
			}
			
			if (d_angle < default_angle) {
				baza_x = wspolrzedne[i][0];
				baza_y = wspolrzedne[i][1];
				default_angle = d_angle;
				siatka_polaczen.push_back({ baza_x, baza_y });
				
			}
			if (baza_x == max_left.first && baza_y == max_left.second) {
				
				koniec = false;
			}
			
		}
		
	}
	return siatka_polaczen;
}



int main() {

	/*Klasyczne otwarcie pliku do odczytu danych.*/

	ifstream punkty("punkty.txt");
	if (!punkty.is_open()){
		cerr << "Blad otwarcia pliku" << endl;
		return 1;
	}
	int liczba_punktow;
	punkty >> liczba_punktow;

	cout << "Liczba punktow wynosi: "<<" "<<liczba_punktow << endl;

	vector<vector<double>> wspolrzedne(liczba_punktow, vector<double>(2));
	for (int i = 0; i < liczba_punktow; i++){
		punkty >> wspolrzedne[i][0] >> wspolrzedne[i][1];
	}
	for (int i = 0; i < liczba_punktow; i++){
		cout <<"("<< wspolrzedne[i][0] << "," << wspolrzedne[i][1] <<")"<< endl;
	}

	/*Wywołanie funkcji wraz z wypisaniem otrzymanych danych*/

	pair<double,double> max_left = max_left_choose(wspolrzedne, liczba_punktow);
	cout << "Punkt wychylony maksymalnie w lewo : " <<"("<< max_left.first << "," << max_left.second <<")"<< endl;

	vector<vector<double>> siatka_polaczen = siatka(max_left, wspolrzedne, liczba_punktow);
	cout <<"Pierwsze polaczenie: ("<<siatka_polaczen[1][0] << "," << siatka_polaczen[1][1] <<")"<< endl;
	
	cout << "Polaczenia tworzace obrys: " << endl;
	for (const auto& punkt : siatka_polaczen) {
		cout << "(" << punkt[0] << "," << punkt[1] << ")" << endl;
	}
	
	/*Zamknięcie pliku i programu*/
	punkty.close();
	return 0;
}