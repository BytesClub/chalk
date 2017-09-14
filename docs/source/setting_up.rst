=====================
Set Up
=====================

Set up git-email
-------------------
- Enable IMAP and POP

 	- Open your mail in a browser.
	- Go to the settings > Forwarding > POP/IMAP.
	- Enable IMAP and POP.

- Set up your terminal.
	
	.. code-block:: console
	
		$ git config --global sendemail.from "YOUR NAME <user@example.org>"

	.. code-block:: console

		$ git config --global sendemail.smtpserver imap.example.org
	
	- imap.example.org will be smtp.googlemail.com for gmail

	.. code-block:: console	

		$ git config --global sendemail.smtpuser USER

	- Enter your email-id in place of USER.

	.. code-block:: console

		$ git config --global sendemail.smtpencryption tls

	.. code-block:: console

		$ git config --global sendemail.smtppass = PASS

- check you settings.

	.. code-block:: console

		$ vi ~/.gitconfig 

	- ( It will open a new file)
	- if not available add :-
		- smtpserverport = 587
		- suppresscc = self

Set up  vimrc
-------------------

	.. code-block:: console
		
		$ vim ~/.vimrc

	- It will open a new file, add below given statements to that file:-
		- set tabstop=8
		- set shiftwidth=8
		- set expandtab


	
		

