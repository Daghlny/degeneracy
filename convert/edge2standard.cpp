

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

// if find v between source1 and (source1+length1)
// return a positive value of v's position
// if not find v, return a negative value 
// whose absolute value is the position v should be inserted.
inline int binarysearch(int *&source1, int length1, int v){
	int low=0;
	int high=length1;
	while(low<high){
		int mid=low+((high-low)/2);
		if(source1[mid]<v)
			low=mid+1;
		else
			high=mid;
	}
	if((low<length1)&&(source1[low]==v))
		return low;
	else
		return -(low+1);
}

// Gu[0] stores the length of Gu[0]
// Gu[1] stores the current degree of @u
inline bool insert(int *&Gu, int v){
	int maxlength=Gu[0];
	int deg=Gu[1];

	if(deg==0){
		Gu[deg+2]=v;
		Gu[1]++;
		return true;
	}else{
		if(deg>=maxlength-2){
            /* expand the Gu's size */
		    Gu=(int *)realloc(Gu,sizeof(int)*(maxlength+100));
		    Gu[0]=maxlength+100;
		}
		int *temp=Gu+2;
		int cur=binarysearch(temp,deg,v);
        //if v is not in G[u], then insert v into G[u] and reorder G[u]
		if(cur<0){
			for(int i=deg+2;i>-cur+1;i--){
				Gu[i]=Gu[i-1];
			}
			Gu[-cur+1]=v;
			Gu[1]++;
			return true;
		}else
			return false;
	}	
}

int main(int argc, char *argv[]){

    if( argc < 3 )
    {
        printf("programName inputfile logfile(for duplicate edges) iterationSteps(default: 10000)\n");
        exit(0);
    }

    /* read graph data file */
	char *filename=(char*)malloc(sizeof(char)*100);
	strcpy(filename,argv[1]);
	FILE *rfile=fopen(filename,"r");
	if(rfile==NULL){
		printf("could not find the graph file\n");
		exit(0);
	}

    /* added By Yinuo */
    /* log the duplicate edges */
    int dupedgenum = 0;
    char *logfilename = (char *)malloc(sizeof(char)*100);
    strcpy(logfilename, argv[2]);
    FILE *logfile = fopen(logfilename, "w");
    if( logfile == NULL ){
        printf("could not open the log file\n");
        exit(0);
    }
    
    const int itsteps = (argc > 3) ? atoi(argv[3]) : 10000;

    /* vertex number */
	int nodenum;
	fscanf(rfile,"%d",&nodenum);
    /* edge number */
	int arcnum=0;

	int u,v;
	int **G=(int **)malloc(sizeof(int *)*nodenum);

	for(int i=0;i<nodenum;i++)
		G[i]=(int*)malloc(sizeof(int)*100);

    /* G[i][0] is the array's size */
    /* G[i][1] is the vertex's current degree */
	for(int i=0;i<nodenum;i++){
		G[i][0]=99;
		G[i][1]=0;
	}

    /* @nnum is the maximum vertex id */
	int i=0;
	int nnum=0;

	while(!feof(rfile)){
		fscanf(rfile,"%d%d",&u,&v);
		if(nnum<u) nnum=u;
		if(nnum<v) nnum=v;
		u = u % nodenum;
		v = v % nodenum;

		if(u==v) continue;

		bool flag1=insert(G[u],v);
		bool flag2=insert(G[v],u);
		if(flag1==true && flag2==true)
			arcnum++;
        else{
            fprintf(logfile, "%d %d\n", u, v); 
            dupedgenum++;
        }

		if(i % itsteps == 0) printf("now it is in edge %d\n",i);
		i++;
	}
    fprintf(logfile, "%d", dupedgenum);
    fclose(logfile);

    /* write new format graph data into file */
	fclose(rfile);
	strcat(filename,"_new.txt");
	FILE *wfile=fopen(filename,"w");
	if(wfile==NULL){
		printf("could not write to files\n");
		exit(0);
	}
	fprintf(wfile,"%d\n",nodenum);
	for(int i=0;i<nodenum;i++){
		fprintf(wfile,"%d,%d",i,G[i][1]);
		for(int j=0;j<G[i][1];j++){
			fprintf(wfile,":%d",G[i][j+2]);
		}
		fprintf(wfile,"\n");
	}
	fclose(wfile);
    printf("@nnum: %d\n", nnum);
    printf("@nodenum: %d\n", nodenum);
	printf("number of edges: %d\n",arcnum);
}

