import matplotlib.pyplot as plt
from subprocess import check_output

TESTS = 1
P_TESTS = [1, 2, 4, 8, 16]
COMMAND = "mpirun --mca opal_warn_on_missing_libcuda 0 -np {0} ./bucket_sort"


def measure_time(p):
    total = 0.0
    comp = 0.0
    comm = 0.0

    for i in range(TESTS):
        out = check_output(COMMAND.format(p).split()).decode("utf-8")
        lines = out.splitlines()
        total += float(out[0].split()[1])
        comp += float(out[1].split()[1])
        comm += float(out[2].split()[1])

    total /= TESTS
    comp /= TESTS
    comm /= TESTS
    return total, comp, comm


def main():
    for p in P_TESTS:
        total, comp, comm = measure_time(p)
        print(f"{p}, {total}, {comp}, {comm}")


if __name__ == "__main__":
    main()
