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
  int len = rows.size();
  // 二分查找剪枝
  if (start > end || len == 0 || rows[start]->a > 3000 || rows[end]->a < 1000) {
    return;
  }
  BinarySearch(rows, start, end, 1000);
  BinarySearch(rows, start, end, 2000);
  BinarySearch(rows, start, end, 3000);
}

/** 
  * @brief  二分查找
  * @param rows 原始数组
  * @param start 起始位置
  * @param end 结束位置
  * @param target 查找的目标数
  */
void BinarySearch(const std::vector<Row*>& rows, size_t start, size_t end, int target) {
  if (rows[start]->a > target || rows[end]->a < target) {
    return;
  }
  int left = start, right = end;
  while (left <= right) {
    int mid = (left + right) / 2;
    if (rows[mid]->a == target) {
      PrintAll(rows, start, end, mid);
    } else if (rows[mid]->a < target) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }
  return;
}

/** 
  * @brief  查找一个目标左右符合条件的row并打印
  * @param rows 原始数组
  * @param start 起始位置
  * @param end 结束位置
  * @param pos 目标位置的索引
  */
void PrintAll(const std::vector<Row*>& rows, size_t start, size_t end, size_t pos) {
  if (rows[pos]->b < 50 && rows[pos]->b >= 10) {
    // 这种情况两边都要考虑
    for (size_t i = pos; i >= start && rows[pos]->b >= 10; --i) {
      if (rows[pos]->b >= 10) {
        PrintOneRow(rows[i]);
      }
    }
    for (size_t j= pos + 1; j <= end; ++j) {
      if (rows[pos]->b < 50) {
        PrintOneRow(rows[pos]);
      }
    }
  } else if (rows[pos]->b < 50) {
    // 这种情况只打印左边
    for (size_t i = pos; i >= start; --i) {
      if (rows[pos]->b >= 10) {
        PrintOneRow(rows[i]);
      }
    }
  } else if (rows[pos]->b >= 10) {
    // 这中情况只打印右边
    for (size_t j= pos + 1; j <= end; ++j) {
      if (rows[pos]->b < 50) {
        PrintOneRow(rows[pos]);
      }
    }
  }
}


void Task2(const std::vector<Row*>& rows) {
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
  // 开启多线程分别执行，因为数据已经按(a,b)进行排序，可以在每个线程中对符合条件的a进行二分查找
  // 同时对二分查找进行剪枝，在找到a符合条件的列以后，再对其左右两侧的数据进行判断，如果b也符合条件，则输出对应的行
  return 0;
}