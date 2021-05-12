FETCH, STORE, PUSH, POP, ADD, SUBTRACT, MULTIPLY, DIVIDE, LT, LTE, EQ, NEQ, JZ, JNZ, JMP, HALT = range(16)


class VirtualMachine:
    @staticmethod
    def run(program):
        var = dict()
        # var = [0 for i in range(26)]
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
            elif op == HALT:
                break

        print('Execution finished.')
        print(var)
        # for i in range(26):
        #     if var[i] != 0:
        #         print('%c = %d' % (chr(i + ord('a')), var[i]))


prg = [2,0,1,"a",2,0,1,"b",0,"a",2,20,8,12,40,0,"a",2,1,4,1,"a",3,2,5,0,"b",8,12,38,0,"b",2,1,4,1,"b",3,14,8,15,15,]
VirtualMachine.run(prg)
