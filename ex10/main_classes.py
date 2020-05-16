import operator

from pyparsing import Literal, ZeroOrMore, Forward, Optional, Suppress, Word, Combine, nums, alphas


class EqParse:
    def __init__(self):
        self.__var_dict = {"+": operator.add,
                           "-": operator.sub,
                           "*": operator.mul,
                           "/": operator.truediv}
        self.__expr_list = None

    def __parse_minus(self, x):
        """
        Parameters
        ----------
        x

        Returns
        -------

        """
        i = 0
        while (i != len(x)) & (x[i] == "-"):
            self.__expr_list.append("minus")
            i += 1

    def bnf(self):
        """
        Returns
        -------

        """
        add = Literal("+")
        sub = Literal("-")

        mul = Literal("*")
        div = Literal("/")

        lpar = Literal("(")
        rpar = Literal(")")

        p_m = Suppress("+") | Literal("-")
        int_num = Word(nums)

        float_num = Combine(Optional(p_m) + int_num + Optional('.' + int_num) + Optional('e' + Optional(p_m) + int_num))

        expr = Forward()
        parse_expr = (ZeroOrMore(p_m) + (float_num.setParseAction(lambda x: self.__expr_list.append(x[0])) |
                                         (lpar + expr + rpar))).setParseAction(self.__parse_minus)

        sum_expr = parse_expr + ZeroOrMore(((mul | div) + parse_expr)
                                           .setParseAction(lambda x: self.__expr_list.append(x[0])))

        expr << sum_expr + ZeroOrMore(((add | sub) + sum_expr)
                                      .setParseAction(lambda x: self.__expr_list.append(x[0])))
        return expr

    def evaluate_stack(self, res_list):  # оформить статическим методом
        op = res_list.pop()
        if op == "minus":
            return -self.evaluate_stack(res_list)
        if op in "+-*/":
            op2 = self.evaluate_stack(res_list)
            op1 = self.evaluate_stack(res_list)
            return self.__var_dict[op](op1, op2)
        elif op[0].isalpha():
            raise Exception("invalid identifier '%s'" % op)
        else:
            return float(op)

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
        greet = Word(alphas) + Suppress("=") + Word(nums + alphas + " +-*/().e")
        greeting = greet.parseString(input_str)
        return greeting[0], greeting[1]

    def __call__(self, input_str: str, correct_val):
        """
        Parameters
        ----------
        input_str

        Returns
        -------

        """
        var, expr = self.__eq_split(input_str)
        self.__var_dict[var] = None
        self.__expr_list = []

        result = self.bnf()
        result.parseString(expr, parseAll=True)
        val = self.evaluate_stack(self.__expr_list.copy())
        print(expr, "=", val, correct_val == val)


if __name__ == "__main__":  # поменять на классические юнитесты !!!

    eq_parse = EqParse()

    eq_parse("a = ++9", 9)
    eq_parse("a = +-9", -9)
    eq_parse("a = ----9", 9)
    eq_parse("a = 9 + 3 + 6", 9 + 3 + 6)
    eq_parse("a = (9 + 3) / 11", (9 + 3.0) / 11)
    eq_parse("a = (9+3) / 11", (9 + 3.0) / 11)
    eq_parse("a = ((9 + 3))", (9 + 3))
    eq_parse("a = -(9 + 3)", -(9 + 3))
    eq_parse("a = 9 - 12 - 6", 9 - 12 - 6)
    eq_parse("a = 9 - (12 - 6)", 9 - (12 - 6))
    eq_parse("a = 2*3.14159", 2 * 3.14159)
    eq_parse("a = 9 * (12 - 6)", 9 * (12 - 6))
    eq_parse("a = -((1 * 2) + (12/6))", -4)
    eq_parse("a = -((1 * 2) - (12/6))", 0)
    eq_parse("a = -((1 * 2) - (12/6))", 0)
    eq_parse("a = -((1 + 2) * 12/6)", -6)



















