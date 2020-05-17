"""
Module with class for mathematics expressions parsing
"""
import sys
import typing
import operator
import pyparsing as ps


class EqParse:
    """
    Class for parsing and calculating math expression
    """

    def __init__(self):
        self.__var_dict = {"+": operator.add,
                           "-": operator.sub,
                           "*": operator.mul,
                           "/": operator.truediv}
        self.__parser = self.__parse_form()
        self.__expr_list = None

    @staticmethod
    def __eq_split(input_str: str) -> typing.Tuple[str, str]:
        """
        Parse initial expression by "="
        Parameters
        ----------
        input_str

        Returns
        -------
        output is variable name and expression
        """
        greet = ps.Word(ps.alphas) + ps.Suppress("=") + ps.Word(ps.alphanums + " +-*/().e")
        try:
            greeting = greet.parseString(input_str)
        except ps.ParseException:
            raise ps.ParseSyntaxException(f"The name of variable is not correct in {input_str}")
        return greeting[0], greeting[1]

    def __parse_minus(self, x: str):
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

    def __parse_form(self):
        """
        Define form for paring
        Returns
        -------

        """
        add = ps.Literal("+")
        sub = ps.Literal("-")

        mul = ps.Literal("*")
        div = ps.Literal("/")

        lbr = ps.Literal("(")
        rbr = ps.Literal(")")

        p_m = ps.Suppress("+") | ps.Literal("-")

        int_num = ps.Word(ps.nums)
        float_num = ps.Combine(ps.Optional(p_m) + int_num + ps.Optional('.' + ps.Optional(int_num)) +
                               ps.Optional('e' + ps.Optional(p_m) + int_num))

        str_num = ps.Combine(ps.Optional(p_m) + ps.Word(ps.alphas))
        parser = ps.Forward()
        term = (ps.ZeroOrMore(p_m) + (str_num.setParseAction(lambda x: self.__expr_list.append(x[0])) |
                                      float_num.setParseAction(lambda x: self.__expr_list.append(x[0])) |
                                      (lbr + parser + rbr))).setParseAction(self.__parse_minus)

        mul_expr = term + ps.ZeroOrMore(((mul | div) + term)
                                        .setParseAction(lambda x: self.__expr_list.append(x[0])))

        parser << mul_expr + ps.ZeroOrMore(((add | sub) + mul_expr)
                                           .setParseAction(lambda x: self.__expr_list.append(x[0])))
        return parser

    def get_val(self, res_list: typing.List[str]):
        """
        Computation method
        Parameters
        ----------
        res_list: buffer with symbols for calculating

        Returns
        -------
        answer
        """
        operation = res_list.pop()
        if operation == "minus":
            return -self.get_val(res_list)
        if operation in "+-*/":
            operation2 = self.get_val(res_list)
            operation1 = self.get_val(res_list)
            return self.__var_dict[operation](operation1, operation2)
        if operation[0].isalpha():
            try:
                return self.__var_dict[operation]
            except KeyError:
                raise KeyError(f"Variable {operation} is not define !")
        else:
            return float(operation)

    def __call__(self, input_str: str) -> typing.Tuple[str, float]:
        """
        Main method for expression paring
        Parameters
        ----------
        input_str

        Returns
        -------

        """
        var_, expr = self.__eq_split(input_str)
        self.__expr_list = []
        try:
            self.__parser.parseString(expr, parseAll=True)
        except ps.ParseException:
            raise ps.ParseSyntaxException(f"Expression '{input_str}' is not correct !")
        val_ = self.get_val(self.__expr_list)
        self.__var_dict[var_] = val_
        return var_, val_


if __name__ == "__main__":
    eq_parse = EqParse()
    while True:
        input_ = sys.stdin.readline()
        if "=" not in input_:
            break
        var, val = eq_parse(input_)
        print(f"{var} = {val}")
