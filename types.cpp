#include<iostream>
#include<vector>
#include<algorithm>
#include<cassert>
#include<random>
#include<stack>
#include<array>
#include<cstdio>
#include<string>
#include<queue>
#include"types.hpp"

int Len::get_x_size() const{
	return this->coordinate.x;
}
int Len::get_y_size() const{
	return this->coordinate.y;
}



void Inits::fillBoard(State_vec2& vec2,State state){
	std::for_each(vec2.begin(),vec2.end(),[state](auto& vec){
			std::fill(vec.begin(),vec.end(),state);
		});
}
void Inits::fillBoardInside(State_vec2 &vec2, State state){
	std::for_each(vec2.begin()+1,vec2.end()-1,[state](auto& vec){
		std::fill(vec.begin()+1,vec.end()-1,state);
		});
}



void MakeHoldBoard::makeBoard(State_vec2& vec2){
	this->fillBoard(vec2, State::OUTLINE);
	this->fillBoardInside(vec2, State::WALL);
}

void MakeExtendBoard::makeBoard(State_vec2& vec2){
	this->fillBoard(vec2, State::OUTLINE);
	this->fillBoardInside(vec2, State::ROAD);
}

void MakeRodBoard::makeBoard(State_vec2& vec2){
	this->fillBoard(vec2, State::OUTLINE);
	this->fillBoardInside(vec2, State::ROAD);
	int n=0,m=0;
	std::for_each(vec2.begin()+1,vec2.end()-1,[n,m](auto& vec1) mutable{
			m++;
			std::for_each(vec1.begin()+1,vec1.end()-1,[n,m](auto& state) mutable{
					if((n++)%2&&!(m%2))
							state=State::WALL;
		      });
		});
}



State Maze::getState(const Coordinate coordinate) const{
	return this->maze.at(coordinate.y).at(coordinate.x);
}
void Maze::setState(const Coordinate coordinate, const State state){
	this->maze.at(coordinate.y).at(coordinate.x)=state;
}
void Maze::releaseSize(){
	this->maze.resize(len.get_y_size());
	std::for_each(this->maze.begin(),this->maze.end(),[this](auto& vec){
			vec.resize(len.get_x_size());
		});
}
Inits* Maze::initsFactory(std::string& name){
	if(name=="Hold")  return new MakeHoldBoard();
	if(name=="Extend")  return new MakeExtendBoard();
	if(name=="Rod")  return new MakeRodBoard();
}
void Maze::disp() const{
	std::for_each(this->maze.begin(),this->maze.end(),[this](auto vec){
			this->eachDisp(vec);
		});
}
void Maze::eachDisp(std::vector<State> vec1) const{
	std::for_each(vec1.begin(),vec1.end(),[this](auto state){
			this->stateDisp(state);
			std::cout<<"\x1b[49m";
		});
	std::cout<<std::endl;
}
void Maze::stateDisp(State state) const{
	switch(state){
	case State::ROAD:
		std::cout<<"\x1b[47m";
		std::cout<<"  ";
		break;
	case State::WALL:
	case State::OUTLINE:
		std::cout<<"\x1b[46m";
		std::cout<<"  ";
		break;
	default:
		assert(!"error");  break;
	}
}



int Random::random(const int min,const int max) const{
	std::random_device rnd;
	std::mt19937_64 mt(rnd());
	std::uniform_int_distribution<> rand(min,max);
	const int random=rand(mt);
	assert((0<=random&&random<=3)&&"out of range3");
	return random;
}
Direction Random::randomDirection() const{
	const int rand=random(0, dir.size()-1);
	const Direction direction=dir.at(rand);
	return direction;
}



void Builder::setBuilderLog(){
	this->builderLog.push(this->builder);
}
Coordinate Builder::getBuilderLogTop() const{
	return this->builderLog.top();
}
Coordinate Builder::getBuilder() const{
	return this->builder;
}
void Builder::setBuilder(const int x, const int y){
	this->builder.x=x;
	this->builder.y=y;
}
void Builder::moveBuilder(const Direction direction){
	switch(direction){
	case Direction::DOWN:
		this->builder.y+=1;	 break;
	case Direction::LEFT:
		this->builder.x-=1;  break;
	case Direction::RIGHT:
		this->builder.x+=1;  break;
	case Direction::UP:
		this->builder.y-=1;  break;
	default:
		assert(!"error");  break;
	}
}
void Builder::backBuilder(){
	this->builder.x=this->builderLog.top().x;
	this->builder.y=this->builderLog.top().y;
	this->builderLog.pop();
}



