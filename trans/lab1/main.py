
def parse_number(expr: str, pos: int):
    res = ""
    last_index = pos
    for i in range(pos, len(expr)):
        ch = expr[i]
        if ch.isdigit() or ch == '.':
            res += ch
        else:
            last_index = i - 1
            break
    return res, last_index


def is_letter(ch):
    return int('a') <= int(ch) <= int('z') or int('A') <= int(ch) <= int('Z')


def operate(operator, lhs, rhs):
    if not lhs and not rhs:
        raise TypeError("some argument is none")
    if operator == '+':
        return lhs + rhs
    elif operator == '-':
        return lhs - rhs
    elif operator == '*':
        return lhs * rhs
    elif operator == '/':
        return lhs / rhs
    elif operator == '^':
        return lhs ** rhs


class Formula:
    def __init__(self, infix=""):
        self.infix = infix
        self.priority = {'(': 0, ')': 0, '+': 1, '-': 1, '*': 2, '/': 2, '^': 3, '~': 4}
        self.postfix = self.to_postfix(infix + "\r")

    def to_postfix(self, expr: str):
        res = ""
        stack = []
        i = 0
        is_digit_str = False
        go_into_pars = False
        while i < len(expr):
            ch = expr[i]
            if ch.isdigit():
                tmp, i = parse_number(expr, i)
                res += tmp + ' '
                if is_digit_str:
                    go_into_pars = True
                is_digit_str = True
            elif ch.isalpha():
                res += ch + ' '
                if is_digit_str:
                    go_into_pars = True
                is_digit_str = True
            elif ch == '(':
                if is_digit_str:
                    go_into_pars = True
                is_digit_str = False
                stack.append(ch)
            elif ch == ')':
                is_digit_str = False
                while len(stack) > 0 and stack[len(stack) - 1] != '(':
                    res += stack.pop() + ' '
                if len(stack) > 0:
                    stack.pop()
            elif ch in self.priority:
                is_digit_str = False
                if ch == '-' and (i == 0 or (i > 1 and expr[i - 1] in self.priority)):
                    ch = '~'
                go_into_pars = True
            if go_into_pars:
                if is_digit_str:
                    ch = '*'
                while len(stack) > 0 and (self.priority[stack[len(stack) - 1]] >= self.priority[ch]):
                    res += stack.pop() + ' '
                stack.append(ch)
                go_into_pars = False
            i += 1
        while len(stack) > 0:
            res += stack.pop() + ' '
        return res

    def count(self):
        stack = []
        i = 0
        while i < len(self.postfix):
            ch = self.postfix[i]
            if ch.isdigit():
                tmp, i = parse_number(self.postfix, i)
                stack.append(float(tmp))
            elif ch.isalpha():
                tmp = input(ch + ': ')
                stack.append(float(tmp))
            elif ch in self.priority:
                if ch == '~':
                    last = 0.0
                    if len(stack) > 0:
                        last = stack.pop()
                    stack.append(operate('-', 0, float(last))[0])
                    # print(ch, last, '=', stack[len(stack) - 1])
                    i += 1
                    continue

                second = None
                if len(stack) > 0:
                    second = stack.pop()
                first = None
                if len(stack) > 0:
                    first = stack.pop()
                tmp = operate(ch, first, second)
                stack.append(tmp)
                # print(first, ch, second, ' = ', stack[len(stack) - 1])
            i += 1
        return stack.pop()


string = input()
f = Formula(string)
try:
    print('Postfix: ', f.postfix)
    print('Result:  ', f.count())
except BaseException as mess:
    print("Wrong expression: ", mess)
