import statistics

if __name__ == "__main__":
    file = open("res", "r")

    res = []
    
    for line in file:
        fields = line.split(' ')
        for field in fields:
            if int(field) >= 0:
                res.append(int(field))

    file.close()

    print(min(res), statistics.median(res), max(res))
