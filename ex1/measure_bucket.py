import matplotlib.pyplot as plt
from subprocess import check_output

TESTS = 1
P_TESTS = [1, 2, 4, 8, 16]
COMMAND = "mpirun --mca opal_warn_on_missing_libcuda 0 -np {0} ./bucket_sort.out"


def measure_time(p):
    total = 0.0

    for i in range(TESTS):
        out = check_output(COMMAND.format(p).split()).decode("utf-8")
        total += float(out.split()[1])

    total /= TESTS
    return total


def main():
    for p in P_TESTS:
        test = measure_time(p)
        print(f"{p}, {test}")


if __name__ == "__main__":
    main()
