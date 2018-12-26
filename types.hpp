#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<random>
#include<stack>
#include<array>
#include<string>
#include<queue>

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
	State getState(const Coordinate coordinate) const;
	void setState(const Coordinate coordinate,const State state);
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
	Direction willMoveDirection() const;
	void digHoldDirection(Maze& maze,const Direction direction);
	bool checkDirection(Maze& maze,const Direction direction);
	bool isPossibleCoordinate(Maze& maze,const Coordinate coordinate);
public:
	DigBuilder(){
		this->builder.setBuilder(1, 1);
		this->builder.setBuilderLog();
	}
	bool isFinish();
	bool isEmptyPossibleDirection() const;
	void digHold(Maze& maze);
	void bakeBuilder();
	void makePossibleDirection(Maze& maze);
};

class RodDown{
private:
	PossibleDirectin possibleDirection;
	std::vector<Coordinate> coordinateList;
private:
	void makePossibleDirection(Maze& maze,const Coordinate coordinate);
	void makePossibleDirectionNoUp(Maze& maze,const Coordinate coordinate);
	void rodDownDirection(Maze& maze,Coordinate coordinate,const Direction direction);
	bool isPossibleDirection(Maze& maze,Coordinate coordinate,const Direction direction);
public:
	void makeCoordinateList(Maze& maze);
	void randomRodDown(Maze& maze);
};

class makeMazeAlgorithm{
private:
	DigBuilder digbuilder;
	RodDown roddown;
public:
	void digHoldAlgorithm();
	void rodDownAlgorithm();
};

