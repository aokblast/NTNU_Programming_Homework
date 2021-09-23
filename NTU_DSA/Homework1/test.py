import random as rd
numLIST = []

def reverse(LIST, start, end) :
    LIST[start:end + 1] = LIST[start:end + 1][::-1]
    return LIST

def query(LIST, start, end, k) :
    newLIST = sorted(LIST[start:end])
    return newLIST[k];

if __name__ == "__main__":
    for i in range(0 ,100):
        numLIST.append(rd.randint(0, 1e5))
    for i in range(0, len(numLIST)):
        if(i % 10 == 0) :
            print()
        print(numLIST[i], end = ' ')
        
    print()
    numLIST.insert(49, 2)
    #numLIST.pop(5678)
    numLIST = reverse(numLIST,40, 59)
    for i in range(0, len(numLIST)):
        if(i % 10 == 0) :
            print()
        print(numLIST[i], end = ' ')
    print()
    #print(query(numLIST, 4000, 6000, 1))
