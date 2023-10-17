/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 16:16:34 by mlaneyri          #+#    #+#             */
/*   Updated: 2023/08/24 21:34:18 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include <cstdlib>
#include <cerrno>
#include <cstdio>
#include <cstdarg>
#include <cstring>

#define RECV_BUFF_SIZE 2048

#endif
