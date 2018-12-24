#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<random>
#include<stack>

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
		coordinate.x=67;
		coordinate.y=39;
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
	void releaseSize();
	void init();
public:
	Maze(){
		this->releaseSize();
		this->inits=new MakeHoldBoard();
		//this->inits=new MakeExtendBoard();
		//this->inits=new MakeRodBoard();
		this->init();
	}
	~Maze(){
		delete inits;
	}
	State getState(const Coordinate coodinate) const;
	void setState(const Coordinate coodinate,const State state);
	void disp() const;
	void eachDisp(State_vec1 vec1) const;
	void stateDisp(State state) const;
};

enum class Direction{
	UP,DOWN,RIGHT,LEFT
};

class Builder{
private:
public:
	int random(const int min,const int max) const;
};

class DigBuilder :public Builder{
private:
	std::stack<Coordinate> builderLog;
	std::vector<Direction> possibleDirection;
	Coordinate builder;
	void moveBuilder(const Direction direction);
	bool checkState(Maze& maze,const Direction direction);
	bool isOutOfRange(Maze& maze,const Coordinate coodinate);
	Direction moveDirection() const;
	void dig(Maze& maze,const Direction direction);
public:
	DigBuilder(){
		this->builder.x=1;
		this->builder.y=1;
		this->builderLog.push(this->builder);
	}
	void inits();
	void backBuilder();
	bool checkMove(Maze& maze);
	void digHold(Maze& maze);
	bool isFinish();
};
