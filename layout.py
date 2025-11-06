import sys
import json
import networkx as nx

data = json.loads(sys.stdin.read())

G = nx.Graph()

for i in range(data["node_count"]):
    G.add_node(i)

for u, v, w in data["edges"]:
    G.add_edge(u, v, weight=w)

pos = nx.spring_layout(G, weight="weight", seed=42, k=0.7)

pos = {str(k): [float(v[0]), float(v[1])] for k, v in pos.items()}

print(json.dumps(pos))
