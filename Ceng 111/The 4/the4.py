def construct_forest(p):
    def space_remover(p):
        a = []
        for i in range(len(p)):
            a = a + [p[i].replace(" ","")]
        return a
    p = space_remover(p)
    def find_opt(inpt):
        inpt = inpt.split("=")[1]
        for i in range(5):
            if ((inpt[i].isalnum()) or (inpt[i] == "(") or (inpt[i] == ")")) == False :
                return inpt[i]
    def function_creator(p):
        a = []
        for i in range(len(p)):
            b = []
            b = b + [p[i][0]] + [find_opt(p[i])] + [[p[i].split("=")[1][:p[i].split("=")[1].index(find_opt(p[i]))]]] + [[p[i].split("=")[1][p[i].split("=")[1].index(find_opt(p[i]))+1:]]]
            a = a + [b]
        return a
    functions = function_creator(p)
    alphabet = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'y', 'z']
    forest = functions[:]
    for i in range(len(functions)):
        if len(functions[i][2][0]) != 1 and len(functions[i][3][0]) == 1:
            for k in range(len(alphabet)):
                    if alphabet[k] in functions[i][2][0]:
                        for l in range(len(functions)):
                            if functions[l][0] == alphabet[k]:
                                functions[i][2] = functions[l]
                                forest.remove(functions[l])                 
        elif len(functions[i][3][0]) != 1 and len(functions[i][2][0]) == 1:
            for k in range(len(alphabet)):
                    if alphabet[k] in functions[i][3][0]:
                        for l in range(len(functions)):
                            if functions[l][0] == alphabet[k]:
                                functions[i][3] = functions[l]
                                forest.remove(functions[l])
        elif len(functions[i][2][0]) != 1 and len(functions[i][3][0]) != 1:
            for j in range(2,4):
                for k in range(len(alphabet)):
                    if alphabet[k] in functions[i][j][0]:
                        for l in range(len(functions)):
                            if functions[l][0] == alphabet[k]:
                                functions[i][j] = functions[l]
                                forest.remove(functions[l])
    return forest