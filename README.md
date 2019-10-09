# G4Arich
Geant4 based simulation of aerogel ring imaging chereknov detector

Compiled against root 6.12/06 and geant4.10.04.p02

To build initialize environment with setup.sh in geant4 directories, and thisroot.sh in root directories.

> mkdir G4Arich-build
> cd G4Arich-build
> ccmake ../Geant4_ARICH/
// change any settings in cmake as needed
> make -j4

Some geometry and settings of materials can be found in the file arich-geom.txt:
hasmirror true
mirrorlength 80.0 mm

Additional settings should be added there to do further studies.

An analysis of the output data is in the directory analysis/. To build the analysis code:

> cd analysis
> make

The analysis code needs root, so you will have to have the environment set up by running thisroot.sh (from your root installation).






