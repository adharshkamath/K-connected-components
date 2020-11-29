# K-connected-components
C++ implementation of an algorithm to find K-edge connected components using graph decomposition. ([Base Paper](http://weiemmazhang.me/sublinks/papers/4.2.2013SIGMOD.Efficiently%20computing%20k-edge%20connected%20components%20via%20graph%20decomposition.pdf))

(NOTE: We mentioned Team 20 in the slides and video, as that was the original number in the Google sheet. However, in the new sheet, our team is marked as Team 11)

### Usage

The input file has to be a text file in the form of an adjacency list (0 based indexing). If you're downloading a dataset from [SNAP](https://snap.stanford.edu/data/index.html), 
use the `read_file.py` script to the generate adjacency list for your dataset.

Make sure the dataset is an undirected graph (if there is an edge from 0 to 1, there should be an edge from 1 to 0).

To compile the project, navigate to src/ and run the makefile.
```bash
cd src
make
```
This generates an executable, a.out.

To run the executable
```bash
./a.out [input file] [Value of K]
```
There are two input files in the src directory `input.txt` (This is the [Enron Email](https://snap.stanford.edu/data/email-Enron.html) dataset converted to adjacency list) and `small-input.txt` (This is the example graph from the research paper)

After execution, the K-edge components so found are stored in `output.txt`, where each line contains the vertices belonging to a component.
