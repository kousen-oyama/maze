#include<iostream>
#include"types.hpp"
int main(){
	Maze maze;
	DigBuilder digbuilder;
	while(1){
		if(digbuilder.checkMove(maze))
			digbuilder.digHold(maze);
		else
			digbuilder.backBuilder();
		maze.disp();
		std::cout<<std::endl;
		if(digbuilder.isFinish())
		  break;
	}
	return 0;
}
