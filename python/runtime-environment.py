import sys

FETCH, STORE, PUSH, POP, ADD, SUBTRACT, MULTIPLY, DIVIDE, LT, LTE, EQ, NEQ, JZ, JNZ, JMP, HALT, PRINT, SCAN = range(18)
TYPE_INT, TYPE_FLOAT, TYPE_BOOLEAN, TYPE_STRING = range(4)


class VirtualMachine:
    @staticmethod
    def run(program):
        var = dict()
        stack = []
        pc = 0
        while True:
            op = program[pc]
            if pc < len(program) - 1:
                arg = program[pc + 1]

            if op == FETCH:
                stack.append(var[arg])
                pc += 2
            elif op == STORE:
                var[arg] = stack.pop()
                pc += 2
            elif op == PUSH:
                stack.append(arg)
                pc += 2
            elif op == POP:
                stack.append(arg)
                stack.pop()
                pc += 1
            elif op == ADD:
                stack[-2] += stack[-1]
                stack.pop()
                pc += 1
            elif op == SUBTRACT:
                stack[-2] -= stack[-1]
                stack.pop()
                pc += 1
            elif op == MULTIPLY:
                stack[-2] *= stack[-1]
                stack.pop()
                pc += 1
            elif op == DIVIDE:
                stack[-2] /= stack[-1]
                stack.pop()
                pc += 1
            elif op == LT:
                if stack[-2] < stack[-1]:
                    stack[-2] = 1
                else:
                    stack[-2] = 0
                stack.pop()
                pc += 1
            elif op == JZ:
                if stack.pop() == 0:
                    pc = arg
                else:
                    pc += 2
            elif op == JNZ:
                if stack.pop() != 0:
                    pc = arg
                else:
                    pc += 2
            elif op == JMP:
                pc = arg
            elif op == PRINT:
                print(stack.pop())
                pc += 1
            elif op == SCAN:
                flag: bool = True
                temp = ""
                while flag:
                    temp = input()
                    try:
                        if arg == TYPE_INT:
                            temp = int(temp)
                        elif arg == TYPE_FLOAT:
                            temp = float(temp)
                        elif arg == TYPE_BOOLEAN:
                            if temp != "true" and temp != "false":
                                raise Exception()
                            else:
                                if temp == "true":
                                    stack.append(True)
                                else:
                                    stack.append(False)
                        flag = False
                    except Exception:
                        flag = True
                        print("Incorrect input type")

                stack.append(temp)
                pc += 2
            elif op == HALT:
                break

        # print(var)


if len(sys.argv) != 2:
    print("Incorrect number of arguments, expected: <executable>")
    exit(-1)

with open(sys.argv[1], "r") as file:
    text: str = file.read()
    prog: list = eval(text)
    if type(prog) is not list:
        print("Invalid bytecode.")
        exit(-2)
    VirtualMachine.run(prog)
