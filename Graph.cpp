#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <random>

class WeightedGraph {
 public:
  WeightedGraph() = default;
  ~WeightedGraph() = default;
  WeightedGraph& operator=(const WeightedGraph&) = default;

 private:
  struct Random {
    std::mt19937 gen;
    std::uniform_real_distribution<double> dist{0.0, 1.0};

    Random() : gen(std::random_device{}()) {}

    double operator()() {
      return dist(gen);
    }
  };
  std::unordered_map<std::string, size_t> vertexes_;  // Залы и их имена
  std::vector<size_t> profits_;  //Залы и их ценности
  std::unordered_map<std::string, std::pair<size_t, size_t>> edges_names_;  // Переходы между залами и их имена
  std::vector<std::unordered_map<size_t, bool>> edges_;  // Переходы между залами и их статус (0 - целый, 1 - обвален)
  size_t curr_ = 0;  // Текущий зал
  Random rand_;  // Вызов rand_() предоставляет рандомное число на [0.0, 1.0), сравнение с любой вероятностью выдаст 1 с этой же вероятностью
  size_t day_cost_ = 0;
  size_t days_ = 0;  // Текущая длительность экспедиции

  void InsertVertex(const std::string& str, size_t profit, bool entrance) {
    if (entrance) {
      curr_ = vertexes_.size();
    }
    vertexes_.emplace(str, profits_.size());
    profits_.emplace_back(profit);
    edges_.emplace_back();
  }

  void InsertEdge(const std::string& first, const std::string& second, bool status, const std::string& edge) {
    edges_names_.emplace(edge, std::make_pair(vertexes_[first], vertexes_[second]));
    edges_[vertexes_[first]].emplace(vertexes_[second], status);
    edges_[vertexes_[second]].emplace(vertexes_[first], status);
  }

  friend std::istream& operator>>(std::istream& is, WeightedGraph& graph);
};

using Graph = WeightedGraph;

std::istream& operator>>(std::istream& is, Graph& graph) {  // Оператор ввода графа
  std::string entrance;
  std::string first;
  std::string second;
  size_t profit;
  std::string edge;
  double probability;
  std::string input;
  while (is >> input) {
    if (input == "entrance") {
      is >> entrance;
    }
    else if (input == "day_cost") {
      is >> graph.day_cost_;
    }
    else if (input == "vertex") {
      is >> first >> profit;
      graph.InsertVertex(first, profit, first == entrance);
    }
    else if (input == "edge") {
      is >> edge >> first >> second >> probability;
      graph.InsertEdge(first, second, (probability > graph.rand_()), edge);
    }
    else if (input == "END") {
      break;
    }
  }
  return is;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  Graph graph;
  std::string str;
  if (std::cin >> str; str == "MAP") {
    std::cin >> graph;
  }
  return 0;
}
