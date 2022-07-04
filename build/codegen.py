from sys import argv
from io import StringIO

def generate() -> str:
    buf = StringIO()

    return buf.getvalue()

def main():
    if len(argv) != 2:
        print('peepeepoopoo')
        exit(1)
    # with open(argv[1], 'wt') as fh:
        # fh.write(generate())


if __name__ == '__main__': main()