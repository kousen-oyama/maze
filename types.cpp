#include<iostream>
#include<vector>
#include<algorithm>
#include<cassert>
#include<random>
#include<stack>
#include<array>
#include<cstdio>
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
void Maze::init(){
	inits->makeBoard(this->maze);
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

int Builder::random(const int min,const int max) const{
	std::random_device rnd;
	std::mt19937_64 mt(rnd());
	std::uniform_int_distribution<> rand(min,max);
	const int random=rand(mt);
	assert((0<=random&&random<=3)&&"out of range3");
	return random;
}


void DigBuilder::moveBuilder(const Direction direction){
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
void DigBuilder::backBuilder(){
	this->builder.x=this->builderLog.top().x;
	this->builder.y=this->builderLog.top().y;
	this->builderLog.pop();
}
bool DigBuilder::checkMove(Maze& maze){
	std::array<Direction,4> direction={Direction::DOWN,Direction::LEFT,Direction::RIGHT,Direction::UP};
	std::for_each(direction.begin(),direction.end(),[this,&maze](auto i){
			if(this->checkState(maze, i))
				this->possibleDirection.push_back(i);
		});
	if(this->possibleDirection.empty())
		return false;
	return true;
}
bool DigBuilder::checkState(Maze& maze,const Direction direction){
	Coordinate state;
	state.x=this->builder.x;
	state.y=this->builder.y;
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
	int rand=this->random(0, this->possibleDirection.size()-1);
	return this->possibleDirection.at(rand);
}
void DigBuilder::digHold(Maze& maze){
	Direction direction=this->moveDirection();
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
	maze.setState(this->builder, State::ROAD);
	while(n++<2){
		this->moveBuilder(direction);
		maze.setState(this->builder, State::ROAD);
	}
	this->possibleDirection.clear();
	this->builderLog.push(this->builder);
}
bool DigBuilder::isFinish(){
	if(this->builderLog.top().x==1&&this->builderLog.top().y==1)
		return true;
	return false;
}

void makeMazeAlgorithm::digHoldAlgorithm(){
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
