#include "ParametricLSystem.h"
#include <iostream>
#include <time.h>
#include <algorithm>
#include "MLUtils.h"
#include "GLUtils.h"
#include <list>

#define SQR(x)	((x) * (x))


#define MAX_ITERATIONS						20//400//1400//200
#define MAX_ITERATIONS_FOR_MC				15
#define NUM_MONTE_CARLO_SAMPLING			300

#define PARAM_EXPLORATION					0.3 //1
#define PARAM_EXPLORATION_VARIANCE			0.1//10

#define LENGTH_ATTENUATION					0.9
#define INITIAL_SIZE						6.0
#define SIZE_ATTENUATION					0.02//0.03 //0.04
#define MASK_RADIUS							100//40//34

//#define DEBUG		1

namespace parametriclsystem {

const double M_PI = 3.141592653592;

Literal::Literal(const string& name, int depth, bool param_defined) {
	this->name = name;
	this->depth = depth;
	this->param_defined = param_defined;
}

Literal::Literal(const string& name, int depth, double param_value) {
	this->name = name;
	this->depth = depth;
	this->param_values.push_back(param_value);
	this->param_defined = true;
}

Literal::Literal(const string& name, int depth, double param_value1, double param_value2) {
	this->name = name;
	this->depth = depth;
	this->param_values.push_back(param_value1);
	this->param_values.push_back(param_value2);
	this->param_defined = true;
}

Literal::Literal(const string& name, int depth, double param_value1, double param_value2, double param_value3) {
	this->name = name;
	this->depth = depth;
	this->param_values.push_back(param_value1);
	this->param_values.push_back(param_value2);
	this->param_values.push_back(param_value3);
	this->param_defined = true;
}

Literal::Literal(const string& name, int depth, double param_value1, double param_value2, double param_value3, double param_value4) {
	this->name = name;
	this->depth = depth;
	this->param_values.push_back(param_value1);
	this->param_values.push_back(param_value2);
	this->param_values.push_back(param_value3);
	this->param_values.push_back(param_value4);
	this->param_defined = true;
}

Literal::Literal(const string& name, int depth, double param_value1, double param_value2, double param_value3, double param_value4, double param_value5) {
	this->name = name;
	this->depth = depth;
	this->param_values.push_back(param_value1);
	this->param_values.push_back(param_value2);
	this->param_values.push_back(param_value3);
	this->param_values.push_back(param_value4);
	this->param_values.push_back(param_value5);
	this->param_defined = true;
}

Literal::Literal(const string& name, int depth, const std::vector<double>& param_values) {
	this->name = name;
	this->depth = depth;
	this->param_values = param_values;
	this->param_defined = true;
}

String Literal::operator+(const Literal& l) const {
	String ret = *this;
	return ret + l;
}

int Literal::type() {
	if (name == "Door" || name == "Window" || name == "Wall") {
		return TYPE_TERMINAL;
	} else {
		return TYPE_NONTERMINAL;
	}
}

String::String() {
	this->cursor = -1;
}

String::String(const string& str, int depth) {
	this->str.push_back(Literal(str, depth));
	this->cursor = 0;
}

String::String(const Literal& l) {
	this->str.push_back(l);
	this->cursor = 0;
}

void String::operator+=(const Literal& l) {
	str.push_back(l);

	if (cursor < 0) cursor = 0;
}

void String::operator+=(const String& str) {
	for (int i = 0; i < str.length(); ++i) {
		this->str.push_back(str[i]);
	}

	if (cursor < 0) cursor = 0;
}

String String::operator+(const String& str) const {
	String new_str = *this;

	for (int i = 0; i < str.length(); ++i) {
		new_str.str.push_back(str[i]);
	}

	if (new_str.cursor < 0 && new_str.length() > 0) new_str.cursor = 0;

	return new_str;
}

void String::setValue(double value) {
	str[cursor].param_values.push_back(value);
	str[cursor].param_defined = true;

	cursor++;

	// 次のリテラルを探す
	nextCursor(str[cursor].depth);
}

void String::replace(const String& str) {
	int depth = this->str[cursor].depth;

	this->str.erase(this->str.begin() + cursor);
	this->str.insert(this->str.begin() + cursor, str.str.begin(), str.str.end());

	// 次のリテラルを探す
	nextCursor(depth);
}

String String::getExpand() const {
	String ret;

	int nest = 0;
	for (int i = cursor; i < str.size(); ++i) {
		if (str[i].name == "[") {
			nest++;
		} else if (str[i].name == "]") {
			nest--;
		}

		if (nest < 0) break;

		ret += str[i];
	}

	return ret;
}

void String::nextCursor(int depth) {
	for (int i = cursor; i < str.size(); ++i) {
		if (str[i].depth != depth) continue;

		if (str[i].type() == Literal::TYPE_NONTERMINAL) {
			cursor = i;
			return;
		} else if (str[i].type() == Literal::TYPE_TERMINAL && !str[i].param_defined) {
			cursor = i;
			return;
		}
	}

	// 同じdepthでリテラルが見つからない場合は、depth+1にする
	depth++;

	for (int i = 0; i < str.size(); ++i) {
		if (str[i].depth != depth) continue;

		if (str[i].type() == Literal::TYPE_NONTERMINAL) {
			cursor = i;
			return;
		} else if (str[i].type() == Literal::TYPE_TERMINAL && !str[i].param_defined) {
			cursor = i;
			return;
		}
	}

	// リテラルが見つからない場合は、-1にする
	cursor = -1;
}

ostream& operator<<(ostream& os, const String& str) {
	os << fixed << setprecision(0);
	for (int i = 0; i < str.length(); ++i) {
		os << str[i].name;
		if (str[i].param_defined) {
			os << "(";
			for (int j = 0; j < str[i].param_values.size(); ++j) {
				if (j > 0) os << ",";
				os << str[i].param_values[j];
			}
			os << ")";
		}
	}

    return os;
}

Action::Action(int action_index, int index, const String& rule) {
	this->type = ACTION_RULE;
	this->action_index = action_index;
	this->index = index;
	this->rule = rule;
}

Action::Action(int action_index, int index, double value) {
	this->type = ACTION_VALUE;
	this->action_index = action_index;
	this->index = index;
	this->value = value;
}

/**
 * 指定されたモデルに、このアクションを適用する。
 *
 * @param model					モデル
 * @return						action適用した後のモデル
 */
String Action::apply(const String& model) {
	String new_model = model;

	if (type == ACTION_RULE) {
		new_model.replace(rule);
	} else {
		new_model.setValue(value);
	}

	return new_model;
}

ostream& operator<<(ostream& os, const Action& a) {
	os << "type(";
	if (a.type == Action::ACTION_RULE) {
		os << "Rule" << "): " << a.rule;
	} else {
		os << "Value" << "): " << a.value;
	}

	return os;
}

Node::Node(const String& model) {
	this->parent = NULL;
	this->model = model;
	best_score = -std::numeric_limits<double>::max();
	visits = 0;
	fixed = false;
}

Node::Node(const String& model, const Action& action) {
	this->parent = NULL;
	this->model = model;
	this->action = action;
	best_score = -std::numeric_limits<double>::max();
	visits = 0;
	fixed = false;
}

/**
 * ノードに、指定されたmodelの子ノードを追加する。
 */
Node* Node::addChild(const String& model, const Action& action) {
	Node* child = new Node(model, action);
	children.push_back(child);
	child->parent = this;
	return child;
}

/**
 * ノードに、actionを登録する。
 * また、untriedActionsに、登録したactionのindex番号のリストをセットする
 *
 * @param actions	actionのリスト
 */
void Node::setActions(std::vector<Action>& actions) {
	this->actions = actions;
	for (int i = 0; i < actions.size(); ++i) {
		untriedActions.push_back(i);
	}
}

/**
 * untriedActionsから1つをランダムに選択して返却する。
 * 選択されたactionは、リストから削除される。
 *
 * @return		選択されたaction
 */
Action Node::randomlySelectAction() {
	int index = ml::genRand(0, untriedActions.size());
	Action action = actions[untriedActions[index]];
	untriedActions.erase(untriedActions.begin() + index);
	return action;
}

/**
 * untriedActionsの中で、指定されたindex値を持つ要素を削除する。
 */
void Node::removeAction(int index) {
	for (int i = 0; i < untriedActions.size(); ++i) {
		if (untriedActions[i] == index) {
			untriedActions.erase(untriedActions.begin() + i);
			return;
		}
	}
}

/**
 * childrenの中で、指定されたindex値を持つ子ノードを返却する。
 */
Node* Node::getChild(int index) {
	for (int i = 0; i < children.size(); ++i) {
		if (children[i]->action.action_index == index) return children[i];
	}
	return NULL;
}

/**
 * UCTアルゴリズムに従い、子ノードを1つ選択する。
 */
Node* Node::UCTSelectChild() {
	double max_uct = -std::numeric_limits<double>::max();
	Node* best_child = NULL;

	for (int i = 0; i < children.size(); ++i) {
		// スコアが確定済みの子ノードは探索対象外とする
		if (children[i]->fixed) continue;

		double uct;
		if (children[i]->visits == 0) {
			uct = 10000 + ml::genRand(0, 1000);
		} else {
			uct = children[i]->best_score
				+ PARAM_EXPLORATION * sqrt(2 * log((double)visits) / (double)children[i]->visits)
				+ PARAM_EXPLORATION_VARIANCE * sqrt(ml::variance(children[i]->scores) + 0.0 / (double)children[i]->visits);
		}

		if (uct > max_uct) {
			max_uct = uct;
			best_child = children[i];
		}
	}

	return best_child;
}

/**
 * ベストスコアの子ノードを返却する。
 *
 * @return		ベストスコアの子ノード
 */
Node* Node::bestChild() {
	double best_score = -std::numeric_limits<double>::max();
	Node* best_child = NULL;

	for (int i = 0; i < children.size(); ++i) {
		if (children[i]->best_score > best_score) {
			best_score = children[i]->best_score;
			best_child = children[i];
		}
	}

	return best_child;
}

ParametricLSystem::ParametricLSystem(int grid_size, const String& axiom) {
	this->grid_size = grid_size;
	this->axiom = axiom;
}

/**
 * 適当な乱数シードに基づいて、ランダムにgenerateする。
 *
 * @param random_seed		乱数シード
 * @return					生成されたモデル
 */
String ParametricLSystem::derive(int random_seed) {
	std::vector<int> derivation_history;

	String result_model;
	ml::initRand(random_seed);
	while (true) {
		result_model = derive(axiom, MAX_ITERATIONS, derivation_history);
		if (result_model.length() > 0) break;
	}

	return result_model;
}

/**
 * 指定された開始モデルからスタートし、ランダムにgenerateする。
 *
 * @param start_model			開始モデル
 * @param max_iterations		繰り返し数
 * @param build_tree			木を生成するか？
 * @param indicator [OUT]		生成されたモデルのindicator
 * @return						生成されたモデル
 */
String ParametricLSystem::derive(const String& start_model, int max_iterations, std::vector<int>& derivation_history) {
	String model = start_model;

	for (int iter = 0; iter < max_iterations; ++iter) {
		std::vector<Action> actions = getActions(model);
		if (actions.size() == 0) break;


		int index = ml::genRand(0, actions.size());
		derivation_history.push_back(index);
		model = actions[index].apply(model);
	}

	return model;
}

void ParametricLSystem::draw(const String& model, RenderManager* renderManager) {
	std::vector<Vertex> vertices;
	std::vector<Vertex> windowVertices;
	std::vector<Vertex> doorVertices;
	std::vector<Vertex> wallVertices;

	const double door_depth = 20.0;
	const double window_depth = 4.0;

	for (int i = 0; i < model.length(); ++i) {
		if (!model[i].param_defined) continue;

		if (model[i].name == "Door") {
			double x = model[i].param_values[0];
			double y = model[i].param_values[1];
			double w = model[i].param_values[2];
			double h = model[i].param_values[3];
			
			// 上の壁を描画
			{
				glm::mat4 mat;
				mat = glm::translate(mat, glm::vec3(x + w * 0.5, y + h * 0.875, 0.0));
				//glutils::drawQuad(w, h * 0.25, glm::vec3(0.8, 0.8, 0.8), mat, vertices);
				glutils::drawQuad(w, h * 0.25, 
					glm::vec3(x * 0.05, (y + h * 0.75) * 0.05, 0.0), 
					glm::vec3((x + w) * 0.05, (y + h * 0.75) * 0.05, 0.0),
					glm::vec3((x + w) * 0.05, (y + h) * 0.05, 0.0),
					glm::vec3(x * 0.05, (y + h) * 0.05, 0.0),
					mat, wallVertices);
			}

			// 左側の横壁
			{
				glm::mat4 mat;				
				mat = glm::translate(mat, glm::vec3(x, y + h * 0.375, -door_depth * 0.5));
				mat = glm::rotate(mat, deg2rad(90), glm::vec3(0, 1, 0));
				glutils::drawQuad(door_depth, h * 0.75, glm::vec3(0.8, 0.8, 0.8), mat, vertices);
			}

			// 右側の横壁
			{
				glm::mat4 mat;				
				mat = glm::translate(mat, glm::vec3(x + w, y + h * 0.375, -door_depth * 0.5));
				mat = glm::rotate(mat, deg2rad(-90), glm::vec3(0, 1, 0));
				glutils::drawQuad(door_depth, h * 0.75, glm::vec3(0.8, 0.8, 0.8), mat, vertices);
			}

			// 上側の横壁
			{
				glm::mat4 mat;				
				mat = glm::translate(mat, glm::vec3(x + w * 0.5, y + h * 0.75, -door_depth * 0.5));
				mat = glm::rotate(mat, deg2rad(90), glm::vec3(1, 0, 0));
				glutils::drawQuad(w, door_depth, glm::vec3(0.8, 0.8, 0.8), mat, vertices);
			}

			// ドアを描画
			{
				glm::mat4 mat;
				mat = glm::translate(mat, glm::vec3(x + w * 0.5, y + h * 0.375, -door_depth));
				glutils::drawQuad(w, h * 0.75, glm::vec3(0.8, 0.5, 0.3), mat, doorVertices);
			}
		} else if (model[i].name == "Window") {
			double x = model[i].param_values[0];
			double y = model[i].param_values[1];
			double w = model[i].param_values[2];
			double h = model[i].param_values[3];
			
			// 上の壁を描画
			{
				glm::mat4 mat;
				mat = glm::translate(mat, glm::vec3(x + w * 0.5, y + h * 0.875, 0.0));
				//glutils::drawQuad(w, h * 0.25, glm::vec3(0.8, 0.8, 0.8), mat, vertices);
				glutils::drawQuad(w, h * 0.25, 
					glm::vec3(x * 0.05, (y + h * 0.75) * 0.05, 0.8), 
					glm::vec3((x + w) * 0.05, (y + h * 0.75) * 0.05, 0.8), 
					glm::vec3((x + w) * 0.05, (y + h) * 0.05, 0.8), 
					glm::vec3(x * 0.05, (y + h) * 0.05, 0.8), 
					mat, wallVertices);
			}

			// 下の壁を描画
			{
				glm::mat4 mat;
				mat = glm::translate(mat, glm::vec3(x + w * 0.5, y + h * 0.105, 0.0));
				//glutils::drawQuad(w, h * 0.21, glm::vec3(0.8, 0.8, 0.8), mat, vertices);
				glutils::drawQuad(w, h * 0.21, 
					glm::vec3(x * 0.05, y * 0.05, 0.8), 
					glm::vec3((x + w) * 0.05, y * 0.05, 0.8), 
					glm::vec3((x + w) * 0.05, (y + h * 0.21) * 0.05, 0.8), 
					glm::vec3(x * 0.05, (y + h * 0.21) * 0.05, 0.8), 
					mat, wallVertices);
			}

			// 左側の横壁
			{
				glm::mat4 mat;				
				mat = glm::translate(mat, glm::vec3(x, y + h * 0.5, -window_depth * 0.5));
				mat = glm::rotate(mat, deg2rad(90), glm::vec3(0, 1, 0));
				glutils::drawQuad(window_depth, h * 0.5, glm::vec3(0.8, 0.8, 0.8), mat, vertices);
			}

			// 右側の横壁
			{
				glm::mat4 mat;				
				mat = glm::translate(mat, glm::vec3(x + w, y + h * 0.5, -window_depth * 0.5));
				mat = glm::rotate(mat, deg2rad(-90), glm::vec3(0, 1, 0));
				glutils::drawQuad(window_depth, h * 0.5, glm::vec3(0.8, 0.8, 0.8), mat, vertices);
			}

			// 上側の横壁
			{
				glm::mat4 mat;				
				mat = glm::translate(mat, glm::vec3(x + w * 0.5, y + h * 0.75, -window_depth * 0.5));
				mat = glm::rotate(mat, deg2rad(90), glm::vec3(1, 0, 0));
				glutils::drawQuad(w, window_depth, glm::vec3(0.8, 0.8, 0.8), mat, vertices);
			}

			// 下側の横壁
			{
				glm::mat4 mat;				
				mat = glm::translate(mat, glm::vec3(x + w * 0.5, y + h * 0.25, -window_depth * 0.25));
				mat = glm::rotate(mat, deg2rad(-90), glm::vec3(1, 0, 0));
				glutils::drawQuad(w, window_depth * 1.5, glm::vec3(0.8, 0.8, 0.8), mat, vertices);
			}

			// 下側の台の前面
			{
				glm::mat4 mat;				
				mat = glm::translate(mat, glm::vec3(x + w * 0.5, y + h * 0.23, window_depth * 0.5));
				glutils::drawQuad(w, h * 0.04, glm::vec3(0.8, 0.8, 0.8), mat, vertices);
			}

			// 下側の台の左側面
			{
				glm::mat4 mat;				
				mat = glm::translate(mat, glm::vec3(x, y + h * 0.23, window_depth * 0.25));
				mat = glm::rotate(mat, deg2rad(-90), glm::vec3(0, 1, 0));
				glutils::drawQuad(window_depth * 0.5, h * 0.04, glm::vec3(0.8, 0.8, 0.8), mat, vertices);
			}

			// 下側の台の右側面
			{
				glm::mat4 mat;				
				mat = glm::translate(mat, glm::vec3(x + w, y + h * 0.23, window_depth * 0.25));
				mat = glm::rotate(mat, deg2rad(90), glm::vec3(0, 1, 0));
				glutils::drawQuad(window_depth * 0.5, h * 0.04, glm::vec3(0.8, 0.8, 0.8), mat, vertices);
			}

			// 下側の台の下側面
			{
				glm::mat4 mat;				
				mat = glm::translate(mat, glm::vec3(x + w * 0.5, y + h * 0.21, window_depth * 0.25));
				mat = glm::rotate(mat, deg2rad(90), glm::vec3(1, 0, 0));
				glutils::drawQuad(w, window_depth * 0.5, glm::vec3(0.8, 0.8, 0.8), mat, vertices);
			}

			// 窓を描画
			{
				glm::mat4 mat;
				mat = glm::translate(mat, glm::vec3(x + w * 0.5, y + h * 0.5, -window_depth));
				glutils::drawQuad(w, h * 0.5, glm::vec3(1, 1, 1), mat, windowVertices);
			}
		} else if (model[i].name == "Wall" || model[i].name == "W") {
			double x = model[i].param_values[0];
			double y = model[i].param_values[1];
			double w = model[i].param_values[2];
			double h = model[i].param_values[3];
			
			// 壁
			{
				glm::mat4 mat;
				mat = glm::translate(mat, glm::vec3(x + w * 0.5, y + h * 0.5, 0.0));
				//glutils::drawQuad(w, h, glm::vec3(0.8, 0.8, 0.8), mat, wallVertices);
				glutils::drawQuad(w, h, 
					glm::vec3(x * 0.05, y * 0.05, 0.0),
					glm::vec3((x + w) * 0.05, y * 0.05, 0.0),
					glm::vec3((x + w) * 0.05, (y + h) * 0.05, 0.0),
					glm::vec3(x * 0.05, (y + h) * 0.05, 0.0),
					mat, wallVertices);
			}
		}
	}

	renderManager->removeObject("object");
	renderManager->removeObject("object2");
	renderManager->removeObject("object3");
	renderManager->removeObject("object4");
	renderManager->addObject("object", "", vertices);
	renderManager->addObject("object2", "textures/window.jpg", windowVertices);
	renderManager->addObject("object3", "textures/door.jpg", doorVertices);
	renderManager->addObject("object4", "textures/wall.jpg", wallVertices);
}

/**
 * 指定された文字列に基づいてモデルを生成し、indicatorを計算して返却する。
 * 
 * @param model				モデルを表す文字列
 * @param mvpMat			カメラ視点への射影行列
 * @param indicator [OUT]	indicator
 */
void ParametricLSystem::computeIndicator(const String& model, const glm::mat4& mvpMat, std::vector<cv::Mat>& indicator) {
	computeIndicator(model, mvpMat, glm::mat4(), indicator);
}

/**
 * 指定された文字列に基づいてモデルを生成し、indicatorを計算して返却する。
 * 
 * @param model				モデルを表す文字列
 * @param mvpMat			カメラ視点への射影行列
 * @param baseModelMat		モデルのベース変換行列
 * @param indicator [OUT]	indicator
 */
void ParametricLSystem::computeIndicator(const String& model, const glm::mat4& mvpMat, const glm::mat4& baseModelMat, std::vector<cv::Mat>& indicator) {
	indicator.resize(NUM_LAYERS);
	for (int i = 0; i < NUM_LAYERS; ++i) {
		indicator[i] = cv::Mat::zeros(grid_size, grid_size, CV_32F);
	}

	std::list<glm::mat4> stack;

	glm::mat4 modelMat = baseModelMat;

	int undefined = 0;
	for (int i = 0; i < model.length(); ++i) {
		if (!model[i].param_defined) break;

		if (model[i].name == "Door") {
			double x = model[i].param_values[0];
			double y = model[i].param_values[1];
			double w = model[i].param_values[2];
			double h = model[i].param_values[3];
			
			// 壁を描画
			{
				glm::vec4 p1(x, y, 0, 1);
				glm::vec4 p2(x + w, y + h, 0, 1);
				p1 = mvpMat * p1;
				p2 = mvpMat * p2;
				cv::rectangle(indicator[0], cv::Point(p1.x + grid_size * 0.5, p1.y + grid_size * 0.5), cv::Point(p2.x + grid_size * 0.5, p2.y + grid_size * 0.5), cv::Scalar(1), -1);
				//cv::line(indicator[0], cv::Point(p1.x + grid_size * 0.5, p1.y + grid_size * 0.5), cv::Point(p2.x + grid_size * 0.5, p1.y + grid_size * 0.5), cv::Scalar(1), 10);
				//cv::line(indicator[0], cv::Point(p1.x + grid_size * 0.5, p2.y + grid_size * 0.5), cv::Point(p2.x + grid_size * 0.5, p2.y + grid_size * 0.5), cv::Scalar(1), 10);
			}

			// ドアを描画
			{
				glm::vec4 p1 = glm::vec4(x, y, 0, 1);
				glm::vec4 p2 = glm::vec4(x + w, y + h * 0.75, 0, 1);
				p1 = mvpMat * p1;
				p2 = mvpMat * p2;
				cv::rectangle(indicator[1], cv::Point(p1.x + grid_size * 0.5, p1.y + grid_size * 0.5), cv::Point(p2.x + grid_size * 0.5, p2.y + grid_size * 0.5), cv::Scalar(1), -1);
			}
		} else if (model[i].name == "Window") {
			double x = model[i].param_values[0];
			double y = model[i].param_values[1];
			double w = model[i].param_values[2];
			double h = model[i].param_values[3];
			
			// 壁を描画
			{
				glm::vec4 p1(x, y, 0, 1);
				glm::vec4 p2(x + w, y + h, 0, 1);
				p1 = mvpMat * p1;
				p2 = mvpMat * p2;
				cv::rectangle(indicator[0], cv::Point(p1.x + grid_size * 0.5, p1.y + grid_size * 0.5), cv::Point(p2.x + grid_size * 0.5, p2.y + grid_size * 0.5), cv::Scalar(1), -1);
				//cv::line(indicator[0], cv::Point(p1.x + grid_size * 0.5, p1.y + grid_size * 0.5), cv::Point(p2.x + grid_size * 0.5, p1.y + grid_size * 0.5), cv::Scalar(1), 10);
				//cv::line(indicator[0], cv::Point(p1.x + grid_size * 0.5, p2.y + grid_size * 0.5), cv::Point(p2.x + grid_size * 0.5, p2.y + grid_size * 0.5), cv::Scalar(1), 10);
			}

			// 窓を描画
			{
				glm::vec4 p1 = glm::vec4(x, y + h * 0.25, 0, 1);
				glm::vec4 p2 = glm::vec4(x + w, y + h * 0.75, 0, 1);
				p1 = mvpMat * p1;
				p2 = mvpMat * p2;
				cv::rectangle(indicator[2], cv::Point(p1.x + grid_size * 0.5, p1.y + grid_size * 0.5), cv::Point(p2.x + grid_size * 0.5, p2.y + grid_size * 0.5), cv::Scalar(1), -1);
			}
		} else if (model[i].name == "Wall" || model[i].name == "W") {
			double x = model[i].param_values[0];
			double y = model[i].param_values[1];
			double w = model[i].param_values[2];
			double h = model[i].param_values[3];
			
			// 壁を描画
			glm::vec4 p1(x, y, 0, 1);
			glm::vec4 p2(x + w, y + h, 0, 1);
			p1 = mvpMat * p1;
			p2 = mvpMat * p2;
			cv::rectangle(indicator[0], cv::Point(p1.x + grid_size * 0.5, p1.y + grid_size * 0.5), cv::Point(p2.x + grid_size * 0.5, p2.y + grid_size * 0.5), cv::Scalar(1), -1);
			//cv::rectangle(indicator[0], cv::Point(p1.x + grid_size * 0.5, p1.y + grid_size * 0.5), cv::Point(p2.x + grid_size * 0.5, p2.y + grid_size * 0.5), cv::Scalar(1), 10);
			//cv::line(indicator[0], cv::Point(p1.x + grid_size * 0.5, p1.y + grid_size * 0.5), cv::Point(p2.x + grid_size * 0.5, p1.y + grid_size * 0.5), cv::Scalar(1), 10);
			//cv::line(indicator[0], cv::Point(p1.x + grid_size * 0.5, p2.y + grid_size * 0.5), cv::Point(p2.x + grid_size * 0.5, p2.y + grid_size * 0.5), cv::Scalar(1), 10);
		}
	}
}

/**
 * 指定されたターゲットindicatorに近いモデルを生成する。
 *
 * @param target			ターゲットindicator
 * @param indicator [OUT]	生成されたモデルのindicator
 * @return					生成されたモデル
 */
String ParametricLSystem::inverse(const std::vector<cv::Mat>& target, const glm::mat4& mvpMat) {
	// UCTを使って探索木を構築していく
	String model = axiom;

	for (int l = 0; l < MAX_ITERATIONS; ++l) {
		model = UCT(model, target, mvpMat, l);

		std::vector<cv::Mat> indicator;
		computeIndicator(model, mvpMat, indicator);
		double sc = score(indicator, target);

		/////// デバッグ ///////
		/*
		char filename[256];
		sprintf(filename, "indicator_%d.png", l);
		computeIndicator(model, mvpMat, indicator);

		ml::mat_save(filename, indicator + target * 0.4);
		*/
		/////// デバッグ ///////

		cout << l << ": " << "Best score=" << setprecision(4) << sc << " : " << model << endl;
		//cout << l << ": " << "Best score=" << setprecision(4) << sc << endl;

		// これ以上、derivationできなら、終了
		if (model.cursor < 0) break;
	}

	// スコア表示
	std::vector<cv::Mat> indicator;
	computeIndicator(model, mvpMat, indicator);
	cout << setprecision(4) << score(indicator, target) << endl;
	
	return model;
}

/**
 * 指定されたmodelからUCTをスタートし、最善のoptionを返却する。
 *
 * @param model		モデル
 * @param target	ターゲット
 * @return			最善のoption
 */
String ParametricLSystem::UCT(const String& current_model, const std::vector<cv::Mat>& target, const glm::mat4& mvpMat, int derivation_step) {
	// これ以上、derivationできなら、終了
	int index = current_model.cursor;
	if (index < 0) return current_model;

	// expandするリテラルを取得する
	String model = current_model.getExpand();

	// 現在のカーソルのdepthを取得
	int depth = current_model[current_model.cursor].depth;

	// マスク画像を作成
	cv::Mat mask = createMask(current_model, mvpMat);

	// ルートノードを作成
	Node* current_node = new Node(model);
	current_node->setActions(getActions(model));

	// ベースとなるindicatorを計算
	std::vector<cv::Mat> baseIndicator;
	computeIndicator(current_model, mvpMat, baseIndicator);

	for (int iter = 0; iter < NUM_MONTE_CARLO_SAMPLING; ++iter) {
		// もしノードがリーフノードなら、終了
		if (current_node->untriedActions.size() == 0 && current_node->children.size() == 0) break;

		// 現在のノードのスコアが確定したら、終了
		if (current_node->fixed) break;

		Node* node = current_node;

		// 探索木のリーフノードを選択
		while (node->untriedActions.size() == 0 && node->children.size() > 0) {
			node = node->UCTSelectChild();
		}

		// 子ノードがまだ全てexpandされていない時は、1つランダムにexpand
		if (node->untriedActions.size() > 0) {// && node->children.size() <= ml::log((double)iter * 0.01 + 1, 1.4)) {
			Action action = node->randomlySelectAction();
			String child_model = action.apply(node->model);
						
			node = node->addChild(child_model, action);
			node->setActions(getActions(child_model));
			num_nodes++;
		}

		// ランダムにderiveする
		std::vector<int> derivation_history;
		String result_model = derive(node->model, MAX_ITERATIONS_FOR_MC, derivation_history);

		// indicatorを計算する
		std::vector<cv::Mat> indicator;
		computeIndicator(result_model, mvpMat, glm::mat4(), indicator);
		for (int i = 0; i < NUM_LAYERS; ++i) {
			indicator[i] += baseIndicator[i];

			// clamp
			cv::threshold(indicator[i], indicator[i], 0.0, 1.0, cv::THRESH_BINARY);
		}

		// スコアを計算する
		double sc = score(indicator, target, mask);
		node->best_score = sc;


		/////// デバッグ ///////
		/*{
			for (int i = 0; i < NUM_LAYERS; ++i) {
				char filename[256];
				sprintf(filename, "images/indicator_%d_%d_%d.png", derivation_step, iter, i);
				cv::Mat img = indicator[i] * 0.7 + target[i] * 0.4;
				img = ml::mat_mask(img, mask, 0.7);

				ml::mat_save(filename, img);

				cout << "   " << filename << " : " << result_model << endl;
			}
		}*/
		/////// デバッグ ///////



		// リーフノードなら、スコアを確定する
		if (node->untriedActions.size() == 0 && node->children.size() == 0) {
			node->fixed = true;
		}

		// スコアをbackpropagateする
		bool updated = false;
		Node* leaf = node;
		while (node != NULL) {
			node->visits++;
			node->scores.push_back(sc);
			if (sc > node->best_score) {
				node->best_score = sc;
				updated = true;
			}

			// 子ノードが全て展開済みで、且つ、スコア確定済みなら、このノードのスコアも確定とする
			if (node->untriedActions.size() == 0) {
				bool fixed = true;
				for (int c = 0; c < node->children.size(); ++c) {
					if (!node->children[c]->fixed) {
						fixed = false;
						break;
					}
				}
				node->fixed = fixed;
			}

			node = node->parent;
		}
	}



	////// デバッグ //////
	/*
	for (int i = 0; i < current_node->children.size(); ++i) {
		cout << "  " << i << ":" << endl;
		if (current_node->children[i]->action.type == Action::ACTION_RULE) {
			cout << "   " << current_node->children[i]->action.rule << endl;
		} else {
			cout << "   " << current_node->children[i]->action.value << endl;
		}
		cout << "    - visits: " << current_node->children[i]->visits << endl;
		cout << "    - best score: " << current_node->children[i]->best_score << endl;
		cout << "    - avg score: " << ml::mean(current_node->children[i]->scores) << endl;
		cout << "    - var score: " << ml::variance(current_node->children[i]->scores) << endl;
	}
	*/
	////// デバッグ //////


	Action best_action = current_node->bestChild()->action;
	String best_model = best_action.apply(current_model);



	/////// デバッグ ///////
	/*
	char filename[256];
	computeIndicator(best_model, 4, bestIndicator);
	sprintf(filename, "images/indicator_%d.png", derivation_step);

	cv::Mat target2;
	cv::resize(target, target2, cv::Size(400, 400));
	cv::Mat img = bestIndicator + target2 * 0.4;
	img = ml::mat_mask(img, mask2, 0.8);

	ml::mat_save(filename, img);
	*/
	/////// デバッグ ///////



	// 探索木のメモリを解放する
	releaseNodeMemory(current_node);
	
	return best_model;
}

/**
 * indicatorのスコアを計算して返却する。
 *
 * @param indicator		indicator
 * @param target		ターゲットindicator
 * @return				スコア
 */
double ParametricLSystem::score(const std::vector<cv::Mat>& indicator, const std::vector<cv::Mat>& target) {
	return score(indicator, target, cv::Mat::ones(target[0].size(), CV_8U));
}

/**
 * indicatorのスコアを計算して返却する。
 *
 * @param indicator		indicator
 * @param target		ターゲットindicator
 * @param mask			マスク
 * @return				スコア
 */
double ParametricLSystem::score(const std::vector<cv::Mat>& indicator, const std::vector<cv::Mat>& target, const cv::Mat& mask) {
	double count = 0;
	double total = 0;

	for (int i = 0; i < indicator.size(); ++i) {
		cv::Mat result;
		cv::subtract(indicator[i], target[i], result, mask);
		
		count += ml::mat_squared_sum(result);

		cv::Mat result2;
		cv::subtract(target[i], cv::Mat::zeros(target[i].size(), target[i].type()), result2, mask);

		total += ml::mat_squared_sum(result2);
	}

	if (total > 0.0) {
		return 1.0 - count / total;
	} else {
		if (count > 0.0) {
			return -10000.0;
		} else {
			return 1.0;
		}
	}
}

/**
 * 指定されたモデルの、次のderivationの候補を返却する。
 * Hardcoding: 各literalは、4つのパラメータを持つ (X座標、Y座標、幅)
 *
 * @param model		モデル
 * @return			次のderivationの候補リスト
 */
std::vector<Action> ParametricLSystem::getActions(const String& model) {
	std::vector<Action> actions;

	// 展開するパラメータを決定
	int i = model.cursor;

	// 新たなderivationがないなら、終了
	if (i == -1) return actions;

	if (model[i].name == "X") {
		for (int k = 2; k <= 2; ++k) {
			String rule;
			double height = model[i].param_values[3] / (double)k;
			for (int l = 0; l < k; ++l) {
				rule += Literal("Floor", model[i].depth + 1, model[i].param_values[0], model[i].param_values[1] + height * l, model[i].param_values[2], height);
			}
			actions.push_back(Action(actions.size(), i, rule));
		}
	} else if (model[i].name == "Floor") {
		String rule = Literal("W", model[i].depth + 1, model[i].param_values[0], model[i].param_values[1], model[i].param_values[2], model[i].param_values[3]);
		actions.push_back(Action(actions.size(), i, rule));

		for (double k = 0.2; k <= 0.8; k += 0.1) {
			for (double l = 0.1; l <= 0.3; l += 0.1) {
				if (model[i].param_values[2] * k < 10.0) continue;
				if (model[i].param_values[2] * l < 10.0) continue;
				if (model[i].param_values[2] * (1.0 - k - l) < 10.0) continue;
				
				String rule = Literal("W", model[i].depth + 1,  model[i].param_values[0], model[i].param_values[1], model[i].param_values[2] * k, model[i].param_values[3])
					+ Literal("Door", model[i].depth + 1, model[i].param_values[0] + model[i].param_values[2] * k, model[i].param_values[1], model[i].param_values[2] * l, model[i].param_values[3])
					+ Literal("W", model[i].depth + 1, model[i].param_values[0] + model[i].param_values[2] * (k + l), model[i].param_values[1], model[i].param_values[2] * (1.0 - k - l), model[i].param_values[3]);
				actions.push_back(Action(actions.size(), i, rule));
			}
		}
	} else if (model[i].name == "W") {
		String rule = Literal("Wall", model[i].depth + 1, model[i].param_values[0], model[i].param_values[1], model[i].param_values[2], model[i].param_values[3]);
		actions.push_back(Action(actions.size(), i, rule));

		for (double k = 0.2; k <= 0.8; k += 0.1) {
			for (double l = 0.1; l <= 0.6; l += 0.1) {
				if (model[i].param_values[2] * k < 10.0) continue;
				if (model[i].param_values[2] * l < 10.0) continue;
				if (model[i].param_values[2] * (1.0 - k - l) < 10.0) continue;

				String rule = Literal("W", model[i].depth + 1, model[i].param_values[0], model[i].param_values[1], model[i].param_values[2] * k, model[i].param_values[3])
					+ Literal("Window", model[i].depth + 1, model[i].param_values[0] + model[i].param_values[2] * k, model[i].param_values[1], model[i].param_values[2] * l, model[i].param_values[3])
					+ Literal("W", model[i].depth + 1, model[i].param_values[0] + model[i].param_values[2] * (k + l), model[i].param_values[1], model[i].param_values[2] * (1.0 - k - l), model[i].param_values[3]);
				actions.push_back(Action(actions.size(), i, rule));
			}
		}
	}

	return actions;
}

/**
 * 現在の座標を計算して返却する。
 */
cv::Mat ParametricLSystem::createMask(const String& model, const glm::mat4& mvpMat) {
	double x = model[model.cursor].param_values[0];
	double y = model[model.cursor].param_values[1];
	double w = model[model.cursor].param_values[2];
	double h = model[model.cursor].param_values[3];

	glm::vec4 p1(x, y, 0, 1);
	glm::vec4 p2(x + w, y + h, 0, 1);
	p1 = mvpMat * p1;
	p2 = mvpMat * p2;
	
	cv::Mat mask = cv::Mat::zeros(grid_size, grid_size, CV_8U);
	cv::rectangle(mask, cv::Point(p1.x + grid_size * 0.5, p1.y + grid_size * 0.5), cv::Point(p2.x + grid_size * 0.5, p2.y + grid_size * 0.5), cv::Scalar(1), -1);
	return mask;
}

/**
 * 指定されたノードをルートとする探索木のメモリを解放する
 *
 * @param node		ノード
 */
void ParametricLSystem::releaseNodeMemory(Node* node) {
	for (int i = 0; i < node->children.size(); ++i) {
		if (node->children[i] != NULL) {
			releaseNodeMemory(node->children[i]);
			num_nodes--;
		}
	}
	delete node;
	node = NULL;
}

float deg2rad(float deg) {
	return deg * M_PI / 180.0;
}

}
