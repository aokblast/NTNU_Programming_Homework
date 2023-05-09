import seaborn as sns
import matplotlib.pyplot as plt
df = []

with open("p1_res") as f:
    for line in file:
        if line.is_nuneric():
            df.append(int(line))

sns.ecdfplot(data=df)
plt.show()
