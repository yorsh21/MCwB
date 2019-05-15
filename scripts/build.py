with open("../instances/lecture.txt", "r") as f:
    output = f.read()
f.closed
    
lectures = output.split("\n")
for lecture in lectures:
	lecarray = lecture.split("\t")
	leninstance = int(lecarray[2])

	file = open("../inputs/" + lecarray[0], "w")
	file.write("3\n")
	file.write(lecarray[3] + "\t" + lecarray[4] + "\t" + lecarray[5] + "\n")
	file.write("\n")
	file.write("3\n")
	file.write(lecarray[6] + "\t" + lecarray[7] + "\t" + lecarray[8] + "\n")
	file.write("1.0\t.7\t.3\n")
	file.write("\n")
	file.write(lecarray[2] + "\n")

	filename = "../instances/data/" + lecarray[1]

	print("Read " + filename)
	with open(filename, "r") as f:
	    output = f.read()
	f.closed

	instance = output.split("\n")
	pattern1 = "param: xcoord	ycoord :="
	pattern2 = "param q:  A   B   C:="
	contador1 = leninstance
	contador2 = 0
	inlecture1 = False
	inlecture2 = False
	linesinstance = []

	for line in instance:
		if pattern1 in line:
			inlecture1 = True
			continue
		elif pattern2 in line:
			inlecture2 = True
			continue
		else:
			if inlecture1 and contador1 > 0:
				linesinstance.append(line.split("\t"))
				contador1 -= 1
			
			if inlecture2 and contador2 < leninstance:
				linesplit = line.split("\t")

				if linesplit[1] != "0":
					milk = "A\t" + linesplit[1]
				elif linesplit[2] != "0":
					milk = "B\t" + linesplit[2]
				elif linesplit[3] != "0":
					milk = "C\t" + linesplit[3]
				else:
					milk = "-\t0"

				linesinstance[contador2].append(milk)
				contador2 += 1

	for line in linesinstance:
		file.write("\t".join(line) + "\n")

	file.closed


print("Finish")