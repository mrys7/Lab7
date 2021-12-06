# PRiR zadania lab 7
Symulator schroniska (wzorowany na symulatorze w javie z wcześniejszych zajęć) wersja MPI
```cpp
static int otwarte = 1; //schronisko jest otwarte
static int zamkniete = 2; //dostawa zaopatrzenia
static int zaopatrzenie_na_wykonczeniu = 3; //schronisko się zamyka
static int warunki_ekstremalne = 4; //zdarzenie ekstremalne/katastrofa - wyjście z pętli
```
Funkcja wysyłająca dane
```cpp
void Wyslij(int proces_nr, int status); 
```
Odbieranie danych MPI_Recv, wyświetlanie komunikatów
```cpp
void Symulator(int proc_il);
```
Manipulacja schroniskiem (zmiana statusów, pomniejszanie zaopatrzenia)
```cpp
void Schronisko();
```
inicjalizacja MPI, sprawdzanie numeru procesu: 
		   ==0 -> wywołanie Symulatora, 
		   !=0 -> wywołanie schroniska dla procesu
```cpp
int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&proc_nr);
	MPI_Comm_size(MPI_COMM_WORLD,&proc_il);
	if(proc_nr == 0) Symulator(proc_il);
	else{
        srand(time(NULL));
		Schronisko();
    }
	MPI_Finalize();
	return 0;
}
```
