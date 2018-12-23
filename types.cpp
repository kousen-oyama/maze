#include<iostream>
#include<vector>
#include<algorithm>
#include<cassert>
#include<random>
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
	assert((this->maze.at(coodinate.x).at(coodinate.y)==State::ROAD||this->maze.at(coodinate.y).at(coodinate.x)==State::WALL)&&"out of range");
	return this->maze.at(coodinate.x).at(coodinate.y);
}
void Maze::setState(const Coordinate coodinate, const State state){
	assert((this->maze.at(coodinate.x).at(coodinate.y)==State::ROAD||this->maze.at(coodinate.y).at(coodinate.x)==State::WALL)&&"out of range");
	this->maze.at(coodinate.x).at(coodinate.y)=state;
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
		std::cout<<"##"; break;
	case State::OUTLINE:
		std::cout<<"**"; break;
	default:
		assert(!"error"); break;
	}
}

Direction DigBuilder::random() const{
	std::random_device rnd;
	std::mt19937_64 mt(rnd());
	std::uniform_int_distribution<> rand4(0,3);
	int random=rand4(mt);
	Direction direction=static_cast<Direction>(random);
	return direction;
}


