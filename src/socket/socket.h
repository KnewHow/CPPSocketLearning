#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef SRC_SOCKET_SOCKET_H_
#define SRC_SOCKET_SOCKET_H_

#include <sys/socket.h>

namespace server_client {

/**
 * @brief TCP write, the function will add 4 bytes number in the data head to indicate what size of the data.
 *        So the total length of TCP message is (dataLen + 4) bytes
 * 
 * @param sockfd The socket file descriptor will be used to write
 * @param data The data will be written to socket
 * @param dataLength The bytes of the data. if not assign, the function will use data's size as the length. 
 *                If you write the binary data, you must assign it
 * @return true Write success
 * @return false Write failure
 */
bool TCPWrite(const int sockfd, const char *data, const int dataLength = 0);


/**
 * @brief This function is a pair with above write function. Read data from socket file descriptor.
 *        It will read fourth bytes as the data length, then parse actually data
 * 
 * @param sockfd The socket file descriptor will be used to read
 * @param data The data will be filled from socket stream
 * @param dataLength The size of data 
 * @return true Read success
 * @return false Read failure
 */
bool TCPRead(const int sockfd, char *data, int *dataLength);

/**
 * @brief Read data from socket file descriptor
 * 
 * @param sockfd The socket file descriptor will use to read
 * @param data the data will be read from
 * @param length the total bytes need to read 
 * @return true Read sucesss
 * @return false Read failure
 */
bool read(const int sockfd, char *data, const size_t length);

/**
 * @brief Write data into socket
 * 
 * @param sockfd The socket file descriptor will use to write
 * @param data The data will be writen
 * @param length The bytes of the data
 * @return true Send data success
 * @return false Send data failure
 */
bool write(const int sockfd, const char *data, const size_t length);

} // namespace server_client


#endif // SRC_SOCKET_SOCKET_H_