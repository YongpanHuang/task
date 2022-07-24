#include <iostream>
#include <vector>
#include <utility>
#include <thread>
#include <chrono>
#include <list>
#include <map>
#include <set>

const std::vector<int> a_vec = {1000,2000,3000,4000,5000};

typedef struct Row {
  int a;
  int b;
} Row;

// 比较函数
bool cmp(const Row* row1, const Row* row2) {
  if (row1->b <= row2->b) {
    return true;
  }
}

void PrintOneRow(Row* row) {
  std::cout << "a is:" << row->a << ", b is" << row->b << std::endl;
}

void DoTask(const std::vector<Row*>& rows, size_t start, size_t end, std::map<int, std::list<Row*>> *list_map) {
  int len = rows.size();
  // 二分查找剪枝
  if (start > end || len == 0 || rows[start]->a > 99000 || rows[end]->a < 1000) {
    return;
  }
  for (auto a: a_vec) {
    if (a >= start && a <= end) {
      list_map->insert({a, BinarySearch(rows, start, end, a)});
    }
  }
}

/** 
  * @brief  二分查找
  * @param rows 原始数组
  * @param start 起始位置
  * @param end 结束位置
  * @param target 查找的目标数
  */
std::list<Row*> BinarySearch(const std::vector<Row*>& rows, size_t start, size_t end, int target) {
  if (rows[start]->a > target || rows[end]->a < target) {
    return;
  }
  int left = start, right = end;
  while (left <= right) {
    int mid = (left + right) / 2;
    if (rows[mid]->a == target) {
      return FindAll(rows, start, end, mid);
    } else if (rows[mid]->a < target) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }
  return;
}

/** 
  * @brief  查找符合条件的所有数并返回一个按b升序排列的list
  * @param rows 原始数组
  * @param start 起始位置
  * @param end 结束位置
  * @param pos 目标位置的索引
  */
std::list<Row*> FindAll(const std::vector<Row*>& rows, size_t start, size_t end, size_t pos) {
  std::list<Row*> row_list;
  if (rows[pos]->b < 50 && rows[pos]->b >= 10) {
    // 这种情况两边都要考虑
    for (size_t i = pos; i >= start && rows[pos]->b >= 10; --i) {
      if (rows[pos]->b >= 10) {
        // 这里得用头插法
        row_list.push_front(rows[pos]);
      }
    }
    for (size_t j= pos + 1; j <= end; ++j) {
      if (rows[pos]->b < 50) {
        // 尾插法
        row_list.push_back(rows[pos]);
      }
    }
  } else if (rows[pos]->b < 50) {
    // 这种情况只打印左边
    for (size_t i = pos; i >= start; --i) {
      if (rows[pos]->b >= 10) {
        // 这里得用头插法
        row_list.push_front(rows[pos]);
      }
    }
  } else if (rows[pos]->b >= 10) {
    // 这中情况只打印右边
    for (size_t j= pos + 1; j <= end; ++j) {
      if (rows[pos]->b < 50) {
        // 尾插法
        row_list.push_back(rows[pos]);
      }
    }
  }
  return row_list;
}


void Task3(const std::vector<Row*>& rows) {
  size_t len = rows.size();
  if (len == 0) {
    return;
  }
  size_t thread_num = 10;
  size_t scope = len / thread_num;
  std::vector<std::map<int, std::list<Row*>>> vec(thread_num + 1);
  for (size_t i = 0; i < thread_num; ++i) {
    size_t start = i * scope;
    size_t end = (i + 1) * scope;
    std::thread t(DoTask, std::ref(rows), start, end, &vec[i]);
  }
  // 单独处理最后部分
  std::thread t(DoTask, std::ref(rows), thread_num * scope, len - 1, vec[thread_num]);
  MergeAll(&vec);
}

void MergeAll(std::vector<std::map<int, std::list<Row*>>> *list_map) {

}

int main() {
  // 开启多线程分别执行，因为数据已经按(a,b)进行排序，可以在每个线程中对符合条件的a进行二分查找
  // 同时对二分查找进行剪枝，找到a符合条件的行。由于task3要求结果按照b升序，不能直接查找到符合条件的行并输出
  // 因此选择在二分查找后对所有a相等的行先进行合并，然后再汇总合并
  return 0;
}