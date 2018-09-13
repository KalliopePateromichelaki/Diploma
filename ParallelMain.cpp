 /*
Fasi0: Diavasma orismatwn apo grammi entolwn,apothikeysi stin klasi Arguments (den exei getters kai setters gia eukolia - an thes vazw)
		Kai analoga me to orisma -f or -r dimioyrgia enos tuxaiou grid (gia to -r) or evresi stilwn kai grammwn tou arxeioy pou epetetai to -f
	O kwdikas tis fasis aytis tha ekteleitai apo tin master process (pid = 0) kai apo kamia alli

	Cmd Execution: ./exe -rows <nooRows> -cols <noofCols> {-f File, -r} --> an dothei i epilogi -f tha anairethei kathe orisma sto -rows, -cols

	Xreiazetai isws kalytero Error Handling  kai an thes polu getters kai setters (den ta theorw anagkaia)

	H fasi 1 epekteinei ton kwdika me idi yparxouses sunartisis: EstimateBlockDimensions(size); EstimateBlocksPerRowandCol(*bld);
	Pou tha elegxthoun gia tin egkirotita tous.
	Kai i eisagwgi cuda an thes kai esu sto kommati tis metadosis aytwn twn arxikwn pliroforiwn stis upoloipes diergasies

*/
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <fstream>

using namespace std;

class Arguments{

public:
	int noofColumns;
	int noofRows;
	char fileOrRandom;
	string filename;
	Arguments(){}
	//Arguments(int c, int r, string ForR): noofColumns (c), noofRows (r) {this->FileOrRandom = ForR;}

	void setMembers(int c, int r, char ForR, string FileName){
		this->noofColumns = c;
		this->noofRows = r;
		this->fileOrRandom = ForR;
		this->filename = FileName;
	}
	void updateMembers(int noofcols, int noofrows)
	{
		this->noofColumns = noofcols;
		this->noofRows = noofrows;
	}
};

void IdentifyArgs(Arguments &Args, int argc, char*argv[])
{
	int noofRows, noofCols;
	char fileOrRandom;
	string fileName;

	for(int i = 1 ; i < argc; i++)
	{
		if(!strcmp(argv[i], "-rows"))
		{
			noofRows= atoi(argv[i+1]);
		}
		else if(!strcmp(argv[i], "-cols"))
		{
			noofCols = atoi(argv[i+1]);
		}
		else if(!strcmp(argv[i],"-f")){
			fileOrRandom = 'f';
			fileName = argv[i+1];
		}
		else if(!strcmp(argv[i],"-r")){
			fileOrRandom = 'r';
			fileName = "RandomArr";
		}
	}

	Args.setMembers(noofCols, noofRows, fileOrRandom, fileName);
}

void findDimensions(Arguments & Args){
    int noofrows= -1;
    ifstream myfile;
    string STRING;
    myfile.open(Args.filename.c_str());
  /*  if(myfile == NULL){ //Kalutero Error Handling
        cerr << "Couldn't open the file" <<endl;
     //   MPI_Finalize();
    	exit(-2);
	}*/
    int noofcols;
    while(!myfile.eof()) {
        noofrows++;
        noofcols = STRING.length();
        getline(myfile,STRING);

    }

    Args.updateMembers(noofcols, noofrows);
	myfile.close();
}

void CreateRandomGrid(int noofColumns,int  noofRows){

    FILE * pFile;
    pFile = fopen ("./RandomArr","wb");
    /*if(pFile == NULL){  //Kalutero Error Handling
        cerr << "Couldn't open the file" <<endl;
        //MPI_Finalize();
        exit(-2);
    }*/
	int num;
	char pchar[2];
	string numberstring;
	for(int i =0 ; i < noofRows ; i++){
		for(int j=0 ; j < noofColumns ; j++){
    		num = rand()%2;
			sprintf(pchar, "%d", num);
    		fwrite(pchar, sizeof(char),1,pFile);
		}
		fwrite("\n", sizeof(char),1,pFile);
	}
    fclose(pFile);
}


