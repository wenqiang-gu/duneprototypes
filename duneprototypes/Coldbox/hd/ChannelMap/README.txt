
Channel map evolution for the HD Coldbox and ProtoDUNE-HD, 2022

We use the PD2HD Channel Map service and map files.  They started
off being developed in the coldbox directory but have been
moved to duneprototypes/protodue/hd/ChannelMap.

Channel map files where the offline channel numbering is based on
the soldered endpoints of the wires.  This is natural for making
plots of pedestal and RMS in electronics coordinates, but wires
wrap a bit before they enter the active volume.

v0:  Use for HD coldbox runs between 13672 and 14174, inclusive.
v1:  Use for HD coldbox runs 13368 and prior
v2:  Use for HD coldbox runs 14175 and later, and for ProtoDUNE-HD

Channel map files where the offline channel numbering is based on
the locations of the wires where they emerge from under the head boards.
This corresponds more exactly to the offline geometry definition of
wire endpoints.  To be used for physics analyses.

v3:  Use for HD coldbox runs 14175 and later, and for ProtoDUNE-HD

See the directory "mapmakers" for ROOT macros that create the channel
map txt files.  They are not compiled or installed with the release
of this product, but can be run interactively once the source has
been checked out.
