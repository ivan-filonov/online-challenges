#include <iostream>
#include <vector>

template <typename num_t> class solver {
private:
  const int INF = 0x7fffffff;

  size_t numRows;
  size_t numColumns;

  // потенциал для [row][column] = rowPotential[row] + columnPotential[column]
  std::vector<num_t> rowPotential;
  std::vector<num_t> columnPotential;

  // паросочетание: для каждого столбца i = 1 ... m хранит номер соответствующей
  // выбранной строки perColumnAssignments[i] (или 0, если пока ничего не
  // выбрано). При этом perColumnAssignments[0] для удобства реализации
  // полагается равным номеру текущей рассматриваемой строки.
  std::vector<int> perColumnAssignments;
  // содержит информацию о том, где эти минимумы достигаются, чтобы мы
  // впоследствии смогли восстановить увеличивающую цепочку.
  std::vector<int> way;

  // вспомогательные минимумы, необходимые для быстрого пересчёта потенциала
  std::vector<num_t> minColumnPotential;
  std::vector<bool> columnUsed;

  int addRowToSolution(const std::vector<std::vector<num_t>> &costsMatrix,
                       size_t row) {
    perColumnAssignments[0] = row;
    int currentColumn = 0;
    minColumnPotential.assign(numColumns + 1, INF);
    columnUsed.assign(numColumns + 1, false);
    for (;;) {
      columnUsed[currentColumn] = true;
      int currentRow = perColumnAssignments[currentColumn];
      // минимальное значение в minColumnPotential для добавляемых на данном
      // этапе строк
      num_t delta = INF;
      int nextColumn = 0;
      for (int column = 1; column <= numColumns; ++column) {
        if (columnUsed[column]) {
          continue;
        }
        int cur = costsMatrix[currentRow][column] - rowPotential[currentRow] -
                  columnPotential[column];
        if (cur < minColumnPotential[column]) {
          minColumnPotential[column] = cur;
          way[column] = currentColumn;
        }
        if (minColumnPotential[column] < delta) {
          delta = minColumnPotential[column];
          nextColumn = column;
        }
      }

      // обновить потенциал только для используемых колонок,
      // для неиспользуемых - обновить только вспомогательные
      // промежуточные значения.
      for (int column = 0; column <= numColumns; ++column) {
        if (columnUsed[column]) {
          rowPotential[perColumnAssignments[column]] += delta;
          columnPotential[column] -= delta;
        } else {
          minColumnPotential[column] -= delta;
        }
      }

      currentColumn = nextColumn;
      if (!perColumnAssignments[currentColumn]) {
        break;
      }
    }

    return currentColumn;
  }

  void updateColumnAssignments(int currentColumn) {
    for (;;) {
      int nextColumn = way[currentColumn];
      perColumnAssignments[currentColumn] = perColumnAssignments[nextColumn];
      currentColumn = nextColumn;
      if (!currentColumn) {
        break;
      }
    }
  }

  std::vector<int> buildAnswer() {
    std::vector<int> answer(numRows + 1);
    for (int column = 1; column <= numColumns; ++column) {
      answer[perColumnAssignments[column]] = column;
    }
    return answer;
  }

public:
  std::vector<int> solve(const std::vector<std::vector<num_t>> &costsMatrix) {
    numRows = costsMatrix.size() - 1;
    numColumns = costsMatrix.at(0).size() - 1;

    rowPotential.assign(numRows + 1, 0);
    columnPotential.assign(numColumns + 1, 0);
    perColumnAssignments.assign(numColumns + 1, 0);
    way.assign(numColumns + 1, 0);

    for (size_t row = 1; row <= numRows; ++row) {
      auto currentColumn = addRowToSolution(costsMatrix, row);
      updateColumnAssignments(currentColumn);
    }

    return buildAnswer();
  }

  num_t cost() const { return -columnPotential[0]; }
};

int main() {
  solver<float> s{};
  std::vector<std::vector<float>> costsMatrix{
      {0, 0, 0}, {0, 5, 15}, {0, 10, 15}};
  auto ans = s.solve(costsMatrix);
  for (size_t i = 1; i < ans.size(); ++i) {
    std::cout << (i > 1 ? " " : "") << ans[i] << " (" << costsMatrix[i][ans[i]]
              << ")";
  }
  std::cout << "\ncost " << s.cost() << "\n";
}
