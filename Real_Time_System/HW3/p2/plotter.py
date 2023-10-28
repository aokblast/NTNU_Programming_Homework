import seaborn as sns
import matplotlib.pyplot as plt
import pandas as pd
df = []

with open("npp_2_res") as f:
    for line in f:
        line = line.strip()
        if line.isnumeric():
            df.append({"value": int(line), "type": "NPP"})

with open("pcp_2_res") as f:
    for line in f:
        line = line.strip()
        if line.isnumeric():
            df.append({"value": int(line), "type": "PCP"})
with open("pip_2_res") as f:
    for line in f:
        line = line.strip()
        if line.isnumeric():
            df.append({"value": int(line), "type": "PIP"})

df = pd.DataFrame(df)
print(df)

sns.ecdfplot(data=df, x = 'value', hue='type')

plt.show()
