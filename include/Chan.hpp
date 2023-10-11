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

		typedef std::map<str, User *>::iterator iterator;

		iterator begin(void);
		iterator end(void);

		void opMode(User * user, int val);
		int isOperator(User * user) const;

		void invite(User * user);
		void uninvite(User * user);
		int isInvited(User * user) const;

		void setPasswd(str const & passwd);
		int checkPasswd(str const & passwd) const;
		str getPasswd( void ) const;

		unsigned int checkMode(unsigned int mode) const;
		void setMode(unsigned int mode, int val);
		unsigned int getMode( void ) const;

		void setTopic(str topic);
		str getTopic( void ) const;

		str getName(void) const;

		void setLimit(int limit);
		int getLimit( void ) const;
		int isFull( void ) const;
};

#endif
