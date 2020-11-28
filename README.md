# K-connected-components
C++ implementation of an algorithm to find K-edge connected components using graph decomposition

Usage:

The input file has to be a text file in the form of an adjacency list (0 based indexing). If you're downloading a dataset from [SNAP](https://snap.stanford.edu/data/index.html), 
use the `read_file.py` script to the generate adjacency list for your dataset.

Make sure the dataset is an undirected graph (if there is an edge from 0 to 1, there should be an edge from 1 to 0).

To compile the project, in the src/ directory,
```bash
make
```
This generates an executable, a.out.

To run the executable
```bash
./a.out [input file] [Value of K]
```
