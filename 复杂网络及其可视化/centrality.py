import Queue
def read_graph(file_name):
    graph = [[] for i in range(2000)]
    graph2 = [[] for i in range(2000)]
    num = 0
    dict = {}
    dict2 = {}
    fp = open(file_name)
    data = fp.read()
    fp.close()
    lines = data.split('\n')
    for i_line in lines[1:-1]:
        tmp_data = i_line.split('\t')
        if not dict.has_key(tmp_data[0]):
            dict[tmp_data[0]] = num
            dict2[num] = tmp_data[0]
            num+=1
        if not dict.has_key(tmp_data[1]):
            dict[tmp_data[1]] = num
            dict2[num] = tmp_data[1]
            num+=1
        graph[dict[tmp_data[0]]].append(dict[tmp_data[1]])
        graph2[dict[tmp_data[1]]].append(dict[tmp_data[0]])
    shortways = [[[2000, 0] for j in range(2000)] for i in range(2000)]
    
    for i in range(num):
        BFS = Queue.Queue(maxsize=2000)
        BFS.put([i,0])
        visited = [False for m in range(num)]
        visited[i] = True
        while not BFS.empty():
            tmp_node = BFS.get()
            length = tmp_node[1]+1
            for j in graph[tmp_node[0]]:
                if shortways[i][j][0] >= length:
                    shortways[i][j][0] = length
                    if shortways[i][tmp_node[0]][0]>=2000:
                        shortways[i][j][1]+=1
                    else:
                        shortways[i][j][1] += shortways[i][tmp_node[0]][1]
                    
                if not visited[j]:
                    visited[j] = True
                    BFS.put([j,length])
    print "stage 1"
    cen = [[[0.0, False] for j in range(2000)] for i in range(2000)]
    for s in range(num):
        print s
        DFS = []
        DFS.append(s)
        visited = [False for m in range(num)]
        while len(DFS)>0:
            tmp_node = DFS[-1]
            
            computed = True
            for w in graph[tmp_node]:
                if not cen[s][w][1] and w not in DFS:
                    computed = False
                    DFS.append(w)
            if computed:
                cen[s][tmp_node][1] = True
                for w in graph[tmp_node]:
                    if shortways[s][w][0] == shortways[s][tmp_node][0]+1 and w not in DFS:
                        cen[s][tmp_node][0] += float(shortways[s][tmp_node][1])/float(shortways[s][w][1])*(1+cen[s][w][0])
                 
                DFS.pop()
    result = [[dict2[i],0] for i in range(num)]
    print "stage 2"
    for i in range(num):
        for j in range(num):
            #print dict2[i],dict2[j],cen[i][j]
            result[j][1]+=cen[i][j][0]
    return result
                    
def sort_centrality(file_name):
    f1 = open(file_name)
    data = f1.read()
    f1.close()
    lines = data.split('\n')
    datas = []
    num = 0
    for i_line in lines[1:-1]:
        tmp_data = i_line.split("\t")
        datas.append((tmp_data[0],tmp_data[1]))
    datas.sort(key=lambda x:float(x[1]), reverse=True)
    f2 = open("sort.txt","w")
    f2.write("Point\tValue\n")
    for i in datas:
        f2.write(str(i[0]))
        f2.write('\t')
        f2.write(str(i[1])+'\n')
    f2.close()
    
if __name__=="__main__":
    re = read_graph("1893.txt")
    f2 = open("1893result.txt","w")
    f2.write("Point\tValue\n")
    for i in re:
        f2.write(str(i[0]))
        f2.write('\t')
        f2.write(str(i[1]))
        f2.write('\n')
    f2.close()