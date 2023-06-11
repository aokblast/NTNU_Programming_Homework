import seaborn as sns
import matplotlib.pyplot as plt
import pandas as pd
df = []

with open("./p1/t1_res") as f:
    for line in f:
        line = line.strip()
        if line.isnumeric():
            df.append({"value": int(line), "type": "BG(500)"})

with open("./p1/t2_res") as f:
    for line in f:
        line = line.strip()
        if line.isnumeric():
            df.append({"value": int(line), "type": "BG(700)"})
with open("./p1/t3_res") as f:
    for line in f:
        line = line.strip()
        if line.isnumeric():
            df.append({"value": int(line), "type": "BG(Aperiodic)"})

with open("./p2/t1_res") as f:
    for line in f:
        line = line.strip()
        if line.isnumeric():
            df.append({"value": int(line), "type": "Def(500)"})

with open("./p2/t2_res") as f:
    for line in f:
        line = line.strip()
        if line.isnumeric():
            df.append({"value": int(line), "type": "Def(700)"})
with open("./p2/t3_res") as f:
    for line in f:
        line = line.strip()
        if line.isnumeric():
            df.append({"value": int(line), "type": "Def(Aperiodic)"})


df = pd.DataFrame(df)
print(df)

sns.ecdfplot(data=df, x = 'value', hue='type')

plt.show()
