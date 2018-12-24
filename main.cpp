#include<iostream>
#include"types.hpp"
void digHoldAlgorithm(){
	Maze maze;
	DigBuilder digbuilder;
	while(1){
		if(digbuilder.checkMove(maze))
			digbuilder.digHold(maze);
		else
			digbuilder.backBuilder();
		std::cout<<std::endl;
		if(digbuilder.isFinish())
		  break;
	}
	maze.disp();
}
int main(){
	digHoldAlgorithm();
	return 0;
}
