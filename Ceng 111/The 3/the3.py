def pattern_search(P,I):
    def rotated(P,Q=0):
        P=P
        P_90=[]
        Q += 90
        str1 = ""
        b = len(P[0])
        c = 0
        for i in range(0,len(P[0])):
            a = len(P)
            if a == 0:
                a = len(P)
            if c == len(P[0]):
                c = 0
            for x in range(0,len(P)):
                str1 += P[a-1][c]
                a -= 1
            P_90 = P_90 + [str1]
            str1 = ""
            c += 1
        return P_90
    def finder(P,I,q):
        k = 0
        inside = False
        for i in range(0, len(I)-len(P)+1):
            if I[i].find(P[0]) != -1:
                Image_cut = []
                a = I[i:i+len(P)]
                for x in range(0,len(P)):
                    Image_cut = Image_cut + [a[x][I[i].find(P[0]):I[i].find(P[0])+len(P[0])]]
                    if Image_cut == P:
                        inside = True
                    else:
                        inside = False
            else:
                inside = False
            n = 0
            while I[i].find(P[0] , n) != -1:
                Image_cut = []
                a = I[i:i+len(P)]
                for x in range(0,len(P)):
                    Image_cut = Image_cut + [a[x][I[i].find(P[0] , n):I[i].find(P[0] , n)+len(P[0])]]
                    if Image_cut == P:
                        inside = True
                        if x == len(P)-1:
                            inside = (i,I[i].find(P[0] , n),q)
                            return inside
                    else:
                        inside = False
                n += 1      
        return inside
    P_normal = P
    P_90 = rotated(P,0)
    P_180 = rotated(P_90,90)
    P_270 = rotated(P_180,180)
    A = finder(P_normal,I,0)
    B = finder(P_90,I,90)
    C = finder(P_180,I,180)
    D = finder(P_270,I,270)
    if A == False and B == False and C == False and D == False:
        return False
    elif A != False:
        return A
    elif B != False:
        return B
    elif C != False:
        return C
    elif D != False:
        return D