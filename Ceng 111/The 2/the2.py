a=input()
a=eval(a)
x0=a[0][0]
y0=a[0][1]
x1=a[1][0]
y1=a[1][1]
x2=a[2][0]
y2=a[2][1]
x3=a[3][0]
y3=a[3][1]
p0=a[0]
p1=a[1]
p2=a[2]
p3=a[3]
b=[x0,x1,x2,x3]
c=[y0,y1,y2,y3]
d=[(x0,y0),(x1,y1),(x2,y2),(x3,y3)]
smallestx=min(b)
biggestx=max(b)
y_smallestx=c[b.index(min(b))]
y_biggestx=c[b.index(max(b))] 
case_minx_1=min(b)
case_yminx_1=c[b.index(min(b))]
case_minx_2=min(list(reversed(b)))
case_yminx_2=list(reversed(c))[list(reversed(b)).index(min(list(reversed(b))))]
case_maxx_1=max(b)
case_ymaxx_1=c[b.index(max(b))]
case_maxx_2=max(list(reversed(b)))
case_ymaxx_2=list(reversed(c))[list(reversed(b)).index(max(list(reversed(b))))]
if (b.count(min(b))==1) and (b.count(max(b))==1):
    smallestx=min(b)
    biggestx=max(b)
    y_smallestx=c[b.index(min(b))]
    y_biggestx=c[b.index(max(b))] 
elif (b.count(min(b))!=1) and (b.count(max(b))==1):   
    if abs(case_yminx_1)<abs(case_yminx_2):
        y_smallestx=case_yminx_1 
    else: 
        y_smallestx=case_yminx_2
elif (b.count(max(b))!=1) and (b.count(min(b))==1):
    if abs(case_ymaxx_1)<abs(case_ymaxx_2):
        y_biggestx=case_ymaxx_1
    else:
        y_biggestx=case_ymaxx_2    
else:
    if abs(case_yminx_1)<abs(case_yminx_2):
        y_smallestx=case_yminx_1 
    else: 
        y_smallestx=case_yminx_2
    if abs(case_ymaxx_1)<abs(case_ymaxx_2):
        y_biggestx=case_ymaxx_1
    else:
        y_biggestx=case_ymaxx_2    
if abs(y_smallestx)<abs(y_biggestx):
        littley=abs(y_smallestx)
        bigy=abs(y_biggestx)
else: 
        littley=abs(y_biggestx)
        bigy=abs(y_smallestx)
ledgep=(smallestx,y_smallestx)
redgep=(biggestx,y_biggestx)
d.remove(ledgep)
d.remove(redgep)
m=((abs(y_biggestx)-abs(y_smallestx))/(biggestx-smallestx))
e=((abs(y_smallestx))+(m*d[0][0])-(m*smallestx))
f=((abs(y_smallestx))+(m*d[1][0])-(m*smallestx))
if ((abs(d[0][1])-abs(e))>0) and ((abs(d[1][1])-abs(f))>0):
    if ((a.index(ledgep),a.index(redgep)) == (1,3)) or ((a.index(ledgep),a.index(redgep))==(0,2)) or ((a.index(ledgep),a.index(redgep))==(3,1)) or ((a.index(ledgep),a.index(redgep))==(2,0)):
        if (b.count(smallestx)!=1)or(b.count(biggestx)!=1):
            if ((d[0][0]-smallestx) == 0) or ((d[0][0]-biggestx) == 0):
              innerpoint=d[1]
            else:
              innerpoint=d[0]     
        elif littley==abs(ledgep[1]):
            if abs((abs(d[0][1])-abs(ledgep[1]))/(d[0][0]-ledgep[0]))<abs((abs(d[1][1])-abs(ledgep[1]))/(d[1][0]-ledgep[0])):
                    innerpoint=d[0]
            else:
                    innerpoint=d[1]
        elif littley==abs(redgep[1]):
            if abs((abs(d[0][1])-abs(redgep[1]))/(redgep[0]-d[0][0]))<abs((abs(d[1][1])-abs(redgep[1]))/(redgep[0]-d[1][0])):
                    innerpoint=d[0]
            else:
                    innerpoint=d[1]
        x_innerpoint=innerpoint[0]
        y_innerpoint=innerpoint[1]          
        area= ((((abs(y_biggestx)+abs(y_smallestx))/2)*(biggestx-smallestx))+((abs((smallestx*y_biggestx+biggestx*y_innerpoint+x_innerpoint*y_smallestx)-(biggestx*y_smallestx+x_innerpoint*y_biggestx+smallestx*y_innerpoint)))/2))     
    else: area= (((abs(y_biggestx)+abs(y_smallestx))/2)*(biggestx-smallestx))
    print("%.2f" % area)
