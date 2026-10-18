#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <random>
#include <cstdint>

class WeightedGraph {
 public:
  WeightedGraph() = default;
  ~WeightedGraph() = default;
  WeightedGraph& operator=(const WeightedGraph&) = default;

  const std::unordered_map<size_t, std::pair<std::string, bool>>& Look() const {
    return edges_[curr_];
  }

  void PrintLook() const { // Иллюстрирующая реализация Look
    for (const auto& [key, pair] : edges_[curr_]) {
      std::cout << "PATH " << pair.first << " " << profits_names_[key].second << " " << (pair.second ? "BROKEN\n" : "OK\n");
    }
  }

  size_t Path(const std::string& path_id) {
    return (edges_names_[path_id].first == curr_ ? edges_names_[path_id].second : edges_names_[path_id].first);
  }

  bool Move(const std::string& path_id) {
    if (!edges_[curr_][Path(path_id)].second) {
      curr_ = Path(path_id);
      SetProfit();
    }
    return !edges_[curr_][Path(path_id)].second;  // Возвращает 1, если успешно переместился
  }

  void PrintMove(const std::string& path_id) {  // Иллюстрирующая реализация Move
    std::cout << (Move(path_id) ? "OK\n" : "FAIL\n");
  }

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
  std::vector<std::pair<std::pair<int64_t, bool>, std::string>> profits_names_;  //Залы, их ценности (храним информацию о посещении зала) и имена
  std::unordered_map<std::string, std::pair<size_t, size_t>> edges_names_;  // Переходы между залами и их имена
  std::vector<std::unordered_map<size_t, std::pair<std::string, bool>>> edges_;  // Переходы между залами, их имена и их статус (0 - целый, 1 - обвален)
  size_t curr_ = 0;  // Текущий зал
  Random rand_;  // Вызов rand_() предоставляет рандомное число на [0.0, 1.0), сравнение с любой вероятностью выдаст 1 с этой же вероятностью
  int64_t day_cost_ = 0;
  size_t days_ = 0;  // Текущая длительность экспедиции
  int64_t profit_ = 0;

  void InsertVertex(const std::string& str, int64_t profit, bool entrance) {
    if (entrance) {
      curr_ = vertexes_.size();
    }
    vertexes_.emplace(str, profits_names_.size());
    profits_names_.emplace_back(std::make_pair(std::make_pair(profit, true), str));
    edges_.emplace_back();
  }

  void InsertEdge(const std::string& first, const std::string& second, bool status, const std::string& edge) {
    edges_names_.emplace(edge, std::make_pair(vertexes_[first], vertexes_[second]));
    edges_[vertexes_[first]].emplace(vertexes_[second], std::make_pair(edge, status));
    edges_[vertexes_[second]].emplace(vertexes_[first], std::make_pair(edge, status));
  }

  void SetProfit() {
    if (profits_names_[curr_].first.second) {
      profit_ += profits_names_[curr_].first.first;
      profits_names_[curr_].first.second = false;
    }
  }

  friend std::istream& operator>>(std::istream& is, WeightedGraph& graph);
};

using Graph = WeightedGraph;

std::istream& operator>>(std::istream& is, Graph& graph) {  // Оператор ввода графа
  std::string entrance;
  std::string first;
  std::string second;
  int64_t profit;
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
  graph.SetProfit();
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
  std::string input;
  std::string path_id;
  while (std::cin >> input) {
    if (input == "LOOK") {
      graph.PrintLook();
    }
    else if (input == "MOVE") {
      std::cin >> path_id;
      graph.PrintMove(path_id);
    }
    else if (input == "END") {
      break;
    }
  }
  return 0;
}
