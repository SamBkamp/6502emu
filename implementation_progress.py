import re
line_regex = "{[a-zA-Z_]*,[\" a-zA-Z_0-9]*,[a-zA-Z_ ]*}"
noimp_regex = "{NULL,[\" a-zA-Z_0-9]*, NULL}"
opcode = 0
noimp_count = 0
already_seen = []

with open("opcode_table.h") as f:
  for line in f:
      x = re.search(line_regex, line)
      if(x):
          opcode +=1 
          m = x.group()
          noimp_match = re.search(noimp_regex, m)
          if(noimp_match):
              noimp_count += 1
              if(noimp_match.group().split(",")[1][2:-1] not in already_seen):
                  already_seen.append(noimp_match.group().split(",")[1][2:-1]);

amt = opcode-noimp_count
total = opcode
perc = round((amt/total)*100)
out_of_twenty = round((perc/100)*20)

print(", ".join(already_seen))

print(str(amt)+"/"+str(total) + " ("+str(perc)+"%)")

for i in range(0, out_of_twenty):
    print("█", end="")
for i in range(out_of_twenty, 20):
    print("░", end="")

print()