elif ((abs(d[0][1])-abs(e))<0) and ((abs(d[1][1])-abs(f))<0):
    if ((a.index(ledgep),a.index(redgep)) == (1,3)) or ((a.index(ledgep),a.index(redgep))==(0,2)) or ((a.index(ledgep),a.index(redgep))==(3,1)) or ((a.index(ledgep),a.index(redgep))==(2,0)):
        if (b.count(smallestx)!=1)or(b.count(biggestx)!=1):
            if ((d[0][0]-smallestx) == 0) or ((d[0][0]-biggestx) == 0):
                innerpoint=d[1]
            else:
                innerpoint=d[0]     
        elif bigy==abs(ledgep[1]):
            if abs((abs(ledgep[1])-abs(d[0][1]))/(d[0][0]-ledgep[0]))<abs((abs(ledgep[1])-abs(d[1][1]))/(d[1][0]-ledgep[0])):
                    innerpoint=d[0]
            else:
                    innerpoint=d[1]
        elif bigy==abs(redgep[1]):
            if abs((abs(redgep[1])-abs(d[0][1]))/(redgep[0]-d[0][0]))< abs((abs(redgep[1])-abs(d[1][1]))/(redgep[0]-d[1][0])):
                    innerpoint=d[0]
            else:
                    innerpoint=d[1] 
        d.remove(innerpoint)
        area= ((((abs(y_biggestx)+abs(y_smallestx))/2)*(biggestx-smallestx))-(((abs((x0*y1+x1*y2+x2*y3+x3*y0)-(x1*y0+x2*y1+x3*y2+x0*y3)))/2))-((abs((smallestx*y_biggestx+biggestx*innerpoint[1]+innerpoint[0]*y_smallestx)-(biggestx*y_smallestx+innerpoint[0]*y_biggestx+smallestx*innerpoint[1])))/2))
    else: area= ((((abs(y_biggestx)+abs(y_smallestx))/2)*(biggestx-smallestx))-(((abs((x0*y1+x1*y2+x2*y3+x3*y0)-(x1*y0+x2*y1+x3*y2+x0*y3)))/2)))
    print( "%.2f" % area)
elif ((abs(d[0][1])-abs(e))<0) and ((abs(d[1][1])-abs(f))>0) or ((abs(d[0][1])-abs(e))>0) and ((abs(d[1][1])-abs(f))<0):
    if ((abs(d[0][1])-abs(e))<0) and ((abs(d[1][1])-abs(f))>0):
        area= ((((abs(y_biggestx)+abs(y_smallestx))/2)*(biggestx-smallestx))-((abs((smallestx*y_biggestx+biggestx*d[0][1]+d[0][0]*y_smallestx)-(biggestx*y_smallestx+d[0][0]*y_biggestx+smallestx*d[0][1])))/2))
    elif ((abs(d[0][1])-abs(e))>0) and ((abs(d[1][1])-abs(f))<0):
        area= ((((abs(y_biggestx)+abs(y_smallestx))/2)*(biggestx-smallestx))-((abs((smallestx*y_biggestx+biggestx*d[1][1]+d[1][0]*y_smallestx)-(biggestx*y_smallestx+d[1][0]*y_biggestx+smallestx*d[1][1])))/2))
    print( "%.2f" % area)