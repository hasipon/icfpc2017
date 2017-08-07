import networkx as nx
import matplotlib.pyplot as plt
import random
import json

G = nx.fast_gnp_random_graph(128, 0.06)
pos = nx.spring_layout(G, scale=1000)

sites = []
rivers = []
mines = set()

for n in G.nodes():
    node = {}
    node['id'] = n
    node['x'] = pos[n][0]
    node['y'] = pos[n][1]
    sites.append(node)

for e in G.edges():
    river = {}
    river['source'] = e[0]
    river['target'] = e[1]
    rivers.append(river)

random.seed()
m = random.randint(1, len(sites) / 2)
while len(mines) < m:
    mines.add(random.randint(0, len(sites) - 1))

out = {}
out['sites'] = sites
out['rivers'] = rivers
out['mines'] = sorted(list(mines))

with open("map.json", 'w') as f:
    json.dump(out, f)

plt.figure(figsize=(6,6))
nx.draw_networkx(G,pos, node_size=10, font_size=0)
plt.axis("off")
plt.savefig("default.png")
plt.show()
