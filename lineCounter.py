import os

folders = [
    "C:/Users/FurWaz/Documents/GitHub/FullBowody/modules/UI",
    "C:/Users/FurWaz/Documents/GitHub/FullBowody/modules/engine",
    "C:/Users/FurWaz/Documents/GitHub/FullBowody/modules",
    "C:/Users/FurWaz/Documents/GitHub/FullBowody/docs",
    "C:/Users/FurWaz/Documents/GitHub/FullBowody"
]
nbLines = 0

for f in folders:
    for e in os.listdir(f):
        if (e.endswith(".cpp") or e.endswith(".hpp") or e.endswith(".py")):
            a = open(f+"/"+e, "r");
            b = a.read()
            nbLines += len(b.split("\n"))
print("Programmed "+str(nbLines)+" lines in total")