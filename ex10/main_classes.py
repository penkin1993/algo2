import operator

from pyparsing import Literal, Group, ZeroOrMore, Forward, nums, ParseException, Optional, Suppress, Word


def bnf():
    expr_stack = []

    def push_uminus(strg, loc, toks):
        for t in toks:
            if t == '-':
                expr_stack.append('unary -')
            else:
                break

    p_m = Suppress("+") | Literal("-")
    int_num = Word(nums)
    float_num = Optional(p_m) + int_num + Optional('.' + int_num) + Optional('e' + Optional(p_m) + int_num)

    add = Literal("+")
    sub = Literal("-")

    mul = Literal("*")
    div = Literal("/")

    lpar = Literal("(")
    rpar = Literal(")")

    expr = Forward()

    atom = (ZeroOrMore(p_m) + ((float_num | nums + lpar + expr + rpar).setParseAction(lambda x: expr_stack.append(x[0])) | Group(lpar + expr + rpar)))\
        .setParseAction(push_uminus)

    term = atom + ZeroOrMore(((mul | div) + atom).setParseAction(lambda x: expr_stack.append(x[0])))
    expr << term + ZeroOrMore(((add | sub) + term).setParseAction(lambda x: expr_stack.append(x[0])))

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

    def test(s, exp_val):
        try:
            results, expr_stack = bnf()
            # print("expr_stack", expr_stack)
            # print("results", results)
            # print(expr_stack == [])
            # expr_stack = []
            results = results.parseString(s, parseAll=True)
            val = evaluate_stack(expr_stack[:])
        except ParseException as e:
            print(s, "failed parse:", str(e))
        except Exception as e:
            print(s, "failed eval:", str(e))
        else:
            if val == exp_val:
                print(s, "=", val, results, "=>", expr_stack)
            else:
                print(s + "!!!", val, "!=", exp_val, results, "=>", expr_stack)


    test("++9", 9)
    test("+-9", -9)
    test("----9", 9)
    test("9 + 3 + 6", 9 + 3 + 6)
    test("(9 + 3) / 11", (9 + 3.0) / 11)
    test("(9+3) / 11", (9 + 3.0) / 11)
    test("((9 + 3))", (9 + 3))
    test("-(9 + 3)", -(9 + 3))
    test("9 - 12 - 6", 9 - 12 - 6)
    test("9 - (12 - 6)", 9 - (12 - 6))
    test("2*3.14159", 2 * 3.14159)
    test("9 * (12 - 6)", 9 * (12 - 6))


from pyparsing import Word, Suppress, alphas, nums


class EqParse:
    def __init__(self):
        self.__var_dict = dict()
        self.__expr, self.__expr_stack = self.bnf()

    @staticmethod
    def __eq_split(input_str: str):
        """
        Parse initial expression.
        Parameters
        ----------
        input_str

        Returns
        -------
        output is variable name and expression
        """
        greet = Word(alphas) + Suppress("=") + Word(nums + alphas + "+-*/()")
        greeting = greet.parseString(input_str)
        return greeting[0], greeting[1]

    def __call__(self, input_str: str):
        """
        Parameters
        ----------
        input_str

        Returns
        -------

        """
        var, expr = self.__eq_split(input_str)
        self.__var_dict[var] = None













