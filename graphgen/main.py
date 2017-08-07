import networkx as nx
import matplotlib.pyplot as plt
import random
import json

G = nx.fast_gnp_random_graph(128, 0.06)
pos = nx.spring_layout(G, scale=1000)

sites = []
rivers = []
mines = set()

id_idx_set = set()
while len(id_idx_set) < len(G):
    id_idx_set.add(random.randint(-2 ** 63, 2 ** 63 - 1))

id_idx = list(id_idx_set)
random.shuffle(id_idx)

for n in G.nodes():
    node = {}
    node['id'] = id_idx[n]
    node['x'] = pos[n][0]
    node['y'] = pos[n][1]
    sites.append(node)

for e in G.edges():
    river = {}
    river['source'] = id_idx[e[0]]
    river['target'] = id_idx[e[1]]
    rivers.append(river)

random.seed()
m = random.randint(1, len(sites) / 2)
while len(mines) < m:
    mines.add(random.randint(0, len(sites) - 1))

out = {}
out['sites'] = sites
out['rivers'] = rivers
out['mines'] = [id_idx[m] for m in mines]

random.shuffle(out['sites'])
random.shuffle(out['rivers'])
random.shuffle(out['mines'])

with open("map.json", 'w') as f:
    json.dump(out, f)

plt.figure(figsize=(6,6))
nx.draw_networkx(G,pos, node_size=10, font_size=0)
plt.axis("off")
plt.savefig("default.png")
plt.show()
