My Python source code is designed for Sam Gross's implementation of Python. Gross's version of Python eliminates the Global Interpreter Lock, a feature which allows threads to be executed only one at a time. Hence, under Gross's version of Python, we allow threads to be executed simultaneously, enabling true parallelism.

Here are the steps for building and running the source code under the No-GIL environment:

1. Enter your bash terminal.

2. Install Docker if you have not done so already:

	a. Install Docker:
		$ sudo apt-get install docker-engine -y
	b. Start Docker:
 		$ sudo service docker start
	c. Verify Docker:
		$ sudo docker run hello-world

3. Pull Gross's implementation of Python
	$ docker pull nogil/python

4. In the src folder, build and run the Docker image
	$ docker build -t my-python-app .
	$ docker run -it --rm --name shell my-python-app

For more information regarding no-gil, please visit Gross's project page: https://github.com/colesbury/nogil

