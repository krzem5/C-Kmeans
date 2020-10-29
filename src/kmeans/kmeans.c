#include <kmeans.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>



Dataset read(const char* fp){
	Dataset o=malloc(sizeof(struct _DATASET));
	FILE* f=NULL;
	fopen_s(&f,fp,"rb");
	fseek(f,0,SEEK_END);
	size_t sz=ftell(f)/(2*sizeof(float));
	rewind(f);
	o->ln=sz;
	o->e=malloc(o->ln*sizeof(struct _DATASET_ELEM));
	o->b=malloc(4*sizeof(float));
	for (size_t i=0;i<sz;i++){
		float t[]={0,0};
		fread(t,2,sizeof(float),f);
		(o->e+i)->x=*t;
		(o->e+i)->y=*(t+1);
		if (i==0||*(o->b)>(o->e+i)->x){
			*(o->b)=(o->e+i)->x;
		}
		if (i==0||*(o->b+1)<(o->e+i)->x){
			*(o->b+1)=(o->e+i)->x;
		}
		if (i==0||*(o->b+2)>(o->e+i)->y){
			*(o->b+2)=(o->e+i)->y;
		}
		if (i==0||*(o->b+3)<(o->e+i)->y){
			*(o->b+3)=(o->e+i)->y;
		}
	}
	fclose(f);
	return o;
}



Kmeans init(size_t g,Dataset dt){
	Kmeans o=malloc(sizeof(struct _KMEANS));
	o->dt=dt;
	o->g=g;
	o->_cp=malloc(g*2*sizeof(float));
	for (size_t i=0;i<g;i++){
		*(o->_cp+i*2)=((float)rand())/RAND_MAX*(*(dt->b+1)-*(dt->b))+*(dt->b);
		*(o->_cp+i*2+1)=((float)rand())/RAND_MAX*(*(dt->b+3)-*(dt->b+2))+*(dt->b+2);
		printf("GROUP: %llu, %f %f\n",i,*(o->_cp+i*2),*(o->_cp+i*2+1));
	}
	return o;
}



void train(size_t i,Kmeans km){
	for (size_t j=0;j<i;j++){
		float* nl=malloc(km->g*2*sizeof(float));
		size_t* nll=malloc(km->g*sizeof(size_t));
		for (size_t k=0;k<km->g;k++){
			*(nl+k*2)=0;
			*(nl+k*2+1)=0;
			*(nll+k)=0;
		}
		for (size_t k=0;k<km->dt->ln;k++){
			float sd=0;
			size_t sdi=0;
			for (size_t l=0;l<km->g;l++){
				float d=((km->dt->e+k)->x-(*(km->_cp+l*2)))*((km->dt->e+k)->x-(*(km->_cp+l*2)))+((km->dt->e+k)->y-(*(km->_cp+l*2+1)))*((km->dt->e+k)->y-(*(km->_cp+l*2+1)));
				if (l==0||d<sd){
					sd=d;
					sdi=l;
				}
			}
			(km->dt->e+k)->_gc=sdi;
			(*(nl+sdi*2))+=(km->dt->e+k)->x;
			(*(nl+sdi*2+1))+=(km->dt->e+k)->y;
			(*(nll+sdi))++;
		}
		for (size_t k=0;k<km->g;k++){
			if (*(nll+k)==0){
				printf("Point (#%llu) has no Nodes. Too many clusters or bad luck.\n",k);
				raise(SIGABRT);
			}
			*(km->_cp+k*2)=*(nl+k*2)/(*(nll+k));
			*(km->_cp+k*2+1)=*(nl+k*2+1)/(*(nll+k));
			if (j==i-1){
				printf("%llu %f %f (%llu)\n",k,*(km->_cp+k*2),*(km->_cp+k*2+1),*(nll+k));
			}
		}
		free(nl);
		free(nll);
	}
}



void elbow(Dataset dt,size_t i,size_t a,size_t b){
	for (size_t j=a;j<=b;j++){
		Kmeans km=init(j,dt);
		train(i,km);
		// free_km(km);
	}
}
