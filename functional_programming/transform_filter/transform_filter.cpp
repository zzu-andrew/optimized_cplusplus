//
// Created by andrew on 2021/9/20.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>    // 数值计算库
#include <string>
#include "person.h"

using namespace std;

/*
 * 使用transform实现过滤
 * */
// functional programming
std::string Name(const Person &person) {
    return person.Name();
}

bool IsFemale(const Person &person) {
    return person.Gender() == Person::female;
}

bool IsNotFemale(const Person &person) {
    return !IsFemale(person);
}


/*
 * filter
 * */
int main(int argc, char* argv[]) {
    std::vector<Person> people {
            {"David", Person::male},
            {"Jane", Person::female},
            {"Martha", Person::female},
            {"Peter", Person::male},
            {"Rose", Person::female},
            {"Tom", Person::male}
    };
    // 1. filter
    // female container
    std::vector<Person> females;
    // 使用条件函数copy_if() 当满足条件的时候执行动作
    // std::back_inserter(females) 返回一个back inserter 的一个迭代器
    // 使用std::back_inserter(females) 是因为要实现一个追加的效果
    std::copy_if(people.cbegin(), people.cend(), std::back_inserter(females), IsFemale);

    // 2. transform
    // transform get name
    std::vector<std::string> names(females.size());
    // 提取person name属性
    std::transform(females.cbegin(), females.cend(), names.begin() /*std::back_inserter(names)*/, Name);

    for (const auto & name : names) {
        std::cout << "name : " << name << std::endl;
    }

    return 0;
}
