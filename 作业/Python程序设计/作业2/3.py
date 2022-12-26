class Vertex:
    def __init__(self,name,edges=[]):
        self.name=name
        self.edges=edges
class Edge:
    def __init__(self,head,tail,weight=1):
        self.head=head
        self.tail=tail
        self.weight=weight
class SimpleGraph:
    def __init__(self,edges=[],verts={}):
        self.verts=verts
        self.edges=edges

    def add_vertex(self,v):
        self.verts[v]=[]
    def add_edge(self,v1,v2):
        self.edges.append(Edge(v1,v2))
        if v1 in self.verts.keys():
            self.verts[v1].append(v2)

    def contains_vertex(self,v):
        if v in self.verts.keys():
            return True
        else:
            return False
    def contains_edge(self,v1,v2):
        for edge in self.edges:
            if edge.head==v1 and edge.tail==v2:
                return True
        return False

    def get_neighbors(self,v):
        if v in self.verts.keys():
            return self.verts[v]

    def is_empty(self):
        if self.verts=={} and self.edges==[]:
            return True
        else:
            return False

    def remove_vertex(self,v):
        self.verts.pop(v)
        for v0 in self.verts:
            if v in self.verts[v0]:
                self.verts[v0].remove(v)
        for edge in self.edges:
            if edge.head==v or edge.tail==v:
                self.edges.remove(edge)
    def remove_edge(self,v1,v2):
        for edge in self.edges:
            if edge.head==v1 and edge.tail==v2:
                self.edges.remove(edge)
                break
        self.verts[v1].remove(v2)

    def is_neighbor(self,v1,v2):
        if v2 in self.verts[v1]:
            return True
        else:
            return False
    def is_reachable(self,v1,v2,sign):
        sign[v1]=1
        if v2 in self.verts[v1]:
            return True
        else:
            for v in self.verts[v1]:
                if sign[v]==0 and self.is_reachable(v,v2,sign):
                    return True



    def clear_all(self):
        self.verts.clear()
        self.edges.clear()

sample=SimpleGraph([Edge(0,1),Edge(0,2),Edge(0,3),
                    Edge(1, 3),
                    Edge(2,0),Edge(2,1),
                    Edge(3,1),Edge(3,0)],{0:[1,2,3],1:[3],2:[0,1],3:[0,1]})

print(sample.verts,'\n')

print((sample.contains_vertex(1)))
print((sample.contains_edge(0,1)),'\n')

print(sample.verts)
sample.remove_edge(0,1)
print(sample.verts)
print((sample.contains_edge(0,1)))
print(sample.is_neighbor(1,2),'\n')

sign=[0,0,0,0]
print(sample.is_reachable(1,2,sign),'\n')

sample.add_vertex(4)
print(sample.verts)
print(sample.is_neighbor(0,4))
sample.add_edge(0,4)
print(sample.verts)
print(sample.is_neighbor(0,4),'\n')

print(sample.is_neighbor(4,0))
sample.add_edge(4,0)
print(sample.verts)
print(sample.is_neighbor(4,0),'\n')

print(sample.get_neighbors(0),'\n')


sample.remove_vertex(0)
print(sample.verts,'\n')


print(sample.is_empty())
sample.clear_all()
print(sample.verts)
print(sample.edges)
print(sample.is_empty(),'\n')

sample.add_vertex(0)
print(sample.is_empty(),'\n')
