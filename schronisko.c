#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "mpi.h"

//status
#define OTWARTE 1
#define ZAMKNIETE 2
#define ZaopatrzenieNaWykonczeniu 3
#define EKSTREMALNIE 4
//~status

int tag = 7;
int wys[2];
int odb[2];
int proc_il;
int proc_nr;
int zaopatrzenie = 30;
MPI_Status mpi_status;

void Wyslij(int proces_nr, int status)
{
	wys[0]=proces_nr;
	wys[1]=status;
	MPI_Send(&wys, 2, MPI_INT, 0, tag, MPI_COMM_WORLD);
	sleep(1);
}

void Symulator(int proc_il) {
	int proces_nr;
	int status;
	printf("Symulator schroniska\n");
	sleep(1);
	int petla = 2;
	while(petla > 1){
		MPI_Recv(&odb,2,MPI_INT,MPI_ANY_SOURCE,tag,MPI_COMM_WORLD, &mpi_status);
		proces_nr = odb[0];
		status = odb[1];
		if(status==1){
			printf("%d: Schronisko otwarte, zaopatrzenie w normie. (%d)\n", proces_nr, zaopatrzenie);
		}
		if(status==2){
			printf("%d: Zamkniete. Dostawa zaopatrzenia czy cos", proces_nr);
		}
		if(status==3){
			printf("%d: Zaopatrzenie na wykonczeniu!", proces_nr);
		}
		if(status==4){ 
			printf("%d: Katastrofa :( \n", proces_nr);
            petla = 1;
		}
	}
}

void Schronisko() {
	int status;
	int petla = 2;
	status=OTWARTE;
	while(petla > 1){
		if(status==1){
			if(rand()%4==1) {
				status = EKSTREMALNIE;
				Wyslij(proc_nr, status);
			}else if(zaopatrzenie > 10){
				printf("%d: Schronisko otwarte, zaopatrzenie w normie. (%d)\n", proc_nr, zaopatrzenie);
				zaopatrzenie -= 7;
				Wyslij(proc_nr, status);
			}else {
				status = ZaopatrzenieNaWykonczeniu;
				Wyslij(proc_nr, status);
			}
		} else if(status==2){
			printf("%d: Zamkniete. Dostawa zaopatrzenia czy cos", proc_nr);
			status = OTWARTE;
			Wyslij(proc_nr, status);
		} else if(status==3){
			printf("%d: Zaopatrzenie na wykonczeniu!", proc_nr);
			status = ZAMKNIETE;
			Wyslij(proc_nr, status);

        } else if(status==4){
           printf("%d: Katastrofa :( \n", proc_nr);
		   petla = 1;
        }
    }
}

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