#ifndef INC_CLI_H_
#define INC_CLI_H_
#include <stdio.h>
#include <String.h>
#include "main.h"

class Cli{
public:
	virtual void doCommand(const char * param) = 0;

	virtual ~Cli(){}
};

struct CliCommandEntry {
	const char * CommandName;
	Cli* command;
};

class CliContainer{
private:
	CliCommandEntry commands[20];
	int funcounter = 0;
public:
	CliContainer(){}
	~CliContainer(){}
	void registerCommand(const  char* CommandName, Cli * clicommand) {
		commands[funcounter].CommandName = CommandName;
		commands[funcounter].command = clicommand;
		funcounter++;
	}

	void doCommand( char * cmd,  char * param){
		for (int j = 0; j <funcounter; j++) {
			if (strcmp(cmd,commands[j].CommandName) == 0) {
				commands[j].command->doCommand(param);
				return;
			}

		}
		printf("Invalid command\r\n");
	}
	void initCliContainer();
};


#endif /* INC_CLI_H_ */
