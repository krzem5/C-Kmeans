#include <kmeans.h>
#include <stdlib.h>
#include <time.h>



int main(int argc,const char** argv){
	srand((int)(time(NULL)));
	Dataset dt=read("..\\o.dt");
	// elbow(dt,10000,2,10);
	Kmeans km=init(3,dt);
	train(10000,km);
	return 0;
}
