#ifndef INC_CLI_H_
#define INC_CLI_H_

class Cli{
public:
	virtual void doCommand(const char * param) = 0;

	virtual ~Cli(){}
};

void initCLI();

#endif /* INC_CLI_H_ */