typedef struct block_dimensions{
	int noofrows;
	int noofcols;
}block_dimensions;

void EstimateBlockDimensions(int noofProcesses, int noofRows, int noofCols, block_dimensions& bld){
  //  block_dimensions bld;

		int totalCells = noofCols*noofRows;
  //  int  rows = N;
  //  int  columns = M;
	//int cellsPerProcess = totalCells/noofProcesses;
    //cout << "MPAINW ME M ROWS:: " << M << " KAI N COLUMNS:: "<< N << endl;
    int block_rows ;
    int block_col ;

    int blocksPerRow = 0;
    int blocksPerCol = 0;
    int cellsperblockRow = 0;
    int cellsperblockCol = 0;

    for (int i = (noofRows / 2); i >= 1; i--){
        if (noofRows % i == 0) //if i fits N then you might have i blocksPerRow
        {
            blocksPerRow = noofRows / i;
            if ((noofProcesses ) % blocksPerRow ==0)
            {
                blocksPerCol = (noofProcesses) / blocksPerRow; //Then your blocksPerCol would be the remaining and if they fit too you have a deal :p
                if (noofCols % blocksPerCol == 0)
                {
                    cellsperblockRow = noofRows / blocksPerRow;
                    cellsperblockCol = noofCols / blocksPerCol;
                    //cout << "Vrhka:: " << blocksPerRow << " sth grammh kai :: " << blocksPerCol << " sthn sthlh!" <<endl;
                    bld.noofrows = cellsperblockRow;
                    bld.noofcols = cellsperblockCol;
                  //  return bld;
                }
            }
            blocksPerRow = 0;
            blocksPerCol = 0;
        }
    }

    //cout << "BLOCK_R:: " << block_rows << "BLOCK-C:: " << block_col <<endl;
	//return bld;
}


/*
Sunartisi i opoia epistrefei posa block prepei na uparxoun se kathe stili kai kathe grammi
*
BlockPerCoord  EstimateBlocksPerRowandCol(block_dimensions  bld){
    BlockPerCoord bpc;
    int i =0 , count =0;
    while(i < noofRows) {
        i = i+ bld.noofcols;
        count++;
	}
    bpc.blocksPerRow = count;
    i=0;
    count=0;
    //cout << "M: N: " << M << N <<endl;
	while(i < N){
		i = i+ bld.noofrows;
		count++;
	}
	bpc.blocksPerCol = count;
	// cout << "blocksPerCol: " << bpc.blocksPerCol << "blocksPerRow " << bpc.blocksPerRow << endl;

	return bpc;
}
*/

int main(int argc, char *argv[]){

	Arguments Args;
	IdentifyArgs(Args, argc, argv);
	if(Args.fileOrRandom == 'r'){
		CreateRandomGrid(Args.noofColumns, Args.noofRows); // na tsekarw orismata
	}
	else if(Args.fileOrRandom == 'f'){
		findDimensions(Args);
	}
	block_dimensions bld;
	EstimateBlockDimensions(4, Args.noofRows, Args.noofColumns, bld);
	cout << "Stiles: " << bld.noofcols<< " Grammes: " << bld.noofrows << endl;
/*    *bpc = EstimateBlocksPerRowandCol(*bld); //epistrefei posa blocks per row and col

        /*Enarksi apostolis vasikwn stoixeiwn*
        details[0] = N;
        details[1] = M;
        details[2] = bld->noofcols;
        details[3] = bld->noofrows;
        details[4] = bpc->blocksPerCol;
        details[5] = bpc->blocksPerRow;

        /*Apostoli se kathe alli diergasia*
        for (int i=1; i<size; i++){
            MPI_Send(&details[0], 6, MPI_INT, i, 0 , MPI_COMM_WORLD);
            MPI_Send(filename.c_str(), filename.size(), MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }
	*/
	//	cout << Args.noofRows << Args.noofColumns << Args.fileOrRandom << Args.filename;
	return 0;
}

