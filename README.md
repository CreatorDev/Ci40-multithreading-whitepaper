#Ci40 Multithreading Whitepaper

The [Ci40 multithreading Whitepaper](https://github.com/CreatorDev/Ci40-multithreading-whitepaper/blob/master/MIPS_Creator_Ci40.Multithreading_Benchmarks.pdf) has been put together as a top level review of multi threading with a MIPS processor in the cXT200 device on the Creator Ci40 development board. cXT200 is a dual core, dual threaded MIPS interAptiv core clocked at 550MHz.  Source for the examples discussed in the whitepaper can be found in the ci40-MT directory.

For discussion on this topic please use the thread on our forum: [https://forum.creatordev.io/t/new-ci40-multithreading-whitepaper-and-source-available](https://forum.creatordev.io/t/new-ci40-multithreading-whitepaper-and-source-available)

Setup
______
1. Give execute permission to setup script(under ci40-MT folder) using 'chmod +x setup.sh'
2. Run the setup.sh script provided (./setup.sh)
3. This should install the MIPS Codescape Toolchain
4. Then run the make file in the respective folders(ci40-MT/accumulator/, ci40-MT/iot/)

