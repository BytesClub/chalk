===============
Sending Patches
===============

After making the changes in the source code follow the following steps.

	.. code-block:: console

		$ git add <files>

	.. code-block:: console

		$ git commit --signoff -v

	- add the commit message

- To create a patch 
		.. code-block:: console

			$ git format-patch master..<branch-name> 

		- It will create a patch file
- To send mail 
		.. code-block:: console

			- git send-mail --to <receiver's mail-id> --cc < mail=id > < name of patch file created >
	 

