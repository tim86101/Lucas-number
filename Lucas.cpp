#include<iostream>
#include<unistd.h>
#include<stdio.h>

#include <string.h>
#include <stdlib.h>

#include <sys/file.h>
#include <sys/mman.h>
#include <sys/wait.h>
using namespace std;
//void Lucas(int number);

void error_and_die(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}
struct Lucas{
   int n_1 = 1;
   int n_2 = 2;
   int n=3;
  int control=0;
};
int main() {
	int number;
	cin>>number;
	int r;

	const char *memname = "Lucas_shm";
	const size_t region_size = sysconf(_SC_PAGE_SIZE);

	int fd = shm_open(memname, O_CREAT | O_TRUNC | O_RDWR, 0666);
	if (fd == -1)//open shm fail
	error_and_die("shm_open");

	r = ftruncate(fd, region_size);//return fd into sizeof(region_size)
	if (r != 0)
	error_and_die("ftruncate");

	void *shm = mmap(0, region_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (shm == MAP_FAILED)
	error_and_die("mmap");
	close(fd);

	
	Lucas *d = (Lucas *) shm;
	d->n_1 = 1;
	d->n_2 = 2;
	d->n=3;
	d->control = 0;
	pid_t pid = fork();
	for(int i=2;i<=number;i++){
		
		//int status=-1;
		if(pid>0){
			while(d->control!=0);
			if(i!=number)
				cout<<"["<<getppid()<<" parent]:"<<d->n_1<<" "<<d->n_2<<endl;
			else
				cout<<"["<<getppid()<<" parent]:"<<"L("<<number<<") = "<< d->n<<endl;
			//wait(&status);
			d->n_2 = d->n_1;
			d->n_1 = d->n;
			d->control = 1;
		}
		else if(pid==0){
			while(d->control != 1);
			d->n = d->n_1 + d->n_2;
			cout<<"["<<getpid()<<" child]:"<<d->n_1<<"+"<<d->n_2<<"="<<d->n<<endl;
			d->control = 0;
			//kill(getpid(),SIGKILL);
		}	
	}
	r = munmap(shm, region_size);
	if (r != 0)
	error_and_die("munmap");

	r = shm_unlink(memname);
	if (r != 0)
	error_and_die("shm_unlink");

	return 0;
}
