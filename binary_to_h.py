import os

with os.scandir('./src/progs') as files:
  for file in files:
    if file.is_dir():
      continue
    if file.name.endswith(".c"):
      raw_name = file.name[0:-2]
      with open(f"build/{raw_name}.bin", "rb") as f:
        data = f.read()
      with open(f"src/progs/{raw_name}_contents.h", "w") as f2:
        f2.write("#include \"../common.h\"\n")
        f2.write(f"u8 {raw_name}_contents[] = " + "{")
        for b in data:
          f2.write(f"{b},")
        f2.write("};\n")
        f2.write(f"u32 {raw_name}_length = {len(data)};");
