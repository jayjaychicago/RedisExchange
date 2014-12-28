# Setting up for building C++ engine

## Using bjam

The initial set up uses bjam. This is not ideal since this tool is no
longer getting the love that other tools get. Over time this should be
replaced and the current top contenders are CMake and more recently
Gradle. I intend to upgrade my codegen to try out Gradle.

### Get bjam

This one should work: http://sourceforge.net/projects/boost/files/boost-jam/3.1.18/

* Download the exe for the MAC
* Download the source and build for linux

### Download boost-build

* So, bjam is just an executable, like Make. It processes .jam
  files. But to build a C++ system jam needs lots of bootstrap jam
  files. You can download these from:
  http://prdownloads.sourceforge.net/boost/boost-build-2.0-m12.zip

* Unzip the file:

          $ unzip ~/Downloads/boost-build-2.0-m12.zip
          unzip ~/Downloads/boost-build-2.0-m12.zip
          Archive:  /Users/dbdavidson/Downloads/boost-build-2.0-m12.zip
             creating: boost-build/
             creating: boost-build/test/
             creating: boost-build/test/v1_testing/
             ...

* Backup these two files in that directory. Note user-config.jam is currently empty, but don't worry about that.

         $ mv site-config.jam site-config.jam.bak
         $ mv user-config.jam user-config.jam.bak


* Softlink those required files to the corresponding files in (RedisExchang/exch/config/). So for me I have:

         /Users/dbdavidson/dev/open_source/boost-build/site-config.jam -> /Users/dbdavidson/dev/open_source/RedisExchange/exch/config/site-config.jam
         /Users/dbdavidson/dev/open_source/boost-build/user-config.jam -> /Users/dbdavidson/dev/open_source/RedisExchange/exch/config/user-config.jam

The *site-config.jam* is generated. I've also checked it in and it obviously will differ for you and me since we'll have different paths. So in the future once things are set up this file can just be deleted (since it is generated).

### Set some env variables

These two variables are used by code generation. Currently the build scripts are generated via the codegen, so having these set and regenerating should create the appropriate build scripts

     BOOST_BUILD_PATH=/Users/dbdavidson/install/boost-build
     BOOST_INSTALL_PATH=/usr/local/Cellar/boost/1.56.0


### Install C++ prereques

* boost: I'm using 1.56. On mac you can use brew. On ubuntu you should be able to use apt-get
* cereal: Just get the latest from github. It is nice since it is header only
* redisclient: Just get the latest from github: https://github.com/nekipelov/redisclient (also header only)
* fcs: This is my own codebase (https://github.com/patefacio/fcs). We are using timestamp from it as well as some other stuff. It is header only for now
* cppformat: https://github.com/cppformat/cppformat - this is used for fast formatting of strings

Unfortunately there is one more ugliness in this setup, similar to *site-config.jam*. Another file that is generated is the *RedisExhcange/exch/cpp/Jamfile* which has to refer to the includes and libs of these prereqs (for those that have libs). This is another file I've checked in and will need to figure out a way for one script to satisfy multiple users' environments. This does not work that way yet - but should not be too hard to address. Just go ahead and hand edit the sections in the *// custom begin // custom end * code blocks to point to the appropriate paths. We can pretty it up later. I'm referring to: [includes](https://github.com/patefacio/RedisExchange/blob/master/exch/cpp/Jamfile#L29-L34) and [libs](https://github.com/patefacio/RedisExchange/blob/master/exch/cpp/Jamfile#L45-L47).

# Using Dart for Code Generation

  You'll need to get the code generation up and running. As I
mentioned before at some point you can do a "drop and run" to stop the
using code generation altogether if that is your preference. But for
now just get it set up.

* Get latest version of Dart

* Get latest version of clang-format and put in your path

  This tool is used by code generation to make code consistent and
  follow a coding standard. I'm using version 3.5.

* In Dart Editor load the project (*Open->Existing Folder...* and select the path (*RedisExhcange/exch/cpp_codegen*)

* Run pub on the project (Right click on the pub file in the *ebisu_cpp* project and do *pub get*).

* In the bin folder there is a file called *exch_codegen.dart*. This file is responsible for generating all the source. Right click on it and run it. This is where the fun begins. Make sure you have a good local git tool like magit or SourceTree to easily see what changes. When run the script will tell you what files have changed. If all goes well and your clang-format version is the same as mine, there should be no updates to the code. There will be changes to the *site-config.jam* file. If there are let me know. There may be slight formatting changes.

### Building the server

* Go to the directory where the server is and build with: *bjam --toolset=darwin* for mac or simply *bjam* for ubuntu

        $ cd ~/dev/open_source/RedisExchange/exch/cpp/app/exch_server/
        $ bjam --toolset=darwin