int PossibleDirectin::possibleDirectionSize() const{
	const int size=possibleDirection.size();
	return size;
}
bool PossibleDirectin::isEmptyPossibleDirection() const{
	bool empty=this->possibleDirection.empty();
	return empty;
}
Direction PossibleDirectin::getRandomPossibleDirection() const{
	const int rand=this->random.random(0, this->possibleDirection.size()-1);
	Direction direction=this->possibleDirection.at(rand);
	return direction;
}
void PossibleDirectin::pushPossibleDirection(const Direction direction){
	this->possibleDirection.push_back(direction);
}
void PossibleDirectin::clearPossibleDirection(){
	this->possibleDirection.clear();
}



void DigBuilder::makePossibleDirection(Maze& maze){
	std::for_each(dir.begin(),dir.end(),[this,&maze](auto direction){
			if(this->checkDirection(maze, direction))
				this->possibleDirection.pushPossibleDirection(direction);
		});
}
bool DigBuilder::isEmptyPossibleDirection() const{
	if(this->possibleDirection.isEmptyPossibleDirection())
		return false;
	return true;
}
bool DigBuilder::checkDirection(Maze& maze,const Direction direction){
	Coordinate coordinate;
	coordinate.x=this->builder.getBuilder().x;
	coordinate.y=this->builder.getBuilder().y;
	switch(direction){
	case Direction::DOWN:
		coordinate.y+=2;
		if(this->isPossibleCoordinate(maze,coordinate))  return true;  break;
	case Direction::LEFT:
		coordinate.x-=2;
		if(this->isPossibleCoordinate(maze,coordinate)) 	return true;  break;
	case Direction::RIGHT:
		coordinate.x+=2;
		if(this->isPossibleCoordinate(maze,coordinate))  return true;  break;
	case Direction::UP:
		coordinate.y-=2;
		if(this->isPossibleCoordinate(maze,coordinate))  return true;  break;
	default:
		assert(!"error");  break;
	}
	return false;
}
bool DigBuilder::isPossibleCoordinate(Maze& maze, const Coordinate coordinate){
	Len len;
	if(coordinate.x<0)  return false;
	if(coordinate.y<0)  return false;
	if(coordinate.x>len.get_x_size()-1)  return false;
	if(coordinate.y>len.get_y_size()-1)  return false;
	State state=maze.getState(coordinate);
	if(state==State::WALL)
		return true;
	return false;
}
Direction DigBuilder::willMoveDirection() const{
	const Direction direction=this->possibleDirection.getRandomPossibleDirection();	
	return direction;
}
void DigBuilder::digHold(Maze& maze){
	const Direction direction=this->willMoveDirection();
	switch(direction){
	case Direction::DOWN:
		this->digHoldDirection(maze, Direction::DOWN);  break;
	case Direction::LEFT:
		this->digHoldDirection(maze, Direction::LEFT);  break;
	case Direction::RIGHT:
		this->digHoldDirection(maze, Direction::RIGHT);  break;
	case Direction::UP:
		this->digHoldDirection(maze, Direction::UP);  break;
	default:
		assert(!"erroe");  break;
	}
}
void DigBuilder::digHoldDirection(Maze& maze, const Direction direction){
	int n=0;
	maze.setState(this->builder.getBuilder(), State::ROAD);
	while(n++<2){
		this->builder.moveBuilder(direction);
		maze.setState(this->builder.getBuilder(), State::ROAD);
	}
	this->possibleDirection.clearPossibleDirection();
	this->builder.setBuilderLog();
}
bool DigBuilder::isFinish(){
	const int coordinate_x=this->builder.getBuilderLogTop().x;
	const int coordinate_y=this->builder.getBuilderLogTop().y;
	if(coordinate_x==1&&coordinate_y==1)
		return true;
	return false;
}
void DigBuilder::bakeBuilder(){
	this->builder.backBuilder();
}



