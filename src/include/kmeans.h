typedef struct _DATASET* Dataset;
typedef struct _KMEANS* Kmeans;



struct _DATASET_ELEM{
	float x;
	float y;
	size_t _gc;
};



struct _DATASET{
	size_t ln;
	struct _DATASET_ELEM* e;
	float* b;
};



struct _KMEANS{
	Dataset dt;
	size_t g;
	float* _cp;
};



Dataset read(const char* fp);



Kmeans init(size_t g,Dataset dt);



void train(size_t i,Kmeans km);



void elbow(Dataset dt,size_t i,size_t a,size_t b);
