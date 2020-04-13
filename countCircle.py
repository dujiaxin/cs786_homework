# Python Program to count
# cycles of length n
# in a given graph.

# Number of vertices
V = 6


def DFS(graph, marked, n, vert, start, count, path):
    # mark the vertex vert as visited
    marked[vert] = True

    # if the path of length (n-1) is found
    if n == 0:

        # mark vert as un-visited to make
        # it usable again.
        marked[vert] = False

        # Check if vertex vert can end with
        # vertex start
        if graph[vert][start] == 1:
            count = count + 1
            path.append(start)
            print(path)
            path.pop()

            return count
        else:
            return count

    # For searching every possible path of
    # length (n-1)
    for i in range(V):
        if marked[i] == False and graph[vert][i] == 1:
            # DFS for searching path by decreasing
            # length by 1
            path.append(i)
            count = DFS(graph, marked, n - 1, i, start, count, path)
            for node in path:
                if not marked[node] == True:
                    path.remove(node)

    # marking vert as unvisited to make it
    # usable again.
    marked[vert] = False
    return count


# Counts cycles of length
# N in an undirected
# and connected graph.
def countCycles(graph, n):
    # all vertex are marked un-visited intially.
    marked = [False] * V

    # Searching for cycle by using v-n+1 vertices
    count = 0
    for i in range(V - (n - 1)):
        path = []
        path.append(i)
        count = DFS(graph, marked, n - 1, i, i, count, path)

        # ith vertex is marked as visited and
        # will not be visited again.
        marked[i] = True

    return int(count / 2)


# main :
graph = [[0, 1, 1, 1, 0, 1],
         [1, 0, 1, 1, 0, 0],
         [1, 1, 0, 0, 0, 1],
         [1, 1, 0, 0, 0, 0],
         [0, 0, 0, 0, 0, 0],
         [1, 0, 1, 0, 0, 0]]

n = 4
print("Total cycles of length ", n, " of the undirected graph are ", countCycles(graph, n))

graph = [[0, 1, 0, 0, 0, 0],
         [1, 0, 1, 1, 0, 0],
         [1, 1, 0, 0, 0, 1],
         [1, 0, 0, 0, 0, 0],
         [0, 0, 0, 0, 0, 0],
         [1, 0, 1, 0, 0, 0]]

n = 4
print("Total cycles of length ", n, " of the directed graph are ", countCycles(graph, n))


# this code is forked from Shivani Ghughtyal
