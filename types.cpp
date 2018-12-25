#include<iostream>
#include<vector>
#include<algorithm>
#include<cassert>
#include<random>
#include<stack>
#include<array>
#include<cstdio>
#include<string>
#include"types.hpp"

int Len::get_x_size() const{
	return this->coordinate.x;
}
int Len::get_y_size() const{
	return this->coordinate.y;
}

void MakeHoldBoard::makeBoard(State_vec2& vec2){
	std::for_each(vec2.begin(),vec2.end(),[](auto& vec){
			std::fill(vec.begin(),vec.end(),State::OUTLINE);
		});
	std::for_each(vec2.begin()+1,vec2.end()-1,[](auto& vec){
			std::fill(vec.begin()+1,vec.end()-1,State::WALL);
		});
}

void MakeExtendBoard::makeBoard(State_vec2& vec2){
	std::for_each(vec2.begin(),vec2.end(),[](auto& vec){
			std::fill(vec.begin(),vec.end(),State::OUTLINE);
		});
	std::for_each(vec2.begin()+1,vec2.end()-1,[](auto& vec){
			std::fill(vec.begin()+1,vec.end()-1,State::ROAD);
		});
}

void MakeRodBoard::makeBoard(State_vec2& vec2){
	std::for_each(vec2.begin(),vec2.end(),[](auto& vec){
			std::fill(vec.begin(),vec.end(),State::OUTLINE);
		});
	std::for_each(vec2.begin()+1,vec2.end()-1,[](auto& vec){
			std::fill(vec.begin()+1,vec.end()-1,State::ROAD);
		});
	int n=0,m=0;
	std::for_each(vec2.begin()+1,vec2.end()-1,[n,m](auto& vec1) mutable{
			m++;
			std::for_each(vec1.begin()+1,vec1.end()-1,[n,m](auto& state) mutable{
					if((n++)%2&&!(m%2))
							state=State::WALL;
		      });
		});
}



State Maze::getState(const Coordinate coodinate) const{
	assert((this->maze.at(coodinate.y).at(coodinate.x)==State::ROAD||this->maze.at(coodinate.y).at(coodinate.x)==State::WALL)&&"out of range1");
	return this->maze.at(coodinate.y).at(coodinate.x);
}
void Maze::setState(const Coordinate coodinate, const State state){
	assert((this->maze.at(coodinate.y).at(coodinate.x)==State::ROAD||this->maze.at(coodinate.y).at(coodinate.x)==State::WALL)&&"out of range2");
	this->maze.at(coodinate.y).at(coodinate.x)=state;
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
		});
	std::cout<<std::endl;
}
void Maze::stateDisp(State state) const{
	switch(state){
	case State::ROAD:
		std::cout<<"  "; break;
	case State::WALL:
	case State::OUTLINE:
		std::cout<<"##"; break;
	default:
		assert(!"error"); break;
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
	return dir.at(rand);
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
		this->builder.y-=1;
		break;
	case Direction::LEFT:
		this->builder.x-=1;
		break;
	case Direction::RIGHT:
		this->builder.x+=1;
		break;
	case Direction::UP:
		this->builder.y+=1;
		break;
	default:
		assert(!"error");
		break;
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
	Random random;
	const int rand=random.random(0, this->possibleDirection.size()-1);
	Direction direction=this->possibleDirection.at(rand);
	return direction;
}
void PossibleDirectin::pushPossibleDirection(const Direction direction){
	this->possibleDirection.push_back(direction);
}
void PossibleDirectin::clearPossibleDirection(){
	this->possibleDirection.clear();
}



bool DigBuilder::checkMove(Maze& maze){
	std::for_each(dir.begin(),dir.end(),[this,&maze](auto i){
			if(this->checkState(maze, i))
				this->possibleDirection.pushPossibleDirection(i);
		});
	if(this->possibleDirection.isEmptyPossibleDirection())
		return false;
	return true;
}
bool DigBuilder::checkState(Maze& maze,const Direction direction){
	Coordinate state;
	/*state.x=this->getBuilder().x;
		state.y=this->getBuilder().y;*/
	state.x=this->builder.getBuilder().x;
	state.y=this->builder.getBuilder().y;


		
	switch(direction){
	case Direction::DOWN:
		state.y-=2;
		if(this->isOutOfRange(maze,state))  return true;
		break;
	case Direction::LEFT:
		state.x-=2;
		if(this->isOutOfRange(maze,state)) 	return true;
	  break;
	case Direction::RIGHT:
		state.x+=2;
		if(this->isOutOfRange(maze,state))  return true;
		break;
	case Direction::UP:
		state.y+=2;
		if(this->isOutOfRange(maze,state))  return true;
		break;
	default:
		assert(!"error");
		break;
	}
	return false;
}
bool DigBuilder::isOutOfRange(Maze& maze, const Coordinate coodinate){
	Len len;
	if(coodinate.x<0)  return false;
	if(coodinate.y<0)  return false;
	if(coodinate.x>len.get_x_size()-1)  return false;
	if(coodinate.y>len.get_y_size()-1)  return false;
	State state=maze.getState(coodinate);
	if(state==State::WALL)
		return true;
	return false;
}
Direction DigBuilder::moveDirection() const{
	const Direction direction=this->possibleDirection.getRandomPossibleDirection();	
	return direction;
}
void DigBuilder::digHold(Maze& maze){
	const Direction direction=this->moveDirection();
	switch(direction){
	case Direction::DOWN:
		this->dig(maze, Direction::DOWN);
		break;
	case Direction::LEFT:
		this->dig(maze, Direction::LEFT);
		break;
	case Direction::RIGHT:
		this->dig(maze, Direction::RIGHT);
		break;
	case Direction::UP:
		this->dig(maze, Direction::UP);
		break;
	default:
		assert(!"erroe");
		break;
	}
}
void DigBuilder::dig(Maze& maze, const Direction direction){
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
	const int x=this->builder.getBuilderLogTop().x;
	const int y=this->builder.getBuilderLogTop().y;
	if(x==1&&y==1)
		return true;
	return false;
}



void makeMazeAlgorithm::digHoldAlgorithm(){
	std::string name("Hold");
	Maze maze(name);
	DigBuilder digbuilder;
	while(1){
		if(digbuilder.checkMove(maze))
			digbuilder.digHold(maze);
		else
			//			digbuilder.backBuilder();
			digbuilder.builder.backBuilder();
		std::cout<<std::endl;
		if(digbuilder.isFinish())
		  break;
	}
	maze.disp();
}
void makeMazeAlgorithm::rodDownAlgorithm(){
	std::string name("Rod");
	Maze maze(name);
	DigBuilder digbuilder;
	
}
