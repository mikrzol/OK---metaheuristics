def calculate_average(scores_list):
    average = 0.0
    for el in scores_list:
        average += el[1]
    average = average / len(scores_list)
    return average


in_file = open('data.txt')
out_file_p = open('pylog-parents.txt', 'w')
out_file_c = open('pylog-children.txt', 'w')

parents_scores = []
children_scores = []
children_in_local_minimum = []
i = 1
for line in in_file:
    if(line.startswith("PARENTS")):
        parents_scores.append((i, float(line.split()[3])))
    if(line.startswith("CHILDREN")):
        children_scores.append((i, float(line.split()[3])))
        if(line.split()[3].endswith(".00")):
            children_in_local_minimum.append((i, float(line.split()[3])))
        i += 1

parents_average = calculate_average(parents_scores)
children_average = calculate_average(children_scores)

for el in parents_scores:
    out_file_p.write(f"{str(el[1])}\n")

for el in children_scores:
    out_file_c.write(f"{str(el[1])}\n")

print(f"CHILDREN IN LOCAL MINIMA: {len(children_in_local_minimum)}")

in_file.close()
out_file_p.close()
out_file_c.close()
