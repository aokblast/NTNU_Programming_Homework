import random as rd
def hcfnaive(a,b): 
    if(b==0): 
        return a 
    else: 
        return hcfnaive(b,a%b) 

tmp = 10**256


for i in range(20):
    a = rd.randint(0, tmp)
    b = rd.randint(0, tmp)
    print (a,'\n', b,'\n')
    print (hcfnaive(b,a))
    print ('\n')