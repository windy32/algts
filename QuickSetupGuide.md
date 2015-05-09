## Quick guide to setup the algts testbed ##

The algts tutorial document gives detailed description of its usage. It is recommended to read it carefully before going on. However, for those who are impatient and just wants to get the testbed start working quickly, this wiki page may just be the right thing you are looking for.

### Step 1. Prepare the host OS ###

The algts testbed requires a Linux operating system as the host. The author uses the old Ubuntu 10.04 with the 3.0 kernel, as it's fast and stable.

Before going on, install the following packages:

```

sudo apt-get install subversion
sudo apt-get install sshpass
sudo apt-get install uml-utilities
sudo apt-get install libqt4-dev```

Note: the **tunctl** program in the uml-utilities package is used to create the virtual ethernet adapters used in the testbed, **sshpass** is used for automatic terminal operations, and **libqt4-dev** is necessary when building algts.

Then, download a script called **setup-tap.sh** from [http://algts.googlecode.com/files/setup-tap.sh](http://algts.googlecode.com/files/setup-tap.sh).

### Step 2. Install and start virtual machines ###

Go to Virtualbox's download page, [https://www.virtualbox.org/wiki/Downloads](https://www.virtualbox.org/wiki/Downloads), and install the lastest version for Ubuntu 10.04 LTS ("Lucid Lynx").

Note: the author used Virtual 4.3.6 when writing this wiki page. If you find any trouble in the later steps, try installing the old 4.3.6.

We need one Linux host, plus two virtual machines, one as the router, and the other as the server. Download the prepared virtualbox images from [http://algts.googlecode.com/files/Voyage Router 0.9.2.tar.xz](http://algts.googlecode.com/files/Voyage%20Router%200.9.2.tar.xz) and [http://algts.googlecode.com/files/Voyage Server 0.9.2.tar.xz](http://algts.googlecode.com/files/Voyage%20Server%200.9.2.tar.xz).

Create a folder named **"VirtualBox VMs"** in the home folder, and extract the xz files to the created folder. Then double click on the .vbox files and add the prepared virtual machine to VirtualBox.

Before starting the virtual machines, create the virtual ethernet adapters by typing:

```

cd ~
sudo ./setup-tap.sh```

Start the two virtual machines, and login in as root. The default password is **"voyage"**.

Finally, you should allocate some IP addresses, start the server daemon, and start the emulator daemon in the server host. These steps are merge into a single script, simply execute:

```

./init.sh```

### Step 3. Download and build the latest development version of algts ###

Type the following commands:

```

cd ~
mkdir algts
cd algts
svn co https://algts.googlecode.com/svn/trunk/ .
cd src
./configure
./make```

Now algts should have been successfully built.

### Step 4. Execute a test script ###

Before going on, ensure that the ssh terminal is working correctly. Do this by typing the following command:

```

ssh root@172.16.0.1```

You'll see something like the following:

<pre>
The authenticity of host '172.16.0.1 (172.16.0.1)' can't be established.<br>
RSA key fingerprint is 96:fa:c5:10:0c:3c:48:fd:78:99:4f:fa:b4:6c:ff:a1.<br>
Are you sure you want to continue connecting (yes/no)?</pre>

Enter "yes", and log into the router, then exit.

Switch into the test-scripts folder, there should have been a number of script files written in C++, choose one, e.g., sample.cpp and execute with the algts-script-helper:

```

cd test-scripts
./script --run sample "172.16.0.16/28 10.0.0.1 3200"```

<font color='#E6041C'><b>Note:</b></font> always remember to update **serverd** and **emulatord** to the latest development version.

<font color='#E6041C'><b>Warning:</b></font> experiment results based on virtual machines may be quite inaccurate, use at your own risk!