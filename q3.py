import subprocess

n = 4000
for p_variable in ["i", 'k', 'j']:
    for threads in range (2, 20):
        command = "sbatch --output={p_variable}_{n}_{threads}.out --error={p_variable}_{n}_{threads}.err --cpus-per-task={threads} hw3.slurm {p_variable} {n} {threads}".format(p_variable=p_variable, n=n, threads=threads)
        process = subprocess.Popen(command.split(), stdout=subprocess.PIPE)
        output, error = process.communicate()
        print(output)


