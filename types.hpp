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
	void fillBoard(State_vec2& vec2,State state);
	void fillBoardInside(State_vec2& vec2,State state);
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
private:
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

	const std::array<Direction,4> dir={Direction::DOWN,Direction::LEFT,Direction::RIGHT,Direction::UP};

class Random{
private:
public:
	int random(const int min,const int max) const;
	Direction randomDirection() const;
	Direction randomDirectionNoUp() const;
};

class Builder{
private:
	Coordinate builder;
	std::stack<Coordinate> builderLog;
public:
  Coordinate getBuilderLogTop() const;
	Coordinate getBuilder() const;
	void setBuilderLog();
	void setBuilder(const int x,const int y);
	void moveBuilder(const Direction direction);
	void backBuilder();
};

class PossibleDirectin{
private:
	std::vector<Direction> possibleDirection;
	Random random;
public:
	int possibleDirectionSize() const;
	bool isEmptyPossibleDirection() const;
	Direction getRandomPossibleDirection() const;
	void pushPossibleDirection(const Direction direction);
	void clearPossibleDirection();
};

class DigBuilder{
private:
	PossibleDirectin possibleDirection;
	Builder builder;
private:
	Direction moveDirection() const;
	void dig(Maze& maze,const Direction direction);
	bool checkState(Maze& maze,const Direction direction);
	bool isPossibleState(Maze& maze,const Coordinate coodinate);
public:
	DigBuilder(){
		this->builder.setBuilder(1, 1);
		this->builder.setBuilderLog();
	}
	void digHold(Maze& maze);
	void bakeBuilder();
	bool isFinish();
	bool checkMove(Maze& maze);
};

class RodDown{
private:
	PossibleDirectin possibleDirection;
	Builder builder;
private:
	void randomRodDown(Coordinate coodinate);
public:
	RodDown(){
		this->builder.setBuilder(1, 1);
	}
};

class makeMazeAlgorithm{
private:
public:
	void digHoldAlgorithm();
	void rodDownAlgorithm();
};

