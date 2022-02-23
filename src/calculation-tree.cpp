#include "calculation-tree.hh"

#include <stdexcept>
#include <string>

namespace calculation {

double Expression::evaluate() const
{
    if (!root_)
        throw std::logic_error("Evaluating empty expression.");
    return root_->calculate();
}


std::string Constant::str() const
{
    if (name_.empty())
        return std::to_string(value_);
    return name_;
}


double UnaryOperator::calculate() const
{
    if (!operator_)
        throw std::logic_error("Calculation of non-bind operator.");
    else if (!operand_)
        throw std::logic_error("Calculation of operator with no operand.");
    return operator_(operand_->evaluate());
}


double BinaryOperator::calculate() const
{
    if (!operator_)
        throw std::logic_error("Calculation of non-bind operator.");
    else if (!left_)
        throw std::logic_error("Calculation of operator with no left operand.");
    else if (!right_)
        throw std::logic_error("Calculation of operator with no right operand.");
    return operator_(left_->evaluate(), right_->evaluate());
}

}   // namespace calculation
