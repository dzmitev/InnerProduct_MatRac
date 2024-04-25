As part of the course "Mathematics with a Computer", I am implementing the Functional encryption scheme for computing inner products by Abdalla, Bourse, De Caro and Pointcheval that is based on the DDH assumption.
The group of choice is the quadratic residue subgroup modulo a safe-prime and the programming lanugage of choice is C++ using the Crypto++ library.

In order to run the scheme, one needs to have all files in the same folder. Then run via the terminal the following commands:

g++ scheme.cpp MasterSecretKey.cpp MasterPublicKey.cpp KeyPair.cpp  my_utilities.cpp -lsodium -lcryptopp -lpthread
./a.out

The function "main" is located in scheme.cpp, which is run.
