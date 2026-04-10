with open("build/prog.bin", "rb") as f:
  data = f.read()
  
with open("src/program_contents.h", "w") as f:
  f.write("#include \"common.h\"\n")
  f.write("u8 program_contents[] = {")
  for b in data:
    f.write(f"{b},")
  f.write("};")