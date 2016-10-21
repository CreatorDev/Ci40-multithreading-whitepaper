#install gcc cross compiler


echo "Downloading mips linux compiler"
MACHINE_TYPE=`uname -m`
if [ ${MACHINE_TYPE} == 'x86_64' ]; then
  wget http://codescape-mips-sdk.imgtec.com/components/toolchain/2016.05-03/Codescape.GNU.Tools.Package.2016.05-03.for.MIPS.MTI.Linux.CentOS-5.x86_64.tar.gz
else
  wget http://codescape-mips-sdk.imgtec.com/components/toolchain/2016.05-03/Codescape.GNU.Tools.Package.2016.05-03.for.MIPS.MTI.Linux.CentOS-5.x86.tar.gz
fi



echo "Extracting mips-linux-gnu-gcc"

tar -xvf Codescape.GNU.Tools.Package.*.tar.gz 
