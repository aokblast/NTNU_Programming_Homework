if __name__ == "__main__":

    with open('input', 'w') as f:
        n  = 10
        f.write(str(n))
        f.write('\n')

        for i in range(0, 10):
            for j in range(0, 10):
                if i == j:
                    f.write(str(-4))
                elif i - j == 1 or j - i == 1:
                    f.write(str(2))
                else:
                    f.write(str(0))
                f.write(' ')
            f.write(str(i + 2))
            f.write('\n')


