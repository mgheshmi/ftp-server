#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <fcntl.h>
#include <dirent.h>
#include <filesystem>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <thread>


int dataSocket;
struct sockaddr_in clientAddr;
int dataPort;
char ip[16];
std::string current_path;

void send_response(int client_socket, const std::string &response) {
    int result = send(client_socket, response.c_str(), response.size(), 0);
    return;
}

void handle_client(int controlSocket) {
    send_response(controlSocket, "220 Welcome to My FTP Server\r");
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));

        int bytesRead = read(controlSocket, buffer, sizeof(buffer));
        if (bytesRead <= 0) {
            break; 
        }
        buffer[bytesRead] = '\0'; 
        std::cout << "Received command: " << buffer << std::endl;

        if(strncmp(buffer, "USER", 4) == 0) {
            send_response(controlSocket, "331 User name okay, need password\n");
        } else if(strncmp(buffer, "PASS", 4) == 0) {
            send_response(controlSocket, "230 User logged in, proceed\n");
        } else if(strncmp(buffer, "PWD", 3) == 0) {
            send_response(controlSocket, "257 \"" + current_path + "\" is current directory.\r\n");
        } else if(strncmp(buffer, "TYPE I", 6) == 0) {
            send_response(controlSocket, "200 TYPE set to I.\r\n");
        } else if(strncmp(buffer, "TYPE A", 6) == 0) {
            send_response(controlSocket, "200 TYPE set to A.\r\n");
        } else  if (strncmp(buffer, "PORT", 4) == 0) {
            char* p = strtok(buffer + 5, ",");
            
            int port1, port2;
            if (p) {
                snprintf(ip, sizeof(ip), "%s.%s.%s.%s", p, strtok(NULL, ","), strtok(NULL, ","), strtok(NULL, ","));
                port1 = atoi(strtok(NULL, ","));
                port2 = atoi(strtok(NULL, ","));
                dataPort = (port1 << 8) + port2; // Combine the two bytes to form the port number

                // Prepare to connect to client's data port
                dataSocket = socket(AF_INET, SOCK_STREAM, 0);
                
                memset(&clientAddr, 0, sizeof(clientAddr));
                clientAddr.sin_family = AF_INET;
                inet_pton(AF_INET, ip, &clientAddr.sin_addr);
                clientAddr.sin_port = htons(dataPort);
                
                if (connect(dataSocket, (struct sockaddr*)&clientAddr, sizeof(clientAddr)) < 0) {
                    std::cout << "Failed to connect to client data port." << std::endl;
                    continue;
                }

                send_response(controlSocket, "200 PORT command successful.\r");

                
            }

        } else if (strncmp(buffer, "LIST", 4) == 0) {

            DIR* dir2 = opendir(current_path.c_str());
            if (dir2 == nullptr) {
                std::cerr << "Could not open directory" << std::endl;
                
                return;
            }
            closedir(dir2);

            send_response(controlSocket, "150 Opening ASCII mode data connection for file list\r\n");


            DIR *dir = opendir(current_path.c_str());
            if (!dir) {
                perror("opendir");
                return;
            }

            struct dirent *entry;
            while ((entry = readdir(dir)) != nullptr) {
                if (entry->d_name[0] == '.') continue; // Skip hidden files
                
                std::string fullPath = current_path + "/" + entry->d_name;
                struct stat fileStat;
                
                if (stat(fullPath.c_str(), &fileStat) == -1) {
                    perror("stat");
                    continue;
                }
                
                std::string fileName = entry->d_name;
                
                off_t fileSize = fileStat.st_size;
                
                char timeBuffer[40];
                struct tm *timeinfo = localtime(&fileStat.st_mtime);
                //strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", timeinfo);
                strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M", timeinfo);
                
                std::string permissions = (S_ISDIR(fileStat.st_mode) ? "d" : "-");
                permissions += (fileStat.st_mode & S_IRUSR ? "r" : "-");
                permissions += (fileStat.st_mode & S_IWUSR ? "w" : "-");
                permissions += (fileStat.st_mode & S_IXUSR ? "x" : "-");
                permissions += (fileStat.st_mode & S_IRGRP ? "r" : "-");
                permissions += (fileStat.st_mode & S_IWGRP ? "w" : "-");
                permissions += (fileStat.st_mode & S_IXGRP ? "x" : "-");
                permissions += (fileStat.st_mode & S_IROTH ? "r" : "-");
                permissions += (fileStat.st_mode & S_IWOTH ? "w" : "-");
                permissions += (fileStat.st_mode & S_IXOTH ? "x" : "-");
                
                struct passwd *pw = getpwuid(fileStat.st_uid);
                struct group  *gr = getgrgid(fileStat.st_gid);
                
                std::ostringstream response;
                response << permissions << " "
                        << pw->pw_name << " "
                        << gr->gr_name << " "
                        << fileSize << " "
                        << timeBuffer << " "
                        << fileName;

                // std::cout << response.str() << std::endl; 
                send(dataSocket, response.str().c_str(), response.str().size(), 0);

                //std::cout << "obj name:1 " << " " << obj.path().filename() << std::endl;
                const char* newline = "\n";
                send(dataSocket, newline, strlen(newline), 0); 
            }
            
            closedir(dir);

            close(dataSocket); 
            
            send_response(controlSocket, "226 Directory send OK\r\n");



        } else if (strncmp(buffer, "CDUP", 4) == 0) {
            std::filesystem::path current_directory = current_path; //std::filesystem::current_path();
            std::filesystem::path parent_path = current_directory.parent_path();
        
            if (parent_path == current_directory) {
                send_response(controlSocket, "550 Failed to change directory: already at root directory.\r\n");
                return;
            }
            else {
                if (chdir(parent_path.c_str()) == 0) {
                    current_directory = parent_path;
                    current_path = current_directory.string();
                    send_response(controlSocket, "250 Directory changed to " + current_directory.string() + "\r\n");
                } else {
                    send_response(controlSocket, "550 Failed to change directory.\r\n");
                }
            }

        } else if (strncmp(buffer, "CWD", 3) == 0) {
            //std::cout << "size of buffer = " << sizeof(buffer) << std::endl;
            std::string bufferstring = std::string(buffer);
            //strncpy(tmp, buffer,bytesRead - 4);CWD /home/mahdi\r\n
            std::string recv_path = bufferstring.substr(4,bytesRead - 2 - 4);
            if(recv_path == "..")
                recv_path = current_path + "/" + recv_path;
            DIR* recv_path_dir = opendir(recv_path.c_str());
            if (recv_path_dir == nullptr) {
                std::cerr << "Could not open directory: " << recv_path << std::endl;
                recv_path = current_path + "/" + recv_path;
                recv_path_dir = opendir(recv_path.c_str());
                if (recv_path_dir == nullptr) {
                    std::cerr << "Could not open directory: " << recv_path << std::endl;
                    return;
                }
            }
            current_path = recv_path;
            std::cout << "current_path = " << current_path << std::endl;

            send_response(controlSocket, "250 Directory changed to \"" + current_path + "\".\r\n");
        } else if (strncmp(buffer, "RETR", 4) == 0) {
            std::string bufferstring = std::string(buffer);
            std::string filename = bufferstring.substr(5,bytesRead - 2 - 5);
            filename = current_path + "/" + filename;
            int file_descriptor = open(filename.c_str(), O_RDONLY);
            if (file_descriptor < 0) {
                send_response(controlSocket, "550 Failed to open file.\n");
                return;
            }
            
            send_response(controlSocket, "150 Opening data connection.\n");

            char filebuffer[1024];
            ssize_t bytes_read;
            while ((bytes_read = read(file_descriptor, filebuffer, sizeof(filebuffer))) > 0) {
                send(dataSocket, filebuffer, bytes_read, 0);
            }

            close(file_descriptor);
            close(dataSocket); 

            send_response(controlSocket, "226 Transfer complete.\n");

        } else if (strncmp(buffer, "STOR", 4) == 0) {
            std::string bufferstring = std::string(buffer);
            std::string filename = bufferstring.substr(5,bytesRead - 2 - 5);
            filename = current_path + "/" + filename;
            int file_descriptor = open(filename.c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
            if (file_descriptor < 0) {
                send_response(controlSocket, "550 Failed to open file.\n");
                return;
            }
            
            send_response(controlSocket, "150 Opening data connection.\n");

            char filebuffer[1024];
            ssize_t bytes_received;
            while ((bytes_received = recv(dataSocket, filebuffer, sizeof(filebuffer), 0)) > 0) {
                write(file_descriptor, buffer, bytes_received);
            }

            close(file_descriptor);
            close(dataSocket); 

            send_response(controlSocket, "226 Transfer complete.\n");

        } else if (strncmp(buffer, "MKD", 3) == 0) {
            std::string dirname = (std::string(buffer)).substr(4,bytesRead - 2 - 4);
            dirname = current_path + "/" + dirname;
            int mkdir_result = mkdir(dirname.c_str(), 0755);
            if(mkdir_result == 0)
                send_response(controlSocket, "257 \"" + dirname + "\" created.\r\n");
            else
                send_response(controlSocket, "550 failed to create directory.\r\n");
                
        } else if (strncmp(buffer, "DELE", 4) == 0) {
            std::string delpath = (std::string(buffer)).substr(5,bytesRead - 2 - 5);
            delpath = current_path + "/" + delpath;
            
            int remove_result = std::remove(delpath.c_str());
            if(remove_result == 0)
                send_response(controlSocket, "250 \"" + delpath + "\" deleted\r\n");
            else
                send_response(controlSocket, "500 Error \r\n");
                
        } else if (strncmp(buffer, "RMD", 3) == 0) {
            std::string delpath = (std::string(buffer)).substr(4,bytesRead - 2 - 4);
            delpath = current_path + "/" + delpath;
            
            int remove_result = std::filesystem::remove_all(delpath.c_str());
            if(remove_result > 0)
                send_response(controlSocket, "250 \"" + delpath + "\" deleted\r\n");
            else
                send_response(controlSocket, "550 Error \r\n");
                
        } else if (strncmp(buffer, "QUIT", 4) == 0) {
            std::cout << "Client requested to quit.\r\n" << std::endl;
            break;
        } else {
            const char* response = "502 Command not implemented.\r";
            write(controlSocket, response, strlen(response));
        }
    }
}

int start_server(int port) {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if(bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
        return -1;
    if(listen(serverSocket, 5) < 0)
        return -2;
    return serverSocket;
}



int main(int argc, char* argv[]) {
    int port_number = atoi(argv[1]);
    current_path = argv[2];
    int serverSocket = start_server(port_number); 
    if(serverSocket > 0)
        std::cout << "FTP Server started on port " << port_number << ". Server files is hosted on '" << current_path << "' ... " << std::endl;
    else
        return 0;

    while (true) {
        struct sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        int controlSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
        if (controlSocket >= 0) {
            std::cout << "Client connected." << std::endl;
            std::thread client_thread(handle_client, controlSocket);
            client_thread.detach(); 
        
        }
    }

    close(serverSocket);
    return 0;
}
