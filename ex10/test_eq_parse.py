import pytest
import pyparsing

from eq_parse import EqParse


def test_trivial():
    eq_parse = EqParse()

    assert eq_parse("a = 1") == ("a", 1)
    assert eq_parse("b = +1") == ("b", 1)
    assert eq_parse("c = -1.") == ("c", -1)
    assert eq_parse("d = -0") == ("d", 0)
    assert eq_parse("d = -0.") == ("d", 0)
    assert eq_parse("e = +0") == ("e", 0)


def test_from_descr():
    eq_parse = EqParse()

    assert eq_parse("a = 2") == ("a", 2)
    assert eq_parse("b = 3") == ("b", 3)
    assert eq_parse("c = 2 * 5") == ("c", 10)
    assert eq_parse("d = c - a") == ("d", 8)


def test_brackets():
    eq_parse = EqParse()

    assert eq_parse("c = 12*(2 + 3)") == ("c", 60)
    assert eq_parse("quarantine = (12 + c)*(12 - 2)") == ("quarantine", 720)
    assert eq_parse("f = (12 + c) + (12 - 2)") == ("f", 82)
    assert eq_parse("e = -((12 + 0 * 12) + 8) + c") == ("e", 40)
    assert eq_parse("e = -((12 + 0 * 12) + 8) / (e / c)") == ("e", -30)
    assert eq_parse("qq = -(-((-12)))") == ("qq", -12)


def test_float():
    eq_parse = EqParse()
    assert eq_parse("a = 1.111") == ("a", 1.111)
    assert eq_parse("a = -1.111") == ("a", -1.111)
    assert eq_parse("a = 0.000001") == ("a", 0.000001)
    assert eq_parse("q = 1 / 5") == ("q", 0.2)
    assert eq_parse("q = 1e-3 / 1e-5") == ("q", 100)
    assert eq_parse("c = 1e-5") == ("c", 1e-5)
    assert eq_parse("d = 1e-5 + 1e-5") == ("d", 2e-5)
    assert abs(eq_parse("d = 1e-5 * 1e-5")[1] - 1e-10) < 1e-20
    assert abs(eq_parse("d = -1e-5 * 1e-5")[1] + 1e-10) < 1e-20
    assert abs(eq_parse("d = -1e-5 * -1e-5")[1] - 1e-10) < 1e-20
    assert abs(eq_parse("d = -(1e-5) * (-1e-5)")[1] - 1e-10) < 1e-20


def test_var():
    eq_parse = EqParse()

    assert eq_parse("a = 1.5")[1] == 1.5
    assert eq_parse("b = -5")[1] == -5
    assert eq_parse("c = a*b")[1] == -7.5
    assert eq_parse("d = a + c")[1] == -6
    assert eq_parse("e = -3e-3*(-(a + b) - (c - d))")[1] == -0.015
    assert eq_parse("e = -3e3*(-(a + b) - (c - d))")[1] == -15000
    assert eq_parse("e = -3e3*(-(a + b) * (c - d))")[1] == 15750
    assert eq_parse("e = -3e3*(-(a + b) / (c - d))")[1] == 7000


def test_exceptions():
    with pytest.raises(ZeroDivisionError):
        eq_parse = EqParse()
        assert eq_parse("a = 1 / 0")

    with pytest.raises(KeyError):
        eq_parse = EqParse()
        assert eq_parse("b = a / 4")

    with pytest.raises(pyparsing.ParseSyntaxException):
        eq_parse = EqParse()
        assert eq_parse("b = 1(")

    with pytest.raises(pyparsing.ParseSyntaxException):
        eq_parse = EqParse()
        assert eq_parse("b = 1+")

    with pytest.raises(pyparsing.ParseSyntaxException):
        eq_parse = EqParse()
        assert eq_parse("2020_asd = (12 -7.5)*(12 - 2)") == ("2020_asd", 45)
        assert eq_parse("quarantine2020 = (12 -7.5)*(12 - 2)") == ("quarantine2020", 45)
        assert eq_parse("___quarantine___ = (12 -7.5)*(12 - 2)") == ("___quarantine___", 45)
        assert eq_parse("_quarantine_2020 = (12 -7.5)*(12 - 2)") == ("_quarantine_2020", 45)

