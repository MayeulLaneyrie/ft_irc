/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chan.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 16:00:00 by mlaneyri          #+#    #+#             */
/*   Updated. 2023/07/13 16:00:00 by mlaneyri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHAN_HPP
#define CHAN_HPP

#include <string>
#include <iostream>

#include <map>
#include <set>

#include "User.hpp"
#include "Msg.hpp"

#define MODE_I 1 // Invite only
#define MODE_T 2 // Topic, only modified by op
#define MODE_K 4 // Password enabled
#define MODE_L 8 // Limit enabled

class User;
class Msg;

class Chan {

	private : // PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE

		std::map<str, User *> _users; // indexed by their nicks
		std::set<User *> _operators;
		std::set<User *> _invited; 

		unsigned int _mode;
		str _name;
		str _passwd;
		str _topic;

		int _usercount;
		int _usermax;

		Serv * _serv;

		Chan(void);

	public : // PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC PUBLIC P

		Chan(Serv * serv, str const & name);
		Chan(Chan const & src);
		~Chan(void);

		Chan & operator=(Chan const & rhs);

		int chan_send(User * source, Msg const & msg);

		void addUser(User * user);
		void rmUser(User * user);
		User * getUser(str const & nick) const;
		void renameUser(User * user, str const & to);

		std::map<str, User *>::const_iterator begin(void) const;
		std::map<str, User *>::const_iterator end(void) const;

		void addOperator(User * user);
		void rmOperator(User * user);
		int isOperator(User * user);

		void invite(User * user);
		void uninvite(User * user);
		int isInvited(User * user);

		void setPasswd(str const & passwd);
		int checkPasswd(str const & passwd);

		unsigned int checkMode(unsigned int mode);
		void setMode(unsigned int mode, int val);

		void setTopic(str topic);
		str getTopic(void) const;

		str getName(void) const;
		int isFull(void);
};

#endif
