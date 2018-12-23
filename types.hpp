#pragma once
#include<iostream>
#include<vector>
#include<algorithm>

enum class State{
  ROAD,
	WALL,
	OUTLINE
};

struct Coordinate{
	int x;
	int y;
};

class Len{
private:
	Coordinate coordinate;
public:
	Len(){
		coordinate.x=31;
		coordinate.y=31;
	}
	~Len(){}
	int get_x_size() const;
	int get_y_size() const;
};

using State_vec2=std::vector<std::vector<State>>;
using State_vec1=std::vector<State>;

class Inits{
private:
public:
	virtual ~Inits(){}
	virtual void makeBoard(State_vec2& vec2)=0;
};

class MakeHoldBoard :public Inits{
public:
	virtual void makeBoard(State_vec2& vec2) override;
};

class MakeExtendBoard :public Inits{
public:
	virtual void makeBoard(State_vec2& vec2) override;
};

class MakeRodBoard :public Inits{
public:
	virtual void makeBoard(State_vec2& vec2) override;
};

class Maze{
private:
	Inits* inits;
	State_vec2 maze;
	const Len len;
public:
	Maze(){
		this->releaseSize();
		//this->inits=new MakeHoldBoard();
		//this->inits=new MakeExtendBoard();
		this->inits=new MakeRodBoard();
	}
	~Maze(){
		delete inits;
	}
	State getState(const int x,const int y) const;
	void setState(const int x,const int y,const State state);
	void releaseSize();
	void init();
	void disp() const;
	void eachDisp(State_vec1 vec1) const;
	void stateDisp(State state) const;
};



