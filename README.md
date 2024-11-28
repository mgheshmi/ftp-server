# ftp-server
An FTP server program in the Linux environment, written in C++ and implementing most of the functions of the FTP protocol.
After building the project with CMake, run the ftp_server executable file from the build folder as follows in the terminal:

./ftp_server port_number ftp_files_host_path

For example: ./ftp_server 21 ~/Desktop/ftp_files/

You can use any ftp client software such as FileZilla to connect to the ftp server.