/*************************************
 * wyshell.c
 * Author: Lucas Manker
 * Date: 10 April 2020
 *
 * scanner for 3750 wyshell project.
 * Parts of this code graciously provided by Dr. Buckner
 ************************************/

#include<stdio.h>
#include"wyscanner.h"


int main()
{
  char *tokens[]={ "QUOTE_ERROR", "ERROR_CHAR", "SYSTEM_ERROR",
                "EOL", "REDIR_OUT", "REDIR_IN", "APPEND_OUT",
                "REDIR_ERR", "APPEND_ERR", "REDIR_ERR_OUT", "SEMICOLON",
                "PIPE", "AMP", "WORD" };
  int rtn;
  char *rpt;
  char buf[1024];
  char *start = ":--:";
  char *inner = " --:";
  int command = 0;
  int redirCount = 0;
  int redirIcount = 0;
  int errorOnLine = 0;
  int expFile = 0;

  while(1) {
    printf("$> ");
    errorOnLine = 0;
    redirCount = 0;
    redirIcount = 0;
    command = 0;
    rpt=fgets(buf,256,stdin);
    if(rpt == NULL) {
      if(feof(stdin)) {
	return 0;
      }
      else {
	perror("fgets from stdin");
	return 1;
      }
    }
    rtn=parse_line(buf);
    while(rtn !=  EOL && !errorOnLine){
      switch(rtn) {
        case WORD:
          if(expFile){
              expFile = 0;
          }
          if(!command){
            printf("%s %s\n",start,lexeme);
            command = 1;
          }
          else{
              printf("%s %s\n",inner,lexeme);
          }
          break;
        case SEMICOLON:
          if(expFile){
              printf("missing filename after redirection");
              errorOnLine = 1;
              break;
          }
          printf(" ;\n");
          command = 0;
          redirCount = 0;
          redirIcount = 0;
          break;
        case PIPE:
          if(expFile){
              printf("missing filename after redirection");
              errorOnLine = 1;
              break;
          }
          if(!command){
              printf("syntax error near '|'");
              errorOnLine = 1;
              break;
          }
          printf(" |\n");
          command = 0;
          redirCount = 0;
          redirIcount = 0;
          break;
        case AMP:
          if(expFile){
              printf("missing filename after redirection");
              errorOnLine = 1;
              break;
          }
          printf(" &\n");
          break;
        case REDIR_OUT:
          if(!command){
              printf("error near '>'");
              errorOnLine = 1;
              break;
          }
          if(expFile){
              printf("missing filename after redirection");
              errorOnLine = 1;
              break;
          }
          if(redirCount >= 1){
              printf("Ambiguous output redirection");
              errorOnLine = 1;
              break;
          }
          else{
              printf(" >\n");
              redirCount = 1;
              expFile = 1;
              break;
          }
        case REDIR_IN:
          if(!command){
              printf("error near '<'");
              errorOnLine = 1;
              break;
          }
          if(expFile){
              printf("missing filename after redirection");
              errorOnLine = 1;
              break;
          }
          if(redirIcount >= 1){
              printf("Ambigous input redirection");
              errorOnLine = 1;
              break;
          }
          else{
              printf(" <\n");
              redirIcount = 1;
              expFile = 1;
              break;
          }
        case APPEND_OUT:
           if(expFile){
              printf("missing filename after redirection");
              errorOnLine = 1;
              break;
          }
          printf(" >>\n");
          break;
        case ERROR_CHAR:
          printf("error char: %d",error_char);
          errorOnLine = 1;
          break;
        case QUOTE_ERROR:
          printf("quote error");
          errorOnLine = 1;
          break;
        case APPEND_ERR:
          if(expFile){
              printf("missing filename after redirection");
              errorOnLine = 1;
              break;
          }
          printf(" 2>>\n");
          break;
        case REDIR_ERR_OUT:
          if(expFile){
              printf("missing filename after redirection");
              errorOnLine = 1;
              break;
          }
          printf(" 2>&1\n");
          break;
        case REDIR_ERR:
           if(expFile){
              printf("missing filename after redirection");
              errorOnLine = 1;
              break;
          }
          printf(" 2>\n");
          break;
        case SYSTEM_ERROR:
          perror("system error");
          return(1);
        
        default:
          printf("%d: %s\n",rtn,tokens[rtn%96]);
      }
      rtn=parse_line(NULL);
    }
    if(rtn == EOL && !errorOnLine){
        if(expFile){
            printf("missing filename after redirection");
            errorOnLine = 1;
        }
        else{
            printf("%s EOL", inner);
        }
        command = 0;
    }
    printf("\n");
  }
}

