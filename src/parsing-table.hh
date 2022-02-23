#pragma once
#ifndef PARSING_TABLE_HH
#define PARSING_TABLE_HH

#include <cctype>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>

#include "calculation-tree.hh"
#include "list.hh"

namespace infix_parsing {

/*
 * A static table that holds entries on how to decode text into math
 */
class ParsingTable {
public:
    class InvalidNameError;
    class NameSearchError;

    ParsingTable() = delete;
    ParsingTable(const ParsingTable &) = delete;
    ParsingTable(ParsingTable &&) = delete;

    static bool is_valid_name(const std::string &name);
    static bool is_starting_digit(char c) { return std::isdigit(c); }
    static bool is_digit(char c) { return std::isdigit(c) || c == '.'; }

    static void register_constant(const std::string &name, const double value);
    static void register_unary(const std::string &name, std::function<double (double)> f);
    static void register_binary(const std::string &name, std::function<double (double, double)> f, unsigned order);

    static bool is_constant(const std::string &name);
    static bool is_unary_operator(const std::string &name);
    static bool is_binary_operator(const std::string &name);

    static std::shared_ptr<calculation::Constant> get_constant(const std::string &name);
    static std::shared_ptr<calculation::UnaryOperator> get_unary_operator(const std::string &name);
    static std::shared_ptr<calculation::BinaryOperator> get_binary_operator(const std::string &name);
private:
    struct ConstantEntry;
    struct UnaryOperatorEntry;
    struct BinaryOperatorEntry;

    ~ParsingTable() = default;

    static data_structs::List<ConstantEntry> constants_;
    static data_structs::List<UnaryOperatorEntry> unary_operators_;
    static data_structs::List<BinaryOperatorEntry> binary_operators_;
};


class ParsingTable::InvalidNameError : public std::invalid_argument {
public:
    InvalidNameError(const std::string &name)
        : invalid_argument("Invalid name \"" + name + "\".")
    {}
};


class ParsingTable::NameSearchError : public std::invalid_argument {
public:
    NameSearchError(const std::string &name)
        : invalid_argument("No such name found \"" + name + "\".")
    {}
};


struct ParsingTable::ConstantEntry {
    ConstantEntry() = delete;
    ConstantEntry(const std::string &name, const double value)
        : data(value, name)
    {}

    const calculation::Constant data;
};


struct ParsingTable::UnaryOperatorEntry {
    UnaryOperatorEntry() = delete;
    UnaryOperatorEntry(const std::string &name, std::function<double(double)> f)
        : data(name, f)
    {}

    const calculation::UnaryOperator data;
};


struct ParsingTable::BinaryOperatorEntry {
    BinaryOperatorEntry() = delete;
    BinaryOperatorEntry(const std::string &name, std::function<double(double, double)> f, unsigned order)
        : data(name, f, order)
    {}

    const calculation::BinaryOperator data;
};

}   // namespace infix_parsing

#endif  // PARSING_TABLE_HH
