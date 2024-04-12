import os
import time
import matplotlib.pyplot as plt

# 选取算法
algorithms = ["bfs.exe", "dijkstra.exe", "A_star.exe"]
algorithm = algorithms[2]

# 指定迷宫（空串则随机生成）
certain_maze = ""
certain_maze = ".\\mazes\\maze_5"

# 随机生成迷宫的大小
maze_m = 35
maze_n = 35

# 中间文件路径名
tmp_path_path = "tmp_path.txt"
tmp_log_path = "tmp_log.txt"
maze_path = "tmp_maze.txt"

def visualize_maze_with_path(maze, path, log):
    plt.figure(figsize=(len(maze[0]), len(maze)))  # 设置图形大小
    plt.imshow(maze, cmap='Greys', interpolation='nearest')  # 使用灰度色图，并关闭插值

    # 设置坐标轴刻度和边框
    plt.xticks(range(len(maze[0])))
    plt.yticks(range(len(maze)))
    plt.gca().set_xticks([x - 0.5 for x in range(1, len(maze[0]))], minor=True)
    plt.gca().set_yticks([y - 0.5 for y in range(1, len(maze))], minor=True)
    plt.grid(which="minor", color="black", linestyle='-', linewidth=2)

    if log:
        print_mod = (len(log) // 50) + 1
        for i in range(len(log)):
            y, x = log[i]
            plt.scatter(x, y, color='green', s=100, zorder=10)   # plot the log
            if i % print_mod == 0:
                plt.draw()
                plt.pause(0.0005)  # Pause for a short time to visualize step by step

    if path:
        for i in range(1, len(path)):
            x1, y1 = path[i-1]
            x2, y2 = path[i]
            plt.plot([y1, y2], [x1, x2], marker='o', markersize=8, color='blue', linewidth=10)  # Plot path segment
            plt.draw()
            time.sleep(0.00005)  # Sleep to ensure smooth visualization

    plt.axis('on')  # 显示坐标轴
    plt.show()


#  获取迷宫二维数组
def get_maze():
    raw_maze = ".\\tmp_raw_maze.txt"
    if (certain_maze == ""):
        os.system(".\\bin\\generator.exe {} {} {}".format(maze_m, maze_n, raw_maze))
    else:
        raw_maze = certain_maze

    f = open(raw_maze, "r")
    maze = []
    for line in f:
        new_line = [c == "1" for c in line]
        maze.append(new_line[:-1])
    f.close()
    return maze

# blank maze
# maze = [[0 for i in range(maze_n)] for j in range(maze_m)]


def run_algorithm(maze):
    # 重置输入格式(算法的输入)
    f = open(maze_path, "w+")
    f.write("{} {}\n".format(len(maze), len(maze[0])))
    for line in maze:
        f.write(" ".join(["1" if x == 1 else "0" for x in line]) + '\n')
    f.close()

    # 运行算法程序得到路径  
    os.system("type {} | .\\bin\\{} {} {}".format(maze_path,algorithm, tmp_path_path, tmp_log_path))


# 读取路径
def read_path():
    f = open(tmp_path_path)         
    path = []
    for line in f:
        path.append( tuple( [int(x) - 1 for x in line.split(" ")] ) )
    f.close()   
    return path

#读取日志
def read_log():
    f = open(tmp_log_path)         
    log = []
    for line in f:
        log.append( tuple( [int(x) - 1 for x in line.split(" ")] ) )
    f.close() 
    return log

# 可视化迷宫及路径



maze = get_maze()
run_algorithm(maze)
path = read_path()
log = read_log()
visualize_maze_with_path(maze, path, log)
