import operator

from pyparsing import Literal, CaselessLiteral, Word, Group, Optional, \
    ZeroOrMore, Forward, nums, alphas, Regex, ParseException

exprStack = []


def push_first(strg, loc, toks):
    exprStack.append(toks[0])


def push_uminus(strg, loc, toks):
    for t in toks:
        if t == '-':
            exprStack.append('unary -')
            # ~ exprStack.append( '-1' )
            # ~ exprStack.append( '*' )
        else:
            break


bnf_ = None


def bnf():
    """
    expop   :: '^'
    multop  :: '*' | '/'
    addop   :: '+' | '-'
    integer :: ['+' | '-'] '0'..'9'+
    atom    :: PI | E | real | fn '(' expr ')' | '(' expr ')'
    factor  :: atom [ expop factor ]*
    term    :: factor [ multop factor ]*
    expr    :: term [ addop term ]*
    """
    global bnf_
    if not bnf_:
        point = Literal(".")
        e = CaselessLiteral("E")
        # ~ fnumber = Combine( Word( "+-"+nums, nums ) +
        # ~ Optional( point + Optional( Word( nums ) ) ) +
        # ~ Optional( e + Word( "+-"+nums, nums ) ) )
        fnumber = Regex(r"[+-]?\d+(:?\.\d*)?(:?[eE][+-]?\d+)?")
        ident = Word(alphas, alphas + nums + "_$")

        plus = Literal("+")
        minus = Literal("-")
        mult = Literal("*")
        div = Literal("/")
        lpar = Literal("(").suppress()
        rpar = Literal(")").suppress()
        addop = plus | minus
        multop = mult | div
        expop = Literal("^")
        pi = CaselessLiteral("PI")

        expr = Forward()
        atom = ((0, None) * minus + (pi | e | fnumber | ident + lpar + expr + rpar | ident).setParseAction(push_first) |
                Group(lpar + expr + rpar)).setParseAction(push_uminus)

        # by defining exponentiation as "atom [ ^ factor ]..." instead of "atom [ ^ atom ]...", we get right-to-left exponents, instead of left-to-righ
        # that is, 2^3^2 = 2^(3^2), not (2^3)^2.
        factor = Forward()
        factor << atom + ZeroOrMore((expop + factor).setParseAction(push_first))

        term = factor + ZeroOrMore((multop + factor).setParseAction(push_first))
        expr << term + ZeroOrMore((addop + term).setParseAction(push_first))
        bnf_ = expr
    return bnf_


# map operator symbols to corresponding arithmetic operations
epsilon = 1e-12
opn = {"+": operator.add,
       "-": operator.sub,
       "*": operator.mul,
       "/": operator.truediv
       }


def evaluate_stack(s):
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


if __name__ == "__main__":

    def test(s, expVal):
        global exprStack
        exprStack = []
        try:
            results = bnf().parseString(s, parseAll=True)
            val = evaluate_stack(exprStack[:])
        except ParseException as e:
            print(s, "failed parse:", str(e))
        except Exception as e:
            print(s, "failed eval:", str(e))
        else:
            if val == expVal:
                print(s, "=", val, results, "=>", exprStack)
            else:
                print(s + "!!!", val, "!=", expVal, results, "=>", exprStack)


    test("9", 9)
    test("-9", -9)
    test("--9", 9)
    test("9 + 3 + 6", 9 + 3 + 6)
    test("9 + 3 / 11", 9 + 3.0 / 11)
    test("(9 + 3)", (9 + 3))
    test("(9+3) / 11", (9 + 3.0) / 11)
    test("9 - 12 - 6", 9 - 12 - 6)
    test("9 - (12 - 6)", 9 - (12 - 6))
    test("2*3.14159", 2 * 3.14159)
    test("9 * (12 - 6)", 9 * (12 - 6))
