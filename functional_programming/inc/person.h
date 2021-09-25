//
// Created by andrew on 2021/9/20.
//

#ifndef OPTIMIZED_CPLUSPLUS_PERSON_H
#define OPTIMIZED_CPLUSPLUS_PERSON_H
#include <iostream>
#include <string>

class Person {
public:
    enum Gender {
        female,
        male,
        other,
    };

    enum OutputFormat {
        nameOnly,
        fullName,
    };

    Person() : m_name("John"), m_surname("Doe"), m_gender(other) {

    }

    Person(std::string name, Gender gender, int age = 0)
        : m_name(name),
        m_surname("Doe"),
        m_gender(gender),
        m_age(age) {

    }

    std::string Name() const {
        return m_name;
    }

    std::string Surname() const {
        return m_surname;
    }

    Gender Gender() const {
        return m_gender;
    }

    int Age() const {
        return m_age;
    }

    void  Print(std::ostream&out, Person::OutputFormat format) const {
        if (format == Person::nameOnly) {
            out << Name() << '\n';
        } else if (format == Person::fullName) {
            out << Name() << ' ' << Surname() << '\n';
        }
    }



private:
    std::string m_name;
    std::string m_surname;
    enum Gender m_gender;
    int m_age;
};


#endif //OPTIMIZED_CPLUSPLUS_PERSON_H
