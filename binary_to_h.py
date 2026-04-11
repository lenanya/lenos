from sys import argv

with open(argv[1], "rb") as f:
  data = f.read()
  
with open(argv[2], "w") as f:
  f.write("#include \"common.h\"\n")
  f.write("u8 program_contents[] = {")
  for b in data:
    f.write(f"{b},")
  f.write("};")