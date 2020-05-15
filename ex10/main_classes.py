import operator

from pyparsing import Literal, Word, Group, ZeroOrMore, Forward, nums, alphas, Regex, ParseException


def bnf():
    expr_stack = []

    def push_uminus(strg, loc, toks):
        for t in toks:
            if t == '-':
                expr_stack.append('unary -')
            else:
                break

    fnumber = Regex(r"[+-]?\d+(:?\.\d*)?(:?[eE][+-]?\d+)?")
    ident = Word(alphas, alphas + nums + "_$")  # можно проще ?

    plus = Literal("+")
    minus = Literal("-")

    mult = Literal("*")
    div = Literal("/")

    lpar = Literal("(")
    rpar = Literal(")")

    expr = Forward()
    atom = ((0, None) * minus + (ident | fnumber | ident + lpar + expr + rpar).
            setParseAction(lambda x: expr_stack.append(x[0])) |
            Group(lpar + expr + rpar)).setParseAction(push_uminus)  # шаг парсинга в рекурсии

    term = atom + ZeroOrMore(((mult | div) + atom).setParseAction(lambda x: expr_stack.append(x[0])))
    expr << term + ZeroOrMore(((plus | minus) + term).setParseAction(lambda x: expr_stack.append(x[0])))

    return expr, expr_stack


opn = {"+": operator.add,
       "-": operator.sub,
       "*": operator.mul,
       "/": operator.truediv
       }


def evaluate_stack(s):  # оформить статическим методом
    op = s.pop()
    if op == 'unary -':
        return -evaluate_stack(s)
    if op in "+-*/":
        op2 = evaluate_stack(s)
        op1 = evaluate_stack(s)
        return opn[op](op1, op2)
    elif op[0].isalpha():
        raise Exception("invalid identifier '%s'" % op)
    else:
        return float(op)


if __name__ == "__main__":  # поменять на классические юнитесты !!!

    def test(s, expVal):
        try:
            results, expr_stack = bnf()
            results = results.parseString(s, parseAll=True)
            # print("exprStack", exprStack)
            # print("results", results)
            val = evaluate_stack(expr_stack[:])
        except ParseException as e:
            print(s, "failed parse:", str(e))
        except Exception as e:
            print(s, "failed eval:", str(e))
        else:
            if val == expVal:
                print(s, "=", val, results, "=>", expr_stack)
            else:
                print(s + "!!!", val, "!=", expVal, results, "=>", expr_stack)


    test("9", 9)
    test("-9", -9)
    test("--9", 9)
    test("9 + 3 + 6", 9 + 3 + 6)
    test("(9 + 3) / 11", (9 + 3.0) / 11)
    test("(9+3) / 11", (9 + 3.0) / 11)
    test("(9 + 3)", (9 + 3))
    test("9 - 12 - 6", 9 - 12 - 6)
    test("9 - (12 - 6)", 9 - (12 - 6))
    test("2*3.14159", 2 * 3.14159)
    test("9 * (12 - 6)", 9 * (12 - 6))
