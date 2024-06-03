As part of the course "Mathematics with a Computer", I am implementing the Functional encryption scheme for computing inner products by Abdalla, Bourse, De Caro and Pointcheval that is based on the DDH assumption.
The group of choice is the quadratic residue subgroup modulo a safe-prime and the programming lanugage of choice is C++ using the Crypto++ library.

The files for the scheme are in the scheme_files folder. In order to use the scheme, one needs to have the libsosdium and cryptopp libraries installed. Then download the files from scheme_files and open a terminal inside the folder.
For Linux and MACOS users, run via the terminal the following commands:

g++ scheme.cpp MasterSecretKey.cpp MasterPublicKey.cpp KeyPair.cpp  my_utilities.cpp -lsodium -lcryptopp -lpthread

./a.out

For Windows users, the ouput file should be a.exe instead.

The function "main" is located in scheme.cpp, which is run. The scheme was tested on a popOS, which is a Ubuntu-based distribution, and works correctly.
