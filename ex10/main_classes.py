import operator
import pyparsing as ps


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
        greet = ps.Word(ps.alphas) + ps.Suppress("=") + ps.Word(ps.nums + ps.alphas + " +-*/().e")
        greeting = greet.parseString(input_str)
        return greeting[0], greeting[1]

    def bnf(self):
        """
        Returns
        -------

        """
        add = ps.Literal("+")
        sub = ps.Literal("-")

        mul = ps.Literal("*")
        div = ps.Literal("/")

        lpar = ps.Literal("(")
        rpar = ps.Literal(")")

        p_m = ps.Suppress("+") | ps.Literal("-")

        int_num = ps.Word(ps.nums)
        float_num = ps.Combine(ps.Optional(p_m) + int_num + ps.Optional('.' + int_num) +
                               ps.Optional('e' + ps.Optional(p_m) + int_num))

        str_num = ps.Combine(ps.Optional(p_m) + ps.Word(ps.alphas))
        expr = ps.Forward()
        parse_expr = (ps.ZeroOrMore(p_m) + (str_num.setParseAction(lambda x: self.__expr_list.append(x[0])) |
                                            float_num.setParseAction(lambda x: self.__expr_list.append(x[0])) |
                                            (lpar + expr + rpar))).setParseAction(self.__parse_minus)

        sum_expr = parse_expr + ps.ZeroOrMore(((mul | div) + parse_expr)
                                              .setParseAction(lambda x: self.__expr_list.append(x[0])))

        expr << sum_expr + ps.ZeroOrMore(((add | sub) + sum_expr)
                                         .setParseAction(lambda x: self.__expr_list.append(x[0])))
        return expr

    def evaluate_stack(self, res_list):  # оформить статическим методом
        operation = res_list.pop()
        if operation == "minus":
            return -self.evaluate_stack(res_list)
        if operation in "+-*/":
            operation2 = self.evaluate_stack(res_list)
            operation1 = self.evaluate_stack(res_list)
            return self.__var_dict[operation](operation1, operation2)
        if operation[0].isalpha():
            try:
                return self.__var_dict[operation]
            except KeyError:
                raise KeyError(f"Variable {operation} is not define !!!")
        else:
            return float(operation)

    def __call__(self, input_str: str, correct_val):
        """
        Parameters
        ----------
        input_str

        Returns
        -------

        """
        var, expr = self.__eq_split(input_str)
        self.__expr_list = []
        result = self.bnf()
        result.parseString(expr, parseAll=True)
        val = self.evaluate_stack(self.__expr_list)
        self.__var_dict[var] = val
        print(expr, "=", val, correct_val == val)


if __name__ == "__main__":  # поменять на классические юнитесты !!!

    eq_parse = EqParse()
    eq_parse("a = ++9", 9)
    eq_parse("b = a + 12", 21)
    eq_parse("a = +-9", -9)
    eq_parse("a = ----9", 9)
    eq_parse("d = a * b", 9*21)
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