void RodDown::makeCoordinateList(Maze& maze){
	Len len;
	Coordinate coordinate;
	for(coordinate.y=0;coordinate.y<len.get_y_size()-1;coordinate.y++){
		for(coordinate.x=0;coordinate.x<len.get_x_size()-1;coordinate.x++){
			if(maze.getState(coordinate)==State::WALL)
				this->coordinateList.push_back(coordinate);
		}
	}
}
void RodDown::makePossibleDirection(Maze &maze,const Coordinate coordinate){
	std::for_each(dir.begin(), dir.end(),[this,&maze,coordinate](auto directoin){
			if(this->isPossibleDirection(maze, coordinate ,directoin))
				this->possibleDirection.pushPossibleDirection(directoin);
		});
}
void RodDown::makePossibleDirectionNoUp(Maze &maze, const Coordinate coordinate){
	std::for_each(dir.begin(), dir.end()-1,[this,&maze,coordinate](auto directoin){
			if(this->isPossibleDirection(maze, coordinate ,directoin))
				this->possibleDirection.pushPossibleDirection(directoin);
		});
}
bool RodDown::isPossibleDirection(Maze &maze, Coordinate coordinate, const Direction direction){
	switch(direction){
	case Direction::DOWN:
		coordinate.y+=1;
		if(maze.getState(coordinate)==State::ROAD)  return true;  break;
	case Direction::LEFT:
		coordinate.x-=1;
		if(maze.getState(coordinate)==State::ROAD)  return true;  break;
	case Direction::RIGHT:
		coordinate.x+=1;
		if(maze.getState(coordinate)==State::ROAD)  return true;  break;
	case Direction::UP:
		coordinate.y-=1;
		if(maze.getState(coordinate)==State::ROAD)  return true;  break;
	default:
		assert(!"error");  break;
	}
	return false;
}
void RodDown::randomRodDown(Maze& maze){
	Direction direction;
	for_each(this->coordinateList.begin(),this->coordinateList.end(),[this,&maze,direction](auto coordinate) mutable{
			if(coordinate.y==2)
				this->makePossibleDirection(maze, coordinate);
			else
				this->makePossibleDirectionNoUp(maze, coordinate);
			direction=this->possibleDirection.getRandomPossibleDirection();
			this->rodDownDirection(maze, coordinate, direction);
			this->possibleDirection.clearPossibleDirection();
		});
}

void RodDown::rodDownDirection(Maze& maze, Coordinate coordinate, const Direction direction){
	switch(direction){
	case Direction::DOWN:
		coordinate.y+=1;
		maze.setState(coordinate, State::WALL);  break;
	case Direction::LEFT:
		coordinate.x-=1;
		maze.setState(coordinate, State::WALL);  break;
	case Direction::RIGHT:
		coordinate.x+=1;
		maze.setState(coordinate, State::WALL);  break;
	case Direction::UP:
		coordinate.y-=1;
		maze.setState(coordinate, State::WALL);  break;
	default:
		assert(!"error");  break;
	}
}


void makeMazeAlgorithm::digHoldAlgorithm(){
	std::string name("Hold");
	Maze maze(name);
	Len len;
	std::cout<<"x_size>"<<len.get_x_size()<<std::endl;
	std::cout<<"y_size>"<<len.get_y_size()<<std::endl;
	while(1){
		this->digbuilder.makePossibleDirection(maze);
		if(this->digbuilder.isEmptyPossibleDirection())
			this->digbuilder.digHold(maze);
		else
			this->digbuilder.bakeBuilder();
		std::cout<<std::endl;
		if(this->digbuilder.isFinish())  break;
	}
	maze.disp();
}
void makeMazeAlgorithm::rodDownAlgorithm(){
	std::string name("Rod");
	Maze maze(name);
	Len len;
	std::cout<<"x_size>"<<len.get_x_size()<<std::endl;
	std::cout<<"y_size>"<<len.get_y_size()<<std::endl;
	this->roddown.makeCoordinateList(maze);
	this->roddown.randomRodDown(maze);
	maze.disp();
}
