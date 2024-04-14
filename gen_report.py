import sys, os

folder = sys.argv[1]
out_file = sys.argv[2]

with open(out_file, 'w') as fp:
    file_list = os.listdir(folder)
    file_list.sort()

    for file in file_list:
        file_path = os.path.join(folder, file)
        cnt = 0
        with open(file_path, 'r') as f:
            line = f.readline().strip()
            if not line:
                cnt = -1
            for ch in line:
                if ch == '1':
                    cnt += 1
        
        fp.write(f"{file.split('.')[0]},{cnt}\n")
