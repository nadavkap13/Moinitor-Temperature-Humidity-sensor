#ifndef INC_COMTASK_H_
#define INC_COMTASK_H_
#include "Cli.h"
#include "string.h"
#include "stdio.h"


class clicommand_entry {
public:
	char* CommandName;
	Cli* command;
};

class CliContainer{
private:
	clicommand_entry * commands[20];
	int funcounter = 0;
public:
	~CliContainer(){}
	void RegisterCommand(char* CommandName, Cli * clicommand) {
		commands[funcounter]->CommandName = CommandName;
		commands[funcounter]->command = clicommand;
		funcounter++;
	}

	void doCommand( const char * cmd, const char * param){
		for (int j = 0; j <funcounter; j++) {
			if (strcmp(cmd,commands[j]->CommandName) == 0) {
				commands[j]->command->doCommand(param);
				return;
			}
			printf("Invalid command\r\n");
		}
	}
};
int commTask();
void handleCommand();

//void RegisterCallbacks(HandlerFunc onFunc, HandlerFunc offFunc,void* object);

#endif /* INC_COMTASK_H_ */
