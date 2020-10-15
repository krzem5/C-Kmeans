typedef stryct _DATASET* Dataset;



struct _DATASET_ELEM{
	float w;
	float h;
};



struct _DATASET{
	size_t ln;
	struct _DATASET_ELEM* e;
};



Dataset read(const char* fp);
