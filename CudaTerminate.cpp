/*THIS IS JUST A TERMINATION CHECK
	The "purpose of this project" isn't shown here.

*/
#include <iostream>

using namespace std;

class Block{
	
	public:
	
	int noofRows;
	int noofCols;
	int *subGridElements[];
	
	Block() {}
	getElement(int row, int col)
	{
		return subGridElements[row][col];
	}
};

int sameGeneration(Block& newGeneration, Block& prevGeneration){
	
	for(int i=0; i < prevGeneration.noofRows ; i++){
		
    	for(int j=0; j < prevGeneration.noofCols ; j++){
			if(newGeneration.getElement(i,j) != prevGeneration.getElement(i,j)){
				return 0;
			}
    	}
	}
	return 1;
}

int areAllDead(Block& Block){

	for(int i=0; i < Block.noofRows ; i++){

    	for(int j=0; j < Block.noofCols ; j++){   
			if(Block.getElement(i,j)){ //einai zwntano
				return 0;      		
      		}
		}		
	}

	return 1;
}

int TerminateCondition(Block& Block, Block& newBlock)
{
	if(areAllDead(newBlock) || sameGeneration(Block,newBlock)){
		return 1;
	}
	return 0;
}

__global__ void Terminate(int noofThreads, Block& Block,Block& newBlock)
{
	__shared__ int terminate;
	int t = threadIdx.x;
  
	if(TerminateCondition(Block, newBlock))
	{
		terminate++;
	}
	cout << "Thread: " << threadIdx.x << "terminate: " << terminate << endl;
  	__syncthreads();
  	if(terminate == noofThreads)
	{
		cout << "THIS SHOULD TERMINATE NOW" << endl;
		return 1;
	}	  
	return 0;
}


int main(void)
{
	Block B;
	Block newBlock;
	
	//MAIN CODE TO BE HERE
	//CONCAT OPENMP_MAIN_FILE
	
	while(1){

		if((int terminate = Terminate<<<blocks_count, t>>>(t, B , newBlock)))
		{
			return 0;
		}
		// UpdateProcessBlock();
	}
	

}
