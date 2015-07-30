﻿#pragma once

#include "RenderManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>
#include <map>
#include <boost/thread.hpp>
#include "Vertex.h"

using namespace std;

namespace parametriclsystem {

const int NUM_LAYERS = 2;
const int GRID_SIZE = 100;

const int MAX_ITERATIONS = 500;
const int MAX_ITERATIONS_FOR_MC = 10;
const int NUM_MONTE_CARLO_SAMPLING = 100;
const double PARAM_EXPLORATION = 0.3;
const double PARAM_EXPLORATION_VARIANCE = 0.1;
const double LENGTH_ATTENUATION = 0.9;
const double INITIAL_SIZE = 7.0;//6.0
const double SIZE_ATTENUATION = 0.02;
const double MASK_RADIUS_RATIO = 0.11;//0.11;

class String;

class Literal {
public:
	static enum { TYPE_TERMINAL = 0, TYPE_NONTERMINAL};

public:
	string name;
	int depth;
	std::vector<double> param_values;
	bool param_defined;

public:
	Literal() {}
	Literal(const string& name, int depth, bool param_defined = false);
	Literal(const string& name, int depth, double param_value);
	Literal(const string& name, int depth, double param_value1, double param_value2);
	Literal(const string& name, int depth, double param_value1, double param_value2, double param_value3);
	Literal(const string& name, int depth, double param_value1, double param_value2, double param_value3, double param_value4);
	Literal(const string& name, int depth, const std::vector<double>& param_values);
	
	String operator+(const Literal& l) const;
	int type();
};

class String {
public:
	vector<Literal> str;
	int cursor;				// expandするリテラルの位置

public:
	String();
	String(const string& str, int depth);
	String(const Literal& l);

	int length() const { return str.size(); }
	Literal operator[](int index) const { return str[index]; }
	Literal& operator[](int index) { return str[index]; }
	void operator+=(const Literal& l);
	void operator+=(const String& str);
	String operator+(const String& str) const;
	void setValue(double value);
	void replace(const String& str);

	String getExpand() const;
	void nextCursor(int depth);
};

ostream& operator<<(ostream& os, const String& dt);

/**
 * 1ステップのderiveを表す。
 */
class Action {
public:
	static enum { ACTION_RULE = 0, ACTION_VALUE };

public:
	int type;		// 0 -- rule / 1 -- value
	int index;		// モデルの何文字目の変数に対するactionか？
	int action_index;	// actionsの中の何番目のactionか？
	String rule;
	double value;

public:
	Action() {}
	Action(int action_index, int index, const String& rule);
	Action(int action_index, int index, double value);

	String apply(const String& model);
};

ostream& operator<<(ostream& os, const Action& a);

/**
 * MCTS用のノードクラス
 */
class Node {
public:
	double best_score;	// ベストスコア
	std::vector<double> scores;
	Node* parent;
	std::vector<Node*> children;
	std::vector<Action> actions;
	std::vector<int> untriedActions;
	Action action;
	int visits;			// このノードの訪問回数
	parametriclsystem::String model;
	bool fixed;

public:
	Node(const String& model);
	Node(const String& model, const Action& action);

	Node* addChild(const String& model, const Action& action);
	void setActions(std::vector<Action>& actions);
	Action randomlySelectAction();
	void removeAction(int index);
	Node* getChild(int index);
	Node* UCTSelectChild();
	Node* bestChild();
};

class ParametricLSystem {
public:
	int grid_size;

	String axiom;
	map<char, vector<string> > rules;

	boost::mutex mtx_tree;

public:
	ParametricLSystem(const String& axiom);
	String derive(int random_seed);
	String derive(const String& start_model, int max_iterations);
	void draw(const String& model, RenderManager* renderManager);
	void computeIndicator(const String& model, const glm::mat4& mvpMat, std::vector<cv::Mat>& indicator);
	void computeIndicator(const String& model, const glm::mat4& mvpMat, const glm::mat4& baseModelMat, std::vector<cv::Mat>& indicator);
	String inverse(const std::vector<cv::Mat>& target, const glm::mat4& mvpMat);
	String inverse_one_step(const String& root_model, const std::vector<cv::Mat>& target, const glm::mat4& mvpMat, int derivation_step);
	void UCT(Node* node, int max_iterations, const std::vector<cv::Mat>& target, const cv::Mat& mask, const glm::mat4& mvpMat, const glm::mat4& baseModelMat, const std::vector<cv::Mat>& baseIndicator);
	double score(const std::vector<cv::Mat>& indicator, const std::vector<cv::Mat>& target);
	double score(const std::vector<cv::Mat>& indicator, const std::vector<cv::Mat>& target, const cv::Mat& mask);

private:
	std::vector<Action> getActions(const String& model);
	glm::vec2 computeCurrentPoint(const String& model, const glm::mat4& mvpMat, glm::mat4& modelMmat);
	void releaseNodeMemory(Node* node);
};

float deg2rad(float deg);

}