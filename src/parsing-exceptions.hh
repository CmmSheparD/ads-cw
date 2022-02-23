#pragma once
#ifndef PARSING_EXCEPTIONS_HH
#define PARSING_EXCEPTIONS_HH

#include <stdexcept>

namespace infix_parsing {

class ParserError : public std::runtime_error {
public:
    ParserError(const std::string about, size_t pos)
        : runtime_error(about), position(pos)
    {}

    size_t position;
};

class SyntaxError : public ParserError {
public:
    SyntaxError(const std::string about, size_t pos)
        : ParserError(about, pos)
    {}
};

class UnexpectedEndOfExpression : public SyntaxError {
public:
    UnexpectedEndOfExpression(size_t pos)
        : SyntaxError("Unexpected end of string expression.", pos)
    {}
    UnexpectedEndOfExpression(const std::string about, size_t pos)
        : SyntaxError(about, pos)
    {}
};

class UnsatisfiedExpectation : public SyntaxError {
public:
    UnsatisfiedExpectation(const std::string about, size_t pos)
        : SyntaxError(about, pos)
    {}
};

class OperandExpectationUnsatisfied : public UnsatisfiedExpectation {
public:
    OperandExpectationUnsatisfied(size_t pos)
        : UnsatisfiedExpectation("Operand was expected, but couldn't be found.", pos)
    {}
};

class BinaryExpectationUnsatisfied : public UnsatisfiedExpectation {
public:
    BinaryExpectationUnsatisfied(size_t pos)
        : UnsatisfiedExpectation("Binary operator was expected, but couldn't be found.", pos)
    {}
};

class TooBigNumber : public ParserError {
public:
    TooBigNumber(size_t pos)
        : ParserError("Given constant is too big to be stored.", pos)
    {}
};

}   // namespace infix_parsing

#endif  // PARSING_EXCEPTIONS_HH
