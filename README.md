This assignment in C++ is a program that simulates “Food Warehouse management system”. The program opens the warehouse, assigns volunteers and customers,assigns orders, handles them, performs steps in the simulation, and more such actions. The program handle memory in C++ using “Rule of 5”. The program receives the path of the config file as the first command-line argument: string configurationFile=argv[1] The config file describes the initial state of the warehouse before we start the simulation. It contains info of the initial customers and volunteers. Once the program starts, it initializes the warehouse according to a given config file, and then starts the simulation by calling the start() method of Warehouse, followed by printing to console: “Warehouse is open!“ . Then, the program waits for the user to enter an action to execute. After each executed action, the program waits for the next action in a loop.
In the project you can find two config files and two pdf files with input and output examples, There is also a make file.
