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

v3 is a channel map file where the offline channel numbering is based on
the locations of the wires where they emerge from under the head boards.
This corresponds more exactly to the offline geometry definition of
wire endpoints.  To be used for physics analyses.

v3:  Use for HD coldbox runs 14175 and later, and for ProtoDUNE-HD,
but see below about the APA numbering.  v2 and v3 also have a bug
swapping APA 3 and APA 4.

v2 and v3 assume APA numbering as given at the 
May 2022 collaboration meeting in a talk by V. Tishchenko:

APA3                 APA4		 
APA_P02NL	     APA_P01NL	 
FEMBs 1-20	     FEMBs 1-20	 
TPS1		     TPS3		 
TPC 2 (3)	     TPC 6 (7)	 
1st channel: 2560    1st channel: 7680

APA2                 APA1		   
APA_P02SU	     APA_P01SU	   
FEMBs 1-20	     FEMBs 1-20	   
TPS0		     TPS2		   
TPC 1 (0)	     TPC 5 (4)	   
1st channel: 0 	     1st channel: 5120  

The APA numbers in the first line are assumed to match the crate 
numbers in the WIB frames.

---------------------------------------------------

Note as of November 21, 2022:    Roger Huang
gave the final APA numbering on Nov. 21, 2022. It is now:

APA3                 APA4		 
APA_P02NL	     APA_P01NL	 
FEMBs 1-20	     FEMBs 1-20	 
TPS1		     TPS3		 
TPC 2 (3)	     TPC 6 (7)	 
1st channel: 2560    1st channel: 7680

APA1                 APA2		   
APA_P02SU	     APA_P01SU	   
FEMBs 1-20	     FEMBs 1-20	   
TPS0		     TPS2		   
TPC 1 (0)	     TPC 5 (4)	   
1st channel: 0 	     1st channel: 5120  

v4:  non-rotated (like v2, but with APAs 1 and 2 swapped)

v5:  rotated     (like v3, but with APAs 1 and 2 swapped)

v5 is meant to be used for physics reco.  v4 may be more useful
for understanding channel numbering plots for debugging.  v4 lists
channels by their anchor points, v5 where they emerge from under the
head boards.

See the directory "mapmakers" for ROOT macros that create the channel
map txt files.  They are not compiled or installed with the release
of this product, but can be run interactively once the source has
been checked out.

Here are the column names for the channel map text files:

offlchan crate APAname wib link femb_on_link cebchan plane planechan femb asic asicchan wibframechan

The APAname strings follow this convention:

APA_P[nn][N,S][U,L] 

  Where "P" means ProtoDUNE
  nn is the row number (01 or 02)
  N: North, S: South
  U: Upper, L: Lower 

In ProtoDUNE-HD, the North APAs are Lower (inverted), and the South APAs are Upper (upright).

