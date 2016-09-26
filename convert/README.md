###Convert

Because the implementation of Batagelj and Zaversnik's O(m) algorithm needs the inputbuffer, I use a new format of graphdata to save memory space. This new format is like this:

vertexID,vertexDegree:Neighbor1:Neighbor2:Neighbor3: ... 

This new format refers to linhong's open source project [MaximumClique](https://github.com/linhongseba/MaximumClique). And she provides two tools transforming the popular edgelist format to this new format. Thanks for her work.


