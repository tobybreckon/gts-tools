
## gts-tools : simple, useful tools for working with GTS surfaces

```
Copyright (C) 2006  Toby Breckon
License : GPL
```
---

## Installation

Requirements:
- [GTS 0.7.x](http://gts.sourceforge.net/) (or greater)
- gts-config and glib-config required to be in path.
- tested on GNU/Linux.

#### Installation instructions

- To install change into repository directory and type make:
```
	cd gts-tools-...; make
```

- Resulting set of GTS tools are built in tools

 To install (in /usr/local/bin and /usr/share/doc/packages/gts-tools/ by default):
```
	make install
```

 Report all installation issues to gts-tools@breckon.me.uk

---

## Tool Details

- gts2vtk      : convert GTS file to VTK format.

- gts2xyz      : convert GTS file to basic ASCII XYZ format.

- gtscutter    : clip GTS surface in x,y,z plane and/or radial distance from point.

- gtsinfo      : display various standard and extra information for GTS surface.

- gtsnormalise : normalise position of GTS surface to origin or specified point.

- gtsnormals   : extract surface normals as basic ASCII XYZ format.

- gtsorientate : consistently orientate GTS surface in terms of surface normal direction (flip, clockwise, anti-clockwise, from surface centroid).

- gtssubtract  : perform a set difference (subtraction) operation between 2 surfaces at vertex level (N.B. not boolean diff.).

- gtstesselate : tesselate specified GTS surface once (or N times).

- gtstricutter : remove triangles of specified area/edge length/# neighbours /orientation from GTS surface.

 All tools use gzip/bzip2 command line tools to handle files with
compression extensions.

---

## References

Used as supporting tools for surface manipulation to generate the following research work (please reference if used in your own work):

[A Hierarchical Extension to 3D Non-parametric Surface Relief Completion](http://community.dur.ac.uk/toby.breckon/publications/papers/breckon12completion.pdf) (T.P. Breckon, R.B. Fisher), In Pattern Recognition, Elsevier, Volume 45, pp. 172-185, 2012. [[pdf](http://community.dur.ac.uk/toby.breckon/publications/papers/breckon12completion.pdf)] [[doi](http://dx.doi.org/10.1016/j.patcog.2011.04.021)]
```
@Article{breckon12completion,
  author = 	 {Breckon, T.P. and Fisher, R.B.},
  title = 	 {A Hierarchical Extension to 3{D} Non-parametric Surface Relief Completion},
  journal = 	 {Pattern Recognition},
  year = 	 {2012},
  volume = 	 {45},
  pages = 	 {172-185},
  month = 	 {September},
  publisher =    {Elsevier},
  doi = 	 {10.1016/j.patcog.2011.04.021},
}
```
[3D Surface Relief Completion Via Non-parametric Techniques](http://community.dur.ac.uk/toby.breckon/publications/papers/breckon08completion.pdf) (T.P. Breckon, R.B. Fisher), In IEEE Transactions on Pattern Analysis and Machine Intelligence, IEEE, Volume 30, No. 12, pp. 2249 - 2255, 2008. [[pdf](http://community.dur.ac.uk/toby.breckon/publications/papers/breckon08completion.pdf)] [[doi](http://dx.doi.org/10.1109/TPAMI.2008.153)]
```
@Article{breckon08completion,
  author = 	 {Breckon, T.P. and Fisher, R.B.},
  title = 	 {3{D} Surface Relief Completion Via Non-parametric Techniques},
  journal = 	 {IEEE Transactions on Pattern Analysis and Machine Intelligence},
  year = 	 {2008},
  volume = 	 {30},
  number = 	 {12},
  pages = 	 {2249 - 2255},
  month = 	 {December},
  publisher = 	 {IEEE},
  doi = 	 {10.1109/TPAMI.2008.153},
}
```

---

## Other useful GTS projects / tools:

- Mesh Viewer :- http://mview.sf.net
- k3d      :- http://www.k-3d.org/
- Smoother :- http://oblique.csail.mit.edu/SMOOTHER/

---
## Change Log (pre-git)

- 0.13
	Fixes to compile with 4.7.1 (#include added to compressed.h as required).
- 0.12
	Fixes to compile with GCC 4.3 (#includes added as required).
- 0.11
  Invert (flip) existing normal functionality added to gtsorientate.
	Tesselate up to N times in single operation added to gtstesselate.
- 0.1
	Initial version released.
