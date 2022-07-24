#include <iostream>
#include <vector>
#include <utility>
#include <thread>
#include <chrono>

typedef struct Row {
  int a;
  int b;
} Row;

void PrintOneRow(Row* row) {
  std::cout << "a is:" << row->a << ", b is" << row->b << std::endl;
}

void DoTask(const std::vector<Row*>& rows, size_t start, size_t end) {
  if (start > end || rows.empty()) {
    return;
  }
  for (int i = start; i < end; ++i) {
    Row * tmp = rows[i];
    if ((tmp->a == 1000 || tmp->a == 2000 || tmp->a == 3000) && (tmp->b >= 10 && tmp->b < 50)) {
      PrintOneRow(tmp);
    }
  }
}

void Task1(const std::vector<Row*>& rows) {
  size_t len = rows.size();
  if (len == 0) {
    return;
  }
  size_t thread_num = 10;
  size_t scope = len / thread_num;
  for (size_t i = 0; i < thread_num; ++i) {
    size_t start = i * scope;
    size_t end = (i + 1) * scope;
    std::thread t(DoTask, std::ref(rows), start, end);
  }
  // 单独处理最后部分
  std::thread t(DoTask, std::ref(rows), thread_num * scope, len - 1);
}

int main() {
  // 开启多线程分别执行
  // 直接输出符合条件的row
  return 0;
}