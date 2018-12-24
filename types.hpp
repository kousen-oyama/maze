#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<random>
#include<stack>
#include<array>
#include<string>

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
	Inits* initsFactory(std::string& name);
public:
	Maze(std::string& name){
		this->releaseSize();
		this->inits=this->initsFactory(name);
		this->inits->makeBoard(this->maze);
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

	
	//pribateに以降
	std::stack<Coordinate> builderLog;
	Coordinate builder;
	void moveBuilder(const Direction direction);
	void backBuilder();

	//別クラスに移行せよ
	const std::array<Direction,4> dir={Direction::DOWN,Direction::LEFT,Direction::RIGHT,Direction::UP};
	int random(const int min,const int max) const;
	Direction randomDirection() const;

	
};

class DigBuilder :public Builder{
private:
	std::vector<Direction> possibleDirection;
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
	bool checkMove(Maze& maze);
	void digHold(Maze& maze);
	bool isFinish();
};

class RodDown :public Builder{
private:
	Coordinate builder;
	void randomRodDown(Coordinate coodinate);
public:
	RodDown(){
		this->builder.x=1;
		this->builder.y=1;
	}
	void Make();
};

class makeMazeAlgorithm{
private:
public:
	void digHoldAlgorithm();
	void rodDownAlgorithm();
};

