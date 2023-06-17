# Welcome to Advanced Programming!

## Part 1 - Logging into the AP Server

First, follow the instructions [here](https://github.com/cs3157-borowski/guides/blob/main/login.md) to
make sure you've successfully logged into the AP Server and have completed the guide for UNIX setup.

## Part 2 - Setting up Git on the Server

The following steps should all be done within the AP server. Refer to the guide from part 1 to log into the class server. Before completing these steps, make sure that you already have a GitHub [account](https://github.com/). 

Verify that your terminal shows

	<YOUR_UNI>@ap:~$

The following steps will generate a new SSH key so that you can use your GitHub account directly from the AP Server. ***You must follow these steps exactly.***

1. Generate a new SSH key. Substitute the email address with the email address for your GitHub account. *Note: this is not necessarily your school email*.

		ssh-keygen -t ed25519 -C "your_email@example.com"

	When you're prompted to "Enter the file in which to save the key", press **Enter** to accept the default file location.

	When you're prompted to enter a passphrase, you can also press **Enter** for an empty passphrase.

2. Start the ssh-agent in the background.

		eval "$(ssh-agent -s)"

3. Add the new SSH key to the ssh-agent.

		ssh-add ~/.ssh/id_ed25519

4. Follow the instructions [here](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/adding-a-new-ssh-key-to-your-github-account) to add the new SSH key to your GitHub account. The type of key should be an authentication key. You can title this key AP.

5. Configure git locally. Replace the email with the email you used for your GitHub account and name with your name.

		git config --global user.email "you@example.com"
		git config --global user.name "Your Name"

To verify that the previous steps have been done correctly:

1. Click the top right profile button and click settings.

2. Go to SSH and GPG keys on the Left Navigation bar.

3. Verify that you see AP on the list of SSH keys (or a different name if you named your key something different).

You should only have to complete the above steps once for the entirety of the semester.

## Part 3 - Homework Workflow

Now, we will clone the repo to the AP Server so you can work on the assignment directly there. Make sure that your terminal shows up as

	<YOUR_UNI>@ap:~$

This is your 'command-prompt'. If your command prompt doesn't match the one above, SSH into the server [again](https://github.com/cs3157-borowski/guides/blob/main/login.md. Then, do the following steps to clone your assignment repo.

1. Create a new directory using the 'mkdir [options] [directory name]' command. This directory is for all your assignments. 

		mkdir ~/cs3157

2. Change your working directory to that new directory.

		cd ~/cs3157
		
Your command-prompt should now be 'YOUR_UNI@ap:`/cs3157$' which indicates that you are now inside your new 'cs3157' directory. You should only have to make this directory once for the entire semester. The following section will describe steps of the homework development workflow that will need to be done for every assignment.

## Part 4 - Cloning the Repo

1. At the start of each assignment, you will git clone the skeleton repository. For hw0, the repo name will be hw0-{username} where the username will be your GitHub username. You can also copy-paste the repo name from the GitHub directory.

		git clone git@github.com:cs3157-borowski-s23/hw0-{username}.git ~/cs3157/hw0

	When it asks if you're sure you want to continue connecting, type yes and press enter.

2. Change your working directory to the homework repo.

		cd hw0

3. Verify that the contents of the folder are correct. The output of the command in the first line should match the second line.

		<UNI>@ap:~/cs3157/hw0-{git-username}$ ls
		instructions.md

If you encounter any issues while doing this setup, reach out to the teaching staff on EdStem.

To learn more about how to use Git, read through the following [tutorial](https://github.com/cs3157-borowski/guides/blob/main/git.md).

## Part 5 - GitHub Username

Fill out [this form](https://forms.gle/qfMjX7QYvieocb2p7) with your UNI and GitHub username so we can invite you to the cs3157-borowski-s23 organization on GitHub. All assignments will be hosted there.

## Part 6 - Partner Selection

Fill out [this form](https://docs.google.com/forms/d/e/1FAIpQLSeKnHX0ZHzcFD7P-9vIAyIxeEKb9gIog5J_i77Y9LMXtLI_wA/viewform) to submit your team (*this should only be submitted once per pair*). If you want to work alone, you must still submit the form, leaving the second field blank. If you would rather have a randomly assigned teammate, please write "random" on the second field.


## Part 7 - Written Response

Create a file called `written.txt`. If you're on the command line, you can create one with vim as follows:

	vim written.txt

Your file should be in the following format

	Name: 
	
	Uni:

	Major, School, Year:
		- Ex: Computer Science, SEAS, 2025

	Anything you want us to know:


To submit:
	
	git add written.txt
	git commit -m "hw0 completed"
	git tag -a handin -m "hw0 submission"
	git push origin main
	git push origin handin

Check the code tagged as "handin" on GitHub to ensure that it is your final submission.

If you want to submit a new version of your assignment, you have to delete the old tag first as follows.

	git push --delete origin handin
	git tag -d handin

***Important**: We will use the timestamp of the last **handin** tag for grading and late days. Additionally, we will know if you backdate the tag and consider it as a case of Academic Dishonesty.*


### Acknowledgments

Parts of this guide were originally written by John Hui and GitHub. This guide was adapted by the following TAs for Spring 2023.
- Mateo Maturana
- Phillip Le
- Claire Liu
