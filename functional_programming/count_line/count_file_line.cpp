//
// Created by andrew on 2021/9/20.
//
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>


using namespace std;

/*
 * 实现对单个文件的函数统计
 * */

int CountLines(const std::string &filename) {

    cout << filename << endl;
    std::ifstream in(filename);
//    // 跳过空白区域
//    in.unsetf(std::ios_base::skipws);

    // std::istreambuf_iterator<char>() 文件结尾
    // '\n'
    auto result = std::count(std::istreambuf_iterator<char>(in >> std::noskipws),
            std::istreambuf_iterator<char>(),
                    '\n');
    cout << "count lines : " << result << endl;
    return result;
}

/**
 * @brief 统计给出文件列表的所有行数
 * @param files 给出的文件列表
 * @return 所有行数
 */
std::vector<int> CountLinesInFiles(const std::vector<std::string>& files) {
    // 定义统计结果 container
    std::vector<int> results(files.size());
    // C++11 之后才支持的
    //  reserve 提前分配内存
//    results.reserve(files.size());
    //
    // v1
//        for (const auto &file : files) {
//            results.push_back(CountLines(file));
//        }
    // v2
    std::transform(files.cbegin(), files.cend(), results.begin(), CountLines);

    return results;
}


int main(int argc, char *argv[]) {

    if (argc < 3) {
        cout << "argc : " << argc << endl;
        return -1;
    }
    // argv[0] 可执行程序的文件名
    auto  results = CountLinesInFiles({argv[1], argv[2]});

    for (auto &result : results) {
        cout << "result : " << result << " line(s)" << endl;
    }

    return 0;
}
