#pragma once
#ifndef CALCULATION_TREE_HH
#define CALCULATION_TREE_HH

#include <functional>
#include <memory>
#include <string>

namespace calculation {


/*
 * Operands are something that operators work with. Operand can be
 * evaluated to get it's value.
 */
class Operand {
public:
    virtual ~Operand() = default;

    virtual double evaluate() const = 0;

    virtual std::string str() const = 0;
};


/*
 * Operator takes its operands and passes them to an underlying math function.
 */
class Operator {
public:
    virtual ~Operator() = default;

    virtual double calculate() const = 0;

    virtual std::string str() const = 0;
    virtual std::string repr() const = 0;
};


/*
 * Constant is an operand, which value is known at parsetime. Constant's
 * value cannot be changed.
 */
class Constant : public Operand {
public:
    Constant() = delete;
    Constant(double value) : value_(value) {}
    Constant(double value, const std::string &name)
        : name_(name), value_(value)
    {}
    Constant(const Constant &other)
        : name_(other.name_), value_(other.value_)
    {}
    Constant(Constant &&other) : name_(other.name_), value_(other.value_) {}

    double evaluate() const { return value_; }

    std::string str() const;
private:
    std::string name_;
    double value_;
};


/*
 * Expression is an operand that needs to calculate a few operators
 * itself, before it can tell its value.
 */
class Expression : public Operand {
public:
    Expression() = default;
    /*
     * Sets a root operator for the expression calculation tree.
     */
    void set_root(const std::shared_ptr<Operator> &op) { root_ = op; }
    std::shared_ptr<Operator> get_root() { return root_; }

    double evaluate() const;

    std::string str() const { return root_->str(); }
private:
    std::shared_ptr<Operator> root_;
};


class UnaryOperator : public Operator {
public:

    UnaryOperator() = delete;
    // UnaryOperator(double (*f)(double)) : operator_(f) {}
    UnaryOperator(const std::string &str, std::function<double(double)> f)
        : operator_(f), str_(str)
    {}
    UnaryOperator(const UnaryOperator &other)
        : operator_(other.operator_), str_(other.str_)
    {}
    UnaryOperator(UnaryOperator &&other)
        : operator_(other.operator_), str_(other.str_)
    {
        operand_.swap(other.operand_);
    }

    void set_operand(std::shared_ptr<Operand> op) { operand_ = op; }
    std::shared_ptr<Operand> get_operand() { return operand_; }

    double calculate() const;

    std::string repr() const { return str_; }
    std::string str() const { return str_ + " " + operand_->str(); }
private:
    std::function<double(double)> operator_;
    std::string str_;
    std::shared_ptr<Operand> operand_;
};


class BinaryOperator : public Operator {
public:
    BinaryOperator() = delete;
    BinaryOperator(const std::string &str, std::function<double(double, double)> f, unsigned order)
        : operator_(f), str_(str), order_(order)
    {}
    BinaryOperator(const BinaryOperator &other)
        : operator_(other.operator_), str_(other.str_), order_(other.order_)
    {}
    BinaryOperator(BinaryOperator &&other)
        : operator_(other.operator_), str_(other.str_), order_(other.order_)
    {
        left_.swap(other.left_);
        right_.swap(other.right_);
    }

    void set_left(std::shared_ptr<Operand> op) { left_ = op; }
    std::shared_ptr<Operand> get_left() { return left_; }

    void set_right(std::shared_ptr<Operand> op) { right_ = op; }
    std::shared_ptr<Operand> get_right() { return right_; }

    unsigned order() const { return order_; }

    std::string repr() const { return str_; }
    std::string str() const { return str_ + " " + left_->str() + " " + right_->str(); }

    double calculate() const;
private:
    std::function<double(double, double)> operator_;
    std::string str_;

    std::shared_ptr<Operand> left_;
    std::shared_ptr<Operand> right_;

    unsigned order_;
};


}   // namespace calculation

#endif  // CALCULATION_TREE_HH
