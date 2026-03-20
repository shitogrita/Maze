#include "crow.h"
#include "../core/maze.h"
#include "../core/cave.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace {
s21::Maze current_maze;
s21::Cave current_cave;

std::string ReadFile(const std::string& path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    return "<h1>index.html not found</h1>";
  }

  std::ostringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

std::string MatrixToJson(const std::vector<std::vector<int>>& matrix) {
  std::ostringstream out;
  out << "[";

  for (size_t i = 0; i < matrix.size(); ++i) {
    out << "[";
    for (size_t j = 0; j < matrix[i].size(); ++j) {
      out << matrix[i][j];
      if (j + 1 < matrix[i].size()) {
        out << ",";
      }
    }
    out << "]";
    if (i + 1 < matrix.size()) {
      out << ",";
    }
  }

  out << "]";
  return out.str();
}

std::string PathToJson(const std::vector<std::pair<int, int>>& path) {
  std::ostringstream out;
  out << "[";

  for (size_t i = 0; i < path.size(); ++i) {
    out << "[" << path[i].first << "," << path[i].second << "]";
    if (i + 1 < path.size()) {
      out << ",";
    }
  }

  out << "]";
  return out.str();
}

std::string MazeToJson(const s21::Maze& maze) {
  std::ostringstream out;

  out << "{";
  out << "\"rows\":" << maze.GetRows() << ",";
  out << "\"cols\":" << maze.GetCols() << ",";
  out << "\"right_walls\":" << MatrixToJson(maze.GetRightWalls()) << ",";
  out << "\"bottom_walls\":" << MatrixToJson(maze.GetBottomWalls());
  out << "}";

  return out.str();
}

std::string CaveToJson(const s21::Cave& cave) {
  std::ostringstream out;

  out << "{";
  out << "\"rows\":" << cave.GetRows() << ",";
  out << "\"cols\":" << cave.GetCols() << ",";
  out << "\"cells\":" << MatrixToJson(cave.GetCells());
  out << "}";

  return out.str();
}
}  // namespace

int main() {
  crow::SimpleApp app;

  CROW_ROUTE(app, "/")([]() {
    return crow::response(ReadFile("web/index.html"));
  });

  CROW_ROUTE(app, "/generate")
  ([](const crow::request& req) {
    const char* rows_param = req.url_params.get("rows");
    const char* cols_param = req.url_params.get("cols");

    if (!rows_param || !cols_param) {
      return crow::response(400, "rows or cols is missing");
    }

    int rows = std::stoi(rows_param);
    int cols = std::stoi(cols_param);

    rows = std::max(1, std::min(rows, 50));
    cols = std::max(1, std::min(cols, 50));

    current_maze.GenerateMaze(rows, cols);

    crow::response res;
    res.code = 200;
    res.set_header("Content-Type", "application/json");
    res.write(MazeToJson(current_maze));
    return res;
  });

  CROW_ROUTE(app, "/solve")
  ([](const crow::request& req) {
    if (current_maze.IsEmpty()) {
      crow::response res;
      res.code = 200;
      res.set_header("Content-Type", "application/json");
      res.write("{\"path\":[]}");
      return res;
    }

    const char* start_row_param = req.url_params.get("start_row");
    const char* start_col_param = req.url_params.get("start_col");
    const char* finish_row_param = req.url_params.get("finish_row");
    const char* finish_col_param = req.url_params.get("finish_col");

    if (!start_row_param || !start_col_param ||
        !finish_row_param || !finish_col_param) {
      return crow::response(400, "solve parameters are missing");
    }

    int start_row = std::stoi(start_row_param);
    int start_col = std::stoi(start_col_param);
    int finish_row = std::stoi(finish_row_param);
    int finish_col = std::stoi(finish_col_param);

    auto path = current_maze.Bfs({start_row, start_col},
                                 {finish_row, finish_col});

    crow::response res;
    res.code = 200;
    res.set_header("Content-Type", "application/json");
    res.write("{\"path\":" + PathToJson(path) + "}");
    return res;
  });

  CROW_ROUTE(app, "/cave/generate")
  ([](const crow::request& req) {
    const char* rows_param = req.url_params.get("rows");
    const char* cols_param = req.url_params.get("cols");
    const char* chance_param = req.url_params.get("chance");
    const char* birth_param = req.url_params.get("birth");
    const char* death_param = req.url_params.get("death");

    if (!rows_param || !cols_param || !chance_param ||
        !birth_param || !death_param) {
      return crow::response(400, "cave parameters are missing");
    }

    int rows = std::stoi(rows_param);
    int cols = std::stoi(cols_param);
    int chance = std::stoi(chance_param);
    int birth = std::stoi(birth_param);
    int death = std::stoi(death_param);

    rows = std::max(1, std::min(rows, 50));
    cols = std::max(1, std::min(cols, 50));
    chance = std::max(0, std::min(chance, 100));
    birth = std::max(0, std::min(birth, 7));
    death = std::max(0, std::min(death, 7));

    current_cave.GenerateCave(rows, cols, chance);
    current_cave.SetLimits(birth, death);

    crow::response res;
    res.code = 200;
    res.set_header("Content-Type", "application/json");
    res.write(CaveToJson(current_cave));
    return res;
  });

  CROW_ROUTE(app, "/cave/step")
  ([](const crow::request& req) {
    if (current_cave.IsEmpty()) {
      crow::response res;
      res.code = 200;
      res.set_header("Content-Type", "application/json");
      res.write("{\"rows\":0,\"cols\":0,\"cells\":[]}");
      return res;
    }

    const char* birth_param = req.url_params.get("birth");
    const char* death_param = req.url_params.get("death");

    if (!birth_param || !death_param) {
      return crow::response(400, "cave step parameters are missing");
    }

    int birth = std::stoi(birth_param);
    int death = std::stoi(death_param);

    birth = std::max(0, std::min(birth, 7));
    death = std::max(0, std::min(death, 7));

    current_cave.SetLimits(birth, death);
    current_cave.Step();

    crow::response res;
    res.code = 200;
    res.set_header("Content-Type", "application/json");
    res.write(CaveToJson(current_cave));
    return res;
  });

  app.port(1234).run();
}