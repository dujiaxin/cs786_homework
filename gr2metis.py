import os


with open("random1.gr", 'r') as r:
    with open("random1.metis", 'w') as w:
        for i in range(7):
            values = r.readline().replace("\n", '').split(' ')
            
        values = r.readline().replace("\n", '').split(' ')
        n = values[2]  # number of vertices
        m = values[3]  # number of edge
        print("n, m: ", n, m )
        m_2 = int(int(m)/2)
        w.writelines(f"{n} {m_2}\n")
        
        # get edges
        lines = r.readlines()
        # print(lines)
        i = 0
        w_line = ''  # new line to be written to new file
        last_v = ''
        w_line = w_line + last_v
        
        w_lines = [" "] * int(n)
 
        
        while i < len(lines) :
            values = lines[i].replace("\n", '').split(' ')
            
            
            v = int(values[1])
            # print(values[1], values[2])
            
            w_lines[v-1] = w_lines[v-1] + " " + values[2]
             
                
            i = i + 1
            
        for i in range(int(n)):
            w.writelines(w_lines[i] + "\n")