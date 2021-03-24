#ifndef __NMSHELL_H__
#define __NMSHELL_H__

/**
 * @brief Runs a N.M's Shell application to execute user's commands
 * @details How to use the NMShell:
           - Use "q" / "quit" / "exit" commands to exit the NMShell
           - Use "h" / "help" commands to show help message of the NMShell
           - All commands and their arguments should be space separated
           - Maximum input length is (128*1024) = 131072 characters
           - Maximum args count is 128
 *
 * @return int - 0, on success / 1, on failure
 */
int RunNMShell(void);

#endif /* #ifndef __NMSHELL_H__ */
